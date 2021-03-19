#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#include "qry.h"
#include "rectmath.h"
#include "dlist.h"
#include "slist.h"
#include "types.h"
#include "svg.h"
#include "getword.h"
#include "path.h"

typedef struct Data PrivateData;

struct Data{

    char* id;

    char* fill;
    char* stroke;

    float x;
    float y;
    float w;
    float h;
};

static void colorize(Node rect_1, Node rect_2, bool useStaticList);

int qryMenu(List list, char* qryPath, char*txtPath, bool useStaticList){

    if(qryPath == NULL){

        return 0;
    }

    FILE* qryFile = fopen(qryPath, "r");

    if(qryFile == NULL){

        puts("ERRO\nNao foi possivel abrir arquivo de consulta. Verifique o diretorio\nespecificado e tente novamente.");
        return 0;
    }

    char* word;
    int i = 0;

    while((word = getword(qryFile)) && strcmp(word, "\0")){

        if(strcmp(word, "tp") == 0){

            tp(list, txtPath, useStaticList);

        }else if(strcmp(word, "tpr") == 0){

            puts("Executando: tpr");

            float x, y, w, h;

            fscanf(qryFile, "%f %f %f %f", &x, &y, &w, &h);

            tpr(list, txtPath, x, y, w, h, useStaticList);

        }else if(strcmp(word, "dpi") == 0){

            puts("Executando: dpi");

            float x, y;

            fscanf(qryFile, "%f %f", &x, &y);

            dpi(list, txtPath, x, y, useStaticList);

        }else if(strcmp(word, "dr") == 0){

            puts("Executando: dr");

            char* id = getword(qryFile);

            dr(list, txtPath, id, useStaticList);

            free(id);

        }else if(strcmp(word, "bbi") == 0){

            puts("Executando: bbi");

            float x, y;

            fscanf(qryFile, "%f %f", &x, &y);

            bbi(list, txtPath, x, y, useStaticList);

        }else if(strcmp(word, "bbid") == 0){

            puts("Executando: bbid");

            char* id = getword(qryFile);

            bbid(list, txtPath, id, useStaticList);

            free(id);

        }else if(strcmp(word, "iid") == 0){

            puts("Executando: iid");

            char* id = getword(qryFile);
            
            char* kch = getword(qryFile);

            int k = atoi(kch);

            iid(list, txtPath, id, k, useStaticList);

            free(id);
            free(kch);

        }else if(strcmp(word, "diid") == 0){

            puts("Executando: diid");

            char* id = getword(qryFile);

            char* kch = getword(qryFile);

            int k = atoi(kch);

            diid(list, txtPath, id, k, useStaticList);

            free(id);
            free(kch);

        }

        free(word);
    }

    free(word);

    fclose(qryFile);

    return 1;
}

void tp(List list, char* qry, bool useStaticList){

    FILE* txt = fopen(qry, "a");

    if(txt == NULL){

        puts("ERRO\nNao foi possivel gerar arquivo de relatorio .txt.");
        return;
    }

    fprintf(txt, "tp\n");

    srand(time(NULL));

    int i = 0;
    Node node_1;
    Node node_2;

    if(useStaticList == 0){

        node_1 = getNode(list, 0);

        while(node_1 != NULL){

            node_2 = getNext(list, node_1);

            while(node_2 != NULL){

                if(rectOverlap(getData(node_1), getData(node_2))){

                    PrivateData* toWrite1 = (PrivateData*) getData(node_1);
                    PrivateData* toWrite2 = (PrivateData*) getData(node_2);

                    fprintf(txt, "%s %s\n", toWrite1->id, toWrite2->id);

                    colorize(node_1, node_2, useStaticList);
                
                } 

                node_2 = getNext(list, node_2);
            }
                
            

            node_1 = getNext(list, node_1);
            i++;
        }

    } else{

        node_1 = getStaticFirst(list);
        
        while(node_1 != NULL){

            node_2 = getStaticNext(list, node_1);

            while(node_2 != NULL){

                if(rectOverlap(getStaticData(node_1), getStaticData(node_2))){

                    PrivateData* toWrite1 = (PrivateData*) getStaticData(node_1);
                    PrivateData* toWrite2 = (PrivateData*) getStaticData(node_2);

                    fprintf(txt, "%s %s\n", toWrite1->id, toWrite2->id);

                    colorize(node_1, node_2, useStaticList);
                
                } 

                node_2 = getStaticNext(list, node_2);
            }
                
            
            node_1 = getStaticNext(list, node_1);
        }

    }

    fclose(txt);
}

