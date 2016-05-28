#ifndef MemoryMapTable
#define MemoryMapTable

/*
======================================================================
Includes
======================================================================
*/
#include "MMT/MemoryMapTableNode.h"
#include "ProgramSettings.h"

/*
======================================================================
Data
======================================================================
*/
typedef struct
{
   size_t size_;/*number of nodes or frames*/
   MMTN *begin_, *end_, *os_;
} MMT;

/*
======================================================================
Function Declarations
======================================================================
*/
void initializeByDefault_MMT(MMT *t);
void initialize_MMT(MMT *t);

MMTN *seekPrevious_MMTN(const MMT t, MMTN *r);
MMTN *seekAvailable_MMTN(MMTN *beginning);
MMTN *seekBeginning_MMTN(MMTN *beginning, Number frame);

bool insertAsFirst_MMTN(MMT *t, MMTN *n);
bool insertAtTheEnd_MMTN(MMT *t, MMTN *n);
MMTN* insertNewAtTheEnd_MMTN(MMT *t);

void erase_MMT(MMT *t);
bool eraseBetween_MMTN(MMT *t, MMTN *p);
MMTN *eraseAsFirst_MMTN(MMT * t);
MMTN *eraseAtTheBeginning_MMTN(MMT * t);
MMTN *eraseAtTheEnd_MMTN(MMT *t);
MMTN *eraseActual_MMTN(MMT *t, MMTN* r);
void eraseExceptOS_MMT(MMT *t);
void vacateExceptOS_MMT(MMT *t);

void print_headers_MMT(void);
void print_headers_MMT_Ln(void);
void print_MMT(const MMT t);
void print_MMT_Ln(const MMT t);

/*
======================================================================
Function Implementations
======================================================================
*/
void initializeByDefault_MMT(MMT *t)
{
   t->size_ = 0;/*number of nodes*/
   t->begin_ = NULL;
   t->end_ = NULL;
   t->os_ = NULL;
}

void initialize_MMT(MMT *t)
{
   MMTN *aux = NULL; size_t i;
   initializeByDefault_MMT(t);
   for(i = 0; i < osFrames(); ++i)/*Insert frames of os*/
   {
      aux = insertNewAtTheEnd_MMTN(t);
      aux->row_.occupied_ = true;
   }

   for(; i < memFrames() - osFrames(); ++i)
      insertNewAtTheEnd_MMTN(t);
   t->os_ = aux->next_;
}

MMTN *seekPrevious_MMTN(const MMT t, MMTN *r)
{
   MMTN *aux = t.begin_;
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

/**
   @brief seekAvailable_MMTN
   seeks an available frame but with the restriction of frame limit.
   The frame limit is according to nMpt, so it needs to be synchronized.
   @param beginning
   @return
 */
MMTN *seekAvailable_MMTN(MMTN *beginning)
{
   MMTN *aux = beginning;
   for(; aux != NULL; aux = aux->next_)
      if(! aux->row_.occupied_)
         return aux;
   return NULL;
}

MMTN *seekBeginning_MMTN(MMTN *beginning, Number frame)
{
   MMTN *aux = beginning;
   for(; aux != NULL; aux = aux->next_)
      if(aux->row_.begin_.prefix_ == frame.prefix_
         && aux->row_.begin_.value_ == frame.value_)
         return aux;
   return NULL;
}

bool insertAsFirst_MMTN(MMT *t, MMTN *n)
{
   n->next_ = NULL;
   t->begin_ = n;
   t->end_ = t->begin_;
   ++ t->size_;
   return true;
}

bool insertAtTheEnd_MMTN(MMT *t, MMTN *n)
{
   if(t->size_ == 0)
      return insertAsFirst_MMTN(t, n);
   else
   {
      t->end_->next_ = n;
      t->end_ = t->end_->next_;
      t->end_->next_ = NULL;

      ++ t->size_;
      return true;
   }
}

MMTN* insertNewAtTheEnd_MMTN(MMT *t)
{
   MMTN *aux = allocate_MMTN();
   aux->row_.id_ = t->size_;
   aux->row_.begin_ = frameSize;
   aux->row_.begin_.value_ = t->size_ * frameSize.value_;
   aux->row_.occupied_ = false;

   insertAtTheEnd_MMTN(t, aux);
   return aux;
}

void erase_MMT(MMT *t)
{
   MMTN *toErase = NULL, *aux = t->begin_;
   for(; aux != NULL;)
   {
      toErase = aux;
      aux = aux->next_;
      deallocate(&toErase);
   }
   initializeByDefault_MMT(t);
}

bool eraseBetween_MMTN(MMT *t, MMTN *p)
{
   MMTN *aux = p->next_, *next = aux->next_;

   if(aux == NULL)
      return false;

   p->next_ = next;
   deallocate(&aux);
   -- t->size_;
   return true;
}

MMTN *eraseAsFirst_MMTN(MMT * t)
{
   deallocate(&t->begin_);
   t->end_ = NULL;
   t->size_ = 0;
   return t->begin_;
}

MMTN *eraseAtTheBeginning_MMTN(MMT * t)
{
   MMTN *aux = t->begin_;

   if(t->size_ == 1)
      return eraseAsFirst_MMTN(t);

   t->begin_ = t->begin_->next_;
   deallocate(&aux);
   --t->size_;
   return t->begin_;
}

MMTN *eraseAtTheEnd_MMTN(MMT *t)
{
   if(t->size_ == 1)
      return eraseAsFirst_MMTN(t);
   else
   {
      MMTN *aux = seekPrevious_MMTN(*t, t->end_);
      if(eraseBetween_MMTN(t, aux))
         t->end_ = aux;
      return t->end_;
   }
}

MMTN *eraseActual_MMTN(MMT *t, MMTN* r)
{
   MMTN *aux = seekPrevious_MMTN(*t, r);

   if(t->begin_ == r)
      return eraseAtTheBeginning_MMTN(t);

   if(t->end_ == r)
      return eraseAtTheEnd_MMTN(t);

   if( eraseBetween_MMTN(t, aux) )
      return aux->next_;
   else
      return aux;
}

void eraseExceptOS_MMT(MMT *t)
{
   MMTN *aux = t->os_;

   while(t->size_ > osFrames() && aux != NULL)
      aux = eraseActual_MMTN(t, aux);
}

void vacateExceptOS_MMT(MMT *t)
{
   MMTN *aux = t->os_;
   for(; aux != NULL; aux = aux->next_)
      aux->row_.occupied_ = false;

}

void print_headers_MMT(void)
{
   size_t i = 0;
   for(; i <mmt_nHeaders; ++i)
      print_underscore_2Sp(mmt_headers[i], mmt_spaces[i], mmt_spaces[i]);
}

void print_headers_MMT_Ln(void)
{
   print_headers_MMT();
   printf("\n");
}

void print_MMT(const MMT t)
{
#if PRINT_MMT == 1
   MMTN *aux = t.begin_;
   print_headers_MMT_Ln();
   if(t.size_ == 0)
      printf("%*.*sempty\n", mmt_spaces[0], mmt_spaces[0], "");
   for(; aux  != NULL; aux = aux->next_)
      print_MMTN_Ln(aux);
#else
   (void) t;
#endif
}

void print_MMT_Ln(const MMT t)
{
   print_MMT(t);
#if PRINT_MMT == 1
   printf("\n");
#endif
}

#endif /* MemoryMapTable*/
