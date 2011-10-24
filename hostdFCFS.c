/*******************************************************************
HOST dispatcher 

    hostd

        hostd is fcfs 'dispatcher' that reads in a list of 'jobs' from a file
        and 'dispatches' them in a first-come-first-served manner.

        time resolution is one second (although this can be changed). 

    usage
   
        hostd <dispatch file>
 
        where
            <dispatch file> is list of process parameters as specified 
                for assignment 2.

    functionality

    1. Initialize dispatcher queue;
    2. Fill dispatcher queue from dispatch list file;
    3. Start dispatcher timer;
    4. While there's anything in the queue or there is a currently running process:
        i. If a process is currently running;
            a. Decrement process remainingcputime;
            b. If times up:
                A. Send SIGINT to the process to terminate it;
                B. Free up process structure memory
       ii. If no process currently running &&
            dispatcher queue is not empty &&
            arrivaltime of process at head of queue is <= dispatcher timer:
            a. Dequeue process and start it (fork & exec)
            b. Set it as currently running process;
      iii. sleep for one second;
       iv. Increment dispatcher timer;
        v. Go back to 4.
    5. Exit

*******************************************************************/

#include "hostd.h"

#define VERSION "1.0"

/******************************************************
 
   internal functions
   
 ******************************************************/

char * StripPath(char*);
void PrintUsage(FILE *, char *);
void SysErrMsg(char *, char *);
void ErrMsg(char *, char *);

/* global storage */


/******************************************************/

int main (int argc, char *argv[])
{
    char * inputfile;             // job dispatch file
    FILE * inputliststream;
    PcbPtr inputqueue = NULL;     // input queue buffer
	PcbPtr inputqueue_tail = NULL;
	PcbPtr curr = NULL; // for creating queue
    PcbPtr currentprocess = NULL; // current process
   // PcbPtr process = NULL;        // working pcb pointer
    int timer = 0;                // dispatcher timer 
	int processCounter = 0;
	
	char buffer[75]; // for reading current line of file
	//int notNeeded[3]; // for storing the unneeded arguements of input line

//  0. Parse command line

    if (argc == 2) inputfile = argv[1];
    else PrintUsage (stderr, argv[0]);
	
	
//  1. Initialize dispatcher queue;
//     (already initialised in assignments above)
    
//  2. Fill dispatcher queue from dispatch list file;
    
    if (!(inputliststream = fopen(inputfile, "r"))) { // open it
          SysErrMsg("could not open dispatch list file:", inputfile);
          exit(2);
    }

	inputqueue = createnullPcb();
	curr = inputqueue;
	
	
    while (!feof(inputliststream)) {  // put processes into input_queue
 // your code goes here
// don't forget to initialize the PCB status
// and put PCB on the queue
		// MH DONE
		
		
		curr->next = createnullPcb();
		curr->args[0] = DEFAULT_PROCESS;
		curr->args[1] = NULL;
		fgets(buffer,sizeof(buffer),inputliststream);
				
		
		sscanf(buffer, 
			   "%d, %d, %d, %d", 
			   &curr->arrivaltime,
			   &curr->priority,
			   &curr->remainingcputime,
			   &curr->mbytes
			   );
		
		printf("%d %d %d %d\n", curr->arrivaltime, curr->priority, curr->remainingcputime, curr->mbytes);
		
		curr = curr->next;
		processCounter++;
		
    }
	printf("process counter %d\n", processCounter);
	inputqueue_tail = curr;
	//curr->next = NULL;
	

//  3. Start dispatcher timer;
//     (already set to zero above)
        
//  4. While there's anything in the queue or there is a currently running process:

    while (processCounter>1 ) {// i'll give you this outer loop, now do the rest
/*
	  i. If a process is currently running;
	             a. Decrement process remainingcputime;
            b. If times up:
                A. Send SIGINT to the process to terminate it;
                B. Free up process structure memory
       ii. If no process currently running &&
            dispatcher queue is not empty &&
            arrivaltime of process at head of queue is <= dispatcher timer:
            a. Dequeue process and start it (fork & exec)
            b. Set it as currently running process;
      iii. sleep for one second;
       iv. Increment dispatcher timer;
        v. Go back to 4.
*/

	printf("process counter in loop %d\n", processCounter);

	
	//If a process is currently running;
	if(currentprocess)
	{
		//a. Decrement process remainingcputime;
		currentprocess->remainingcputime = currentprocess->remainingcputime - 1;
		// b. If times up:
		if(currentprocess->remainingcputime == 0 ) 
			//A. Send SIGINT to the process to terminate it;
			//B. Free up process structure memory
			if(terminatePcb(currentprocess)!=NULL){
				PcbPtr toFree = currentprocess;
				free(toFree);
				currentprocess=NULL;
				processCounter--;
				
			}
	 }
	/* 
	 ii. If no process currently running &&
	 dispatcher queue is not empty &&
	 arrivaltime of process at head of queue is <= dispatcher timer:
	 a. Dequeue process and start it (fork & exec)
	 b. Set it as currently running process;
	 */
	 if(!currentprocess 
		&& inputqueue!=NULL 
		&& inputqueue->arrivaltime <= timer)
	 {
		 PcbPtr dequed = deqPcb(&inputqueue);
		 currentprocess = startPcb(dequed);
		 printf("new process with arrival time %d\n",currentprocess->arrivaltime);
		 printf("cputime %d\n", currentprocess->remainingcputime);
	 }	 
		 
	
			
		
		//iii. sleep for one second;
		sleep(1);
		
		//increment dispatch timer
		timer++;
		/*printf("current process: %d, remainingcputime: $d, 
			   dispatch timer: $d", currentProcess->pid, currentProcess->remainingcputime,
			   timer);*/
	
	}

		// 5. Exit
    exit (0);
}    


/*******************************************************************

char * StripPath(char * pathname);

  strip path from file name

  pathname - file name, with or without leading path

  returns pointer to file name part of pathname
    if NULL or pathname is a directory ending in a '/'
        returns NULL
*******************************************************************/

char * StripPath(char * pathname)
{
    char * filename = pathname;\

    if (filename && *filename) {           // non-zero length string
        filename = strrchr(filename, '/'); // look for last '/'
        if (filename)                      // found it
            if (*(++filename))             //  AND file name exists
                return filename;
            else
                return NULL;
        else
            return pathname;               // no '/' but non-zero length string
    }                                      // original must be file name only
    return NULL;
}

/*******************************************************
 * print usage
 ******************************************************/
void PrintUsage(FILE * stream, char * progname)
{
    if(!(progname = StripPath(progname))) progname = DEFAULT_NAME;
    
    fprintf(stream,"\n"
"%s process dispatcher (version " VERSION "); usage:\n\n"
"  %s <dispatch file>\n"
" \n"
"  where <dispatch file> is list of process parameters \n\n",
    progname,progname);

    exit(127);
}
/********************************************************
 * print an error message on stderr
 *******************************************************/

void ErrMsg(char * msg1, char * msg2)
{
    if (msg2)
        fprintf(stderr,"ERROR - %s %s\n", msg1, msg2);
    else
        fprintf(stderr,"ERROR - %s\n", msg1);
    return;
}

/*********************************************************
 * print an error message on stderr followed by system message
 *********************************************************/

void SysErrMsg(char * msg1, char * msg2)
{
    if (msg2)
        fprintf(stderr,"ERROR - %s %s; ", msg1, msg2);
    else
        fprintf(stderr,"ERROR - %s; ", msg1);
    perror(NULL);
    return;
}



                                          
