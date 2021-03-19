#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "collectdata.h"
#include "dlist.h"
#include "slist.h"


void newTempFile(char* path){

    FILE* tempFile = fopen(path, "w+");

    if(tempFile == NULL){

        puts("\nERRO\nFalha ao criar arquivo temporario de coleta de dados.");

        return;
    }

    fprintf(tempFile, "size visits\n");

    fclose(tempFile);
}

void updateTempFile(char* path, List list, bool usesStaticList){

    FILE* tempFile = fopen(path, "a");

    if(tempFile == NULL){

        puts("\nERRO\nFalha ao abrir arquivo temporario de coleta de dados.");

        return;
    }

    size_t visits, size;

    if(usesStaticList == 0){ 
       
        size = getListSize(list);
        visits = getVisits(list);

    } else{ 
        
        size = getStaticSize(list);
        visits = getStaticVisits(list);
    }

    fprintf(tempFile, "%ld\t%ld\n", size, visits);

    fclose(tempFile);
}