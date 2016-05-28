#ifndef CPUMANAGER_H
#define CPUMANAGER_H

/*
======================================================================
Includes
======================================================================
*/
#include "ProgramSettings.h"
#include "Core/Array.h"
#include "PCB/ProcessControlBlockTable.h"
#include "MMT/MemoryMapTable.h"
#include "BinarySemaphore.h"

/*
======================================================================
Data
======================================================================
*/
typedef enum ProcessPlanningAlgorithm
{
   FCFS,
   SJF,
   RoundRobin
} PlanningAlgorithm;

const PlanningAlgorithm algorithm = FCFS;
array_type_t *returnTime, *executionTime, *cycleExe;
number_type_t totalProcesses, timeMs, quantum, nQuantum, nRound;
/*
======================================================================
Function Declarations
======================================================================
*/
void cpuManager_init(void);
void deallocate_analysisData(void);
void cpuManager_exit(void);

size_t standbyTime(const size_t i);
double averageReturnTime(void);

void print_standbyTime(void);
void print_standbyTime_Ln(void);

void loadIntoMemory(PCBT *t, MMT *m);
void cycles_print(const PCBT t, PCBN *aux);
void print_analysisData(void);
void execute_cycles(const PCBT t, PCBN **aux);
void planning_execute(const PCBT t);
void load(PCBT *t, MMT *m);
void unload(PCBT *t, MMT *m);
void final_results(void);
void planning_initialize(PCBT *t);
void planningProcesses(PCBT *t, MMT *m);

/*
======================================================================
Function implementations
======================================================================
*/

void cpuManager_init(void)
{
   binary_semaphore_init();
   returnTime = NULL;
   executionTime = NULL;
   totalProcesses = 0;
   timeMs = 0;
   quantum = 4;
   nRound = 1;
   cycleExe = NULL;
}

void deallocate_analysisData(void)
{
   deallocate(&returnTime);
   deallocate(&executionTime);
   deallocate(&cycleExe);
}

void cpuManager_exit(void)
{
   deallocate_analysisData();
   binary_semaphore_exit();
}

size_t standbyTime(const size_t i)
{
   if(i > 0u)
      return returnTime[i-1];
   else
      return 0u;
}

double averageReturnTime(void)
{
   size_t trp = 0;
   int i = 0;
   for(; i < totalProcesses; ++i)
      trp += returnTime[i];
   return ((double)trp)/((double)totalProcesses);
}

void print_standbyTime(void)
{
   int i = 0;
   for(; i < totalProcesses; ++i)
      printf("%u ", standbyTime(i));
}

void print_standbyTime_Ln(void)
{
   print_standbyTime();
   newLine(1);
}

void loadIntoMemory(PCBT *t, MMT *m)
{
   PCBN *aux = t->begin_;
   for(; aux != NULL; aux = aux->next_)
      if(aux->row_.status_.value_ == 0)
      {
         MMTN *mN = seekAvailable_MMTN(m->begin_);
         if(mN)
         {
            mN->row_.occupied_ = true;
            aux->row_.status_.value_ = 2;
         }
      }
}

void cycles_print(const PCBT t, PCBN *aux)
{
   if(algorithm == RoundRobin)
   {
      printf("Round ");
      setConsoleTextColor(text_blue);
      printf("%d", nRound);
      setConsoleTextColor(default_text);
      printf("...\t");
   }

   printf("Attending to process ");
   setConsoleTextColor(text_green);
   print_number(aux->row_.id_);
   setConsoleTextColor(default_text);

   printf(", current cycle ");
   setConsoleTextColor(text_green);
   printf("%d", cycleExe[aux->row_.id_.value_]);
   setConsoleTextColor(default_text);

   printf(", remaining cycles ");
   setConsoleTextColor(text_green);
   print_number(aux->row_.cycles_);
   setConsoleTextColor(default_text);

   printf(", time: ");
   setConsoleTextColor(text_green);
   printf("%d", timeMs);
   setConsoleTextColor(default_text);

   printf("...\n");
   print_PCBT_Ln(t);
}

