#ifndef SVG_H
#define SVG_H

#include "types.h"

Svg newSvg(char* path);

Svg copySvg(Svg dest, const Svg svg);

Svg catSvg(Svg dest, const Svg source);

void drawRect(Svg svg, double x, double y, double w, double h, char* fill, double fill_op, char* stroke, double stroke_op, double stroke_width);

void drawRectLabel(Svg svg, double x, double y, double w, double h, char* fill, double fill_op, char* stroke, double stroke_op, double stroke_width, unsigned int sheltered);

void drawNve(Svg svg, double x, double y, double s);

void drawCircle(Svg svg, double x, double y, double r, char* id, char* fill, double fill_op, char* stroke, double stroke_op, double stroke_width, double start_x, double start_y, double radiation);

void drawCross(Svg svg, double x, double y, double r);

void drawLine(Svg svg, double x_1, double y_1, double x_2, double y_2, double width, char* color);

void drawPath(Svg svg, double x_1, double y_1, double x_2, double y_2);

void viewSvg(Svg svg);

void writeSvg(Svg svg);

void freeSvg(Svg svg);

#endif