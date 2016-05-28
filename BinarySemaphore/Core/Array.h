#ifndef ARRAYSIZE_T_H
#define ARRAYSIZE_T_H

/*
======================================================================
Includes
======================================================================
*/
#include <stddef.h>

/*
======================================================================
Data
======================================================================
*/
typedef int array_type_t;

/*
======================================================================
Function Declarations
======================================================================
*/
array_type_t *allocate_array(const size_t n);

void incrementAll(array_type_t *array, const size_t n);
void setValue_array(array_type_t *array, const size_t n, const size_t value);

int seekMaximum(const array_type_t *array, const size_t n);
int seekMinimumNotZero(const array_type_t *array, const size_t n);

void print_Array(array_type_t *array, size_t n);
void print_Array_Ln(array_type_t *array, size_t n);

/*
======================================================================
Function Implementations
======================================================================
*/
array_type_t *allocate_array(const size_t n)
{
   array_type_t *r = (array_type_t *) calloc(n, sizeof(size_t));/*all must be zero*/
   setValue_array(r, n, 0);
   return r;
}

void incrementAll(array_type_t *array, const size_t n)
{
   size_t i = 0;

   for(; i < n; ++i)
      ++ array[i];
}

void setValue_array(array_type_t *array, const size_t n, const size_t value)
{
   size_t i = 0;

   for(; i < n; ++i)
      array[i] = value;
}

/**
   @brief seekMaximum for seek the index of maximum item.
   @param array
   @param n as minum is greater than 1
   @return the index of maximum item.
 */
int seekMaximum(const array_type_t *array, const size_t n)
{
   size_t maximum = 0, i = 1;

   if(n <= 1)
      return 0;

   for(; i < n; ++i)
      if(array[i] > array[maximum])
         maximum = i;
   return maximum;
}

/**
   @brief seekMaximum for seek the index of maximum item.
   @param array
   @param n as minum is greater than 1
   @return the index of maximum item.
 */
int seekMinimumNotZero(const array_type_t *array, const size_t n)
{
   size_t minimum = 0, i = 1;

   if(n <= 1)
      return 0;

   for(; i < n; ++i)
   {
      if(array[minimum] == 0 && array[i] > 0)
         minimum = i;
      if(array[i] > 0 && array[i] < array[minimum])
         minimum = i;
   }
   return minimum;
}

void print_Array(array_type_t *array, size_t n)
{
   size_t i = 0;

   for(; i < n; ++i)
      printf("%u ", array[i]);
}

void print_Array_Ln(array_type_t *array, size_t n)
{
   print_Array(array, n);
   printf("\n");
}

#endif /*ARRAYSIZE_T_H*/
