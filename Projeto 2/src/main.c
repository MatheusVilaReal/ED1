#include <stdio.h>

#include "environment.h"
#include "resolver.h"
#include "colony.h"
#include "svg.h"

#include "shapes.h"
#include "bintree.h"

int main(int argc, char* argv[]){

    Env env = NULL;

    if((env = setEnv(argc, argv)) == NULL)
        return -1;

    Svg svg_1 = newSvg(solveSvgPath(env)), 
        svg_2 = newSvg(solveQrySvgPath(env)),
        querries = newSvg(solveQrySvgPath(env));

    Colony newGrimesland = buildColony();

    solveGeo(newGrimesland, env);
    solveSvg(newGrimesland, svg_1, NULL);

    // printTree(getRectangles(newGrimesland), printRect);

    solveQry(newGrimesland, svg_2, querries, env);
    solveSvg(newGrimesland, svg_2, querries);

    destroyColony(newGrimesland);

    writeSvg(svg_1);
    writeSvg(svg_2);

    freeSvg(svg_1);
    freeSvg(svg_2);
    freeSvg(querries);

    freeEnv(env);

    return 0;
}