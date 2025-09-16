#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/syscall.h"
#include "kernel/memlayout.h"
#include "kernel/riscv.h"

int main(int argc, char *argv[]) 
{
    if(argc!=3)
    {
        return 1;
    }
    int pid = atoi(argv[1]);
    int priority = atoi(argv[2]);
    int result=set_priority(pid,priority);
    if(result==-1)
    {
        printf("Unsuccessful in updating priority!!!\n");
        return 1;
    }

    printf("Successful in updating priority :)\n");
    return 0;
}