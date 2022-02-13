#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "svg.h"

#define HEADER "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n<svg\n\txmlns=\"http://www.w3.org/2000/svg\"\n\txmlns:xlink=\"http://www.w3.org/1999/xlink\">\n\n"
#define FOOTER "\n</svg>"
#define SPACING 5
#define ANIMATION_DUR 5
#define CROSS 0x86U

/*NÍVEIS DE RADADIÇÃO*/
#define MSV_25   "#00FFFF"
#define MSV_50   "#00FF00"
#define MSV_100  "#FF00FF"
#define MSV_250  "#0000FF"
#define MSV_600  "#800080"
#define MSV_1000 "#000080"
#define MSV_8000 "#FF0000"
#define MSV_8001 "#000000"

typedef struct Svg{

    char* path;
    char* body;
    bool hasFooter;
    
} Svg_t;

static int addHeader(Svg svg){

    Svg_t* toAdd = (Svg_t*) svg;

    char* buf = (char*) realloc(toAdd->body, strlen(toAdd->body) + 1 + sizeof(HEADER) * sizeof(char));

    if(buf)
        toAdd->body = buf;

    return snprintf(toAdd->body, strlen(toAdd->body) + 1 + sizeof(HEADER) * sizeof(char), "%s", HEADER);
}

static int addFooter(Svg svg){

    Svg_t* toAdd = (Svg_t*) svg;

    char* buf = (char*) realloc(toAdd->body, strlen(toAdd->body) + 1 + sizeof(FOOTER) * sizeof(char));

    if(buf){
    
        toAdd->body = buf;
        strncat(toAdd->body, FOOTER, sizeof(FOOTER));
    }

    return sizeof(FOOTER);
}

Svg newSvg(char* path){

    Svg_t* svg = (Svg_t*) malloc(sizeof(Svg_t));

    svg->path = path;
    svg->body = (char*) calloc(sizeof(char), 1);
    svg->hasFooter = false;

    addHeader(svg);

    return svg;
}

Svg copySvg(Svg dest, const Svg svg){

    Svg_t* receiver = (Svg_t*) dest;
    const Svg_t* toCopy = (const Svg_t*) svg;

    receiver->body = strncpy(receiver->body, toCopy->body, strlen(toCopy->body) + 1);

    return receiver;
}

Svg catSvg(Svg dest, const Svg source){

    Svg_t* toCat = (Svg_t*) dest;
    Svg_t* buf = (Svg_t*) source;

    char* temp = (char*) realloc(toCat->body, (strlen(toCat->body) + strlen(buf->body) - strlen(HEADER) + 1) * sizeof(char));

    if(temp)
        toCat->body = temp;
    else
        return toCat;

    strncat(toCat->body, buf->body + sizeof(HEADER) - 1, strlen(buf->body) + 1);

    toCat->hasFooter = buf->hasFooter;

    return toCat;
}

void drawRect(Svg svg, double x, double y, double w, double h, char* fill, double fill_op, char* stroke, double stroke_op, double stroke_width){

    Svg_t* toDraw = (Svg_t*) svg;

    char buf_1[1024];
    int allocLen_1 = 0;

    allocLen_1 = snprintf(buf_1, sizeof(buf_1), "\t<rect x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" style=\"fill: %s; fill-opacity: %lf; stroke: %s; stroke-opacity: %lf; stroke-width: %lf%%\"/>\n", 
    x, y, w, h, fill, fill_op, stroke, stroke_op, stroke_width);

    char *buf = realloc(toDraw->body, strlen(toDraw->body) + allocLen_1 + 1);

    if(buf){

        toDraw->body = buf;

        strncat(toDraw->body, buf_1, allocLen_1);
    }
}

void drawRectLabel(Svg svg, double x, double y, double w, double h, char* fill, double fill_op, char* stroke, double stroke_op, double stroke_width, unsigned int sheltered){

    Svg_t* toDraw = (Svg_t*) svg;

    char buf_1[1024], buf_2[1024];
    int allocLen_1 = 0, allocLen_2 = 0;
    
    allocLen_1 = snprintf(buf_1, sizeof(buf_1), "\t<rect x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" style=\"fill: %s; fill-opacity: %lf; stroke: %s; stroke-opacity: %lf; stroke-width: %lf%%\"/>\n", 
        x, y, w, h, fill, fill_op, stroke, stroke_op, stroke_width);

    allocLen_2 = snprintf(buf_2, sizeof(buf_2), "<text x=\"%lf\" y=\"%lf\" font-size=\"smaller\" fill=\"black\" stroke=\"transparent\">%u</text>",
        x + SPACING, y + SPACING, sheltered);

    strncat(buf_1, buf_2, allocLen_2);

    allocLen_1 += allocLen_2;
    
    char *buf = realloc(toDraw->body, strlen(toDraw->body) + allocLen_1 + 1);

    if(buf){

        toDraw->body = buf;

        strncat(toDraw->body, buf_1, allocLen_1);
    }
}

