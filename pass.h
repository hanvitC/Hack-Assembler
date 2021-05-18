// Project: cs140 pass
// Instructor: John Magee
// Students: Han Choi & Enbang Wu

#ifndef PASS
#define PASS

#include <stdio.h>
#include <stdlib.h>
#include "assemblerHelper.h"
#include "map.h"

/**************************
 Documentation of pass.c 
 ***************************/

/*********************************************************************************
 First pass
 :receives the input file from assembler.c and saves the symbols in the 
 input file into the symbol table

Details
 1. Read the file line-by-line (ignore the whitespace and comments)
 2.  Ignore all the A_COMMANDS and C_COMMANDS
     COUNT all the lines as we go: start at 0 and increment the count only if A/C 
 3. Translate L_COMMANDS
	Insert a key-value pair our symbol table: pair(label, next line number)

At this point: symbol table contains the pre-defined symbols + the labels, but NOT
yet the variables such as @i
 * *******************************************************************************/
void first_pass(FILE *in);

/*********************************************************************************
 Second pass
 :receives the input file from assembler.c and translates each command in the file
 and prints it into a output file of machine codes associated with each command 
  
Details
 1. Read the file line-by-line (ignore the whitespace and comments)
 2. Ignore all the L_COMMANDS 
 3. Translate A_COMMANDS or C_COMMANDS into machine codes
 4. print each parsed line into the output file
 * *******************************************************************************/
void second_pass(FILE *in, FILE *out);

/*********************************************************************************
 Construction of maps
 Three maps
 1. Comp map
 2. Dest/Jump map
 3. Symbol map
 * *******************************************************************************/
map createComp();
map createDestJump();
map createSymbols();
#endif