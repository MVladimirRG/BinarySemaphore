#ifndef COREDATA_H
#define COREDATA_H

/*
======================================================================
Includes
======================================================================
*/
#include "Core/Bool.h"
#include "Core/CoreFunctions.h"
#include "Core/Console.h"
#include <stdio.h>
#include <stddef.h>
#include <string.h>

/*
======================================================================
Data
======================================================================
*/
typedef enum SortingType
{
   Ascendant, Descendant
} SortingType;

typedef enum Prefix
{
   Yocto, Zepto, Atto, Femto,
   Pico, Nano, Micro, Mili, Unit,/*8*/
   Kilo, Mega, Giga, Tera,
   Peta, Exa, Zetta, Yotta
} Prefix;

typedef enum MeasurementUnit
{
   Dimensionless, Bit, Byte, Second
} MeasurementUnit;

typedef short number_type_t;/*size = 2*/

typedef struct Number
{
   number_type_t value_;
   u_char prefix_, unit_;
   /*size = sizeof(number_type_t) + 1 + 1*/
} Number;

typedef enum
{
   value_, prefix_, unit_
}NumberAttribute;

const Number zeroNumber = {0, Unit, Dimensionless};

/*
======================================================================
Function Declarations
======================================================================
*/
bool compareNumber(const Number number1, const ComparisonType comparisonType,
                   const Number number2);
Number Number_(number_type_t value, const u_char prefix, const u_char unit);
void getAttribute_Number(const Number number, const NumberAttribute attribute,
                         ...);
double realValue(const Number number);

void getWholePrefix(const Prefix prefix, char *destination);
char getPrefix(const Prefix prefix);

void getWholeMeasurementUnit(const MeasurementUnit measurementUnit, char *destination);
char getMeasurementUnit(const MeasurementUnit m);

void print_number_Width_2Sp(const Number number, size_t width,
                            const size_t leftSpaces, const size_t rightSpaces);
void print_number_Width_Sp(const Number m, const size_t width,
                           const size_t leftSpaces);
void print_number_Width(const Number m, const size_t width);
void print_number(const Number m);

void getAttribute_Number(const Number n, const NumberAttribute attribute, ...);

void getWholePrefix(const Prefix p, char *dest);
char getPrefix(const Prefix p);

void getWholeMeasurementUnit(const MeasurementUnit m, char *dest);
char getMeasurementUnit(const MeasurementUnit m);

void dataTypes_init(void);
void dataTypes_exit(void);

/*
======================================================================
Function Implementations
======================================================================
*/
bool compareNumber(const Number number1, const ComparisonType comparisonType,
                   const Number number2)
{
   return compareValue(realValue(number1), comparisonType, realValue(number2));
}

Number Number_(number_type_t value, const u_char prefix, const u_char unit)
{
   Number n;
   n.value_ = value; n.prefix_ = prefix; n.unit_ = unit;
   return n;
}

double realValue(const Number number)
{
   double factor = 1, value = (double) number.value_, base = 10,
         coeficientExp = 3, exponent;
   if(number.unit_ == Byte)
      factor = 8;
   if(number.unit_ == Bit || number.unit_ == Byte)
   {
      base = 2;
      coeficientExp = 10;
   }
   exponent =  ((double) number.prefix_ - Unit)*coeficientExp;
   return factor * value * pow(base, exponent);
}

void print_number_Width_2Sp(const Number number, size_t width,
                            const size_t leftSpaces, const size_t rightSpaces)
{
   char string[16], p = getPrefix(number.prefix_), u = getMeasurementUnit(number.unit_), m = ' ';

   if(p == ' ' && u == ' ')
      m = '\0';
   if(p == ' ')
      p = '\0';
   if(u == ' ')
      u = '\0';

   sprintf(string, "%i%c%c%c", number.value_, m, p, u);
   if(width == 0)
      width = strlen(string);
   print_string_2Sp(string, width, leftSpaces, rightSpaces);
}

void print_number_Width_Sp(const Number m, const size_t width,
                           const size_t leftSpaces)
{
   print_number_Width_2Sp(m, width, 0, leftSpaces);
}

void print_number_Width(const Number m, const size_t width)
{
   print_number_Width_Sp(m, width, 0);
}

void print_number(const Number m)
{
   print_number_Width(m, 0);
}

void getAttribute_Number(const Number number, const NumberAttribute attribute,
                         ...)
{
   va_list args;
   va_start(args, attribute);
   switch (attribute)
   {
      case value_:
      {
         number_type_t *p = va_arg(args, number_type_t*);
         *p = number.value_;
      }break;
      case prefix_:
      {
         u_char *p = va_arg(args, u_char*);
         *p = number.prefix_;
      }break;
      case unit_:
      {
         u_char *p = va_arg(args, u_char*);
         *p = number.unit_;
      }break;
      default: break;
   }
}

void getWholePrefix(const Prefix prefix, char *destination)
{
   switch (prefix)
   {
      case Yocto: strcpy(destination, "Yocto"); break;
      case Zepto: strcpy(destination, "Zepto"); break;
      case Atto: strcpy(destination, "Atto"); break;
      case Femto: strcpy(destination, "Femto"); break;
      case Pico: strcpy(destination, "Pico"); break;
      case Nano: strcpy(destination, "Nano"); break;
      case Micro: strcpy(destination, "uMicro"); break;
      case Mili: strcpy(destination, "Mili"); break;
      case Unit: strcpy(destination, " "); break;
      case Kilo: strcpy(destination, "Kilo"); break;
      case Mega: strcpy(destination, "Mega"); break;
      case Giga: strcpy(destination, "Giga"); break;
      case Tera: strcpy(destination, "Tera"); break;
      case Peta: strcpy(destination, "Peta"); break;
      case Exa: strcpy(destination, "Exa"); break;
      case Zetta: strcpy(destination, "Zetta"); break;
      case Yotta: strcpy(destination, "Yotta"); break;
      default: strcpy(destination, " "); break;
   }
}

char getPrefix(const Prefix prefix)
{
   char buffer[16], c;
   getWholePrefix(prefix, buffer);
   c = buffer[0];
   return (prefix < Unit) ?
            (char) tolower(c) : (char) toupper(c);
}

void getWholeMeasurementUnit(const MeasurementUnit measurementUnit,
                             char *destination)
{
   switch(measurementUnit)
   {
      case Dimensionless: strcpy(destination, " "); break;
      case Bit: strcpy(destination, "bits"); break;
      case Byte: strcpy(destination, "Bytes"); break;
      case Second: strcpy(destination, "seconds"); break;
      default: strcpy(destination, " "); break;
   }
}

char getMeasurementUnit(const MeasurementUnit m)
{
   char buffer[16];
   getWholeMeasurementUnit(m, buffer);
   return buffer[0];
}

#endif /*COREDATA_H*/
