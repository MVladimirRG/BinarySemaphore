#ifndef PROCESSCONTROLBLOCKNODE_H
#define PROCESSCONTROLBLOCKNODE_H

/*
======================================================================
Includes
======================================================================
*/
#include "PCB/ProcessControlBlockRow.h"

/*
======================================================================
Data
======================================================================
*/
typedef struct ProcessControlBlockNode
{
   PCB row_;
   struct ProcessControlBlockNode *next_;
} PCBN;

/*
======================================================================
Function declarations
======================================================================
*/
PCBN *allocate_PCBN(void);
void print_PCBN(const PCBN *p);
void print_PCBN_Ln(const PCBN *p);

/*
======================================================================
Function Implementations
======================================================================
*/
PCBN *allocate_PCBN(void)
{
   return (PCBN *) malloc( sizeof(PCBN) );
}

void print_PCBN(const PCBN *p)
{
   if(p)
      print_PCB(p->row_);
   else
      printf("not exist");
}

void print_PCBN_Ln(const PCBN *p)
{
   print_PCBN(p);
   printf("\n");
}

#endif /*PROCESSCONTROLBLOCKNODE_H*/
