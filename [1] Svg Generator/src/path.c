#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "path.h"

#define SLASH '/'

static bool hasSlash(char* path);

static bool hasSlash(char* path){

    return (path[strlen(path) - 1] == SLASH)? true:false;
}

char* getPath(char* fullPath){

    size_t nameSize = 0;

    for(int i = strlen(fullPath); i > 0; i--){

        if(fullPath[i] == SLASH)
            break;

        nameSize++;
    }

    char* path = calloc(strlen(fullPath) - nameSize + 1, sizeof(char));

    strncpy(path, fullPath, strlen(fullPath) - nameSize);

    return path;
}

char* getFileName(const char* fullPath){

    int pos = strlen(fullPath) - 1;
    int i = 0;
    int j = 0;

    char* output = NULL;

    while(fullPath[pos-i] != SLASH && i <= pos)
        i++;

    output = calloc(i+1, sizeof(char));

    while(--i >= 0)
        output[j++] = fullPath[pos-i];

    return output;
}

char* getSuffix(char* filename){

    int suffixSize = 0;
    size_t strSize = strlen(filename);

    for(int i = strSize - 1; i > 0; i--){

        if(filename[i] == '.'){

            i = 0;

            char* suffix = calloc(suffixSize+1, sizeof(char));

            while(i < suffixSize){

                suffix[i] = filename[strSize - suffixSize + i];
                i++;
            }

            return suffix;
        }

        suffixSize++;
    }

    return "\0";
}

char* concatPathFile(char* path, char* filename, const char* separator){

    char *output = NULL;

    if(hasSlash(path)){

        output = (char*) calloc(strlen(path) + strlen(filename) + 1, sizeof(char));

        snprintf(output, strlen(path) + strlen(filename) + 1, "%s%s", path, filename);
    
    } else if(separator != NULL){

        output = (char*) calloc(strlen(path) + strlen(separator) + strlen(filename) + 1, sizeof(char));

        snprintf(output, strlen(path) + strlen(separator) + strlen(filename) + 1, "%s%s%s", path, separator, filename);

    } else{

        output = (char*) calloc(strlen(path) + strlen(filename) + 2, sizeof(char));

        snprintf(output, strlen(path) + strlen(filename) + 2, "%s/%s", path, filename);
    }

    return output;
}

char* removeSuffix(char* filename){

    int pos = strlen(filename);
    int i = 0;

    char* output = NULL;

    while(filename[pos-i] != '.' && i < pos)
        i++;

    output = calloc(pos-i+1, sizeof(char));

    while(++i <= pos){

        output[pos-i] = filename[pos-i];
    }

    puts(filename);

    return output;
}

bool verifySuffix(char* filename, const char* suffix){

    char* fileSuffix = getSuffix(filename);

    int output = strcmp(fileSuffix, suffix);

    if(strcmp(fileSuffix, "\0")) free(fileSuffix);

    return output;
}

void addSuffix(char** filename, const char* suffix){

    short startsWithDot = (suffix[0] == '.')? 0:1;

    char* temp = (char*) realloc(*filename, strlen(*filename) + sizeof(suffix) + startsWithDot);

    if(temp) *filename = temp;

    if(startsWithDot) *filename = strcat(*filename, ".");

    *filename = strcat(*filename, suffix);
}

//=============================================================================================================//

void* verifyArg(int argc, char** argv, const char* arg, char param){

    int i = 1;
    char* temp = NULL;

    if(param == 'c'){

        while(i < argc){

            if (!(strcmp(arg, argv[i]))){

                if(i != argc - 1){
                    
                    temp = calloc(strlen(argv[i+1]) + 1, sizeof(char));

                    sprintf(temp, "%s", argv[i+1]);

                    return temp;
                }
            }

            i++;
        }

    } else {

        while(i < argc){

            if (!(strcmp(arg, argv[i]))){

                return (void*) 1;
            }

            i++;
        }
    }

    return NULL;
}

int setRequiredParams(int argc, char** argv, char** bsd, char** geoName){

    *bsd = verifyArg(argc, argv, "-o", 'c');
    *geoName = verifyArg(argc, argv, "-f", 'c');

    if(*bsd && *geoName) return EXIT_SUCCESS;
    else{

        free(*bsd);
        free(*geoName);

        puts("\nERRO: Parametros insuficientes\nPressione qulquer tecla pra continuar:");
        
        getchar();

        return EXIT_FAILURE;
    }
}

void setOptionalParams(int argc, char** argv, int* dataCollection, bool* list,
                        char** qryName, char** bed, char** fbPath, char** fbName){

    *bed = verifyArg(argc, argv, "-e", 'c');
    *qryName = verifyArg(argc, argv, "-q", 'c');

    if(verifyArg(argc, argv, "-ib", 'i'))
        
        *dataCollection = 1;

        else if(verifyArg(argc, argv, "-cb", 'i'))

            *dataCollection = 2;

            else if(verifyArg(argc, argv, "-fb", 'i')){

                *dataCollection = 3;

                *fbPath = verifyArg(argc, argv, "-fb", 'c');

                *fbName = verifyArg(argc, argv, *fbPath, 'c');
            }

    if(verifyArg(argc, argv, "-lse", 'i'))
        *list = true;
}