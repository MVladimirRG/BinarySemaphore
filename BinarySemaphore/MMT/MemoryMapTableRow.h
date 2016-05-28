#ifndef MemoryMapTableROW
#define MemoryMapTableROW

/*
======================================================================
Includes
======================================================================
*/
#include "MMT/MemoryMapTableCore.h"

/*
======================================================================
Data
======================================================================
*/
typedef struct
{
   size_t id_;
   Number begin_;
   bool occupied_;
} MMTR;

/*
======================================================================
Function Declarations
======================================================================
*/
void print_MMTR(const MMTR r);
void print_MMTR_Ln(const MMTR r);

/*
======================================================================
Function Implementations
======================================================================
*/
void print_MMTR(const MMTR r)
{
   size_t idx = 0;

   print_string("", mmt_spaces[idx]);
   printf("%*u", mmt_lengths[idx], r.id_);
   print_string("", mmt_spaces[idx]);
   ++idx;

   print_number_Width_2Sp(r.begin_, mmt_lengths[idx], mmt_spaces[idx],
                          mmt_spaces[idx]);
   ++idx;

   print_string("", mmt_spaces[idx]);
   if(r.occupied_)
      print_string("yes", mmt_lengths[idx]);
   else
      print_string("no", mmt_lengths[idx]);
   print_string("", mmt_spaces[idx]);
   ++idx;
   (void) idx;
}

void print_MMTR_Ln(const MMTR r)
{
   print_MMTR(r);
   printf("\n");
}

#endif /* MemoryMapTableROW*/
