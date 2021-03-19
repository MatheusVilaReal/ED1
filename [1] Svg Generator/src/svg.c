#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "svg.h"
#include "types.h"
#include "getword.h"

#define MARGIN 50

typedef struct Svg PrivateSvg;
typedef struct Data PrivateData; 

struct Svg{

    char* body;
    bool hasFooter;
    
};

struct Data{

    char* id;

    char* fill;
    char* stroke;

    float x;
    float y;
    float w;
    float h;
};

static void addHeader(Svg svg);
static void addFooter(Svg svg);

Svg newSvg(){

    PrivateSvg* svg = (PrivateSvg*) malloc(sizeof(PrivateSvg));

    svg->hasFooter = false;

    addHeader(svg);

    return svg;
}

void rectangle(Svg svg, void* data){

    PrivateSvg* privateSvg = (PrivateSvg*) svg;
    PrivateData* toPrint = (PrivateData*) data;

    int allocLen_1, allocLen_2;
    char rect[512], text[1024];

    if(strcmp(toPrint->id, "#bbi")){

        allocLen_1 = sprintf
        (rect,
        "\n\t<rect x=\"%f\" y=\"%f\" width=\"%f\" height=\"%f\" style=\"fill: %s; fill-opacity:0.8; stroke: %s; stroke-width: 2;\"/>\n",
        toPrint->x, toPrint->y, toPrint->w, toPrint->h,
        toPrint->fill, toPrint->stroke);

        allocLen_2 = sprintf
        (text,
        "\t<text x=\"%f\" y=\"%f\" font-size=\"%f%%\" style=\"fill:black; stroke:white; stroke-width:0.2\">%s</text>\n",
        toPrint->x+1, toPrint->y+5, toPrint->w-5, toPrint->id);

        char *temp = realloc(privateSvg->body, strlen(privateSvg->body)+allocLen_1+allocLen_2+1);

        if(temp){

            privateSvg->body = temp;

            strcat(privateSvg->body, rect);
            strcat(privateSvg->body, text);
        }
    
    } else{

        boundingBox(svg, toPrint->x, toPrint->y, toPrint->w, toPrint->h, 3, toPrint->stroke);

        float x = 0, y = 0;

        sscanf(toPrint->fill, "%f %f", &x, &y);

        if(x && y) point(svg, x, y, 5, "blue", "yellow");
    }
}

void rectangleCoord(Svg svg, float x, float y, float w, float h, float fill_opacity, char* fill, char* stroke){

    PrivateSvg* privateSvg = (PrivateSvg*) svg;

    char rect[512];

    int allocLen_1 = sprintf
    (rect,
    "\n\t<rect x=\"%f\" y=\"%f\" width=\"%f\" height=\"%f\" style=\"fill: %s; fill-opacity:%f; stroke: %s; stroke-width: 2;\"/>\n",
    x, y, w, h, fill, fill_opacity, stroke);

    char *temp = realloc(privateSvg->body, strlen(privateSvg->body)+allocLen_1+1);

    if(temp){

        privateSvg->body = temp;

        strcat(privateSvg->body, rect);
    }
}

void boundingBox(Svg svg, float x, float y, float w, float h, float width, char* color){

    PrivateSvg* privateSvg = (PrivateSvg*) svg;

    char bbi[512];

    int allocLen_1 = snprintf(bbi, sizeof(bbi),
    "\n\t<rect id=\"one\" x=\"%f\" y=\"%f\" width=\"%f\" height=\"%f\" fill=\"transparent\" stroke=\"%s\" stroke-width=\"%f\" stroke-dasharray=\"6 2\"/>\n",
    x, y, w, h, color, width);

    char *temp = realloc(privateSvg->body, (strlen(privateSvg->body)+allocLen_1+1)*sizeof(char));

    if(temp){

        privateSvg->body = temp;

        strncat(privateSvg->body, bbi, strlen(bbi));
    }
}

void point(Svg svg, float x, float y, float r, char* fill, char* stroke){

    PrivateSvg* privateSvg = (PrivateSvg*) svg;

    char point[512];

    int allocLen_1 = snprintf(point, sizeof(point),
    "\n\t<circle id=\"two\" cx=\"%f\" cy=\"%f\" r=\"%f\" fill=\"%s\" stroke=\"%s\" stroke-width=\"2\"/>\n",
    x, y, r, fill, stroke);

    char *temp = realloc(privateSvg->body, (strlen(privateSvg->body)+allocLen_1+1)*sizeof(char));

    if(temp){

        privateSvg->body = temp;

        strncat(privateSvg->body, point, strlen(point));
    }
}

void line(Svg svg, float x_1, float y_1, float x_2, float y_2, float width, char* color){

    PrivateSvg* privateSvg = (PrivateSvg*) svg;

    char line[512];

    int allocLen_1 = snprintf(line, sizeof(line),
    "\n\t<line x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" stroke=\"%s\" stroke-width=\"%f\"/>\n",
    x_1, y_1, x_2, y_2, color, width);

    char *temp = realloc(privateSvg->body, (strlen(privateSvg->body)+allocLen_1+1)*sizeof(char));

    if(temp){

        privateSvg->body = temp;

        strncat(privateSvg->body, line, strlen(line));
    }
}

