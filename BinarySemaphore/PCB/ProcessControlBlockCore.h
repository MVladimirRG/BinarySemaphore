#ifndef PROCESSCONTROLBLOCKCORE_H
#define PROCESSCONTROLBLOCKCORE_H
/*
======================================================================
Includes
======================================================================
*/
#include <stddef.h>

/*
======================================================================
Preprocessor
======================================================================
*/
#define pcb_nHeaders    12
#define pcb_max_width   16

/*
======================================================================
Data
======================================================================
*/
char pcb_headers[pcb_nHeaders][pcb_max_width+1] =
{
   "Id",
   "Task id",
   "Page",
   "Arriving",/*8*/

   "Cycles",
   "Status",/*6*/
   "I/O Dev",/*7*/
   "Files",

   "Type",/*4*/
   "Memory",/*6*/
   "Start Of Cri Sec",/*16*/
   "Dur Of Cri Sec"/*14*/
};

const size_t pcb_spaces[pcb_nHeaders] =
{
   1, 1, 1, 1,
   1, 1, 1, 1,
   1, 1, 1, 1
};

size_t pcb_lengths[pcb_nHeaders];

/*
======================================================================
Function Declarations
======================================================================
*/
void pcb_init(void);

/*
======================================================================
Function Implementations
======================================================================
*/
void pcb_init(void)
{
   size_t i = 0;
   for(; i < pcb_nHeaders; ++i)
      pcb_lengths[i] = strlen(pcb_headers[i]);
}

#endif /*PROCESSCONTROLBLOCKCORE_H*/
