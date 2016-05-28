#ifndef PROCESSCONTROLBLOCKTABLE_H
#define PROCESSCONTROLBLOCKTABLE_H

/*
======================================================================
Includes
======================================================================
*/
#include <stddef.h>

#include "PCB/ProcessControlBlockNode.h"
#include "Core/Console.h"
#include "ProgramSettings.h"

typedef struct PCBTable
{
   size_t size_;
   PCBN *begin_, *end_;
} PCBT;

/*
======================================================================
Function Declarations
======================================================================
*/
void load_Processes(PCBT *t);
void initialize_PCBT(PCBT *t);

void insertAtTheEnd_PCBN(PCBT *t, PCBN *p);
PCBN *insertNewAtTheEnd_PCBN(PCBT *t, const size_t idT1, const size_t page1);

void erase_PCBT(PCBT *t);

void print_headers_PCBT(void);
void print_headers_PCBT_Ln(void);
void print_PCBT(const PCBT t);
void print_PCBT_Ln(const PCBT t);
void sort(PCBT *t);

/*
======================================================================
Function Implementations
======================================================================
*/
void load_Processes(PCBT *t)
{
   int i = 0, j = 0;
   for(i = 0; i < MAX_PROCESSES; ++i)
      for(j = 0; j < getRandom(1, MAX_PAGES); ++j)
         insertNewAtTheEnd_PCBN(t, i, j);
   (void) j;
}

void initialize_PCBT(PCBT *t)
{
   t->size_ = 0;
   t->begin_ = NULL;
   t->end_ = NULL;
   load_Processes(t);
}

void insertAtTheEnd_PCBN(PCBT *t, PCBN *p)
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

PCBN *insertNewAtTheEnd_PCBN(PCBT *t, const size_t idT1, const size_t page1)
{
   PCBN *newP = allocate_PCBN();
   PCB *p = &newP->row_;
   p->id_ = Number_(t->size_, Unit, Dimensionless);
   p->taskId_ = Number_(idT1, Unit, Dimensionless);
   p->page_ = Number_(page1, Unit, Dimensionless);
   p->arrivingTime_ = Number_(t->size_, Mili, Second);
   p->cycles_ = Number_(getRandom(1, MAX_CYCLES), Unit, Dimensionless);
   p->status_ = zeroNumber;
   p->sizeInMemory_ = Number_(getRandom(1, MAX_SIZEMEM), Kilo, Byte);
   p->ioDevices_ = Number_(getRandom(0, MAX_IODEV), Unit, Dimensionless);
   p->files_ = Number_(getRandom(0, MAX_FILES), Unit, Dimensionless);
   p->processType_ = Number_(getRandom(0, 2), Unit, Dimensionless);
   p->startOfCriSec_ = Number_(getRandom(0, p->cycles_.value_), Unit, Dimensionless);
   p->durOfCriSec_ = Number_(getRandom(ALWAYS_CRISEC, p->cycles_.value_
                                       - p->startOfCriSec_.value_),
                             Unit, Dimensionless);
   insertAtTheEnd_PCBN(t, newP);
   return newP;
}

void erase_PCBT(PCBT *t)
{
   PCBN
         *p = t->begin_,
         *erased = NULL;
   while(p != NULL)
   {
      erased = p;
      p = p->next_;
      deallocate(& erased);
   }
}

PCBN *seekPrevious_PCBN(const PCBT t, PCBN *r)
{
   PCBN *aux = t.begin_;

   for(; aux != NULL; aux = aux->next_)
   {
      if(aux->next_ != NULL)
      {
         if(r == aux->next_)
            return aux;
      }
      else
         return NULL;
   }
   return NULL;
}

bool eraseBetween_PCBN(PCBT *t, PCBN *p)
{
   PCBN *aux = p->next_, *next = aux->next_;

   if(aux == NULL)
      return false;

   p->next_ = next;
   deallocate(&aux);
   -- t->size_;
   return true;
}

PCBN *eraseAsFirst_PCBN(PCBT * t)
{
   deallocate(&t->begin_);
   t->end_ = NULL;
   -- t->size_;
   return t->begin_;
}

PCBN *eraseAtTheBeginning_PCBN(PCBT * t)
{
   PCBN *aux = t->begin_;

   if(t->size_ == 1)
      return eraseAsFirst_PCBN(t);

   t->begin_ = t->begin_->next_;
   deallocate(&aux);
   --t->size_;
   return t->begin_;
}

PCBN *eraseAtTheEnd_PCBN(PCBT *t)
{
   if(t->size_ == 1)
      return eraseAsFirst_PCBN(t);
   else
   {
      PCBN *aux = seekPrevious_PCBN(*t, t->end_);
      if(eraseBetween_PCBN(t, aux))
         t->end_ = aux;
      return t->end_;
   }
}

PCBN *eraseActual_PCBN(PCBT *t, PCBN* r)
{
   PCBN *aux = seekPrevious_PCBN(*t, r);

   if(t->begin_ == r)
      return eraseAtTheBeginning_PCBN(t);

   if(t->end_ == r)
      return eraseAtTheEnd_PCBN(t);

   eraseBetween_PCBN(t, aux);
   if(aux->next_)
      return aux->next_;
   else
      return aux;
}

void eraseOutgoing_PCBT(PCBT *t)
{
   PCBN* aux = t->begin_;
   while(aux != NULL)
   {
      if(aux->row_.status_.value_ == 5)
         aux = eraseActual_PCBN(t, aux);
      else
         aux = aux->next_;
   }
}

void print_headers_PCBT(void)
{
   PCBAttribute i = 0;
   for(; i < pcb_nHeaders; ++i)
      print_underscore_2Sp(pcb_headers[i], pcb_spaces[i], pcb_spaces[i]);
}

void print_headers_PCBT_Ln(void)
{
   print_headers_PCBT();
   printf("\n");
}

void print_PCBT(const PCBT t)
{
#if PRINT_PCBT == 1
   PCBN *aux = t.begin_;
   print_headers_PCBT_Ln();
   if(t.size_ == 0)
      printf("%*.*sempty\n", pcb_spaces[0], pcb_spaces[0], "");
   for(; aux  != NULL; aux = aux->next_)
      print_PCBN_Ln(aux);
#else
   (void) t;
#endif
}

void print_PCBT_Ln(const PCBT t)
{
   print_PCBT(t);
#if PRINT_PCBT == 1
   printf("\n");
#endif
}

void sort_PCBT(PCBT *t, SortingType type, PCBAttribute attribute)
{
   size_t i = 0;
   PCBN *aux = NULL;
   PCBN *n = NULL;
   ComparisonType comparison;

   if(type == Ascendant)
      comparison = GreaterThan;
   else
      comparison = LessThan;

   for(; i < t->size_; ++i)
   {
      aux = t->begin_;
      for(; aux != NULL; aux = aux->next_)
         if(aux->next_ != NULL)
         {
            n = aux->next_;
            if(compare_PCB(aux->row_,attribute, comparison, n->row_, attribute))
            {
               PCB r = aux->row_;
               aux->row_ = n->row_;
               n->row_ = r;
            }
         }
   }
}

#endif /*PROCESSCONTROLBLOCKTABLE_H*/
