// Project: cs140 assembler
// Instructor: John Magee
// Students: Han Choi & Enbang Wu

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "map.h"

///////////////////////////////////////////////////////////////////////
// GLOBAL VARIABLES

// 1. three maps constructed in pass.c
extern map compMap;
extern map destjmpMap;
extern map symbolMap;

// 2. next availabe address of A_COMMAND variables: used in translate_A()
int address = 16;

// 3. next_line global variable in pass.c: used in translate_L()
extern int next_line;
////////////////////////////////////////////////////////////////////////

// cleaning a linebuffer: whitespace and comments removal
int clean(char *str)
{
    int count = 0;
    int i;
    size_t length = strlen(str);
    for (i = 0; i < length; i++)
    {
        // removes comments
        if (str[i] == '/')
            break; // run into comments then break;
        // remove white spaces
        if (isspace(str[i]) == 0)
            str[count++] = str[i];
    }
    str[count] = '\0'; // add a null char to terminate the string

    if (strlen(str) == 0) // return false if the string is now empty
        return 0;
    else // otherwise return true
        return 1;
}

// helper functionf for translate_A(): converting a decimal to 15-bit binary
char *decimal_to_binary(int n, char *result)
{
    int myind = 15;
    char binary;
    int c;

    // initially setting the array with all 0's which is 0 base ten
    for (c = 15; c > -1; c--)
        result[c] = '0';

    if (n != 0) // otherwise convert into binary number
    {
        while (n != 1)
        {
            if (n % 2 == 0)
                binary = '0';
            else
                binary = '1';
            result[myind] = binary;
            myind--;
            n = (int)n / 2;
        }
        result[myind] = '1';
    }

    result[16] = '\0'; // null character at the end of the result
    return result;
}

// helper function for translate_A(): inserting variables in A_COMMANDS
// - receives an A_COMMAND and returns the address of the variable
// - checks if the key is already in the symbol table and if not, insert it
char *insertVariables(char *searchKey)
{
    // frist check if map contains searchKey
    int i = containsKey(symbolMap, searchKey);
    char myindex[100];
    char *value;

    if (i != -1) // searchKey found in symbol table
    {
        value = lookupIndex(symbolMap, i); // simply return the values found in the symbol table
    }
    else // searchKey not found
    {
        sprintf(myindex, "%d", address);          // 1. get the string version of address
        myindex[strlen(myindex)] = '\0';          // null terminterd
        insertKey(symbolMap, searchKey, myindex); // 2. insert the pair(searchKey, myindex) into symbolMap
        address++;                                // 3. increment next available address as we insert one
        value = lookupKey(symbolMap, searchKey);  // 4. return the value in the symbol table
    }
    return value;
}

// A_COMMANDS translation: str can be @8 (int) or @i (variable)
// - receives a linebuffer and an empty buffer
// - modifies assembly code and saves now a machine code of 16-bit binary into mychar buffer
void translate_A(char *str, char *mychar)
{
    str++; // disregard the first character '@'

    if (isdigit(str[0]) != 0) // if it's non-variable case like '256'
    {
        int decimal = atoi(str);                     // convert the str decimal to int decimal
        mychar = decimal_to_binary(decimal, mychar); // now mychar = 17-bit binary code of decimal str
    }
    else // else the variables 'sum'
    {
        char *value = insertVariables(str);          // variable inserted in the symbol table and the address of the variable returned
        int decimal = atoi(value);                   // convert the address of the variable to binary
        mychar = decimal_to_binary(decimal, mychar); // now mychar = 17-bit binary code of decimal variable address
    }
}

// L_COMMAND translation: str starts with '(' and ends with ')'
// receives a linebuffer and an empty buffer and inserts a pair (label, nextline) into symbol table
void translate_L(char *str, char *mychar)
{
    char mynum[200];
    sprintf(mynum, "%d", next_line); // convert int next_line into a string mynum

    strncpy(mychar, str + 1, strlen(str) - 2); // copy label into mychar
    mychar[strlen(str) - 2] = '\0';            // null character at the end of mychar

    insertKey(symbolMap, mychar, mynum); // insert pair (label, nextline) into symbolMap
}

