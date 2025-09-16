# Modified Priority Based Scheduler in xv-6

This part of the project involves the implementation of preemptive priority based scheduling which involves the following steps:
1. In the proc struct add variables static_priority, rbi, run_time, wait_time, sleep_time, no_of_times_scheduled
2. Initialise these variables in allocproc rbi to 25, static_priority to 50 and other to 0
3. In scheduler, iterate through the process array and calculate rbi and dynamic_priority for every process
4. rbi=max(0,int((3*run_time-sleep_time-wait_time)/(run_time+sleep_time+wait_time+1))*50)
5. dp=min(sp+rbi,100)
6. Process with min value of dp(highest priority) is selected
7. If more than 2 processes have same priority then tie breakers are used
8. Process which is scheduled lesser no of times is prioritised and if tie further exists then process created late is prioritised
9. Once the highest priority process is selected, schedule it in the cpu and set its run_time and sleep_time to 0 and increment the no_of_times_scheduled 

Set priority system call  
This syscall returns the old static_priority and assigns new priority to the process  
The pid of the process and the new priority is taken in from command line arguments  
If the new priority is higher (< in value) than old one then yield is called  

Analysis  
The method schedules the processes on the basis of priority assigned to each process and is a good scheduling technique where processes need to be schedule on the basis of their importance   
- Average rtime 16,  wtime 167 for PBS
- Average rtime 18,  wtime 171 for RR

# COW Copy on Write Fork in xv-6

This implementation saves the memory when multiple processes are forked and each of them gets new pages exactly copied from the parent   
COW shares the pages until some process wants to write. Now that particular page which is to be modified is copied for the process and hence saves memory by avoiding unnecessary copies  

Implementation:  
Where the copy is made for the pages, we introduce a new flag PTE_COW and set it to 1 and make shared pages unwritable by modifying the PTE_W flag  
Whenever interrupt happens for writing to the shared page we allocate a new copy to the process by cow_alloc and copy the contents of the shared page to it and it can then modify the page  
Also made changes to copyout to check for cow_alloc while getting from kernel to user mode  
To free a page, we maintain a reference_count which is initialised to 1, its incremented when its shared and decremented when the process sharing it exits until its value is 0, upon which the page is freed  

xv6 kernel is booting  

hart 2 starting  
hart 1 starting  
init: starting sh  
$ cowtest  
simple: ok  
simple: ok  
three: ok  
three: ok  
three: ok  
file: ok  
ALL COW TESTS PASSED 