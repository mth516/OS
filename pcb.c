/*******************************************************************

HOST dispatcher
 
  pcb - process control block functions for HOST dispatcher

   PcbPtr startPcb(PcbPtr process) - start (or restart) a process
    returns:
      PcbPtr of process
      NULL if start (restart) failed

   PcbPtr suspendPcb(PcbPtr process) - suspend a process
    returns:
      PcbPtr of process
      NULL if suspend failed

   PcbPtr terminatePcb(PcbPtr process) - terminate a process
    returns:
      PcbPtr of process
      NULL if terminate failed

   PcbPtr printPcb(PcbPtr process, FILE * iostream)
    - print process attributes on iostream
    returns:
      PcbPtr of process

   void printPcbHdr(FILE *) - print header for printPcb
    returns:
      void
      
   PcbPtr createnullPcb(void) - create inactive Pcb.
    returns:
      PcbPtr of newly initialised Pcb
      NULL if malloc failed

   PcbPtr enqPcb (PcbPtr headofQ, PcbPtr process)
      - queue process (or join queues) at end of queue
      - enqueues at "tail" of queue. 
    returns head of queue

   PcbPtr deqPcb (PcbPtr * headofQ);
      - dequeue process - take Pcb from "head" of queue.
    returns:
      PcbPtr if dequeued,
      NULL if queue was empty
      & sets new head of Q pointer in adrs at 1st arg

*******************************************************************/

#include "pcb.h"

/*******************************************************
 * PcbPtr startPcb(PcbPtr process) - start (or restart)
 *    a process
 * returns:
 *    PcbPtr of process
 *    NULL if start (restart) failed
 ******************************************************/
PcbPtr startPcb (PcbPtr p) 
{ 
	int pidFromFork;
	
    if (p->pid == 0) {                // not yet started
		 pidFromFork = fork ();    //  so start it
    /********************
	 your code goes here
	*********************/
		if(pidFromFork < 0)
			fprintf(stderr, "fork failed for process %d\n", getpid());
		else if(pidFromFork == 0){ // it is the child, so execute process
			
			if(execvp(p->args[0],p->args)){
				fprintf(stderr, "exec of process %d failed", 
						(int)p->pid);
				return NULL;		}
		}
		else{
			
			p->pid = pidFromFork;
		}
	} else { // already started & suspended so continue
           /********************
			your code goes here
			*********************/
		if(kill(p->pid, SIGCONT)){
			fprintf(stderr, "resume of process %d failed", 
					(int)p->pid);
		return NULL;
		}		
		
    }    
    p->status = PCB_RUNNING;
	
    return p; 
} 

/*******************************************************
 * PcbPtr suspendPcb(PcbPtr process) - suspend
 *    a process
 * returns:
 *    PcbPtr of process
 *    NULL if suspend failed
 ******************************************************/
 PcbPtr suspendPcb(PcbPtr p)
 {
    
            /********************
			your code goes here
			*********************/
	 if(kill(p->pid, SIGSTOP))
	 fprintf(stderr, "suspend of %d failed", 
			 (int)p->pid);
	 return NULL;
	 
	 p->status = PCB_SUSPENDED;
	 
     return p;
 }
 
/*******************************************************
 * PcbPtr terminatePcb(PcbPtr process) - terminate
 *    a process
 * returns:
 *    PcbPtr of process
 *    NULL if terminate failed
 ******************************************************/
PcbPtr terminatePcb(PcbPtr p)
{

          /********************
			your code goes here
			*********************/
	if(kill(p->pid, SIGTERM)){
		fprintf(stderr, "terminated of %d failed", 
				(int)p->pid);
		
		return NULL;
	}
	
	
	p->status = PCB_TERMINATED;
	return p;
}  

/*******************************************************
 * PcbPtr printPcb(PcbPtr process, FILE * iostream)
 *  - print process attributes on iostream
 *  returns:
 *    PcbPtr of process
 ******************************************************/
 
PcbPtr printPcb(PcbPtr p, FILE * iostream)
{
    fprintf(iostream, "%7d%7d%7d%7d  ",
        (int) p->pid, p->arrivaltime, p->priority,
            p->remainingcputime);
    switch (p->status) {
        case PCB_UNINITIALIZED:
            fprintf(iostream, "UNINITIALIZED");
            break;
        case PCB_INITIALIZED:
            fprintf(iostream, "INITIALIZED");
            break;
        case PCB_READY:
            fprintf(iostream, "READY");
            break;
        case PCB_RUNNING:
            fprintf(iostream, "RUNNING");
            break;
        case PCB_SUSPENDED:
            fprintf(iostream, "SUSPENDED");
            break;
        case PCB_TERMINATED:
            fprintf(iostream, "PCB_TERMINATED");
            break;
        default:
            fprintf(iostream, "UNKNOWN");
    }
    fprintf(iostream,"\n");
    
    return p;     
}
   
/*******************************************************
 * void printPcbHdr(FILE *) - print header for printPcb
 *  returns:
 *    void
 ******************************************************/  
 
void printPcbHdr(FILE * iostream) 
{  
    fprintf(iostream,"    pid arrive  prior    cpu  status\n");

}
       
/*******************************************************
 * PcbPtr createnullPcb() - create inactive Pcb.
 *
 * returns:
 *    PcbPtr of newly initialised Pcb
 *    NULL if malloc failed
 ******************************************************/
 
PcbPtr createnullPcb()
{
    return malloc(sizeof(Pcb));
	
			/********************
			your code goes here
			do not forget you are creating a list of PCB, so you need to
			allocate some heap memory
			*********************/ 
}   

/*******************************************************
 * PcbPtr enqPcb (PcbPtr headofQ, PcbPtr process)
 *    - queue process (or join queues) at end of queue
 * 
 * returns head of queue
 *****************************************************/
PcbPtr enqPcb(PcbPtr headofQ, PcbPtr process){
        PcbPtr t = headofQ;
        while(t->next) 
			t= t->next;
        t->next = process;
        return headofQ;
}
 

/*******************************************************
 * PcbPtr deqPcb (PcbPtr * headofQ);
 *    - dequeue process - take Pcb from head of queue.
 *
 * returns:
 *    PcbPtr if dequeued,
 *    NULL if queue was empty
 *    & sets new head of Q pointer in adrs at 1st arg
 *******************************************************/
 
PcbPtr deqPcb(PcbPtr * headofQ)
{
	pcbptr temp;
	
	if(*headofQ == NULL){
		return NULL;
	}
	
	tmemp = *headofQ;
	*headofQ = (*headofQ)->next;
	return temp;
	
	 /********************
			your code goes here
	*********************/
	
	
	
	
}

