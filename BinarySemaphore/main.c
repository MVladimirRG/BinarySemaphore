#include <stdio.h>

#include "Core/Core.h"
#include "MMT/MemoryMapTable.h"
#include "CPUManager.h"

int main()
{
   MMT m;
   PCBT t;

   core_init();
   mmt_init();
   pcb_init();
   cpuManager_init();

   initialize_MMT(&m);
   initialize_PCBT(&t);

   planningProcesses(&t, &m);

   erase_PCBT(&t);
   erase_MMT(&m);

   cpuManager_exit();
   core_exit();
   return 0;
}
