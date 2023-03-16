/* THIS FILE WAS CONTRIBUTED BY INSTRUCTOR, DR. JESSICA SHERETTE. */

#ifndef _processSimulator_h
#define _processSimulator_h
#include <stdlib.h>
#include <stdbool.h>

#include "process.h"

processData* initializeProcessData( char *processName );
bool runProcess( char *pName, char **ppSystemCall, int *pNumSteps );
void freeProcessData( );
void promoteProcess( char *pName, processData *pData );
void loadProcessData( processData *pData );

int max( int a, int b );
int powInt( int a, int b );

#endif
