#include <stdlib.h>
#include <math.h>

// #include "shapes.h"
#include "rectmath.h"

bool isPointInternal(double pt_x, double pt_y, double rect_x, double rect_y, double rect_w, double rect_h){

    if(pt_x < rect_x || pt_x > rect_x + rect_w)
        return false;

    if(pt_y < rect_y || pt_y > rect_y + rect_h)
        return false;

    return true;
}

bool isRectInternal(double x_1, double y_1, double w_1, double h_1, double x_2, double y_2, double w_2, double h_2){

    if(x_1 + w_1 >= x_2 + w_2 && x_1 <= x_2)
        if(y_1 + h_1 >= y_2 + h_2 && y_1 <= y_2)
            return true;

    return false;
}

bool isCircInternal(double x_1, double y_1, double r, double x_2, double y_2){

    return (hypot(x_2 - x_1, y_2 - y_1) < r);
}

double rand_range(double lower, double upper){

    return (fmod(rand(), upper - lower + 1) + lower);
}

double slope(double x_1, double y_1, double x_2, double y_2){

    return ((double) y_2 - y_1 / (double) x_2 - x_1);
}

// bool rectOverlap(Data r1, Data r2){

//     PrivateData* rect_1 = (PrivateData*) r1;
//     PrivateData* rect_2 = (PrivateData*) r2;

//     double x_1 = rect_1_x;
//     double y_1 = rect_1_y;
//     double w_1 = rect_1_w;
//     double h_1 = rect_1_h;
//     double x_2 = rect_2_x;
//     double y_2 = rect_2_y;
//     double w_2 = rect_2_w;
//     double h_2 = rect_2_h;

//     if(x_1 > x_2+w_2 || x_2 > x_1+w_1) 
//         return false;

//     if(y_1 > y_2+h_2 || y_2 > y_1+h_1)
//         return false;

//     return true;
// }

// bool isPointInternal(double x, double y, Data r){

//     PrivateData* rect = (PrivateData*) r;

//     if(x < rect_x || x > rect_x + rect_w)
//         return false;

//     if(y < rect_y || y > rect_y + rect_h)
//         return false;

//     return true;
// }

// void swapRect(char** rect_1, char** rect_2){

//     char* temp = *rect_1;
//     *rect_1 = *rect_2;
//     *rect_2 = temp;
// }