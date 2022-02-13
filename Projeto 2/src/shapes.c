#include <stdio.h>          //snprintf
#include <stdlib.h>         //malloc
#include <string.h>         //strlen
#include <math.h>           //atan2

#include "shapes.h"

/*NÍVEIS DE RADADIÇÃO*/
#define MSV_25   "#00FFFF"
#define MSV_50   "#00FF00"
#define MSV_100  "#FF00FF"
#define MSV_250  "#0000FF"
#define MSV_600  "#800080"
#define MSV_1000 "#000080"
#define MSV_8000 "#FF0000"
#define MSV_8001 "#000000"

typedef struct Point{

    double x, y;

} Point_t;

typedef struct Segment{

    Point_t a, b;

} Seg_t;

typedef struct Style{

    char* fill, *stroke;
    double fill_op, stroke_op, stroke_width;
    int dasharrayStroke;

} Style_t;

typedef struct Circle{

    char* id;
    Style_t* style;

    Point_t anchor, startPos;
    double r;

    double radiation;

    bool sheltered;

} Circ_t;

typedef struct Rectangle{

    char* id;
    Style_t* style;

    Point_t anchor;
    double w, h;

    unsigned int sheltered;

} Rect_t;

typedef struct Polygon{

    char* id;
    Style_t* style;

    Point_t* points[5];
    unsigned long n_pts;

} Polyg_t;

Shape newRect(const char* id, double x, double y, double w, double h, const Style style){

    Rect_t* rect = (Rect_t*) malloc(sizeof(Rect_t));

    rect->id = (char*) calloc(sizeof(char), strlen(id) + 1);
    rect->id = strncpy(rect->id, id, strlen(id));

    rect->style = style;

    rect->anchor.x = x;
    rect->anchor.y = y;
    rect->w = w;
    rect->h = h;

    rect->sheltered = 0;

    return rect;
}

Shape newCirc(const char* id, double x, double y, double r, const Style style){

    Circ_t* circ = (Circ_t*) malloc(sizeof(Circ_t));

    circ->id = (char*) calloc(sizeof(char), strlen(id) + 1);
    circ->id = strncpy(circ->id, id, strlen(id));

    circ->style = style;

    circ->anchor.x = x;
    circ->startPos.x = x;
    circ->anchor.y = y;
    circ->startPos.y = y;
    circ->r = r;

    circ->radiation = 0;

    circ->sheltered = 0;

    return circ;
}

Shape newPolyg(const char* id, Point* points, unsigned long n_pts){

    Polyg_t* polygon = (Polyg_t*) malloc(sizeof(Polyg_t));
    Point_t** pts = (Point_t**) points;

    polygon->id = (char*) calloc(sizeof(char), strlen(id) + 1);
    polygon->id = strncpy(polygon->id, id, strlen(id));

    for(unsigned long i = 0; i < n_pts; i++)
        polygon->points[i] = pts[i];

    polygon->n_pts = n_pts;

    return polygon;
}

Shape newSegment(Point a, Point b){

    Seg_t* segment = (Seg_t*) malloc(sizeof(Seg_t));

    Point_t* newA = (Point_t*) a;
    Point_t* newB = (Point_t*) b;

    segment->a.x = newA->x;
    segment->a.y = newA->y;

    segment->b.x = newB->x;
    segment->b.y = newB->y;

    return segment;
}

Point newPoint(double x, double y){

    Point_t* point = (Point_t*) malloc(sizeof(Point_t));

    point->x = x;
    point->y = y;

    return point;
}

char* rect_getId(Shape rect){

    if(rect == NULL)
        return 0;

    Rect_t* toGet = (Rect_t*) rect;

    return toGet->id;
}

double rect_getX(Shape rect){

    if(rect == NULL)
        return 0;

    Rect_t* toGet = (Rect_t*) rect;

    return toGet->anchor.x;
}

