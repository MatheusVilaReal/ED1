#ifndef QRY_H
#define QRY_H

#include <stdbool.h>

#include "types.h"
#include "svg.h"

int qryMenu(List list, char* qryPath, char*txtPath, bool useStaticList);
void tp(List list, char* qry, bool useStaticList);
void tpr(List list, char* qry, float x, float y, float w, float h, bool useStaticList);
void dpi(List list, char* qry, float x, float y, bool useStaticList);
void dr(List list, char* qry, char* id, bool useStaticList);
void bbi(List list, char* qry, float x, float y, bool useStaticList);
void bbid(List list, char* qry, char* id, bool useStaticList);
void iid(List list, char* qry, char* id, int k, bool useStaticList);
void diid(List list, char* qry, char* id, int k, bool useStaticList);

#endif