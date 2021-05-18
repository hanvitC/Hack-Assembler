// Project: cs140 assembler
// Instructor: John Magee
// Students: Han Choi & Enbang Wu

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assemblerHelper.h"
#include "pass.h"

#define MAX_LINE_LENGTH 200

int main(int argc, char **argv)
{
    if (argc != 3) // Must have a command line arguments
    {
        printf("File not found.\n\n");
        exit(1);
    }

    // receive the input file and the output file from terminal
    char *inputFilename = argv[1]; // puts(inputFilename); // puts() = putstring
                                   // returns the input file name
    char *outputFilename = argv[2];

    // handling an input file and an output file
    FILE *inFile = fopen(inputFilename, "r");   // open inFile for (r)eading
    FILE *outFile = fopen(outputFilename, "w"); // open outFile for (w)riting

    if (inFile != NULL) // check that this was successful : check if inFile != NULL
    {
        first_pass(inFile);
        second_pass(inFile, outFile);
    }

    fclose(inFile);
    fclose(outFile); // close the output file after done with process

    printf("Hello world!\n");
    return 0;
}