double rect_getY(Shape rect){

    if(rect == NULL)
        return 0;

    Rect_t* toGet = (Rect_t*) rect;

    return toGet->anchor.y;
}

double rect_getW(Shape rect){

    if(rect == NULL)
        return 0;

    Rect_t* toGet = (Rect_t*) rect;

    return toGet->w;
}

double rect_getH(Shape rect){

    if(rect == NULL)
        return 0;

    Rect_t* toGet = (Rect_t*) rect;

    return toGet->h;
}

Style rect_getStyle(Shape rect){

    if(rect == NULL)
        return NULL;

    Rect_t* toGet = (Rect_t*) rect;

    return toGet->style;
}

char* circ_getId(Shape circ){

    if(circ == NULL)
        return 0;

    Circ_t* toGet = (Circ_t*) circ;

    return toGet->id;
}

double circ_getX(Shape circ){

    if(circ == NULL)
        return 0;

    Circ_t* toGet = (Circ_t*) circ;

    return toGet->anchor.x;
}

double circ_getY(Shape circ){

    if(circ == NULL)
        return 0;

    Circ_t* toGet = (Circ_t*) circ;

    return toGet->anchor.y;
}

double circ_getStartX(Shape circ){

    if(circ == NULL)
        return 0;

    Circ_t* toGet = (Circ_t*) circ;

    return toGet->startPos.x;
}

double circ_getStartY(Shape circ){

    if(circ == NULL)
        return 0;

    Circ_t* toGet = (Circ_t*) circ;

    return toGet->startPos.y;
}

double circ_getR(Shape circ){

    if(circ == NULL)
        return 0;

    Circ_t* toGet = (Circ_t*) circ;

    return toGet->r;
}

double circ_getRadiation(Shape circ){

    if(circ == NULL)
        return 0;
    
    Circ_t* toGet = (Circ_t*) circ;

    return toGet->radiation;
}

Style circ_getStyle(Shape circ){

    if(circ == NULL)
        return NULL;

    Rect_t* toGet = (Rect_t*) circ;

    return toGet->style;
}

Style newStyle(char* fill, float fill_op, char* stroke, double stroke_op, double stroke_width, int dasharray){

    Style_t* style = (Style_t*) malloc(sizeof(Style_t));

    if(strcmp(fill, "@")){

        style->fill = (char*) calloc(strlen(fill) + 1, sizeof(char));
        style->fill = strncpy(style->fill, fill, strlen(fill));

    } else{

        style->fill = (char*) calloc(sizeof("transparent"), sizeof(char));
        snprintf(style->fill, sizeof("transparent") * sizeof(char), "transparent");
    }

    style->stroke = (char*) calloc(strlen(stroke) + 1, sizeof(char));
    style->stroke = strncpy(style->stroke, stroke, strlen(stroke));

    style->fill_op = fill_op;
    style->stroke_op = stroke_op;
    style->stroke_width = stroke_width;

    style->dasharrayStroke = dasharray;

    return style;
}

char* getFill(Style style){

    if(style == NULL)
        return NULL;

    Style_t* toGet = (Style_t*) style;

    return toGet->fill;
}

char* getStroke(Style style){

    Style_t* toGet = (Style_t*) style;

    if(style == NULL)
        return NULL;

    return toGet->stroke;
}

double getFill_op(Style style){

    if(style == NULL)
        return 0;

    Style_t* toGet = (Style_t*) style;

    return toGet->fill_op;
}

double getStroke_op(Style style){

    if(style == NULL)
        return 0;

    Style_t* toGet = (Style_t*) style;

    return toGet->stroke_op;
}

double getStroke_width(Style style){

    if(style == NULL)
        return 0;

    Style_t* toGet = (Style_t*) style;

    return toGet->stroke_width;
}

unsigned int rect_getSheltered(Shape rect){

    if(rect == NULL)
        return 0;

    Rect_t* toGet = (Rect_t*) rect;

    return toGet->sheltered;
}