// C_COMMAND translation
// - receives a linebuffer and an empty buffer
// - translates the received assembly code and saves now a machine code into mychar buffer
void translate_C(char *str, char *mychar)
{
    //finding the indices of '=' and ';' if any
    //strchr() explained https://www.cplusplus.com/reference/cstring/strncpy/?kw=strncpy
    char *equal_sign = strchr(str, '=');
    char *semicolon = strchr(str, ';');

    // converting the pointer to an index explained here:
    // https://stackoverflow.com/questions/3217629/how-do-i-find-the-index-of-a-character-within-a-string-in-c#:~:text=Just%20subtract%20the%20string%20address,example%20it%20will%20be%202.
    // index < 0 if not found?
    int iequal = (int)(equal_sign - str);    // index of equal char
    int isemicolon = (int)(semicolon - str); // index of semicolon

    // initializing arrays for each parts of C_COMMAND
    char comp[8];
    char dest[4];
    char jump[4];
    char *value;

    // DEST
    // 1. '=' not found
    if (equal_sign == NULL)
    {
        value = lookupKey(destjmpMap, "null");
        strcpy(dest, value); // copy "000" into dest part
    }

    // 2. '=' found
    else
    {
        strncpy(dest, str, iequal); // copy the substring into dest by length of iequal
        dest[iequal] = '\0';        // null character
        value = lookupKey(destjmpMap, dest);
        strcpy(dest, value);
    }

    // JUMP
    // 1. ';' not found
    if (semicolon == NULL)
    {
        value = lookupKey(destjmpMap, "null");
        strcpy(jump, value); // copy "000" into jump part
    }

    // 2. ';' found
    else
    {
        strncpy(jump, str + isemicolon + 1, 3); // get the substring
        jump[3] = '\0';                         // null character
        value = lookupKey(destjmpMap, jump);
        strcpy(jump, value);
    }

    // COMP
    // 1. comp dest jump
    if (equal_sign != NULL && semicolon != NULL)
    {
        strncpy(comp, str + iequal + 1, isemicolon - iequal);
        comp[isemicolon - iequal] = '\0'; // null character
    }

    // 2. dest comp
    else if (equal_sign != NULL && semicolon == NULL)
    {
        strncpy(comp, str + iequal + 1, 3);
        comp[3] = '\0'; // null character
    }

    // 3. comp jump
    else if (equal_sign == NULL && semicolon != NULL)
    {
        strncpy(comp, str, isemicolon);
        comp[isemicolon] = '\0'; // null character
    }

    else // 4. comp
        strcpy(comp, str);

    value = lookupKey(compMap, comp); // look up value of our substring comp
    strcpy(comp, value);              // replace the associated binary string into comp part

    // Combining COMP, DEST, JUMP and save it into mychar
    strcpy(mychar, "111");
    strcat(mychar, comp);
    strcat(mychar, dest);
    strcat(mychar, jump);
}

//parsing each linebuffer in first pass: detailed description in header file
int first_pass_line(char *str)
{
    int count = 0;   // instruction count
    char mychar[50]; // empty linebuffer that saves the parsed line
    char firstchar = str[0];

    // if str is empty
    if (strlen(str) == 0)
        return count; // return 0;

    // else str not empty: identify the command type based on the first char of the input linebuffer
    else
    {
        // A/C_COMMANDS
        if (firstchar == '@' || firstchar != '(')
            count++; // increment the count and do nothing

        else // L_COMMANDS
        {
            translate_L(str, mychar); // translate str using
            strcpy(str, mychar);      // what's in mychar is copied into str, or linebuffer in this case
        }
    }
    return count; // then return count
}

//parsing each linebuffer in second pass: detailed description in header file
int second_pass_line(char *str)
{
    int count = 0;
    char mychar[50];
    char firstchar = str[0];

    if (strlen(str) == 0)
        return count;
    else
    {
        if (firstchar == '@') // A command
        {
            translate_A(str, mychar);
            strcpy(str, mychar); // what's in mychar is copied into str, or linebuffer in this case
        }
        else if (firstchar != '(') // C command
        {
            translate_C(str, mychar);
            strcpy(str, mychar); // what's in mychar is copied into str, or linebuffer in this case
        }
        else // L command
            count++;
    }
    return count;
}
