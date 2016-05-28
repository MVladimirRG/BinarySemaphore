#ifndef QUEUENODE_H
#define QUEUENODE_H

/*
======================================================================
Includes
======================================================================
*/
#include "QUEUE/QueueRow.h"

/*
======================================================================
Data
======================================================================
*/
typedef struct QueueNode
{
   QueueR row_;
   struct QueueNode *next_;
} QueueNode;

/*
======================================================================
Function declarations
======================================================================
*/
QueueNode *allocate_QueueN(void);
void print_QueueN(const QueueNode *n);
void print_QueueN_Ln(const QueueNode *n);

/*
======================================================================
Function Implementations
======================================================================
*/
QueueNode *allocate_QueueN(void)
{
   return (QueueNode *) malloc( sizeof(QueueNode) );
}

void print_QueueN(const QueueNode *n)
{
   if(n)
      print_QueueR(n->row_);
   else
      printf("not exist");
}

void print_QueueN_Ln(const QueueNode *n)
{
   print_QueueN(n);
   printf("\n");
}

#endif /*QUEUENODE_H*/
