#ifndef _process_h
#define _process_h

typedef enum priority { FOREGROUND, BACKGROUND } priority; 

typedef struct processData{ int heap[  30        ]; char       PN21[       21];char TLN        [4  ];}   processData ;

/* struct process (only accessed in student written code)
 *
 * The data associated with a specific process.
 * At minimum you need to track the process name,
 * and a pointer to the processData struct returned 
 * by initializeProcessData
 */
typedef struct process
{
	char* name;
	processData* data;
	priority prior;
	int stepAdded;
	int stepsExecuted;

}  process;

#endif

