#ifndef CONSOLE_H
#define CONSOLE_H

/*
======================================================================
Includes
======================================================================
*/
#include <windows.h>
#include <stdio.h>
#include <stddef.h>
#include "Core/Borland.h"
#include "Core/Bool.h"
#include "ProgramSettings.h"

/*
======================================================================
Data
======================================================================
*/
typedef enum TextColor
{
   text_black,     /*0x0*//*0000*/
   text_navy_blue, /*0x1*//*0001*/
   text_green,     /*0x2*//*0010*/
   text_teal,      /*0x3*//*0011*/

   text_maroon,    /*0x4*//*0100*/
   text_purple,    /*0x5*//*0101*/
   text_olive,     /*0x6*//*0110*/
   text_silver,    /*0x7*//*0111*/

   text_gray,      /*0x8*//*1000*/
   text_blue,      /*0x9*//*1001*/
   text_lime,      /*0xA*//*1010*/
   text_aqua,      /*0xB*//*1011*/

   text_red,       /*0xC*//*1100*/
   text_fuchsia,   /*0xD*//*1101*/
   text_yellow,    /*0xE*//*1110*/
   text_white      /*0xF*//*1111*/
} TextColor;

typedef enum BackgroundColor
{
   backGround_black       = 0x00,  /*0000  0000*/
   backGround_navy_blue   = 0x10,  /*0001  0000*/
   backGround_green       = 0x20,  /*0010  0000*/
   backGround_teal        = 0x30,  /*0011  0000*/

   backGround_maroon      = 0x40,  /*0100  0000*/
   backGround_purple      = 0x50,  /*0101  0000*/
   backGround_olive       = 0x60,  /*0110  0000*/
   backGround_silver      = 0x70,  /*0111  0000*/

   backGround_gray         = 0x80,  /*1000  0000*/
   backGround_blue        = 0x90,  /*1001  0000*/
   backGround_lime        = 0xA0,  /*1010  0000*/
   backGround_aqua        = 0xB0,  /*1011  0000*/

   backGround_red         = 0xC0,  /*1100  0000*/
   backGround_fuchsia     = 0xD0,  /*1101  0000*/
   backGround_yellow      = 0xE0,  /*1110  0000*/
   backGround_white       = 0xF0   /*1111  0000*/
} BackgroundColor;


WORD default_text;
WORD default_backGround;
bool default_underscore;

/*
======================================================================
Function Declarations
======================================================================
*/
COORD Coord_(const SHORT x, const SHORT y);

WORD getConsoleTextAttributes(void);
COORD getCursorPosition(void);
void setCursorPosition(const COORD coordinate);

WORD getConsoleTextColor(void);
WORD getConsoleBackGroundColor(void);
bool getConsoleUnderscore(void);

void setConsoleTextColor(WORD textColor);
void setConsoleBkGndColor(WORD backGroundColor);
void setConsoleUnderscore(bool underscored);

void print_string(char *string, const size_t width);
void print_string_Sp(char *string, const size_t width,
                     const size_t rightSpaces);
void print_string_2Sp(char *string, const size_t width, const size_t leftSpaces,
                      const size_t rightSpaces);

void print_special_Width(char *message, bool underscored, WORD backGroundColor,
                         WORD textColor, const size_t width);
void print_special(char *message, bool underscored, WORD backGroundColor,
                   WORD textColor);

void print_underscore_Width_2Sp(char *message, const size_t width,
                                const size_t leftSpaces,
                                const size_t rightSpaces);
void print_underscore_Width(char *message, const size_t width);
void print_underscore_2Sp(char *message, const size_t leftSpaces,
                          const size_t rightSpaces);
void print_underscore(char *message);

void print_color_Width(char *message, WORD textColor, const size_t width);
void print_color(char *message, WORD textColor);

void pause_Ln(const size_t lines);
void newLine(const size_t lines);
void clrscr(void);

void console_init(void);

/*
======================================================================
Function Implementations
======================================================================
*/
COORD Coord_(const SHORT x, const SHORT y)
{
   COORD c;
   c.X = x;
   c.Y = y;
   return c;
}

COORD getCursorPosition(void)
{
   CONSOLE_SCREEN_BUFFER_INFO csbi;
   if(GetConsoleScreenBufferInfo(
            GetStdHandle(STD_OUTPUT_HANDLE),
            &csbi))
      return csbi.dwCursorPosition;
   else
      return Coord_(-1, -1);
}

