#include <stdlib.h>

#include "multilevelQueueScheduler.h"

void attemptPromote( schedule *ps );
int min( int x, int y );

static const int STEPS_TO_PROMOTION = 50;
static const int FOREGROUND_QUEUE_STEPS = 5;

static process* tempProc = NULL;
static bool  promotionImminent = false;
#define pass (void)0

/* printNames
 * input: none
 * output: none
 *
 * Prints names of the students who worked on this solution
 */
void printNames( )
{
    /* TODO : Fill in you and your partner's names (or N/A if you worked individually) */
    printf("\nThis solution was completed by:\n");
    printf("Lauren Grissom\n");
    printf("Christian Walker\n\n");
}

/* createSchedule
 * input: none
 * output: a schedule
 *
 * Creates and return a schedule struct.
 */
schedule* createSchedule( ) {
	
    /* TODO: malloc space for a new schedule and initialize the data in it */
	schedule* s = (schedule*)malloc(sizeof(schedule));
	s->foreQueue = createQueue();
	s->backQueue = createQueue();
	s->stepsTotal = 0;
	
    return s; /* TODO: Replace with your return value */
}

/* isScheduleUnfinished
 * input: a schedule
 * output: bool (true or false)
 *
 * Check if there are any processes still in the queues.
 * Return TRUE if there is.  Otherwise false.
 */
bool isScheduleUnfinished( schedule *ps ) {
    /* TODO: check if there are any process still in a queue.  Return TRUE if there is. */
	if(!isEmpty(ps->foreQueue) || !isEmpty(ps->backQueue)){
		return true;
	}
	else{
		return false;
	}
}

/* addNewProcessToSchedule
 * input: a schedule, a string, a priority
 * output: void
 *
 * Create a new process with the provided name and priority.
 * Add that process to the appropriate queue
 */
void addNewProcessToSchedule( schedule *ps, char *processName, priority p ) {
	processData* pD = initializeProcessData(processName);
	process* proc = (process*)malloc(sizeof(process));
	proc->prior = p;
	proc->name = processName;
	proc->data = pD;
	proc->stepAdded = (ps->stepsTotal);
	proc->stepsExecuted = FOREGROUND_QUEUE_STEPS;
	
	if(p == FOREGROUND){
		enqueue(ps->foreQueue, proc);
	}
	else{
		enqueue(ps->backQueue, proc);
	}
}

/* runNextProcessInSchedule
 * input: a schedule
 * output: a string
 *
 * Use the schedule to determine the next process to run and for how many time steps.
 * Call "runProcess" to attempt to run the process.  You do not need to print anything.
 * You should return the string received from "runProcess".  You do not need to use/modify this string in any way.
 */
char* runNextProcessInSchedule( schedule *ps ) {
    char *ret = NULL;
    int numSteps = 0, numExecuted;
	bool b;
	
	if(promotionImminent){
		attemptPromote(ps);
		promotionImminent = false;
	}
	
	if((tempProc != NULL && tempProc->prior == FOREGROUND && tempProc->stepsExecuted != FOREGROUND_QUEUE_STEPS && !(isEmpty(ps->foreQueue)))){
		numSteps = tempProc->stepsExecuted;
	}
	else{
        attemptPromote( ps );
		
        if( !isEmpty(ps->foreQueue) ){
            tempProc = getNext(ps->foreQueue);
			
            if(isEmpty(ps->backQueue)){
				if(tempProc->stepsExecuted != FOREGROUND_QUEUE_STEPS){
					numSteps = tempProc->stepsExecuted;
				}
                else{
					numSteps = FOREGROUND_QUEUE_STEPS;
				}
            }
            else{
				numSteps = min((getNext(ps->backQueue)->stepAdded + STEPS_TO_PROMOTION ) - ps->stepsTotal, FOREGROUND_QUEUE_STEPS);
			}
        }
        else if( !isEmpty(ps->backQueue) ){
            tempProc = getNext(ps->backQueue);
            numSteps = (tempProc->stepAdded + STEPS_TO_PROMOTION ) - ps->stepsTotal;
        }
	}
	
	numExecuted = numSteps;
	loadProcessData(tempProc->data);
	b = runProcess( tempProc->name, &ret, &numExecuted );
	
	if(b){
		tempProc = (tempProc->prior == FOREGROUND) ? dequeue(ps->foreQueue) : dequeue(ps->backQueue);
		freeProcessData();
        free(tempProc->name);
        free(tempProc);
        if(tempProc != NULL){
			tempProc = NULL;
		}
    }
	else if(tempProc->prior == BACKGROUND) pass;
    else{
		if(numExecuted == numSteps){
			if( !isEmpty(ps->backQueue) && (((getNext(ps->backQueue)->stepAdded + STEPS_TO_PROMOTION) - (ps->stepsTotal + numExecuted)) == 0)){
				promotionImminent = true;
				tempProc->stepsExecuted -= numExecuted;
				
				if(tempProc->stepsExecuted == 0){
					enqueue(ps->foreQueue, dequeue(ps->foreQueue));
					tempProc = NULL;
				}
			}
			else{
				tempProc->stepsExecuted = FOREGROUND_QUEUE_STEPS;
				enqueue(ps->foreQueue, dequeue(ps->foreQueue));
			}
		}
		else{
			tempProc->stepsExecuted = numSteps - numExecuted;
		}
	}
	
	ps->stepsTotal += numExecuted;
	
	/*promoted = false;*/
    return ret; 
}

/* attemptPromote
 * input: a schedule
 * output: none
 *
 * Promote every background process that has been waiting for 50 time steps.
 * This function might be tricky so you might save it for last.
 */
void attemptPromote( schedule *ps ) {
    /* TODO: complete this function.
    The function "promoteProcess" in processSimulator.c will be useful in completing this. */
	int count = 0;
	
	do{
		if(isEmpty(ps->backQueue)){
			return;
		}
		if((getNext(ps->backQueue))->stepAdded + STEPS_TO_PROMOTION  - (ps->stepsTotal) == 0){
			promoteProcess(getNext(ps->backQueue)->name, getNext(ps->backQueue)->data);
			getNext(ps->backQueue)->prior = FOREGROUND;
			enqueue(ps->foreQueue, dequeue(ps->backQueue));
			/*promoted = true;*/
		}
		else{
			count++;
		}
	}while(count == 0);
}
/* freeSchedule
 * input: a schedule
 * output: none
 *
 * Free all of the memory associated with the schedule.
 */
void freeSchedule( schedule *ps ) {
	if(isEmpty(ps->foreQueue)){
		freeQueue(ps->foreQueue);
	}
	else{
		printf("\nSOMETHING WENT WRONG: FOREQUEUE ISNT EMPTY\n");
		exit(-1);
	}
	
	if(isEmpty(ps->backQueue)){
		freeQueue(ps->backQueue);
	}
	else{
		printf("\nSOMETHING WENT WRONG: BACKQUEUE ISNT EMPTY\n");
		exit(-1);
	}
	
	free(ps);
	
}

/* PROVIDED FUNCTION
 * freeSchedule
 * input: two ints
 * output: the smaller of the 2 ints
 */
int min( int x, int y ){
    if( x<y )
        return x;
    return y;
}
