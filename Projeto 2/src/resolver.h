#ifndef RESOLVER_H
#define RESOLVER_H

#include "types.h"

int solveGeo(Colony colony, Env env);

int solveSvg(Colony colony, Svg svgFile, Svg querries);

int solveQry(Colony colony, Svg svgFile, Svg querries, Env environment);

void drawTree(Tree tree, Svg svg, void (*draw)(Node, Svg));

#endif