#ifndef MemoryMapTableCORE
#define MemoryMapTableCORE

/*
======================================================================
Includes
======================================================================
*/
#include <stddef.h>

#include "Core/Core.h"

/*
======================================================================
Preprocessor
======================================================================
*/
#define mmt_nHeaders    3
#define mmt_max_width   8

/*
======================================================================
Data
======================================================================
*/
char mmt_headers[mmt_nHeaders][mmt_max_width+1] =
{
   "Id",
   "Begin",
   "Occupied"/*8*/
};
size_t mmt_spaces[mmt_nHeaders] =
{
   1,
   2,
   1
};
Number memSize, frameSize, osSize;
size_t mmt_lengths[mmt_nHeaders], lines;

/*
======================================================================
Function Declarations
======================================================================
*/
size_t memFrames(void);
size_t osFrames(void);
void mmt_init(void);

/*
======================================================================
Function Implementations
======================================================================
*/
size_t memFrames(void)
{
   return (size_t) ceil( ( (double) realValue(memSize) )
                         / ( (double) realValue(frameSize) ) );
}

size_t osFrames(void)
{
   return (size_t) ceil( ( (double) realValue(osSize) )
                          / ( (double) realValue(frameSize) ) );
}

void mmt_init(void)
{
   size_t i = 0;
   for(; i < mmt_nHeaders; ++i)
      mmt_lengths[i] = strlen(mmt_headers[i]);

   memSize = Number_(64, Kilo, Byte);
   frameSize = Number_(4, Kilo, Byte);
   osSize = Number_(8, Kilo, Byte);
   lines = 100;
}

#endif /* MemoryMapTableCORE*/