void drawCircle(Svg svg, double x, double y, double r, char* id, char* fill, double fill_op, char* stroke, double stroke_op, double stroke_width, double start_x, double start_y, double radiation){

    Svg_t* toDraw = (Svg_t*) svg;

    char buf_1[1024];
    int allocLen_1 = 0;
    
    allocLen_1 = snprintf(buf_1, sizeof(buf_1), "\t<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" style=\"fill: %s; fill-opacity: %lf; stroke: %s; stroke-opacity: %lf; stroke-width: %lf%%\">\n\t\t<animate attributeName=\"cx\" values=\"%lf;%lf\" dur=\"%d\"/>\n\t\t<animate attributeName=\"cy\" values=\"%lf;%lf\" dur=\"%d\"/>\n\t</circle>\n", 
    x, y, r, fill, fill_op, stroke, stroke_op, stroke_width,
    start_x, x, ANIMATION_DUR,
    start_y, y, ANIMATION_DUR);

    char *buf = realloc(toDraw->body, strlen(toDraw->body) + allocLen_1 + 1);

    if(buf){

        toDraw->body = buf;

        strncat(toDraw->body, buf_1, allocLen_1);
    }

    if(radiation > 8000)
        drawCross(svg, x, y, r);
}

void drawCross(Svg svg, double x, double y, double r){

    drawLine(svg, x, y - 1.5, x, y + 1.5, 1, "white");
    drawLine(svg, x - 1.5, y - 0.3, x + 1.5, y - 0.3, 1, "white");
}

void drawNve(Svg svg, double x, double y, double s){

    Svg_t* toDraw = (Svg_t*) svg;

    char buf_1[1024], buf_2[1024], color[8];
    int allocLen_1 = 0, allocLen_2 = 0;

    if(s > 8000){

        strncpy(color, MSV_8001, sizeof(MSV_8001));

    } else if(s > 1000){

        strncpy(color, MSV_8000, sizeof(MSV_8000));
    
    } else if(s > 600){

        strncpy(color, MSV_1000, sizeof(MSV_1000));
    
    } else if(s > 250){

        strncpy(color, MSV_600, sizeof(MSV_600));
    
    } else if(s > 100){

        strncpy(color, MSV_250, sizeof(MSV_250));
    
    } else if(s > 50){

        strncpy(color, MSV_100, sizeof(MSV_100));
    
    } else if(s > 25){

        strncpy(color, MSV_50, sizeof(MSV_50));
    
    } else{

        strncpy(color, MSV_25, sizeof(MSV_25));
    }

    allocLen_1 = snprintf(buf_1, sizeof(buf_1), "\t<rect x=\"%lf\" y=\"%lf\" rx=\"2\" ry=\"2\" width=\"10\" height=\"10\" style=\"fill: %s; fill-opacity: %lf; stroke: %s; stroke-opacity: %lf; stroke-width: %lf%%\"/>\n", 
    x, y, color, 0.8, color, 0.8, (double) 0);

    allocLen_2 = snprintf(buf_2, sizeof(buf_2), "<text x=\"%lf\" y=\"%lf\" font-size=\"10px\" fill=\"black\" stroke=\"white\" stroke-width=\"0.2px\">%lf</text>",
    x + SPACING, y + SPACING, s);

    strncat(buf_1, buf_2, allocLen_2);

    allocLen_1 += allocLen_2;

    char *buf = realloc(toDraw->body, strlen(toDraw->body) + allocLen_1 + 1);

    if(buf){

        toDraw->body = buf;

        strncat(toDraw->body, buf_1, allocLen_1);
    }
}

void drawLine(Svg svg, double x_1, double y_1, double x_2, double y_2, double width, char* color){

    Svg_t* toDraw = (Svg_t*) svg;

    char buf_1[1024];

    int allocLen_1 = snprintf(buf_1, sizeof(buf_1), "\n\t<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" style=\"stroke: %s; stroke-width: %lf; stroke-opacity: 1\"/>\n",
    x_1, y_1, x_2, y_2, color, width);

    char *buf = realloc(toDraw->body, strlen(toDraw->body) + allocLen_1 * sizeof(char) + 1);

    if(buf){

        toDraw->body = buf;

        strncat(toDraw->body, buf_1, allocLen_1);
    }
}

void drawPath(Svg svg, double x_1, double y_1, double x_2, double y_2){

    Svg_t* toDraw = (Svg_t*) svg;

    char buf_1[1024];

    int allocLen_1 = snprintf(buf_1, sizeof(buf_1), "\t<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" stroke=\"red\" stroke-width=\"3\" stroke-dasharray=\"3\"/>\n",
    x_1, y_1, x_2, y_2);

    char *buf = realloc(toDraw->body, strlen(toDraw->body) + allocLen_1 * sizeof(char) + 1);

    if(buf){

        toDraw->body = buf;

        strncat(toDraw->body, buf_1, allocLen_1);
    }
}

void viewSvg(Svg svg){

    Svg_t* toView = (Svg_t*) svg;

    puts(toView->body);
}

void writeSvg(Svg svg){

    Svg_t* toWrite = (Svg_t*) svg;

    FILE* file = fopen(toWrite->path, "w+");

    if(toWrite && file){

        if(toWrite->hasFooter == false)
            addFooter(svg);

        fwrite(toWrite->body, sizeof(char), strlen(toWrite->body), file);
    } else
        return;

    fclose(file);
}

void freeSvg(Svg svg){

    Svg_t* toFree = (Svg_t*) svg;

    free(toFree->path);
    free(toFree->body);
    free(toFree);
}