int getDasharray(Style style){

    if(style == NULL)
        return 0;

    Style_t* toGet = (Style_t*) style;

    return toGet->dasharrayStroke;
}

double pt_getX(Point point){

    if(point == NULL)
        return 0;

    Point_t* toGet = (Point_t*) point;

    return toGet->x;
}

double pt_getY(Point point){

    if(point == NULL)
        return 0;

    Point_t* toGet = (Point_t*) point;

    return toGet->y;
}

int compareRect_x(const void* a, const void* b){

    // int x_a = (int) (*((Rect_t**) a))->anchor.x, x_b = (int) (*((Rect_t**) b))->anchor.x;

    // return (x_a - x_b);

    Rect_t** A = (Rect_t**) a, 
          ** B = (Rect_t**) b;

    if((*A)->anchor.x < (*B)->anchor.x)
        return -1;
    else
        if((*A)->anchor.x == (*B)->anchor.x)
            return 0;
        else
            return 1;         
}

int compareRect_y(const void* a, const void* b){

    // int y_a = (int) (*((Rect_t**) a))->anchor.y, y_b = (int) (*((Rect_t**) b))->anchor.y;

    // return (y_a - y_b);

    Rect_t** A = (Rect_t**) a, 
          ** B = (Rect_t**) b;

    if((*A)->anchor.y < (*B)->anchor.y)
        return -1;
    else
        if((*A)->anchor.y == (*B)->anchor.y)
            return 0;
        else
            return 1;  
}

int compareCirc_x(const void* a, const void* b){

    // int x_a = (int) (*((Circ_t**) a))->anchor.x, x_b = (int) (*((Circ_t**) b))->anchor.x;

    // return (x_a - x_b);

    Circ_t** A = (Circ_t**) a, 
          ** B = (Circ_t**) b;

    if((*A)->anchor.x < (*B)->anchor.x)
        return -1;
    else
        if((*A)->anchor.x == (*B)->anchor.x)
            return 0;
        else
            return 1;
}

int compareCirc_y(const void* a, const void* b){

    // int y_a = (int) (*((Circ_t**) a))->anchor.y, y_b = (int) (*((Circ_t**) b))->anchor.y;

    // return (y_a - y_b);

    Circ_t** A = (Circ_t**) a, 
          ** B = (Circ_t**) b;

    if((*A)->anchor.y < (*B)->anchor.y)
        return -1;
    else
        if((*A)->anchor.y == (*B)->anchor.y)
            return 0;
        else
            return 1;
}

int comparePoint(const void* a, const void* b){

    Point_t** pt_a = (Point_t**) a;
    Point_t** pt_b = (Point_t**) b;

    return ((atan2((*pt_a)->y, (*pt_a)->x) > atan2((*pt_b)->y, (*pt_b)->x))? 1 : -1);
}

void circ_updateX(Shape circ, double newX){

    if(circ == NULL)
        return;

    Circ_t* toUpdate = (Circ_t*) circ;

    toUpdate->anchor.x = newX;
}

void circ_updateY(Shape circ, double newY){

    if(circ == NULL)
        return;

    Circ_t* toUpdate = (Circ_t*) circ;

    toUpdate->anchor.y = newY;
}

