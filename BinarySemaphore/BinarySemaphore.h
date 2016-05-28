#ifndef BINARYSEMAPHORE_H
#define BINARYSEMAPHORE_H

/*
======================================================================
Includes
======================================================================
*/
#include "Core/Console.h"
#include "QUEUE/Queue.h"
#include "ProgramSettings.h"

/*
======================================================================
Macros
======================================================================
*/
#define binSem_nHeaders1   4
#define binSem_max_width1  16

#define binSem_nHeaders2   2
#define binSem_max_width2  7

#define binSem_message_L       128

/*
======================================================================
Data
======================================================================
*/
char binSem_headers1[binSem_nHeaders1][binSem_max_width1+1] =
{
   "Time",/*4*/
   "Processes status",/*16*/
   "Semaphore",/*9*/
   "Process in"/*7*/
};
size_t binSem_spaces1[binSem_nHeaders1] =
{
   1, 1, 1, 1
};

char binSem_headers2[binSem_nHeaders1][binSem_max_width2+1] =
{
   "Cri Sec",/*7*/
   "Queue"/*5*/
};
size_t binSem_spaces2[binSem_nHeaders1] =
{
   1, 1
};

size_t binSem_lengths1[binSem_nHeaders1], binSem_lengths2[binSem_nHeaders1];

typedef enum BinSemStatus
{
   Invalid,
   Waiting,
   Wait,
   Signal,
   CriSec,
   OtherOper
} BinSemStatus;

size_t timeBS;
int semaphore;
PCBN *criticalSection;
Queue queue;
BinSemStatus binSemOper;
char **binSem_message;
size_t binSem_message_N;

/*
======================================================================
Functions declaration
======================================================================
*/
void binary_semaphore_init(void);
void binary_semaphore_exit(void);

void print_BinSem_Header(void);
void print_BinSem_Header_Ln(void);
void print_BinSem(void);
void print_BinSem_Ln(void);

void attend_CriticalSection(void);
void signal(void);
int wait(void);
void binary_semaphore(PCBN *pcb);

/*
======================================================================
Functions implementation
======================================================================
*/
void binary_semaphore_init(void)
{
#if EXE_BINSEM == 1
   size_t i = 0;
   for(i = 0; i < binSem_nHeaders1; ++i)
      binSem_lengths1[i] = strlen(binSem_headers1[i]);
   for(i = 0; i < binSem_nHeaders2; ++i)
      binSem_lengths2[i] = strlen(binSem_headers2[i]);

   timeBS = 0;
   semaphore = 1;
   criticalSection = NULL;
   initialize_Queue(&queue);
   binSemOper = Invalid;
   binSem_message = NULL;
   binSem_message_N = 0;
#endif
}

void binary_semaphore_exit(void)
{
#if EXE_BINSEM == 1
   erase_Queue(&queue);
   deallocate(&binSem_message);
#endif
}

void print_BinSem_Header(void)
{
#if EXE_BINSEM == 1
   size_t i = 0;
   for(i = 0; i < binSem_nHeaders1; ++i)
      print_underscore_2Sp(binSem_headers1[i], binSem_spaces1[i],
                           binSem_spaces1[i]);

   newLine(1);
   i = binSem_lengths1[0] + 2*binSem_spaces1[0]
       + binSem_lengths1[1] + 2*binSem_spaces1[1];
   print_string("", i);
   print_string_2Sp("1", binSem_lengths1[2], binSem_spaces1[2],
         binSem_spaces1[2]);

   for(i = 0; i < binSem_nHeaders2; ++i)
      print_underscore_2Sp(binSem_headers2[i], binSem_spaces2[i],
                           binSem_spaces2[i]);
#endif
}

void print_BinSem_Header_Ln(void)
{
#if EXE_BINSEM == 1
   print_BinSem_Header();
   printf("\n");
#endif
}

void print_BinSem(void)
{
#if EXE_BINSEM == 1
   size_t idx = 0, aux = 0;
   QueueNode *node = queue.begin_;
   char string[binSem_message_L] = "";

   ++binSem_message_N;
   binSem_message = (char **)
                realloc(binSem_message, binSem_message_N*sizeof(char *));
   binSem_message[binSem_message_N - 1] = (char *)
                calloc(binSem_message_L, sizeof(char));

   print_BinSem_Header_Ln();

   sprintf(string, "%s%*.*s", string, binSem_spaces1[idx], binSem_spaces1[idx],
           "");
   sprintf(string, "%s%*u", string, binSem_lengths1[idx], timeBS);
   sprintf(string, "%s%*.*s", string, binSem_spaces1[idx], binSem_spaces1[idx],
           "");
   ++idx;

   sprintf(string, "%s%*.*s", string,
           binSem_lengths1[idx] + 2*binSem_spaces1[idx],
           binSem_lengths1[idx] + 2*binSem_spaces1[idx], "");
   ++idx;

   sprintf(string, "%s%*.*s", string, binSem_spaces1[idx], binSem_spaces1[idx],
           "");
   sprintf(string, "%s%*i", string, binSem_lengths1[idx], semaphore);
   sprintf(string, "%s%*.*s", string, binSem_spaces1[idx], binSem_spaces1[idx],
           "");
   ++idx;

   aux = (criticalSection) ?
            (criticalSection->row_.id_.value_) :
            (-1);
   idx = 0;
   sprintf(string, "%s%*.*s", string, binSem_spaces2[idx], binSem_spaces2[idx],
           "");
   sprintf(string, "%s%*i", string, binSem_lengths2[idx], aux);
   sprintf(string, "%s%*.*s", string, binSem_spaces2[idx], binSem_spaces2[idx],
           "");
   ++idx;

   sprintf(string, "%s%*.*s", string, queue_space, queue_space, "");
   for(; node != NULL; node = node->next_)

   sprintf(string, "%s%*.*s", string, queue_space, queue_space, "");
   print_Queue(queue);
#endif
}

void print_BinSem_Ln(void)
{
#if EXE_BINSEM == 1
   print_BinSem();
   printf("\n");
#endif
}

void attend_CriticalSection(void)
{
#if EXE_BINSEM == 1
   binSemOper = CriSec;
   semaphore = 0;
   --criticalSection->row_.cycles_.value_;
   --criticalSection->row_.durOfCriSec_.value_;
#endif
}

void signal(void)
{
#if EXE_BINSEM == 1
   binSemOper = Signal;
   semaphore = 1;
#endif
}

int wait(void)
{
#if EXE_BINSEM == 1
   binSemOper = Wait;
   if(semaphore >= 0)
      return --semaphore;
   else
      return semaphore;
#else
   return -2;
#endif
}

void binary_semaphore(PCBN *pcb)
{
#if EXE_BINSEM == 1
   if(!p && !criticalSection && queue.size_ == 0)
   {
      binSemOper = Invalid;
      return;
   }

   if(!p)
   {
      if(criticalSection)
      {
         if(criticalSection->row_.durOfCriSec_.value_ > 0)
            attend_CriticalSection();
         else
         {
            signal();
            criticalSection->row_.status_.value_ = 2;
            criticalSection = NULL;
         }
      }
      else
         binary_semaphore(pop(&queue));
   }
   else
   {
      p->row_.status_.value_ = 4;
      if(wait() == 0)
         criticalSection = p;
      else
         push(&queue, p);
   }
   print_BinSem_Ln();
   pause_Ln(1);
   ++timeBS;
#else
   (void) pcb;
#endif
}

#endif /*BINARYSEMAPHORE_H*/
