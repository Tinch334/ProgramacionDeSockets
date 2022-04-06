#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dictionary.h"

/*
    Implements a very basic dictonary for using strings with switch statements. It has almost no testing or error handling, but it works.
*/

//Number of keys.
#define NKEYS (sizeof(DICT_MASTER_TABLE) / sizeof(dictStruct))

int dictLookup(char *givenKey) {
    //Try to find an element in the dict with the same key.
    for (int i = 0; i < NKEYS; i++) {
        dictStruct elem = DICT_MASTER_TABLE[i];

        if (strcmp(elem.key, givenKey) == 0)
            return elem.val;
    }

    //If a key was not found return accordingly.
    return BAD_KEY;
}