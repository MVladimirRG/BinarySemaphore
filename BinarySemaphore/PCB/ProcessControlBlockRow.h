#ifndef PROCESSCONTROLBLOCKROW_H
#define PROCESSCONTROLBLOCKROW_H

/*
======================================================================
Includes
======================================================================
*/
#include "PCB/ProcessControlBlockCore.h"
#include "Core/Core.h"

/*
======================================================================
Data
======================================================================
*/
typedef struct ProcessControlBlockStructure
{
   Number
   id_,
   taskId_,
   page_,
   arrivingTime_,

   cycles_,
   status_,
   ioDevices_,
   files_,

   processType_,
   sizeInMemory_,
   startOfCriSec_,
   durOfCriSec_;
} PCB;

typedef enum
{
   id_,
   taskId_,
   page_,
   arrivingTime_,

   cycles_,
   status_,
   ioDevices_,
   files_,

   processType_,
   sizeInMemory_,
   startOfCriSec_,
   durOfCriSec_,
   wholePCB_
} PCBAttribute;

/*
======================================================================
Function Declarations
======================================================================
*/
void print_PCB(const PCB pcb);
void print_PCB_Ln(const PCB pcb);

/*
======================================================================
Function Implementations
======================================================================
*/
Number getAttribute_PCB(const PCB pcb, PCBAttribute attribute)
{
   switch (attribute)
   {
      case id_: return pcb.id_;
      case taskId_: return pcb.taskId_;
      case page_: return pcb.page_;
      case arrivingTime_: return pcb.arrivingTime_;
      case cycles_: return pcb.cycles_;
      case status_: return pcb.status_;
      case ioDevices_: return pcb.ioDevices_;
      case files_: return pcb.files_;
      case processType_: return pcb.processType_;
      case sizeInMemory_: return pcb.sizeInMemory_;
      case startOfCriSec_: return pcb.startOfCriSec_;
      case durOfCriSec_: return pcb.durOfCriSec_;
      case wholePCB_: return zeroNumber;
      default: return zeroNumber;
   }
}

bool compare_PCB(const PCB pcb1, PCBAttribute attribute1,
                 ComparisonType type,
                 const PCB pcb2, PCBAttribute attribute2)
{
   return compareNumber(getAttribute_PCB(pcb1, attribute1), type,
                        getAttribute_PCB(pcb2, attribute2));
}

void print_PCB(const PCB pcb)
{
   int i = 0;
   for(; i < pcb_nHeaders; ++i)
      print_number_Width_2Sp(getAttribute_PCB(pcb, i), pcb_lengths[i],
                             pcb_spaces[i], pcb_spaces[i]);
}

void print_PCB_Ln(const PCB pcb)
{
   print_PCB(pcb);
   printf("\n");
}

#endif /*PROCESSCONTROLBLOCKROW_H*/
