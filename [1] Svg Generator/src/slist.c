#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "slist.h"
#include "getword.h"
#include "types.h"

typedef struct StaticList PrivateStaticList;
typedef struct StaticNode PrivateStaticNode;
typedef struct Data PrivateStaticData; 

struct Data{

    char* id;

    char* fill;
    char* stroke;

    float x;
    float y;
    float w;
    float h;
};

struct StaticNode{

    PrivateStaticData data;

    PrivateStaticNode* next;
};

struct StaticList{

    PrivateStaticNode* nodes;

    PrivateStaticNode* head;

    size_t visits;
    size_t size, free;
};

List newStaticList(size_t size){

    PrivateStaticList* list = (PrivateStaticList*) malloc(sizeof(PrivateStaticList));

    list->nodes = (PrivateStaticNode*) calloc(size, sizeof(PrivateStaticNode));
    list->size = size;
    list->free = 0;
    list->head = &list->nodes[0];
    list->visits = 0;

    for(size_t i = 0; i < size; i++)
        list->nodes[i].next = NULL;

    return list;
}

Node getStaticNode(List list, size_t index){

    PrivateStaticList* privateList = (PrivateStaticList*) list;

    if(index < privateList->free)
        return &privateList->nodes[index];

    return NULL;
}

Node staticInsert(List list, Data data, size_t index){

    PrivateStaticList* privateList = (PrivateStaticList*) list;
    PrivateStaticData* privateData = (PrivateStaticData*) data;

    if(index < privateList->size){    
        
        privateList->nodes[index].data.id = (char*) calloc(strlen(privateData->id)+1, sizeof(char));
        privateList->nodes[index].data.fill = (char*) calloc(strlen(privateData->fill)+1, sizeof(char));
        privateList->nodes[index].data.stroke = (char*) calloc(strlen(privateData->stroke)+1, sizeof(char));

        privateList->nodes[index].data.id = strcpy(privateList->nodes[index].data.id, privateData->id);
        privateList->nodes[index].data.fill = strcpy(privateList->nodes[index].data.fill, privateData->fill);
        privateList->nodes[index].data.stroke = strcpy(privateList->nodes[index].data.stroke, privateData->stroke);

        privateList->nodes[index].data.x = privateData->x;
        privateList->nodes[index].data.y = privateData->y;
        privateList->nodes[index].data.w = privateData->w;
        privateList->nodes[index].data.h = privateData->h;

        if(index <= privateList->free){
        
            privateList->nodes[index].next = NULL;

            if(index > 0)
                privateList->nodes[index-1].next = &privateList->nodes[index];
        }

        privateList->free++;

        puts("Novo item adicionado");
        
        puts(privateList->nodes[index].data.id);

        
        return &privateList->nodes[index];
    }

    return NULL;
}

Node staticPush(List list, Data data){

    PrivateStaticList* privateList = (PrivateStaticList*) list;

    return staticInsert(list, data, privateList->free);
}

Node staticInsertBefore(List list, Data data, size_t index){

    return staticInsert(list, data, index-1);
}

Node staticInsertAfter(List list, Data data, size_t index){

    return staticInsert(list, data, index+1);
}

Node getStaticFirst(List list){

    PrivateStaticList* privateList = (PrivateStaticList*) list;

    privateList->visits++;

    return privateList->head;
}

Node getStaticNext(List list, Node node){

    PrivateStaticList* privateList = (PrivateStaticList*) list;
    PrivateStaticNode* privateNode = (PrivateStaticNode*) node;

    privateList->visits++;

    return privateNode->next;
}

Node getStaticLast(List list){

    PrivateStaticList* privateList = (PrivateStaticList*) list;

    return &privateList->nodes[privateList->free-1];
}

Node getStaticPrevious(List list, Node node){

    PrivateStaticList* privateList = (PrivateStaticList*) list;
    PrivateStaticNode* privateNode = (PrivateStaticNode*) node;

    PrivateStaticNode* temp = &privateList->nodes[0];

    if(privateNode == temp) return NULL;

    while(temp->next != privateNode && temp->next != NULL){
        
        temp = temp->next;
        privateList->visits++;
    }

    return temp;
}

Data getStaticData(Node node){

    PrivateStaticNode* privateNode = (PrivateStaticNode*) node;

    if(node == NULL) return NULL;

    return &privateNode->data;
}

size_t getNx(char* streamId){

    FILE* stream = NULL;
    char* word = NULL;
    size_t output = 0;

    if(strcmp(streamId, "stdin") == 0)
        stream = stdin;
    else
        stream = fopen(streamId, "r");

    word = getword(stream);

    if(strcmp(word, "nx") == 0)
        fscanf(stream, "%zu", &output);

    free(word);
    fclose(stream);

    return output;
}

size_t getStaticSize(List list){

    PrivateStaticList* privateList = (PrivateStaticList*) list;

    return privateList->free;
}

