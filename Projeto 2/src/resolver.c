#include <stdio.h>              //FILE*, fprintf, fgets
#include <stdlib.h>             //malloc
#include <string.h>

#include "colony.h"
#include "environment.h"
#include "bintree.h"
#include "shapes.h"
#include "strinput.h"
#include "path.h"
#include "svg.h"
#include "qry.h"
#include "resolver.h"

#define BASE_SIZE_A 2

static void drawRectBranch(Node node, Svg svg, bool fg){

    if(node == NULL)
        return;

    if(!fg)
        drawRect(svg,   rect_getX(getNodeData(node)), rect_getY(getNodeData(node)), rect_getW(getNodeData(node)),
                        rect_getH(getNodeData(node)), getFill(rect_getStyle(getNodeData(node))),
                        getFill_op(rect_getStyle(getNodeData(node))), getStroke(rect_getStyle(getNodeData(node))), getStroke_op(rect_getStyle(getNodeData(node))),
                        getStroke_width(rect_getStyle(getNodeData(node))));
    else
        drawRectLabel(svg,   rect_getX(getNodeData(node)), rect_getY(getNodeData(node)), rect_getW(getNodeData(node)),
                        rect_getH(getNodeData(node)), getFill(rect_getStyle(getNodeData(node))),
                        getFill_op(rect_getStyle(getNodeData(node))), getStroke(rect_getStyle(getNodeData(node))), getStroke_op(rect_getStyle(getNodeData(node))),
                        getStroke_width(rect_getStyle(getNodeData(node))), rect_getSheltered(getNodeData(node)));

    drawRectBranch(getLeftChild(node), svg, fg);
    drawRectBranch(getRightChild(node), svg, fg);
}

static void drawCircBranch(Node node, Svg svg){

    if(node == NULL)
        return;

    drawCircle(svg, circ_getX(getNodeData(node)), circ_getY(getNodeData(node)), circ_getR(getNodeData(node)),
                    circ_getId(getNodeData(node)), getFill(circ_getStyle(getNodeData(node))),
                    getFill_op(circ_getStyle(getNodeData(node))), getStroke(circ_getStyle(getNodeData(node))), getStroke_op(circ_getStyle(getNodeData(node))),
                    getStroke_width(circ_getStyle(getNodeData(node))), circ_getStartX(getNodeData(node)), circ_getStartY(getNodeData(node)), circ_getRadiation(getNodeData(node)));

    drawCircBranch(getLeftChild(node), svg);
    drawCircBranch(getRightChild(node), svg);
}

int solveGeo(Colony colony, Env env){

    char* geoPath = getGeoFile(env);

    FILE* geo = fopen(geoPath, "r");

    if(geo == NULL){

        fprintf(stderr, "ERRO\nNao foi possivel abrir arquivo geo.\nVerifique o diretorio especificado e tente novamente.\n");
    
        return -1;
    }

    char* word = NULL, *id = NULL;
    char def_rectFill[20], def_rectStroke[20], def_circFill[20], def_circStroke[20];

    int nr, nc = 0;

    fscanf(geo, "nx %d\n", &nr);

    nc = nr;

    long size_rects = 0;
    long size_circs = 0;

    Node* rects = (Node*) malloc(sizeof(Node) * nr);
    Node* circs = (Node*) malloc(sizeof(Node) * nc);

    double x = 0, y = 0, w = 0, h = 0, r = 0;

    while((word = getword(word, geo)) && strcmp(word, "\0")){

        if(strcmp(word, "cc") == 0)
            fscanf(geo, " %s ", def_rectStroke);
        
        else if(strcmp(word, "cp") == 0)
            fscanf(geo, " %s ", def_rectFill);

        else if(strcmp(word, "bc") == 0)
            fscanf(geo, " %s ", def_circStroke);

        else if(strcmp(word, "pc") == 0)
            fscanf(geo, " %s ", def_circFill);

        else if(strcmp(word, "r") == 0){

            if(size_rects == nr){

                nr *= 2;
                rects = realloc(rects, sizeof(Node) * nr);
            }

            id = getword(id, geo);

            fscanf(geo, " %lf %lf %lf %lf ", &x, &y, &w, &h);

            Shape toAdd = newRect(id, x, y, w, h, newStyle(def_rectFill, OP_DEFAULT, def_rectStroke, OP_DEFAULT, WIDTH_DEFAULT, 0));

            rects[size_rects++] = newNode(toAdd);
        
        } else if(strcmp(word, "c") == 0){
            
            if(size_circs == nc){

                nc *= 2;
                circs = realloc(circs, sizeof(Node) * nc);
            }

            id = getword(id, geo);

            fscanf(geo, " %lf %lf %lf ", &x, &y, &r);

            Shape toAdd = newCirc(id, x, y, r, newStyle(def_circFill, OP_DEFAULT, def_circStroke, OP_DEFAULT, WIDTH_DEFAULT, 0));

            circs[size_circs++] = newNode(toAdd);
        }
    }

    kd_buildTree(getRectangles(colony), rects, size_rects, compareRect_x, compareRect_y);
    kd_buildTree(getCircles(colony), circs, size_circs, compareCirc_x, compareCirc_y);

    free(rects);
    free(circs);

    free(word);
    free(id);

    fclose(geo);

    return 0;
}

