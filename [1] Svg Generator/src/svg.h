#ifndef SVG_H
#define SVG_H

typedef void* Svg;

Svg newSvg();
Svg drawGraph(char* dataInput, char* name);
void rectangle(Svg svg, void* data);
void boundingBox(Svg svg, float x, float y, float w, float h, float width, char* color);
void rectangleCoord(Svg svg, float x, float y, float w, float h, float fill_opacity, char* fill, char* stroke);
void point(Svg svg, float x, float y, float r, char* fill, char* stroke);
void line(Svg svg, float x_1, float y_1, float x_2, float y_2, float width, char* color);
void numText(Svg svg, float x, float y, float size, char* fill, char* stroke, int num);
void saveSvg(Svg file, const char* path);
void freeSvg(Svg svg);

#endif