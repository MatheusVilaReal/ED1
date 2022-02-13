#ifndef BUILD_ENV_H
#define BUILD_ENV_H

#include "types.h"

Env setEnv(int argc, char* argv[]);

char* getGeoFile(Env env);

char* getBsd(Env env);

char* getQryFile(Env env);

char* getBed(Env env);

char* solveSvgPath(Env env);

char* solveQrySvgPath(Env env);

char* solveTxtPath(Env env);

// bool setRequired(int argc, char* argv[], char** geoFile, char** bsd);

// void setOptional(int argc, char* argv[], char** qryFile, char** bed);

void freeEnv(Env env);

#endif