void tpr(List list, char* qry, float x, float y, float w, float h, bool useStaticList){

    FILE* txt = fopen(qry, "a");

    if(txt == NULL){

        puts("ERRO\nNao foi possivel gerar arquivo de relatorio .txt.");
        return;
    }

    int i = 0;
    srand(time(NULL));

    fprintf(txt, "tpr\n");
    
    if(useStaticList == 0){

        Node node_1 = getFirst(list);
        Node node_2;

        while(node_1 != NULL){

            node_2 = getNext(list, node_1);
                
            if(isRectInternal(x, y, w, h, getData(node_1))){
                
                while(node_2 != NULL){

                    if(isRectInternal(x, y, w, h, getData(node_2))){
                        
                        if(rectOverlap(getData(node_1), getData(node_2))){

                            PrivateData* toWrite1 = (PrivateData*) getData(node_1);
                            PrivateData* toWrite2 = (PrivateData*) getData(node_2);

                            fprintf(txt, "%s %s\n", toWrite1->id, toWrite2->id);

                            colorize(node_1, node_2, useStaticList);
                        
                        } 
                        
                        
                    }

                    node_2 = getNext(list, node_2);
                }
            }

            node_1 = getNext(list, node_1);
            i++;
        }

    } else{

        Node node_1 = getStaticFirst(list);
        Node node_2;

        while(node_1 != NULL){

            node_2 = getStaticNext(list, node_1);

            if(isRectInternal(x, y, w, h, getStaticData(node_1))){
                
                while(node_2 != NULL){

                    if(isRectInternal(x, y, w, h, getStaticData(node_2))){
                        
                        if(rectOverlap(getStaticData(node_1), getStaticData(node_2))){

                            PrivateData* toWrite1 = (PrivateData*) getData(node_1);
                            PrivateData* toWrite2 = (PrivateData*) getData(node_2);

                            fprintf(txt, "%s %s\n", toWrite1->id, toWrite2->id);

                            colorize(node_1, node_2, useStaticList);
                        
                        }
                    }

                    node_2 = getStaticNext(list, node_2);
                }
            }

            node_1 = getStaticNext(list, node_1);
        }
    }

    fclose(txt);
}

void dpi(List list, char* qry, float x, float y, bool useStaticList){

    FILE* txt = fopen(qry, "a");

    if(txt == NULL){

        puts("ERRO\nNao foi possivel gerar arquivo de relatorio .txt.");
        return;
    }

    fprintf(txt, "dpi %.2f %.2f\n", x, y);

    Node node;
    Node temp;

    if(useStaticList == 0){

        node = getNode(list, 0);

        while(node != NULL){

            if(isPointInternal(x, y, getData(node))){

                PrivateData* toWrite = (PrivateData*) getData(node);

                fprintf(txt, "%s\n", toWrite->id);

                temp = getNext(list, node);

                delNode(list, node);

                node = temp;

            } else node = getNext(list, node);
        }
    
    } else{

        node = getStaticFirst(list);

        while(node != NULL){

            if(isPointInternal(x, y, getStaticData(node))){

                PrivateData* toWrite = (PrivateData*) getStaticData(node);

                fprintf(txt, "%s\n", toWrite->id);

                temp = getStaticNext(list, node);

                delStaticNode(list, node);

                node = temp;

            } else node = getStaticNext(list, node);
        }
    }

    fclose(txt);
}

