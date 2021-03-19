#include <stdio.h>
#include <stdlib.h>

#include "getword.h"

char* getword(FILE* stream){

    char ch;
    size_t i = 0, size = 2;

    char *str = (char*) malloc(sizeof(char)*size);

    if(str == NULL) 
        return str;

    while((ch = fgetc(stream)) != EOF && ch != '\n' && ch != ' ' && ch != '\t'){

        if(ch != '\n' && ch != '\t')
            str[i++] = ch;

        if(i == size){

            str = realloc(str, sizeof(char)*(size *= i));

            if(str == NULL)
                return str;
        }
    }

    str[i++]='\0';

    return realloc(str, sizeof(char)*i);
}