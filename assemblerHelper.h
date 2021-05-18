// Project: cs140 assembler
// Instructor: John Magee
// Students: Han Choi & Enbang Wu

#ifndef ASSEMBLER_HELPER
#define ASSEMBLER_HELPER

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "map.h"

/***********************************
 Documentation of assemblerHelper.c
 ***********************************/

/***************************************************
 This function handles the task of removing any whitespace
 or comments in the asm file.
 Implementation example:

char * str = "Hello world // test comment";
clean (str);

returns "Helloworld"
****************************************************/
int clean(char *str);

/****************************************************
 Helper function for pass.c (first pass)
 - receives a linebuffer and returns 1 if A/C COMMANDS, 
   0 otherwise
 - If an L_COMMAND, translates the linebuffer. 
   Otherwise (A/C commands), increment the count and do nothing
 * **************************************************/
int first_pass_line(char *str);

/*****************************************************
 Helper function for pass.c (second pass)
 - receives a linebuffer and returns 1 if input str is an 
   L command, 0 otherwise
 - If an A/C commands, translates the linebuffer. 
   Otherwise (L command), increment the count and do nothing
 * ***************************************************/
int second_pass_line(char *str);
#endif