void dr(List list, char* qry, char* id, bool useStaticList){

    FILE* txt = fopen(qry, "a");

    if(txt == NULL){

        puts("ERRO\nNao foi possivel gerar arquivo de relatorio .txt.");
        return;
    }

    fprintf(txt, "dr %s\n", id);

    PrivateData* privateData;
    Node rect;
    Node temp;

    if(useStaticList == 0){

        rect = getFirst(list);

        privateData = (PrivateData*) getData(rect);

        while(rect != NULL && strcmp(id, privateData->id)){

            rect = getNext(list, rect);
            privateData = (PrivateData*) getData(rect);
        }

        rect = getFirst(list);

        while(rect != NULL){
                
            if(isRectInternal(privateData->x, privateData->y, privateData->w, privateData->h, getData(rect))
               && (PrivateData*) getData(rect) != privateData){

                PrivateData* toWrite = (PrivateData*) getData(rect);

                fprintf(txt, "%s\n", toWrite->id);

                temp = getNext(list, rect);

                delNode(list, rect);

                rect = temp;

            } else rect = getNext(list, rect);
        }

    } else{

        rect = getStaticFirst(list);

        privateData = (PrivateData*) getStaticData(rect);

        while(rect != NULL && strcmp(id, privateData->id)){

            rect = getStaticNext(list, rect);
            privateData = (PrivateData*) getStaticData(rect);
        }

        rect = getStaticFirst(list);
        PrivateData* toWrite;

        while(rect != NULL){

            if(isRectInternal(privateData->x, privateData->y, privateData->w, privateData->h, getStaticData(rect)) 
               && (PrivateData*) getStaticData(rect) != privateData){

                toWrite = (PrivateData*) getStaticData(rect);

                fprintf(txt, "%s\n", toWrite->id);

                delStaticNode(list, rect);
            }

            rect = getStaticNext(list, rect); 
        }
    }

    fclose(txt);
}

void bbi(List list, char* qry, float x, float y, bool useStaticList){

    FILE* txt = fopen(qry, "a");

    if(txt == NULL){

        puts("ERRO\nNao foi possivel gerar arquivo de relatorio .txt.");
        return;
    }

    fprintf(txt, "bbi %f %f\n", x, y);

    PrivateData rect_s;

    Node rect;
    float minX = 999999999;
    float minY = 999999999;
    float maxW = 0;
    float maxH = 0;

    if(useStaticList == 0){

        rect = getFirst(list);

        while(rect != NULL){

            if(isPointInternal(x, y, getData(rect))){

                PrivateData* toWrite = (PrivateData*) getData(rect);

                fprintf(txt, "%s %s %s\n", toWrite->id, toWrite->fill, toWrite->stroke);

                swapRect(&toWrite->fill, &toWrite->stroke);
 
                if(toWrite->x < minX) minX = toWrite->x;
                if(toWrite->y < minY) minY = toWrite->y;
                if(toWrite->w + toWrite->x > maxW) maxW = toWrite->w + toWrite->x;
                if(toWrite->h + toWrite->y > maxH) maxH = toWrite->h + toWrite->y;
            }

            rect = getNext(list, rect);
        }

        rect_s.id = calloc(sizeof("#bbi"), sizeof(char));
        sprintf(rect_s.id, "#bbi");
        rect_s.stroke = calloc(sizeof("red"), sizeof(char));
        sprintf(rect_s.stroke, "red");
        rect_s.fill = calloc(sizeof("9999.000000 9999.000000"), sizeof(char));
        sprintf(rect_s.fill, "%f %f", x, y);

        rect_s.x = minX;
        rect_s.y = minY;
        rect_s.w = maxW-minX;
        rect_s.h = maxH-minY;

        push(list, &rect_s);

        free(rect_s.id);
        free(rect_s.fill);
        free(rect_s.stroke);

        // point(svg, x, y, 6, "blue", "yellow");
        // boundingBox(svg, minX, minY, maxW-minX, maxH-minY, 2, "red");

    } else{

        rect = getStaticFirst(list);

        while(rect != NULL){

            if(isPointInternal(x, y, getStaticData(rect))){

                PrivateData* toWrite = (PrivateData*) getStaticData(rect);

                fprintf(txt, "%s %s %s\n", toWrite->id, toWrite->fill, toWrite->stroke);

                swapRect(&toWrite->fill, &toWrite->stroke);
 
                if(toWrite->x < minX) minX = toWrite->x;
                if(toWrite->y < minY) minY = toWrite->y;
                if(toWrite->w + toWrite->x > maxW) maxW = toWrite->w + toWrite->x;
                if(toWrite->h + toWrite->y > maxH) maxH = toWrite->h + toWrite->y;
            }

            rect = getStaticNext(list, rect);
        }

        rect_s.id = calloc(sizeof("#bbi"), sizeof(char));
        sprintf(rect_s.id, "#bbi");
        rect_s.stroke = calloc(sizeof("red"), sizeof(char));
        sprintf(rect_s.stroke, "red");
        rect_s.fill = calloc(sizeof("9999.000000 9999.000000"), sizeof(char));
        sprintf(rect_s.fill, "%f %f", x, y);

        rect_s.x = minX;
        rect_s.y = minY;
        rect_s.w = maxW-minX;
        rect_s.h = maxH-minY;

        staticPush(list, &rect_s);

        free(rect_s.id);
        free(rect_s.fill);
        free(rect_s.stroke);

        // point(svg, x, y, 6, "blue", "yellow");
        // boundingBox(svg, minX, minY, maxW-minX, maxH-minY, 2, "red");
    }

    fclose(txt);
}

