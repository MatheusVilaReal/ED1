#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "get_args.h"
#include "path.h"
#include "environment.h"

typedef struct Environment{

    struct Required{

        char geoFile[PATH_MAX];
        char bsd[PATH_MAX];

    } required;

    struct Optional{

        char qryFile[PATH_MAX];
        char bed[PATH_MAX];

    } optional;

} Env_t;

static void setOptional(int argc, char* argv[], Env_t* env){

    strncpy(env->optional.bed, getArg(argc, argv, "e"), sizeof(env->optional.bed));

    char* q = getArg(argc, argv, "q");

    if(q)    
        if(strcmp(env->optional.bed, "\0"))
            catPathFile(env->optional.qryFile, env->optional.bed, q, sizeof(env->optional.qryFile));
        else
            strncpy(env->optional.qryFile, q, sizeof(env->optional.qryFile));
}

static bool setRequired(int argc, char* argv[], Env_t* env){

    strncpy(env->required.bsd, getArg(argc, argv, "o"), sizeof(env->required.bsd));

    char* f = getArg(argc, argv, "f");

    if(strcmp(env->optional.bed, "\0"))
        catPathFile(env->required.geoFile, env->optional.bed, f, sizeof(env->optional.qryFile));
    else
        strncpy(env->required.geoFile, f, sizeof(env->required.geoFile));

    return (env->required.geoFile && env->required.bsd);
}

Env setEnv(int argc, char* argv[]){

    Env_t* env = (Env_t*) malloc(sizeof(Env_t));

    memset(env->optional.bed, 0, sizeof(env->optional.bed));
    memset(env->optional.qryFile, 0, sizeof(env->optional.qryFile));
    memset(env->required.bsd, 0, sizeof(env->required.bsd));
    memset(env->required.geoFile, 0, sizeof(env->required.geoFile));

    setOptional(argc, argv, env);

    if(!setRequired(argc, argv, env)){
    
        freeEnv(env);

        return NULL;
    }

    srand(time(NULL));

    return env;
}

char* getGeoFile(Env env){

    if(env == NULL)
        return NULL;

    Env_t* toGetGeo = (Env_t*) env;

    return toGetGeo->required.geoFile;
}

char* getBsd(Env env){

    if(env == NULL)
        return NULL;

    Env_t* toGetBsd = (Env_t*) env;

    return toGetBsd->required.bsd;
}

char* getQryFile(Env env){

    if(env == NULL)
        return NULL;

    Env_t* toGetQry = (Env_t*) env;

    return toGetQry->optional.qryFile;
}

char* getBed(Env env){

    if(env == NULL)
        return NULL;

    Env_t* toGetBed = (Env_t*) env;

    return toGetBed->optional.bed;
}

char* solveSvgPath(Env env){

    char svg[PATH_MAX], buf_svgPath[PATH_MAX], buf2_svgPath[PATH_MAX];

    getFilename(svg, getGeoFile(env), sizeof(svg));

    addSuffix(buf_svgPath, svg, ".svg", sizeof(svg));

    catPathFile(buf2_svgPath, getBsd(env), buf_svgPath, sizeof(buf_svgPath));

    char* svgPath = (char*) calloc(strlen(buf2_svgPath) + 1, sizeof(char));
    
    svgPath = strncpy(svgPath, buf2_svgPath, strlen(buf2_svgPath) + 1 * sizeof(char));

    return svgPath;
}

char* solveQrySvgPath(Env env){

    char svg[PATH_MAX], buf_svgPath[PATH_MAX], buf2_svgPath[PATH_MAX], geoFile[PATH_MAX], qryFile[PATH_MAX];

    getFilename(geoFile, getGeoFile(env), sizeof(geoFile));

    rmSuffix(svg, geoFile, sizeof(svg));

    getFilename(qryFile, getQryFile(env), sizeof(qryFile));

    strncat(svg, "-", sizeof("-"));

    strncat(svg, qryFile, sizeof(svg));

    addSuffix(buf_svgPath, svg, ".svg", sizeof(svg));

    catPathFile(buf2_svgPath, getBsd(env), buf_svgPath, sizeof(buf_svgPath));

    char* svgPath = (char*) calloc(strlen(buf2_svgPath) + 1, sizeof(char));
    
    svgPath = strncpy(svgPath, buf2_svgPath, strlen(buf2_svgPath) + 1 * sizeof(char));

    return svgPath;
}

char* solveTxtPath(Env env){

    char txt[PATH_MAX], buf_txtPath[PATH_MAX], buf2_txtPath[PATH_MAX], geoFile[PATH_MAX], qryFile[PATH_MAX];

    getFilename(geoFile, getGeoFile(env), sizeof(txt));

    getFilename(qryFile, getQryFile(env), sizeof(qryFile));

    rmSuffix(txt, geoFile, sizeof(txt));
    
    strncat(txt, "-", sizeof("-"));

    strncat(txt, qryFile, sizeof(buf_txtPath));

    addSuffix(buf_txtPath, txt, ".txt", sizeof(txt));

    catPathFile(buf2_txtPath, getBsd(env), buf_txtPath, sizeof(buf_txtPath));

    char* txtPath = (char*) calloc(strlen(buf2_txtPath) + 1, sizeof(char));
    
    txtPath = strncpy(txtPath, buf2_txtPath, strlen(buf2_txtPath) + 1 * sizeof(char));

    return txtPath;
}

void freeEnv(Env env){

    free(env);

    env = NULL;
}