void print_analysisData(void)
{
   print_color("executionTime:\n\t", text_green);
   print_Array(executionTime, totalProcesses);
   print_color(" ms\n", text_blue);

   print_color("returnTime:\n\t", text_green);
   print_Array(returnTime, totalProcesses);
   print_color(" ms\n", text_blue);

   print_color("standbyTime:\n\t", text_green);
   print_standbyTime();
   print_color(" ms\n", text_blue);

   print_color("averageReturnTime:\n\t", text_green);
   printf("%f", averageReturnTime());
   print_color(" ms\n", text_blue);

   print_color("Total time of execution:\n\t", text_green);
   printf("%u", timeMs);
   print_color(" ms\n", text_blue);
}

void execute_cycle_PCBN(const PCBT t, PCBN *aux)
{
   if(algorithm == RoundRobin)
      --nQuantum;
   ++timeMs;
   cycles_print(t, aux);
}

void execute_cycle_BinSem(PCBN **aux)
{
#if EXE_BINSEM == 1
   binary_semaphore(NULL);
   while(*aux != NULL && (*aux)->row_.durOfCriSec_.value_ > 0
         && (*aux)->row_.startOfCriSec_.value_
         == cycleExe[(*aux)->row_.id_.value_])
   {
      binary_semaphore(*aux);
      *aux = (*aux)->next_;
   }
#else
   (void) aux;
#endif
}

void execute_cycles_init(PCBN *aux)
{
   clrscr();
   nQuantum = quantum;
   /*Updates the execution time for aux*/
   if(algorithm == RoundRobin && nRound == 1)
      executionTime[aux->row_.id_.value_] = timeMs;
   else
      executionTime[aux->row_.id_.value_] = timeMs;
}

void execute_cycles_exit(PCBN *aux)
{
   if(aux->row_.cycles_.value_ == 0)
   {
      aux->row_.status_.value_ = 5;
      returnTime[aux->row_.id_.value_] = timeMs;
      print_analysisData();
   }
   pause_Ln(0);
}

void execute_cycles(const PCBT t, PCBN **aux)
{
   execute_cycles_init(*aux);
   for(; (*aux)->row_.cycles_.value_ > 0 && nQuantum > 0;
       --(*aux)->row_.cycles_.value_, ++cycleExe[(*aux)->row_.id_.value_])
   {
      execute_cycle_PCBN(t, *aux);
      execute_cycle_BinSem(aux);
   }
   execute_cycles_exit(*aux);
}

void planning_execute(const PCBT t)
{
   PCBN *aux = t.begin_;
   bool b = true;
   for(; b; ++nRound)
   {
      b = false;
      aux = t.begin_;
      for(; aux; aux = aux->next_)
         if(aux->row_.status_.value_ == 2)
         {
            execute_cycles(t, &aux);
            b = true;
         }
   }
}

void load(PCBT *t, MMT *m)
{
   clrscr();
   print_color("Loading Processes into MMT...\n", text_green);
   loadIntoMemory(t, m);
   print_PCBT_Ln(*t);
   print_MMT_Ln(*m);
   pause_Ln(0);
}

void unload(PCBT *t, MMT *m)
{
   clrscr();
   print_color("Erasing processes in state 5...\n", text_green);
   eraseOutgoing_PCBT(t);
   vacateExceptOS_MMT(m);
   print_PCBT_Ln(*t);
   print_MMT_Ln(*m);
   pause_Ln(0);
}

void final_results(void)
{
   clrscr();
   print_color("Final results:\n", text_blue);
   print_analysisData();
   deallocate_analysisData();
   pause_Ln(1);
}

void planning_initialize(PCBT *t)
{
   totalProcesses = t->size_;
   returnTime = allocate_array(totalProcesses);
   executionTime = allocate_array(totalProcesses);
   cycleExe = allocate_array(totalProcesses);
   timeMs = 0;
   switch (algorithm)
   {
      case FCFS: break;
      case SJF: sort_PCBT(t, Ascendant, cycles_); break;
      case RoundRobin: sort_PCBT(t, Ascendant, sizeInMemory_); break;
      default: break;
   }
}

void planningProcesses(PCBT *t, MMT *m)
{
   planning_initialize(t);
   while(t->size_ > 0)
   {
      load(t, m);
      planning_execute(*t);
      unload(t, m);
   }
   final_results();
}

#endif /*CPUMANAGER_H*/
