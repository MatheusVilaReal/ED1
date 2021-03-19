#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dlist.h"
#include "slist.h"
#include "qry.h"
#include "path.h"
#include "svg.h"
#include "types.h"
#include "collectdata.h"

#define default false

int main(int argc, char* argv[]){
    
    char* bsd = NULL, *bed = NULL;
    char* geo = NULL, *qry = NULL, *sufx = NULL;
    char* fbName = NULL, *fbPath = NULL;
    char* svgFileName = NULL, *svgQryName = NULL, *txtFileName = NULL;
    char* buffer = NULL, *buffer_2 = NULL, *buffer_3 = NULL, *buffer_4 = NULL, *buffer_5 = NULL;
    
    Svg svgOutput, svgQryOutput;

    List rectangles;

    Node listNode;

    size_t nx = 0;

    int dataCollection = default;
    bool useStaticList = default;

/*=====================================================================================*/
/*=====================================================================================*/

    if(setRequiredParams(argc, argv, &bsd, &geo))
        return EXIT_FAILURE;

    setOptionalParams(argc, argv, &dataCollection, &useStaticList, &qry, &bed, &fbPath, &fbName);

    if(verifySuffix(geo, ".geo") == 0){

        puts("ERRO\nArquivo de entrada .geo invalido;\nDigite qualquer tecla para continuar.");
        getc(stdin);

        return EXIT_FAILURE;
    }
        
    if(bed){ 
        
        char* buffer_geo = concatPathFile(bed, geo, NULL);
        free(geo);
        geo = buffer_geo;
    }

    buffer = getFileName(geo);
    buffer_2 = concatPathFile(bsd, buffer, NULL);

    svgFileName = removeSuffix(buffer_2);

    if(fbPath){
            
        char* buffer_fbPath = concatPathFile(bsd, fbPath, NULL);
        free(fbPath);
        fbPath = buffer_fbPath;
        addSuffix(&fbPath, "svg");
    }
    
    
    if(qry){

        if(bed) buffer_5 = concatPathFile(bed, qry, NULL);
        free(qry);
        qry = buffer_5;
        buffer_4 = getFileName(qry);
        buffer_3 = removeSuffix(buffer_4);
        svgQryName = concatPathFile(svgFileName, buffer_3, "-");
        txtFileName = concatPathFile(svgFileName, buffer_3, "-");
        addSuffix(&svgQryName, "svg");
        
        addSuffix(&txtFileName, "txt");
    }

    addSuffix(&svgFileName, "svg");
/*=====================================================================================*/
/*=====================================================================================*/

    svgOutput = newSvg();
    svgQryOutput = newSvg();

    if(useStaticList == false){

        rectangles = newList();

        geoToList(rectangles, geo);

        printList(rectangles);

        listNode = getFirst(rectangles);

        while(listNode != NULL){

            rectangle(svgOutput, getData(listNode));

            listNode = getNext(rectangles, listNode);
        }

        saveSvg(svgOutput, svgFileName);

        if(dataCollection > 0){

            if(dataCollection == 1) newTempFile("temp.txt");

            updateTempFile("temp.txt", rectangles, useStaticList);

            if(dataCollection == 3){

                Svg graph = drawGraph("temp.txt", fbName);

                saveSvg(graph, fbPath);

                freeSvg(graph);

                free(graph);
            }
        }
        
        if((qryMenu(rectangles, qry, txtFileName, useStaticList))){

            listNode = getFirst(rectangles);

            while(listNode != NULL){

                rectangle(svgQryOutput, getData(listNode));

                listNode = getNext(rectangles, listNode);
            }

            saveSvg(svgQryOutput, svgQryName);
        }

        freeList(rectangles);

/*========================== ESTÃTICA \/============= DINÂMICA /\ ========================== */

    } else{

        nx = getNx(geo);

        rectangles = newStaticList(nx);

        geoToStaticList(rectangles, geo);

        printStaticList(rectangles);

        listNode = getStaticFirst(rectangles);

        while(listNode != NULL){

            rectangle(svgOutput, getStaticData(listNode));

            listNode = getStaticNext(rectangles, listNode);
        }

        saveSvg(svgOutput, svgFileName);

        if(dataCollection > 0){

            if(dataCollection == 1) newTempFile("temp.txt");

            updateTempFile("temp.txt", rectangles, useStaticList);

            if(dataCollection == 3){

                Svg graph = drawGraph("temp.txt", fbName);

                saveSvg(graph, fbPath);

                freeSvg(graph);

                free(graph);
            }
        }

        if((qryMenu(rectangles, qry, txtFileName, useStaticList))){
    
            listNode = getStaticFirst(rectangles);

            while(listNode != NULL){

                rectangle(svgQryOutput, getStaticData(listNode));

                listNode = getStaticNext(rectangles, listNode);
            }

            saveSvg(svgQryOutput, svgQryName);
        
        }

        freeStaticList(rectangles);
    }
    
/*=====================================================================================*/
/*=====================================================================================*/

    free(buffer);
    free(buffer_2);
    free(buffer_3);
    free(buffer_4);
    free(svgFileName);
    free(svgQryName);
    free(txtFileName);

    freeSvg(svgOutput);
    freeSvg(svgQryOutput);

    free(bsd);
    free(geo);
    free(bed);
    free(qry);
    free(sufx);
    free(fbName);
    free(fbPath);

    return 0;
}