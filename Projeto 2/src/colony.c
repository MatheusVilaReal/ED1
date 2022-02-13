#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "rectmath.h"
#include "shapes.h"
#include "bintree.h"
#include "colony.h"

#define RENDER_BOX 10000

typedef struct Colony{

    Tree rectangles, circles, shadows;

    double radiation;

    bool fg;

} Colony_t;

static void updateCircles(FILE* txt, Node node, double radiation){

    if(node == NULL)
        return;

    if(!circ_getShelter(getNodeData(node)))
        circ_updateRadiation(getNodeData(node), radiation);
    else
        circ_updateRadiation(getNodeData(node), 0);

    if(circ_getRadiation(getNodeData(node)) > 8000)
        fprintf(txt, "Morte instantanea: %s\n", circ_getId(getNodeData(node)));
        
    else if(circ_getRadiation(getNodeData(node)) > 1000)
        fprintf(txt, "Morte iminente: %s\n", circ_getId(getNodeData(node)));


    updateCircles(txt, getLeftChild(node), radiation);
    updateCircles(txt, getRightChild(node), radiation);
}

static void traverse_rect(Node rect, Node circ){

    if(rect == NULL)
        return;

    if(isPointInternal(circ_getX(getNodeData(circ)), circ_getY(getNodeData(circ)), rect_getX(getNodeData(rect)), rect_getY(getNodeData(rect)),
                       rect_getW(getNodeData(rect)), rect_getH(getNodeData(rect)))){

        circ_updateShelter(getNodeData(circ), 1);
        rect_incSheltered(getNodeData(rect));
    }

    traverse_rect(getLeftChild(rect), circ);
    traverse_rect(getRightChild(rect), circ);
}

static void wrapped_updateSheltered(Node rect, Node circ){

    if(circ == NULL)
        return;

    traverse_rect(rect, circ);    

    wrapped_updateSheltered(rect, getLeftChild(circ));
    wrapped_updateSheltered(rect, getRightChild(circ));
}

static void wrapped_getShadows(Node node, double x, double y){

    if(node == NULL)
        return;

    Point points[] = {newPoint(rect_getX(getNodeData(node)), rect_getY(getNodeData(node))),
                      newPoint(rect_getX(getNodeData(node)) + rect_getW(getNodeData(node)), rect_getY(getNodeData(node))),
                      newPoint(rect_getX(getNodeData(node)), rect_getY(getNodeData(node)) + rect_getH(getNodeData(node))),
                      newPoint(rect_getX(getNodeData(node)) + rect_getW(getNodeData(node)), rect_getY(getNodeData(node)) + rect_getH(getNodeData(node)))};

    // for(int i = 0; i < 4; i++)
    //     printPoint(points[i]);

    // printf("\n");

    qsort(points, 4, sizeof(Point), comparePoint);

    // for(int i = 0; i < 4; i++)
    //     printPoint(points[i]);

    // printf("\n");

    Point polyg_pts[4] = {points[0], points[3],
                          NULL, NULL};

    for(int i = 0; i < 2; i++){
      
        if(x, y, pt_getX(polyg_pts[i]), pt_getY(polyg_pts[i])) > slope(x, y, RENDER_BOX, 0)){

            
        }
    }
    freePoint(points[1]);
    freePoint(points[2]);

    wrapped_getShadows(getLeftChild(node), x, y);
    wrapped_getShadows(getRightChild(node), x, y);
}

Colony buildColony(){

    Colony_t* colony = (Colony_t*) malloc(sizeof(Colony_t));

    colony->rectangles = newTree();
    colony->circles = newTree();
    colony->radiation = 0;

    colony->fg = 0;

    return colony;
}

Tree getRectangles(Colony colony){

    if(colony == NULL)
        return NULL;

    Colony_t* toGet = (Colony_t*) colony;

    return toGet->rectangles;
}

Tree getCircles(Colony colony){

    if(colony == NULL)
        return NULL;

    Colony_t* toGet = (Colony_t*) colony;

    return toGet->circles;
}

double colony_getRadiation(Colony colony){

    if(colony == NULL)
        return 0;

    Colony_t* toGet = (Colony_t*) colony;

    return toGet->radiation;
}

void updateSheltered(Colony colony){

    wrapped_updateSheltered(getRoot(getRectangles(colony)), getRoot(getCircles(colony)));
}

void updateRadiation(FILE* txt, Colony colony, double radiation){

    Colony_t* toRefresh = (Colony_t*) colony;

    toRefresh->radiation += radiation;

    updateCircles(txt, getRoot(getCircles(colony)), radiation);
}

void updateFg(Colony colony){

    if(colony == NULL)
        return;

    Colony_t* toUpdate = (Colony_t*) colony;

    toUpdate->fg = 1;
}

void getShadows(Colony colony, double x, double y){

    // Colony_t* toGet = (Colony_t*) colony;

    wrapped_getShadows(getRoot(getRectangles(colony)), x, y);
}

bool getFg(Colony colony){

    if(colony == NULL)
        return 0;

    Colony_t* toGet = (Colony_t*) colony;

    return toGet->fg;
}

void destroyColony(Colony colony){

    Colony_t* toDestroy = (Colony_t*) colony;

    chopTree(toDestroy->rectangles, freeRect);
    chopTree(toDestroy->circles, freeCirc);

    free(toDestroy);
}