#ifndef QUEUEROW_H
#define QUEUEROW_H

/*
======================================================================
Includes
======================================================================
*/
#include "PCB/ProcessControlBlockNode.h"
#include "Core/Console.h"

/*
======================================================================
Macros
======================================================================
*/
#define queue_space     1
#define queue_length    4

/*
======================================================================
Data
======================================================================
*/
typedef struct
{
   PCBN *pcb_;
} QueueR;

/*
======================================================================
Function Declarations
======================================================================
*/
QueueR QueueR_(PCBN *p);
void print_QueueR(const QueueR r);
void print_QueueR_Ln(const QueueR r);

/*
======================================================================
Function Implementations
======================================================================
*/
QueueR QueueR_(PCBN *p)
{
   QueueR r;
   r.pcb_ = p;
   return r;
}

void print_QueueR(const QueueR r)
{
   int id = (r.pcb_) ? ((int) r.pcb_->row_.id_.value_) : -1;
   print_string("", queue_space);
   printf("%*i ", queue_length, id);
   print_string("", queue_space);
}

void print_QueueR_Ln(const QueueR r)
{
   print_QueueR(r);
   printf("\n");
}

#endif /*QUEUEROW_H*/
