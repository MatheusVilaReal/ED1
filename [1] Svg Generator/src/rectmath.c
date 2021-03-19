#include <stdbool.h>

#include "rectmath.h"
#include "types.h"

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

bool rectOverlap(Data r1, Data r2){

    PrivateData* rect_1 = (PrivateData*) r1;
    PrivateData* rect_2 = (PrivateData*) r2;

    float x_1 = rect_1->x;
    float y_1 = rect_1->y;
    float w_1 = rect_1->w;
    float h_1 = rect_1->h;
    float x_2 = rect_2->x;
    float y_2 = rect_2->y;
    float w_2 = rect_2->w;
    float h_2 = rect_2->h;

    if(x_1 > x_2+w_2 || x_2 > x_1+w_1) 
        return false;

    if(y_1 > y_2+h_2 || y_2 > y_1+h_1)
        return false;

    return true;
}

bool isRectInternal(float x_1, float y_1, float w_1, float h_1, Data r2){

    PrivateData* rect_2 = (PrivateData*) r2;
    
    float x_2 = rect_2->x;
    float y_2 = rect_2->y;
    float w_2 = rect_2->w;
    float h_2 = rect_2->h;

    if(x_1+w_1 >= x_2+w_2 && x_1 <= x_2)
        if(y_1+h_1 >= y_2+h_2 && y_1 <= y_2)
            return true;

    return false;
}

bool isPointInternal(float x, float y, Data r){

    PrivateData* rect = (PrivateData*) r;

    if(x < rect->x || x > rect->x + rect->w)
        return false;

    if(y < rect->y || y > rect->y + rect->h)
        return false;

    return true;
}

void swapRect(char** rect_1, char** rect_2){

    char* temp = *rect_1;
    *rect_1 = *rect_2;
    *rect_2 = temp;
}