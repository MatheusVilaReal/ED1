#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#include "dlist.h"
#include "getword.h"

typedef struct Node PrivateNode;
typedef struct List PrivateList;
typedef struct Data PrivateData; 

static void freeNode(Node node);

struct Data{

    char* id;

    char* fill;
    char* stroke;

    float x;
    float y;
    float w;
    float h;
};

struct Node{

    PrivateData data;

    PrivateNode* next;
    PrivateNode* prev;

};

struct List{

    PrivateNode* head;
    PrivateNode* tail;

    size_t size;

    size_t visits;

};

List newList(){

    PrivateList* list = (PrivateList*) malloc(sizeof(PrivateList));

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    list->visits = 0;

    return list;
}

Node getNode(List list, size_t index){

    PrivateList* privateList = (PrivateList*) list;

    PrivateNode* node = privateList->head;

    while(index > 0 && node != NULL){
     
        node = node->next;
        privateList->visits++;
        index--;
    }

    return node;
}

Node getFirst(List list){

    PrivateList* privateList = (PrivateList*) list;

    return privateList->head;
}

Node getNext(List list, Node node){

    PrivateList* privateList = (PrivateList*) list;
    PrivateNode* privateNode = (PrivateNode*) node;

    privateList->visits++;
    return privateNode->next;
}

Node getLast(List list){

    PrivateList* privateList = (PrivateList*) list;

    return privateList->tail;
}

Node getPrevious(List list, Node node){

    PrivateList* privateList = (PrivateList*) list;
    PrivateNode* privateNode = (PrivateNode*) node;

    privateList->visits++;
    return privateNode->prev;
}

Node insertAfter(List list, Data data, Node node){

    PrivateList* privateList = (PrivateList*) list;
    PrivateNode* privateNode = (PrivateNode*) node;
    PrivateData* privateData = (PrivateData*) data;

    PrivateNode* newNode = (PrivateNode*) malloc(sizeof(PrivateNode));

    newNode->data.id = (char*) calloc(strlen(privateData->id)+1, sizeof(char));
    newNode->data.fill = (char*) calloc(strlen(privateData->fill)+1, sizeof(char));
    newNode->data.stroke = (char*) calloc(strlen(privateData->stroke)+1, sizeof(char));

    newNode->data.id = strcpy(newNode->data.id, privateData->id);
    newNode->data.fill = strcpy(newNode->data.fill, privateData->fill);
    newNode->data.stroke = strcpy(newNode->data.stroke, privateData->stroke);

    newNode->data.x = privateData->x;
    newNode->data.y = privateData->y;
    newNode->data.w = privateData->w;
    newNode->data.h = privateData->h;

    newNode->next = NULL;
    newNode->prev = NULL;

    if(privateList->head == NULL){                                //Lista vazia

        privateList->head = newNode;
        privateList->tail = newNode;

    } else
    if(privateNode == privateList->head){                          //Inserindo no início da Lista

        newNode->next = privateList->head;     
        privateList->head = newNode;

    } else
    if(privateNode == privateList->tail){             //Inserindo no final da Lista

        newNode->prev = privateList->tail;
        privateList->tail->next = newNode;
        privateList->tail = newNode;

    } else{

        PrivateNode* newNode2 = privateNode;

        newNode->prev = newNode2;
        newNode->next = newNode2->next;
        newNode2->next = newNode;
    }

    privateList->size++;

    return newNode;
}

Node push(List list, Data data){

    PrivateList* newList = (PrivateList*) list;
    newList->visits++;

    printf("Item adicionado na lista, tamanho: %ld\n", newList->size);

    return insertAfter(list, data, newList->tail);
}

Node insertBefore(List list, Data data, Node node){

    PrivateList* privateList = (PrivateList*) list;
    PrivateNode* privateNode = (PrivateNode*) node;
    PrivateData* privateData = (PrivateData*) data;

    PrivateNode* newNode = (PrivateNode*) malloc(sizeof(PrivateNode));

    newNode->data.id = (char*) calloc(strlen(privateData->id)+1, sizeof(char));
    newNode->data.fill = (char*) calloc(strlen(privateData->fill)+1, sizeof(char));
    newNode->data.stroke = (char*) calloc(strlen(privateData->stroke)+1, sizeof(char));

    newNode->data.id = strcpy(newNode->data.id, privateData->id);
    newNode->data.fill = strcpy(newNode->data.fill, privateData->fill);
    newNode->data.stroke = strcpy(newNode->data.stroke, privateData->stroke);

    newNode->data.x = privateData->x;
    newNode->data.y = privateData->y;
    newNode->data.w = privateData->w;
    newNode->data.h = privateData->h;

    newNode->next = NULL;
    newNode->prev = NULL;


    if(privateNode != NULL){

        newNode->prev = privateNode->prev;
        newNode->next = privateNode;
        if(privateNode->prev != NULL) privateNode->prev->next = newNode;
        privateNode->prev = newNode;

    } else privateNode = newNode;

    privateList->head = newNode;
    privateList->tail = newNode;

    while(newNode->next != NULL){

        privateList->tail = newNode->next;
    }

    while(newNode->prev != NULL){

        privateList->head = newNode->prev;
    }

    privateList->size++;

    return newNode;
}

Data getData(Node node){

    PrivateNode* privateNode = (PrivateNode*) node;

    if(node == NULL) return NULL;

    return &privateNode->data;
}

size_t getListSize(List list){

    PrivateList* privatelist = (PrivateList*) list;

    return privatelist->size;
}

void geoToList(List list, char* source){

    FILE* stream = NULL;
    PrivateData newData;
    
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

            push(list, &newData);

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

void delNode(List list, Node node){

    PrivateList* privateList = (PrivateList*) list;

    PrivateNode* toFree = (PrivateNode*) node;
    PrivateNode* temp = privateList->head;

    if(toFree == privateList->head){

        privateList->head = privateList->head->next;

        freeNode(toFree);

        return;
    }

    while(temp->next != toFree){

        temp = (PrivateNode*) getNext(list, temp);
    }

    temp->next = toFree->next;

    if(temp->next != NULL) temp->next->prev = temp;

    privateList->size--;

    freeNode(toFree);
}

void printList(List list){

    PrivateList* privateList = (PrivateList*) list;
    PrivateNode* toPrint = privateList->head;

    int i = 0;

    while(toPrint){
     
        printf("\nItem %d, %s %f %f %f %f\n%s %s\n", i++,
        toPrint->data.id,
        toPrint->data.x,
        toPrint->data.y,
        toPrint->data.w,
        toPrint->data.h,
        toPrint->data.fill,
        toPrint->data.stroke);

        toPrint = (PrivateNode*) getNext(list, toPrint);
    }

    return;
}

void freeList(List list){

    PrivateList* privateList = (PrivateList*) list;

    PrivateNode* temp;

    while(privateList->head != NULL){

        temp = privateList->head;

        privateList->head = privateList->head->next;
        privateList->visits++;
        
        freeNode(temp);
    }

    free(privateList);
    privateList = NULL;
}

size_t getVisits(List list){

    PrivateList* privateList = (PrivateList*) list;

    unsigned long output = 0;

    if(list) output = privateList->visits;

    return output;
}

static void freeNode(Node node){

    if(node == NULL) return;

    PrivateNode* toFree = (PrivateNode*) node;

    free(toFree->data.id);
    toFree->data.id = NULL;

    free(toFree->data.fill);
    toFree->data.fill = NULL;

    free(toFree->data.stroke);
    toFree->data.stroke = NULL;

    free(toFree);
    toFree = NULL;
}