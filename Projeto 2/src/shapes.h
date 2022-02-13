#ifndef SHAPES_H
#define SHAPES_H

#include "types.h"

#define OP_DEFAULT 0.8F
#define WIDTH_DEFAULT 0.15F

Shape newRect(const char* id, double x, double y, double w, double h, const Style style);

Shape newCirc(const char* id, double x, double y, double r, const Style style);

Shape newPolyg(const char* id, Point* points, unsigned long n_pts);

Shape newSegment(Point a, Point b);

Point newPoint(double x, double y);

char* rect_getId(Shape rect);

Style rect_getStyle(Shape rect);

double rect_getX(Shape rect);

double rect_getY(Shape rect);

double rect_getW(Shape rect);

double rect_getH(Shape rect);

char* circ_getId(Shape circ);

Style circ_getStyle(Shape circ);

double circ_getX(Shape circ);

double circ_getY(Shape circ);

double circ_getStartX(Shape circ);

double circ_getStartY(Shape circ);

double circ_getR(Shape circ);

double circ_getRadiation(Shape circ);

Style newStyle(char* fill, float fill_op, char* stroke, double stroke_op, double stroke_width, int dasharray);

char* getFill(Style style);

char* getStroke(Style style);

double getFill_op(Style style);

double getStroke_op(Style style);

double getStroke_width(Style style);

unsigned int rect_getSheltered(Shape rect);

int getDasharray(Style style);

double pt_getX(Point point);

double pt_getY(Point point);

int compareRect_x(const void* a, const void* b);

int compareRect_y(const void* a, const void* b);

int compareCirc_x(const void* a, const void* b);

int compareCirc_y(const void* a, const void* b);

int comparePoint(const void* a, const void* b);

bool circ_getShelter(Shape circ);

void circ_updateX(Shape circ, double newX);

void circ_updateY(Shape circ, double newY);

void circ_updateRadiation(Shape circ, double radiation);

void circ_updateShelter(Shape circ, bool isSheltered);

void rect_incSheltered(Shape rect);

void printRect(Shape rect);

void printCirc(Shape circ);

void printPolyg(Shape polyg);

void printPoint(Point point);

void freeStyle(Style style);

void freeRect(Shape rect);

void freeCirc(Shape circ);

void freePolyg(Shape polyg);

void freePoint(Point point);

#endif