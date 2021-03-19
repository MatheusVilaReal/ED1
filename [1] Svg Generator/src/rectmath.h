#ifndef RECTMATH_H
#define RECTMATH_H

#include "types.h"

bool rectOverlap(Data r1, Data r2);
bool isRectInternal(float x_1, float y_1, float w_1, float h_1, Data r2);
bool isPointInternal(float x, float y, Data r);
void swapRect(char** rect_1, char** rect_2);

#endif