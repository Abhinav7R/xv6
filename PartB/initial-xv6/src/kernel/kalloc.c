// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"

void freerange(void *pa_start, void *pa_end);

extern char end[]; // first address after kernel.
                   // defined by kernel.ld.

struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  struct run *freelist;
} kmem;

struct spinlock ref_lock;
int num_pages=(PGROUNDUP(PHYSTOP))/PGSIZE;
// int ref_count[(PGROUNDUP(PHYSTOP)-KERNBASE)/PGSIZE];
int ref_count[PGROUNDUP(PHYSTOP)/PGSIZE];

void decrement_reference_count(void* pa)
{
    acquire(&ref_lock);
    ref_count[PGROUNDUP((uint64)pa)/PGSIZE]--;
    release(&ref_lock);
}

void increment_reference_count(void* pa)
{
    acquire(&ref_lock);
    ref_count[PGROUNDUP((uint64)pa)/PGSIZE]++;
    release(&ref_lock);
}

int get_reference_count(void* pa)
{
    acquire(&ref_lock);
    int count=ref_count[PGROUNDUP((uint64)pa)/PGSIZE];
    release(&ref_lock);
    return count;
}

void
kinit()
{
    initlock(&kmem.lock, "kmem");
    initlock(&ref_lock, "ref_lock");
    for(int i = 0; i < num_pages; i++)
        ref_count[i] = 1;   

  freerange(end, (void*)PHYSTOP);
}

void
freerange(void *pa_start, void *pa_end)
{
  // printf("hi2\n");
  char *p;
  p = (char*)PGROUNDUP((uint64)pa_start);
  // printf("hi3\n");
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE)
  {
    // printf("hi4\n");
    kfree(p);
  }
}

// Free the page of physical memory pointed at by pa,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(void *pa)
{
  struct run *r;

  if(((uint64)pa % PGSIZE) != 0 || (char*)pa < end || (uint64)pa >= PHYSTOP)
    panic("kfree");

  // printf("%d\n", get_reference_count(pa));
  if(get_reference_count(pa) <= 0)
  {
      panic("kfree: reference count is <= 0");
  }
  decrement_reference_count(pa);
  

  //free only when reference count is 0 matlab child parent all done
  //even if any of the processes is accessing the page then count > 0 and dont free
  if(get_reference_count(pa) == 0)
  {
    // Fill with junk to catch dangling refs.
    memset(pa, 1, PGSIZE);

    r = (struct run*)pa;

    acquire(&kmem.lock);
    r->next = kmem.freelist;
    kmem.freelist = r;
    release(&kmem.lock);
  }
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *
kalloc(void)
{
  struct run *r;

  acquire(&kmem.lock);
  r = kmem.freelist;
  if(r)
    kmem.freelist = r->next;
  release(&kmem.lock);

  if(r)
  {
    memset((char*)r, 5, PGSIZE); // fill with junk
    increment_reference_count((void*)r);
  }
  return (void*)r;
}