void numText(Svg svg, float x, float y, float size, char* fill, char* stroke, int num){

    PrivateSvg* privateSvg = (PrivateSvg*) svg;

    char text[5024];

    int allocLen_1 = sprintf
    (text,
    "\t<text x=\"%f\" y=\"%f\" font-size=\"%f%%\" style=\"fill:%s; stroke:%s; stroke-width:0.2\">%d</text>\n",
    x, y, size, fill, stroke, num);

    char *temp = realloc(privateSvg->body, (strlen(privateSvg->body)+allocLen_1+1)*sizeof(char));

    if(temp){

        privateSvg->body = temp;

        strncat(privateSvg->body, text, strlen(text));
    }
}

void alphaText(Svg svg, float x, float y, float size, char* fill, char* stroke, char* alpha){

    PrivateSvg* privateSvg = (PrivateSvg*) svg;

    char text[5024];

    int allocLen_1 = sprintf
    (text,
    "\t<text x=\"%f\" y=\"%f\" font-size=\"%f%%\" style=\"fill:%s; stroke:%s; stroke-width:0.2\">%s</text>\n",
    x, y, size, fill, stroke, alpha);

    char *temp = realloc(privateSvg->body, (strlen(privateSvg->body)+allocLen_1+1)*sizeof(char));

    if(temp){

        privateSvg->body = temp;

        strncat(privateSvg->body, text, strlen(text));
    }
}

void saveSvg(Svg svg, const char* path){

    PrivateSvg* privateSvg = (PrivateSvg*) svg;

    FILE* toSave = fopen(path, "w+");

    if(privateSvg && toSave){

        if(privateSvg->hasFooter == false)
            addFooter(svg);

        fwrite(privateSvg->body, sizeof(char), strlen(privateSvg->body), toSave);
    }

    fclose(toSave);
}

void freeSvg(Svg svg){

    PrivateSvg* toFree = (PrivateSvg*) svg;

    free(toFree->body);
    free(toFree);
}

Svg drawGraph(char* dataInput, char* name){

    FILE* data = fopen(dataInput, "r");

    if(data == NULL){

        puts("\nERRO\nFalha ao abrir arquivo de dados.");

        return NULL;
    }

    int x, y;
    int maxX = 0, maxY = 0;

    char ch;
    char* word;

    while((ch = fgetc(data)) != EOF && ch != '\n')
        continue;

    while((word = getword(data)) && strcmp(word, "\0")){

        x = atoi(word);

        free(word);

        word = getword(data);

        y = atoi(word);

        if(maxX < x) maxX = x;
        if(maxY < y) maxY = y;

        free(word);
    }

    free(word);

    Svg graph = newSvg();

    rectangleCoord(graph, 0, 0, maxX+2*MARGIN, maxY+2*MARGIN, 1, "white", "white");

    rewind(data);

    while((ch = fgetc(data)) != EOF && ch != '\n')
        continue;

    while((word = getword(data)) && strcmp(word, "\0")){

        x = atoi(word);

        free(word);

        word = getword(data);

        y = atoi(word);

        boundingBox(graph, 0, (maxY+MARGIN) - y, x+MARGIN, (maxY+2*MARGIN)-((maxY+MARGIN) - y), 1, "grey");

        point(graph, x+MARGIN, (maxY+MARGIN) - y, 1, "black", "black");

        free(word);
    }   

    rectangleCoord(graph, 0, 0, MARGIN, maxY+2*MARGIN, 1, "white", "transparent");
    rectangleCoord(graph, 0, maxY+MARGIN, maxX+2*MARGIN, (maxY+2*MARGIN)-(maxY+MARGIN), 1, "white", "white");

    line(graph, MARGIN, MARGIN/2, MARGIN, (maxY+2*MARGIN)-MARGIN/2, 1, "black");
    line(graph, MARGIN/2, maxY+MARGIN, maxX+MARGIN+MARGIN/2, maxY+MARGIN, 1, "black");

    int j = 10;

    for(int i = maxY+MARGIN-10; i >= MARGIN; i -= 10){

        line(graph, MARGIN-10, i, MARGIN, i, 1, "black");
        numText(graph, MARGIN-25, i+2, 50, "black", "transparent", j);

        j += 10;
    }

    j = 10;

    for(int i = MARGIN+10; i <= maxX+MARGIN; i += 10){

        line(graph, i, maxY+MARGIN, i, maxY+MARGIN+10, 1, "black");
        numText(graph, i-3, maxY+MARGIN+15, 50, "black", "transparent", j);

        j += 10;
    }

    alphaText(graph, MARGIN, MARGIN/4, 70, "blue", "black", name);

    free(word);

    fclose(data);

    return graph;
}

static void addHeader(Svg svg){

    PrivateSvg* privateSvg = (PrivateSvg*) svg;

    char header[] = "<svg width=\"100%%\" height=\"100%%\">\n";

    privateSvg->body = (char*) calloc(sizeof(header), sizeof(char));

    if(privateSvg->body) sprintf(privateSvg->body, "%s", header);
}

static void addFooter(Svg svg){

    PrivateSvg* privateSvg = (PrivateSvg*) svg;

    char footer[] = "\n</svg>";

    char* temp = realloc(privateSvg->body, strlen(privateSvg->body) + sizeof(footer));

    if(temp){

    privateSvg->body = temp;

    strcat(privateSvg->body, footer);
    privateSvg->hasFooter = true;
    }
}