void bbid(List list, char* qry, char* id, bool useStaticList){

    FILE* txt = fopen(qry, "a");

    if(txt == NULL){

        puts("ERRO\nNao foi possivel gerar arquivo de relatorio .txt.");
        return;
    }

    fprintf(txt, "bbid %s\n", id);

    Node rect;
    Node temp;
    PrivateData* temp_2;
    PrivateData* temp_3;
    float minX = 999999999;
    float minY = 999999999;
    float maxW = 0;
    float maxH = 0;

    PrivateData rect_s;

    if(useStaticList == 0){

        temp = getFirst(list);
        temp_2 = getData(temp);

        while(temp != NULL && strcmp(temp_2->id, id)){

            temp = getNext(list, temp);
            temp_2 = getData(temp);
        }

        rect = getFirst(list);

        while(rect != NULL){

            temp_3 = getData(rect);

            if(isRectInternal(temp_2->x, temp_2->y, temp_2->w, temp_2->h, temp_3)
                && strcmp(temp_3->id, id)){

                PrivateData* toWrite = (PrivateData*) getData(rect);

                fprintf(txt, "%s %s %s\n", toWrite->id, toWrite->fill, toWrite->stroke);

                swapRect(&toWrite->fill, &toWrite->stroke);
 
                if(toWrite->x < minX) minX = toWrite->x;
                if(toWrite->y < minY) minY = toWrite->y;
                if(toWrite->w + toWrite->x > maxW) maxW = toWrite->w + toWrite->x;
                if(toWrite->h + toWrite->y > maxH) maxH = toWrite->h + toWrite->y;
            }

            rect = getNext(list, rect);
        }

        rect_s.id = calloc(sizeof("#bbi"), sizeof(char));
        sprintf(rect_s.id, "#bbi");
        rect_s.stroke = calloc(sizeof("red"), sizeof(char));
        sprintf(rect_s.stroke, "red");
        rect_s.fill = calloc(sizeof("0 0"), sizeof(char));
        sprintf(rect_s.fill, "0 0");

        rect_s.x = minX;
        rect_s.y = minY;
        rect_s.w = maxW-minX;
        rect_s.h = maxH-minY;

        push(list, &rect_s);

        free(rect_s.id);
        free(rect_s.fill);
        free(rect_s.stroke);

    } else{

        temp = getStaticFirst(list);
        temp_2 = getStaticData(temp);

        while(temp != NULL && strcmp(temp_2->id, id)){

            temp = getStaticNext(list, temp);
            temp_2 = getStaticData(temp);
        }

        rect = getStaticFirst(list);

        while(rect != NULL){

            temp_3 = getStaticData(rect);

            if(isRectInternal(temp_2->x, temp_2->y, temp_2->w, temp_2->h, temp_3)
                && strcmp(temp_3->id, temp_2->id)){

                PrivateData* toWrite = (PrivateData*) getStaticData(rect);

                fprintf(txt, "%s %s %s\n", toWrite->id, toWrite->fill, toWrite->stroke);

                swapRect(&toWrite->fill, &toWrite->stroke);
 
                if(toWrite->x < minX) minX = toWrite->x;
                if(toWrite->y < minY) minY = toWrite->y;
                if(toWrite->w + toWrite->x > maxW) maxW = toWrite->w + toWrite->x;
                if(toWrite->h + toWrite->y > maxH) maxH = toWrite->h + toWrite->y;
            }

            rect = getStaticNext(list, rect);
        }

        rect_s.id = calloc(sizeof("#bbi"), sizeof(char));
        sprintf(rect_s.id, "#bbi");
        rect_s.stroke = calloc(sizeof("red"), sizeof(char));
        sprintf(rect_s.stroke, "red");
        rect_s.fill = calloc(sizeof("0 0"), sizeof(char));
        sprintf(rect_s.fill, "0 0");

        rect_s.x = minX;
        rect_s.y = minY;
        rect_s.w = maxW-minX;
        rect_s.h = maxH-minY;

        staticPush(list, &rect_s);

        free(rect_s.id);
        free(rect_s.fill);
        free(rect_s.stroke);
    }

    fclose(txt);
}