size_t getStaticVisits(List list){

    PrivateStaticList* privateList = (PrivateStaticList*) list;

    size_t output = 0;

    if(list) output = privateList->visits;

    return output;
}

void geoToStaticList(List list, char* source){

    FILE* stream = NULL;
    PrivateStaticData newData;
    
    if(strcmp(source, "stdin") == 0)
        stream = stdin;
    else
        stream = fopen(source, "r");

    if(stream == NULL){

        puts("Falha de leitura");
        
        fclose(stream);

        return;
    }

    newData.id = NULL;
    newData.fill = NULL;
    newData.stroke = NULL;
    char* word = NULL;

    int i = 0;

    printf("Arquivo aberto da fonte: ");
    puts(source);

    while((word = getword(stream)) && strcmp(word, "\0") != 0){
        
        if(strcmp(word, "r") == 0){

            free(newData.id);
            newData.id = getword(stream);
        
            fscanf(stream, "%f %f %f %f", &newData.x, &newData.y, &newData.w, &newData.h);

            staticPush(list, &newData);

            fseek(stream, 1, SEEK_CUR);

        } else
        if(!(strcmp(word, "cp"))){

            free(newData.fill);
            newData.fill = getword(stream);

            if(strcmp(newData.fill, "@") == 0){

                newData.fill = realloc(newData.fill, sizeof("transparent"));
                newData.fill = strcpy(newData.fill, "transparent");
            }

            puts("Cor de preenchimento atualizada");

        } else
        if(!(strcmp(word, "cc"))){

            free(newData.stroke);
            newData.stroke = getword(stream);

            if(!(strcmp(word, "@"))){

                newData.stroke = realloc(newData.stroke, sizeof("transparent"));
                newData.stroke = strcpy(newData.stroke, "transparent");
            }

            puts("Cor de contorno atualizada");
        }

        printf("Iteracao: %d\n\n", i++);
        
        free(word);
    }

    free(newData.id);
    free(newData.fill);
    free(newData.stroke);
    free(word);

    fclose(stream);
}

void printStaticList(List list){

    PrivateStaticNode* toPrint = (PrivateStaticNode*) getStaticFirst(list);

    int i = 0;

    while(toPrint != NULL){

        printf("\nItem %d: %s %f %f %f %f\n%s %s\n", i++,
        toPrint->data.id,
        toPrint->data.x,
        toPrint->data.y,
        toPrint->data.w,
        toPrint->data.h,
        toPrint->data.fill,
        toPrint->data.stroke);

        toPrint = (PrivateStaticNode*) getStaticNext(list, toPrint);
    }

    puts("\n/*=============/*=============/*=============/*=============/*");
}

void delStaticNode(List list, Node node){

    PrivateStaticList* privateList = (PrivateStaticList*) list;
    PrivateStaticNode* temp = getStaticFirst(list);
    PrivateStaticNode* toDel = (PrivateStaticNode*) node;

    int index = 0;

    if(node == privateList->head){

        privateList->head = privateList->head->next;

        return;
    }
    
    while(temp->next != node){

        temp = getStaticNext(list, temp);
        
        index++;
    }

    temp->next = toDel->next;

/*
        privateList->nodes[privateList->free-1].next = NULL;

        privateList->nodes[privateList->free].data.id = privateList->nodes[index].data.id;

        privateList->nodes[privateList->free].data.fill = privateList->nodes[index].data.fill;

        privateList->nodes[privateList->free].data.stroke = privateList->nodes[index].data.stroke;

        privateList->free;   

    if(index < privateList->free){

        while(index < privateList->free-1){

            privateList->nodes[index].data.id = privateList->nodes[index+1].data.id;
            privateList->nodes[index].data.fill = privateList->nodes[index+1].data.fill;
            privateList->nodes[index].data.stroke = privateList->nodes[index+1].data.stroke;
            privateList->nodes[index].data.x = privateList->nodes[index+1].data.x;
            privateList->nodes[index].data.y = privateList->nodes[index+1].data.y;
            privateList->nodes[index].data.w = privateList->nodes[index+1].data.w;
            privateList->nodes[index].data.h = privateList->nodes[index+1].data.h;

            index++;
        }   

    }
*/
}

void freeStaticList(List list){

    PrivateStaticList* toFree = (PrivateStaticList*) list;

    for(size_t i = 0; i < toFree->free; i++){

        free(toFree->nodes[i].data.id);
        toFree->nodes[i].data.id = NULL;

        free(toFree->nodes[i].data.fill);
        toFree->nodes[i].data.fill = NULL;

        free(toFree->nodes[i].data.stroke);
        toFree->nodes[i].data.stroke = NULL;
    }

    free(toFree->nodes);
    toFree->nodes = NULL;

    free(toFree);
    toFree = NULL;
}