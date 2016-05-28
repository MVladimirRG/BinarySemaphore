#ifndef MEMORYMAPTABLENODE
#define MEMORYMAPTABLENODE

/*
======================================================================
Includes
======================================================================
*/
#include "MMT/MemoryMapTableRow.h"

/*
======================================================================
Data
======================================================================
*/
typedef struct MemoryMapTableNode
{
   MMTR row_;
   struct MemoryMapTableNode  *next_;
} MMTN;

/*
======================================================================
Function Declarations
======================================================================
*/
MMTN *allocate_MMTN(void);

void print_MMTN(MMTN *n);
void print_MMTN_Ln(MMTN *n);

/*
======================================================================
Function Implementations
======================================================================
*/
MMTN *allocate_MMTN(void)
{
   return (MMTN *) malloc(sizeof(MMTN));
}

void print_MMTN(MMTN *n)
{
   if(!n)
      printf("not exist");
   else
      print_MMTR(n->row_);
}

void print_MMTN_Ln(MMTN *n)
{
   print_MMTN(n);
   printf("\n");
}

#endif /* MEMORYMAPTABLENODE*/