void iid(List list, char* qry, char* id, int k, bool useStaticList){

    FILE* txt = fopen(qry, "a");

    if(txt == NULL){

        puts("ERRO\nNao foi possivel gerar arquivo de relatorio .txt.");
        return;
    }

    fprintf(txt, "iid %s %d\n", id, k);

    Node temp;
    PrivateData* temp_2;

    if(useStaticList == 0){

        temp = getFirst(list);
        temp_2 = getData(temp);

        while(temp != NULL && strcmp(temp_2->id, id)){

            temp = getNext(list, temp);
            temp_2 = getData(temp);
        }

        while(temp != NULL && k <= 0){

            fprintf(txt, "%s %f %f %f %f %s %s\n", temp_2->id, temp_2->x, temp_2->y, temp_2->w, temp_2->h, temp_2->fill, temp_2->stroke);

            temp = getPrevious(list, temp);
            temp_2 = getData(temp);
        
            k++;
        }

        temp = NULL;

        while(temp != NULL && k >= 0){

            fprintf(txt, "%s %f %f %f %f %s %s\n", temp_2->id, temp_2->x, temp_2->y, temp_2->w, temp_2->h, temp_2->fill, temp_2->stroke);

            temp = getNext(list, temp);
            temp_2 = getData(temp);
        
            k--;
        }

    } else {

        temp = getStaticFirst(list);
        temp_2 = getStaticData(temp);

        while(temp != NULL && strcmp(temp_2->id, id)){

            temp = getStaticNext(list, temp);
            temp_2 = getStaticData(temp);
        }

        while(temp != NULL && k <= 0){

            fprintf(txt, "%s %f %f %f %f %s %s\n", temp_2->id, temp_2->x, temp_2->y, temp_2->w, temp_2->h, temp_2->fill, temp_2->stroke);

            temp = getStaticPrevious(list, temp);
            temp_2 = getStaticData(temp);
        
            k++;
        }

        while(temp != NULL && k >= 0){

            fprintf(txt, "%s %f %f %f %f %s %s\n", temp_2->id, temp_2->x, temp_2->y, temp_2->w, temp_2->h, temp_2->fill, temp_2->stroke);

            temp = getStaticNext(list, temp);
            temp_2 = getStaticData(temp);
        
            k--;
        }
    }

    fclose(txt);
}

