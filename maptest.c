
// maptest.c
//
// Basic testing of the map.c and map.h code
//
// Code for Clark University CSCI 140 assignments.
//
// To compile and test:
// gcc maptest.c map.c -o maptest
// ./maptest
//
// To test for memory leaks:
// valgrind --leak-check=yes ./maptest
//
//     More info on Valgrind: http://valgrind.org/docs/manual/quick-start.html
//
// To download to linux:
// wget https://mathcs.clarku.edu/~jmagee/cs140/examples/map/maptest.c
//
// Author: John Magee
// Date: 10/5/2017
// Version 1.0
//



#include <stdio.h>
#include "map.h"




int main(int argc, char** argv) {

    // create a map
    map myMap = createMap(200);

    // add a few key,value pairs
    insertKey(myMap, "testing", "123");
    insertKey(myMap, "hello", "world");

    // test looking up by key
    char * value;
    value =  lookupKey(myMap, "testing");
    printf("Expecting 123: %s\n", value);
    value = lookupKey(myMap, "hello");
    printf("Expecting world: %s\n", value);
    value = lookupKey(myMap, "not in there");
    if(value == NULL) {
        puts("Lookup not found returned NULL as expected\n");
    } else {
        printf("UNEXPECTED: lookupKey should have returned NULL");
    }

    int index;
    index = containsKey(myMap, "testing");
    printf("Expecting 0: %d\n", index);
    value = lookupIndex(myMap, index);
    printf("Expecting 123: %s\n", value);

    index = containsKey(myMap, "hello");
    printf("Expecting 1: %d\n", index);
    value = lookupIndex(myMap, index);
    printf("Expecting world: %s\n", value);


    index = containsKey(myMap, "not in there");
    if(index == -1) {
        puts("Lookup returned -1 as expected\n");
    } else {
        printf("UNEXPECTED: containsKey did not return -1");
    }

    freeMap(myMap);


    myMap = createMap(2);
    int retval;
    retval = insertKey(myMap, "hello", "world");
    printf("Expected 1: %d\n", retval); 

    retval = insertKey(myMap, "hello", "world");
    printf("Expected 1: %d\n", retval); 

    printf("Testing inserting beyond capacity. Should cause an error message\n");

    retval = insertKey(myMap, "hello", "world");
    printf("Expected 0: %d\n", retval); 

    freeMap(myMap);


}
