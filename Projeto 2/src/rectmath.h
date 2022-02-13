#ifndef RECTMATH_H
#define RECTMATH_H

#include "types.h"

bool isPointInternal(double pt_x, double pt_y, double rect_x, double rect_y, double rect_w, double rect_h);

bool isRectInternal(double x_1, double y_1, double w_1, double h_1, double x_2, double y_2, double w_2, double h_2);

bool isCircInternal(double x_1, double y_1, double r, double x_2, double y_2);

double rand_range(double lower, double upper);

double slope(double x_1, double y_1, double x_2, double y_2);

// bool rectOverlap(Data r1, Data r2);

// bool isPointInternal(float x, float y, Data r);

// void swapRect(char** rect_1, char** rect_2);

#endif