void setCursorPosition(const COORD coordinate)
{
   SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinate);
}

WORD getConsoleTextAttributes(void)
{
   CONSOLE_SCREEN_BUFFER_INFO buffer;
   GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &buffer);
   return buffer.wAttributes;
}

WORD getConsoleTextColor(void)
{
   return getConsoleTextAttributes() & ((WORD) 0x000F);
}

WORD getConsoleBackGroundColor(void)
{
   return (WORD) (getConsoleTextAttributes() & 0x00F0);
}

bool getConsoleUnderscore(void)
{
#if !Borland502
   return (getConsoleTextAttributes() & 0xF000) == COMMON_LVB_UNDERSCORE;
#else
   return false;
#endif
}

void setConsoleTextColor(WORD textColor)
{
#if Borland502
   SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                           (WORD) (textColor + getConsoleBackGroundColor()) );
#else
   if(getConsoleUnderscore())
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                              textColor + getConsoleBackGroundColor()
                              + COMMON_LVB_UNDERSCORE);
   else
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                              textColor + getConsoleBackGroundColor());
#endif
}

void setConsoleBkGndColor(WORD backGroundColor)
{
#if Borland502
   SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                           (WORD) (getConsoleTextColor() + backGroundColor) );
#else
   if(getConsoleUnderscore())
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                              getConsoleTextColor() + backGroundColor
                              + COMMON_LVB_UNDERSCORE);
   else
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                              getConsoleTextColor() + backGroundColor);
#endif
}

void setConsoleUnderscore(bool underscored)
{
#if !Borland502
   BackgroundColor backGroundColor = getConsoleBackGroundColor();
   TextColor textColor = getConsoleTextColor();

   if(underscored)
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                              backGroundColor + textColor + COMMON_LVB_UNDERSCORE);
   else
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                              backGroundColor + textColor);
#else
   (void) underscored;
#endif
}


void print_string(char *string, const size_t width)
{
   printf("%*.*s", width, width, string);
}

void print_string_Sp(char *string, const size_t width, const size_t rightSpaces)
{
   print_string(string, width);
   print_string("", rightSpaces);
}

void print_string_2Sp(char *string, const size_t width,
                      const size_t leftSpaces, const size_t rightSpaces)
{
   print_string("", leftSpaces);
   print_string_Sp(string, width, rightSpaces);
}

void print_special_Width(char *message, bool underscored, WORD backGroundColor,
                         WORD textColor, const size_t width)
{
   setConsoleUnderscore(underscored);
   setConsoleBkGndColor(backGroundColor);
   setConsoleTextColor(textColor);

   print_string(message, width);

   setConsoleUnderscore(default_underscore);
   setConsoleBkGndColor(default_backGround);
   setConsoleTextColor(default_text);
}

void print_special(char *message, bool underscored, WORD backGroundColor,
                   WORD textColor)
{
   print_special_Width(message, underscored, backGroundColor, textColor,
                       strlen(message));
}

void print_underscore_Width_2Sp(char *message, const size_t width, const size_t l,
                                const size_t rightSpaces)
{
   print_string("", l);
   setConsoleUnderscore(true);
   print_string(message, width);
   setConsoleUnderscore(false);
   print_string("", rightSpaces);
}

void print_underscore_Width(char *message, const size_t width)
{
   print_underscore_Width_2Sp(message, width, 0, 0);
}

void print_underscore_2Sp(char *message, const size_t l, const size_t rightSpaces)
{
   print_underscore_Width_2Sp(message, strlen(message), l, rightSpaces);
}

void print_underscore(char *message)
{
   print_underscore_Width(message, strlen(message));
}

void print_color_Width(char *message, WORD textColor, const size_t width)
{
   print_special_Width(message, default_underscore, default_backGround, textColor, width);
}

void print_color(char *message, WORD textColor)
{
   print_color_Width(message, textColor, strlen(message));
}

void newLine(const size_t lines)
{
   size_t i = 0;
   for(; i < lines; ++i)
      printf("\n");
}

void pause_Ln(const size_t lines)
{
#if WAIT == 1
   newLine(lines);
   system("pause");
#else
   (void) total;
#endif
}

/**
   @brief clrscr clear the screen
 */
void clrscr(void)
{
   system("cls");
}

void console_init(void)
{
#if !Borland502
   default_underscore = getConsoleUnderscore();
#endif
   default_backGround = getConsoleBackGroundColor();
   default_text = getConsoleTextColor();
}

#endif /*CONSOLE_H*/
