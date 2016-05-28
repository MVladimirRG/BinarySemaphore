#ifndef QUEUE_H
#define QUEUE_H
/*
======================================================================
Includes
======================================================================
*/
#include <stddef.h>
#include "QUEUE/QueueNode.h"

typedef struct
{
   size_t size_;
   QueueNode *begin_, *end_;
} Queue;

/*
======================================================================
Function Declarations
======================================================================
*/
void initialize_Queue(Queue *q);

void insertAtTheEnd_QueueN(Queue *t, QueueNode *p);
void push(Queue *t, PCBN *n);

QueueNode *eraseAtTheBeginning_QueueN(Queue *t);
PCBN* pop(Queue *t);
void erase_Queue(Queue *t);

void print_headers_Queue(void);
void print_Queue(const Queue t);
void print_Queue_Ln(const Queue t);

/*
======================================================================
Function Implementations
======================================================================
*/
void initialize_Queue(Queue *t)
{
   t->size_ = 0;
   t->begin_ = NULL;
   t->end_ = NULL;
}

void insertAtTheEnd_QueueN(Queue *t, QueueNode *p)
{
   p->next_ = NULL;
   if(t->size_ == 0)
   {
      t->begin_ = p;
      t->end_ = p;
   }
   else
   {
      t->end_->next_ = p;
      t->end_ = t->end_->next_;
      t->end_->next_ = NULL;
   }
   ++t->size_;
}

void push(Queue *t, PCBN *n)
{
   QueueNode *qn = allocate_QueueN();
   qn->row_ = QueueR_(n);
   insertAtTheEnd_QueueN(t, qn);
}

QueueNode *eraseAsFirst_QueueN(Queue * t)
{
   deallocate(&t->begin_);
   t->end_ = NULL;
   -- t->size_;
   return t->begin_;
}

QueueNode *eraseAtTheBeginning_QueueN(Queue * t)
{
   QueueNode *aux = t->begin_;

   if(t->size_ == 1)
      return eraseAsFirst_QueueN(t);

   t->begin_ = t->begin_->next_;
   deallocate(&aux);
   --t->size_;
   return t->begin_;
}

PCBN *pop(Queue *t)
{
   PCBN *p = NULL;
   if(t->size_ > 0)
   {
      p = t->begin_->row_.pcb_;
      eraseAtTheBeginning_QueueN(t);
   }
   return p;
}

void erase_Queue(Queue *t)
{
   QueueNode
         *p = t->begin_,
         *erased = NULL;
   while(p != NULL)
   {
      erased = p;
      p = p->next_;
      deallocate(&erased);
   }
}

void print_Queue(const Queue t)
{
   QueueNode *aux = t.begin_;
   if(t.size_ == 0)
      printf("%*.*sempty\n", queue_space, queue_space, "");
   for(; aux  != NULL; aux = aux->next_)
      print_QueueN_Ln(aux);
}

void print_Queue_Ln(const Queue t)
{
   print_Queue(t);
   printf("\n");
}

#endif /*QUEUE_H*/
