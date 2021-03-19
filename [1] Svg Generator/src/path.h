#ifndef PATH_H
#define PATH_H

#include <stdbool.h>

char* getPath(char *fullPath);
char* getFileName(const char *fullPath);
char* getSuffix(char *name);
char* concatPathFile(char* path, char* filename, const char* separator);
char* removeSuffix(char* path);
bool verifySuffix(char* filename, const char* suffix);
void addSuffix(char** filename, const char* suffix);

void* verifyArg(int numArgs, char** argList, const char* arg, char param);
int setRequiredParams(int argc, char** argv, char** bsd, char** geoName);
void setOptionalParams(int numArgs, char** argList, int* dataCollection, bool* list, char** qryName, char** bed, char** fbPath, char** fbName);

#endif