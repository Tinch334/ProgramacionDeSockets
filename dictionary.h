#ifndef __DICT_H__
#define __DICT_H__

#include <stddef.h>

enum {
    USER_DICT,
    PASS_DICT,
    PORT_DICT,
    RETR_DICT,
    STOR_DICT,
    QUIT_DICT,
    BAD_KEY = -1
};


typedef struct {
    char *key;
    int val;
} dictStruct;


//Elements of the dictionary.
static dictStruct DICT_MASTER_TABLE[] = {
    { "USER", USER_DICT },
    { "PASS", PASS_DICT },
    { "PORT", PORT_DICT },
    { "RETR", RETR_DICT },
    { "STOR", STOR_DICT },
    { "QUIT", QUIT_DICT }
};


/*
    Looks the given key in the dict and returns the corresponding element.
*/
int dictLookup(char *givenKey);


#endif /* __DICT_H__ */