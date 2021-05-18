// Project: cs140 pass
// Instructor: John Magee
// Students: Han Choi & Enbang Wu

#include <stdio.h>
#include <stdlib.h>
#include "assemblerHelper.h"
#include "map.h"

#define MAX_LINE_LENGTH 200

///////////////////////////////////////////////////////////////////////////////////////////////////
//GLOBAL VARIABLES

//1. maps (symbol tables)
map createComp();
map createDestJump();
map createSymbols();

map compMap;
map destjmpMap;
map symbolMap;

// 2. next line: keeps track of the next line number in an input file
int next_line = 0;

// 3. linebuffer: an array of characters that holds a string of each line
char linebuffer[MAX_LINE_LENGTH + 1]; // since we don't dynamically treat string like we do in JAVA
////////////////////////////////////////////////////////////////////////////////////////////////////

// First pass of parsing file: description in details in pass.h
void first_pass(FILE *in)
{
    // initialize/construct Maps(symbol table) for handling the commands
    symbolMap = createSymbols();

    // getting the first line from the input file before entering while loop
    // prevents the error looping for once more than needed
    fgets(linebuffer, MAX_LINE_LENGTH, in);

    while (!feof(in)) // Inside the input file
    {
        int cleaned = clean(linebuffer);          // 1. remove space and comments
        next_line += first_pass_line(linebuffer); // 2. increment instr_count only if A/C commands
        fgets(linebuffer, MAX_LINE_LENGTH, in);   // 3. get a new line
    }
    fseek(in, 0, SEEK_SET); // after accessing the file for once, move
                            // the pointer of input file back to the beginning
}

// Second pass of parsing file: description in details in pass.h
void second_pass(FILE *in, FILE *out)
{
    // initialize/construct Maps(symbol table) for handling the commands
    compMap = createComp();
    destjmpMap = createDestJump();

    // getting the first line in the input file before entering while loop
    fgets(linebuffer, MAX_LINE_LENGTH, in);
    while (!feof(in)) // Inside the input file
    {
        // 1. remove space and comments
        int cleaned = clean(linebuffer); // cleaned == 1: the clean line is NOT empty

        // 2. process the line received
        int isLcommand = second_pass_line(linebuffer); // isLcommand == 1: the command is an L command
        if (isLcommand != 1)
        {
            fputs(linebuffer, out); // print in output file only if the parsed line is NOT an L command
        }

        // 3. get the next line in the input file
        char *next = fgets(linebuffer, MAX_LINE_LENGTH, in);

        // 4. mannually put a new line character at the end of each linebuffer
        // print the new line char at the end of linebuffer only if all conditions satisfied
        // parsed line is NOT empty, NOT an L command, AND the next line is NOT null (no extra new line character in the last line)
        if (isLcommand != 1 && cleaned == 1 && next != NULL)
            fputs("\n", out);
    }

    // deallocate the memories dynamically allocated in map structure and the map itself
    freeMap(symbolMap);
    freeMap(compMap);
    freeMap(destjmpMap);
}

map createComp()
{
    map mymap = createMap(2000);
    // these are the comp destinations
    insertKey(mymap, "0", "0101010");
    insertKey(mymap, "1", "0111111");
    insertKey(mymap, "-1", "0111010");
    insertKey(mymap, "D", "0001100");
    insertKey(mymap, "A", "0110000"); // if a = 0
    insertKey(mymap, "M", "1110000"); // if a = 1
    insertKey(mymap, "!D", "0001101");
    insertKey(mymap, "!A", "0110001");
    insertKey(mymap, "!M", "1110001");
    insertKey(mymap, "-D", "0001111");
    insertKey(mymap, "-A", "0110011");
    insertKey(mymap, "-M", "1110011");
    insertKey(mymap, "D+1", "0011111");
    insertKey(mymap, "A+1", "0110111");
    insertKey(mymap, "M+1", "1110111");
    insertKey(mymap, "D-1", "0001110");
    insertKey(mymap, "A-1", "0110010");
    insertKey(mymap, "M-1", "1110010");
    insertKey(mymap, "D+A", "0000010");
    insertKey(mymap, "D+M", "1000010");
    insertKey(mymap, "D-A", "0010011");
    insertKey(mymap, "D-M", "1010011");
    insertKey(mymap, "A-D", "0000111");
    insertKey(mymap, "M-D", "1000111");
    insertKey(mymap, "D&A", "0000000");
    insertKey(mymap, "D&M", "1000000");
    insertKey(mymap, "D|A", "0010101");
    insertKey(mymap, "D|M", "1010101");

    return mymap;
}

map createDestJump()
{
    // dest bits
    map mymap = createMap(2000);
    insertKey(mymap, "null", "000");
    insertKey(mymap, "M", "001");
    insertKey(mymap, "D", "010");
    insertKey(mymap, "MD", "011");
    insertKey(mymap, "A", "100");
    insertKey(mymap, "AM", "101");
    insertKey(mymap, "AD", "110");
    insertKey(mymap, "AMD", "111");

    // jmp bits
    insertKey(mymap, "JGT", "001");
    insertKey(mymap, "JEQ", "010");
    insertKey(mymap, "JGE", "011");
    insertKey(mymap, "JLT", "100");
    insertKey(mymap, "JNE", "101");
    insertKey(mymap, "JLE", "110");
    insertKey(mymap, "JMP", "111");

    return mymap;
}

map createSymbols()
{
    // frist define the predefined symbols
    map mymap = createMap(2000); // we can change it into dynamic allocation if needs more spaces
    insertKey(mymap, "SP", "0");
    insertKey(mymap, "LCL", "1");
    insertKey(mymap, "ARG", "2");
    insertKey(mymap, "THIS", "3");
    insertKey(mymap, "THAT", "4");
    insertKey(mymap, "SCREEN", "16384");
    insertKey(mymap, "R0", "0");
    insertKey(mymap, "R1", "1");
    insertKey(mymap, "R2", "2");
    insertKey(mymap, "R3", "3");
    insertKey(mymap, "R4", "4");
    insertKey(mymap, "R5", "5");
    insertKey(mymap, "R6", "6");
    insertKey(mymap, "R7", "7");
    insertKey(mymap, "R8", "8");
    insertKey(mymap, "R9", "9");
    insertKey(mymap, "R10", "10");
    insertKey(mymap, "R11", "11");
    insertKey(mymap, "R12", "12");
    insertKey(mymap, "R13", "13");
    insertKey(mymap, "R14", "14");
    insertKey(mymap, "R15", "15");

    return mymap;
}