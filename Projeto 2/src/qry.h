#ifndef QRY_H
#define QRY_H

#include "types.h"

void dpi(FILE* txt, Tree tree, double x, double y);

void dr(FILE* txt, Tree tree, const char* id);

void fg(FILE* txt, Svg svgFile, Tree rectangles, Tree circles, double x, double y, double r);

void im(FILE* txt, Colony colony, Svg svgfile, Tree circles, double x, double y, double s);

void t30(FILE* txt, Colony colony);

void nve(FILE* txt, Svg svgFile, Colony colony, double x, double y);

#endif