void diid(List list, char* qry, char* id, int k, bool useStaticList){

    FILE* txt = fopen(qry, "a");

    if(txt == NULL){

        puts("ERRO\nNao foi possivel gerar arquivo de relatorio .txt.");
        return;
    }

    fprintf(txt, "diid %s %d\n", id, k);

    Node temp, temp_3;
    PrivateData* temp_2;

    if(useStaticList == 0){

        temp = getFirst(list);
        temp_2 = getData(temp);

        while(temp != NULL && strcmp(temp_2->id, id)){

            temp = getNext(list, temp);
            temp_2 = getData(temp);
        }

        while(temp != NULL && k < 0){

            fprintf(txt, "%s %f %f %f %f %s %s\n", temp_2->id, temp_2->x, temp_2->y, temp_2->w, temp_2->h, temp_2->fill, temp_2->stroke);

            temp_3 = getPrevious(list, temp);

            temp_2 = getData(temp_3);

            delNode(list, temp);

            temp = temp_3;
        
            k++;
        }

        while(temp != NULL && k > 0){

            fprintf(txt, "%s %f %f %f %f %s %s\n", temp_2->id, temp_2->x, temp_2->y, temp_2->w, temp_2->h, temp_2->fill, temp_2->stroke);
            
            temp_3 = getNext(list, temp);

            temp_2 = getData(temp_3);

            delNode(list, temp);

            temp = temp_3;

            k--;
        }

    } else {

        temp = getStaticFirst(list);
        temp_2 = getStaticData(temp);

        while(temp != NULL && strcmp(temp_2->id, id)){

            temp = getStaticNext(list, temp);
            temp_2 = getStaticData(temp);
        }

        while(temp != NULL && k < 0){

            fprintf(txt, "%s %f %f %f %f %s %s\n", temp_2->id, temp_2->x, temp_2->y, temp_2->w, temp_2->h, temp_2->fill, temp_2->stroke);

            temp_3 = getStaticPrevious(list, temp);

            temp_2 = getStaticData(temp_3);

            delStaticNode(list, temp);

            temp = temp_3;
        
            k++;
        }

        while(temp != NULL && k > 0){

            fprintf(txt, "%s %f %f %f %f %s %s\n", temp_2->id, temp_2->x, temp_2->y, temp_2->w, temp_2->h, temp_2->fill, temp_2->stroke);

            temp_3 = getStaticNext(list, temp);

            temp_2 = getStaticData(temp_3);

            delStaticNode(list, temp);

            temp = temp_3;

            k--;
        }
    }

    fclose(txt);
}

static void colorize(Node node_1, Node node_2, bool useStaticList){

    unsigned int random = rand() % 0x1000000;

    if(useStaticList){

        PrivateData* rect_1 = (PrivateData*) getData(node_1);
        PrivateData* rect_2 = (PrivateData*) getData(node_2);

        char* temp = realloc(rect_1->fill, sizeof("#FFFFFF") * sizeof(char));

        if(temp == NULL)
            return;

        rect_1->fill = temp;
        snprintf(rect_1->fill, sizeof("#FFFFFF"), "#%X", random);

        char* node_2 = realloc(rect_2->fill, sizeof("#FFFFFF") * sizeof(char));

        if(node_2 == NULL)
            return;
        
        rect_2->fill = node_2;
        snprintf(rect_2->fill, sizeof("#FFFFFF"), "#%X", random);

    } else{

        PrivateData* rect_1 = (PrivateData*) getStaticData(node_1);
        PrivateData* rect_2 = (PrivateData*) getStaticData(node_2);

        char* temp = realloc(rect_1->fill, sizeof("#FFFFFF") * sizeof(char));

        if(temp == NULL)
            return;

        rect_1->fill = temp;
        snprintf(rect_1->fill, sizeof("#FFFFFF"), "#%X", random);

        char* node_2 = realloc(rect_2->fill, sizeof("#FFFFFF") * sizeof(char)+1);

        if(node_2 == NULL)
            return;
        
        rect_2->fill = node_2;
        snprintf(rect_2->fill, sizeof("#FFFFFF"), "#%X", random);
    }
}