int solveSvg(Colony colony, Svg svgFile, Svg querries){

    drawRectBranch(getRoot(getRectangles(colony)), svgFile, getFg(colony));

    if(querries != NULL)
        catSvg(svgFile, querries);
    
    drawCircBranch(getRoot(getCircles(colony)), svgFile);

    return 0;
}

int solveQry(Colony colony, Svg svgFile, Svg querries, Env env){

    char* qryPath = getQryFile(env);

    if(strcmp(qryPath, "\0") == 0)
        return 1;

    FILE* qry = fopen(qryPath, "r");

    if(qry == NULL){

        fprintf(stderr, "ERRO\nNao foi possivel abrir arquivo de consulta.\nVerifique o diretorio especificado e tente novamente.\n");

        return -1;
    }

    char* txtPath = solveTxtPath(env), *word = NULL, *id = NULL;
    double x = 0, y = 0, r = 0, s = 0;

    FILE* txt = fopen(txtPath, "a+");

    if(txt == NULL){

        fprintf(stderr, "ERRO\nNao foi possivel gerar arquivo de texto.\nVerifique o diretorio especificado e tente novamente.\n");
        
        free(txtPath);

        return -1;
    }

    updateSheltered(colony);

    while((word = getword(word, qry)) && strcmp(word, "\0")){

        if(strcmp(word, "dpi") == 0){

            // puts("Executando: dpi");

            fscanf(qry, " %lf %lf\n", &x, &y);

            dpi(txt, getRectangles(colony), x, y);

        } else if(strcmp(word, "dr") == 0){

            // puts("Executando: dr");

            id = getword(id, qry);

            dr(txt, getRectangles(colony), id);

            free(id);
            id = NULL;

        } else if(strcmp(word, "fg") == 0){

            // puts("Executando: fg");

            fscanf(qry, " %lf %lf %lf\n", &x, &y, &r);

            fg(txt, querries, getRectangles(colony), getCircles(colony), x, y, r);
        
        } else if(strcmp(word, "im") == 0){

            // puts("Executando: im");

            fscanf(qry, " %lf %lf %lf\n", &x, &y, &s);

            im(txt, colony, svgFile, getCircles(colony), x, y, s);

        } else if(strcmp(word, "t30") == 0){

            t30(txt, colony);

        } else if(strcmp(word, "nve") == 0){

            fscanf(qry, " %lf %lf\n", &x, &y);

            nve(txt, querries, colony, x, y);
        }
    }

    fprintf(txt, "\n\nNOME: MATHEUS PIRES VILA REAL\t\tn 202000560352\n");

    free(word);

    free(txtPath);

    fclose(qry);
    fclose(txt);

    return 0;
}