void circ_updateRadiation(Shape circ, double radiation){

    if(circ == NULL)
        return;

    Circ_t* toUpdate = (Circ_t*) circ;

    toUpdate->radiation += radiation;

    char* temp = (char*) realloc(toUpdate->style->fill, sizeof(char) * sizeof(MSV_25));

    if(temp)
        toUpdate->style->fill = temp;
    else
        return;

    temp = (char*) realloc(toUpdate->style->stroke, sizeof(char) * sizeof(MSV_25));

    if(temp)
        toUpdate->style->stroke = temp;
    else
        return;
    
    if(toUpdate->radiation > 8000){

        strncpy(toUpdate->style->fill, MSV_8001, sizeof(MSV_8001));
        strncpy(toUpdate->style->stroke, MSV_8001, sizeof(MSV_8001));

    } else if(toUpdate->radiation > 1000){

        strncpy(toUpdate->style->fill, MSV_8000, sizeof(MSV_8000));
        strncpy(toUpdate->style->stroke, MSV_8000, sizeof(MSV_8000));
    
    } else if(toUpdate->radiation > 500){

        strncpy(toUpdate->style->fill, MSV_1000, sizeof(MSV_1000));
        strncpy(toUpdate->style->stroke, MSV_1000, sizeof(MSV_1000));
    
    } else if(toUpdate->radiation > 250){

        strncpy(toUpdate->style->fill, MSV_600, sizeof(MSV_600));
        strncpy(toUpdate->style->stroke, MSV_600, sizeof(MSV_600));
    
    } else if(toUpdate->radiation > 100){

        strncpy(toUpdate->style->fill, MSV_250, sizeof(MSV_250));
        strncpy(toUpdate->style->stroke, MSV_250, sizeof(MSV_250));
    
    } else if(toUpdate->radiation > 50){

        strncpy(toUpdate->style->fill, MSV_100, sizeof(MSV_100));
        strncpy(toUpdate->style->stroke, MSV_100, sizeof(MSV_100));
    
    } else if(toUpdate->radiation > 25){

        strncpy(toUpdate->style->fill, MSV_50, sizeof(MSV_50));
        strncpy(toUpdate->style->stroke, MSV_50, sizeof(MSV_50));
    
    } else{

        strncpy(toUpdate->style->fill, MSV_25, sizeof(MSV_25));
        strncpy(toUpdate->style->stroke, MSV_25, sizeof(MSV_25));
    }
}

bool circ_getShelter(Shape circ){

    if(circ == NULL)
        return 0;

    Circ_t* toGet = (Circ_t*) circ;
    
    return toGet->sheltered;
}

void circ_updateShelter(Shape circ, bool isSheltered){

    if(circ == NULL)
        return;

    Circ_t* toUpdate = (Circ_t*) circ;

    toUpdate->sheltered = isSheltered;
}

void rect_incSheltered(Shape rect){

    if(rect == NULL)
        return;

    Rect_t* toInc = (Rect_t*) rect;

    toInc->sheltered++;
}

void printRect(Shape rect){

    Rect_t* toPrint = (Rect_t*) rect;

    printf("(%lf,%lf)", toPrint->anchor.x, toPrint->anchor.y);
}

void printCirc(Shape circ){

    Circ_t* toPrint = (Circ_t*) circ;

    printf("(%lf,%lf)", toPrint->anchor.x, toPrint->anchor.y);
}

void printPoint(Point point){

    if(point == NULL)
        return;

    Point_t* toPrint = (Point_t*) point;

    printf("(%lf) ", atan2(toPrint->y, toPrint->x));
}

void printPolyg(Shape polyg){

    if(polyg == NULL)
        return;

    Polyg_t* toPrint = (Polyg_t*) polyg;

    for(unsigned long i = 0; i < toPrint->n_pts; i++)
        printPoint(toPrint->points[i]);
}

void freeStyle(Style style){

    Style_t* toFree = (Style_t*) style;

    free(toFree->fill);
    free(toFree->stroke);

    free(toFree);
}

void freeRect(Shape rect){

    Rect_t* toFree = (Rect_t*) rect;

    free(toFree->id);
    freeStyle(toFree->style);

    free(rect);
}

void freeCirc(Shape circ){

    Circ_t* toFree = (Circ_t*) circ;

    free(toFree->id);
    freeStyle(toFree->style);

    free(circ);
}

void freePolyg(Shape polyg){

    Polyg_t* toFree = (Polyg_t*) polyg;

    free(toFree->id);

    for(unsigned long i = 0; i < toFree->n_pts; i++)
        freePoint(toFree->points[i]);

    free(polyg);
}

void freePoint(Point point){

    free(point);
}