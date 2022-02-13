#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "colony.h"
#include "svg.h"
#include "rectmath.h"
#include "environment.h"
#include "bintree.h"
#include "qry.h"
#include "shapes.h"

#define PROPORTION 0.05

static void cmpPoint_traversal(FILE* txt, Tree tree, Node node, double x, double y){

    if(node == NULL)
        return;

    cmpPoint_traversal(txt, tree, getLeftChild(node), x, y);
    cmpPoint_traversal(txt, tree, getRightChild(node), x, y);

    if(isPointInternal(x, y, rect_getX(getNodeData(node)), rect_getY(getNodeData(node)), rect_getW(getNodeData(node)),
                       rect_getH(getNodeData(node)))){

        fprintf(txt, "%s\n", rect_getId(getNodeData(node)));

        kd_rmNode(tree, node, rect_getX, rect_getY, freeRect);
    }
}

static void cmpRect_traversal(FILE* txt, Tree tree, Node node, double x, double y, double w, double h){

    if(node == NULL)
        return;

    cmpRect_traversal(txt, tree, getLeftChild(node), x, y, w, h);
    cmpRect_traversal(txt, tree, getRightChild(node), x, y, w, h);

    if(isRectInternal(x, y, w, h, rect_getX(getNodeData(node)), rect_getY(getNodeData(node)),
                      rect_getW(getNodeData(node)), rect_getH(getNodeData(node)))){

        fprintf(txt, "%s\n", rect_getId(getNodeData(node)));

        kd_rmNode(tree, node, rect_getX, rect_getY, freeRect);
    }
}

static void cmpCirc_traversal(FILE* txt, Svg svgFile, Tree rectangles, Tree circles, Node node, double x, double y, double r){

    if(node == NULL)
        return;

    Shape circ = getNodeData(node);

    if(isCircInternal(x, y, r, circ_getX(circ), circ_getY(circ)) && circ_getShelter(circ) == 0){

        // printCirc(getNodeData(node));

        drawCircle(svgFile, circ_getX(circ), circ_getY(circ), circ_getR(circ), "@", "lightgray",            //Rastro cinza
                   OP_DEFAULT, "lightgray", OP_DEFAULT, WIDTH_DEFAULT, circ_getX(circ), circ_getY(circ), 0);

        Shape nearestRect = getNodeData(searchNearest(getRoot(rectangles), circ_getX(getNodeData(node)),
                                        circ_getY(getNodeData(node)), 0, rect_getX, rect_getY));

        rect_incSheltered(nearestRect);
        circ_updateShelter(circ, 1);
        // circ_updateRadiation(circ, s);

        double randX = rand_range(rect_getX(nearestRect) + circ_getR(circ), rect_getX(nearestRect) + rect_getW(nearestRect) - circ_getR(circ));
        double randY = rand_range(rect_getY(nearestRect) + circ_getR(circ), rect_getY(nearestRect) + rect_getH(nearestRect) - circ_getR(circ));

        drawPath(svgFile, circ_getX(circ), circ_getY(circ), randX, randY);

        circ_updateX(circ, randX);
        circ_updateY(circ, randY);

        fprintf(txt, "%s -> %s\n", circ_getId(circ), rect_getId(nearestRect));

        // insertNode(circles, circ, getNodeIndex(getRoot(circles)), kd_filter, circ_getX, circ_getY);
    }

    cmpCirc_traversal(txt, svgFile, rectangles, circles, getLeftChild(node), x, y, r);
    cmpCirc_traversal(txt, svgFile, rectangles, circles, getRightChild(node), x, y, r);
}

static void t30_traversal(FILE* txt, Node node){

    if(node == NULL)
        return;

    if(circ_getRadiation(getNodeData(node)) > 1000 && circ_getRadiation(getNodeData(node)) < 8000){

        circ_updateRadiation(getNodeData(node), 8000);
        fprintf(txt, "Morte apos 30 min: %s\n", circ_getId(getNodeData(node)));
    }

    t30_traversal(txt, getLeftChild(node));
    t30_traversal(txt, getRightChild(node));
}

static bool nve_traversal(Node node, double x, double y, bool pt_internal){

    if(node == NULL)
        return pt_internal;

    if(isPointInternal(x, y, rect_getX(getNodeData(node)), rect_getY(getNodeData(node)), rect_getW(getNodeData(node)),
                       rect_getH(getNodeData(node))))
        return 1;

    bool output = nve_traversal(node, x, y, pt_internal);
    output = nve_traversal(node, x, y, output);
}

static Node searchById(Node node, char* (*filter)(Shape), const char* key){

    if(node == NULL || strcmp(filter(getNodeData(node)), key) == 0)
        return node;

    Node node_2 = searchById(getLeftChild(node), filter, key);

    if(node_2 == NULL)
        node_2 = searchById(getRightChild(node), filter, key);

    return node_2;
}

void dpi(FILE* txt, Tree tree, double x, double y){

    cmpPoint_traversal(txt, tree, getRoot(tree), x, y);
}

void dr(FILE* txt, Tree tree, const char* id){

    Node foundNode = searchById(getRoot(tree), rect_getId, id);

    cmpRect_traversal(txt, tree, getRoot(tree), rect_getX(getNodeData(foundNode)), rect_getY(getNodeData(foundNode)),
                      rect_getW(getNodeData(foundNode)), rect_getH(getNodeData(foundNode)));
}

void fg(FILE* txt, Svg svgFile, Tree rectangles, Tree circles, double x, double y, double r){

    drawCircle(svgFile, x, y, r, "@", "@", 0, "red", OP_DEFAULT, WIDTH_DEFAULT, x, y, 0);                  //Área de fuga

    cmpCirc_traversal(txt, svgFile, rectangles, circles, getRoot(circles), x, y, r);
}

void im(FILE* txt, Colony colony, Svg svgfile, Tree circles, double x, double y, double s){

    drawCircle(svgfile, x, y, s * PROPORTION, "im", "dimgray", OP_DEFAULT, "gray", OP_DEFAULT, WIDTH_DEFAULT, x, y, 0);

    updateRadiation(txt, colony, s);

    getShadows(colony, x, y);
}

void t30(FILE* txt, Colony colony){

    t30_traversal(txt, getRoot(getCircles(colony)));
}

void nve(FILE* txt, Svg svgFile, Colony colony, double x, double y){

    fprintf(txt, "x: %lf y: %lf s: %lf\n", x, y, colony_getRadiation(colony));

    drawNve(svgFile, x, y, colony_getRadiation(colony));
}