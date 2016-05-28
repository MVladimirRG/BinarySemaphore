#ifndef BOOL_H
#define BOOL_H

/*
======================================================================
Includes
======================================================================
*/
#include "Core/Borland.h"

/*
======================================================================
Data
======================================================================
*/
#if !defined(__cplusplus)
#if Borland502
typedef enum BoolEnum
{
   false,
   true
} bool;
#else/*Not Using Borland 5.02*/
#include <stdbool.h>
#endif /*Borland 5.02*/
#endif /*__STDC__*/


#endif /*BOOL_H*/
