#ifndef _multilevelQueueScheduler_h
#define _multilevelQueueScheduler_h
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>

#include "process.h"
#include "queue.h"
#include "processSimulator.h"

/* struct schedule (only accessed in student written code)
 *
 * Data related to the order the processes should be scheduled
 * foreQueue and backQueue are the FOREGROUND and BACKGROUND queues respectively.
 */
typedef struct schedule
{
    Queue *foreQueue;
    Queue *backQueue;
	int stepsTotal;


}  schedule;

void printNames( );

schedule* createSchedule( );
bool isScheduleUnfinished( schedule *ps );
void addNewProcessToSchedule( schedule *ps, char *processName, priority p );
char* runNextProcessInSchedule( schedule *ps );
void freeSchedule( schedule *ps );

#endif

