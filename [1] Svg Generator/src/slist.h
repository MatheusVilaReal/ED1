#ifndef SLIST_H
#define SLIST_H

#include "types.h"
#include <stddef.h>

List newStaticList(size_t size);
Node staticInsert(List list, Data data, size_t index);
Node staticPush(List list, Data data);
Node staticInsertBefore(List list, Data data, size_t index);
Node staticInsertAfter(List list, Data data, size_t index);
Node getStaticFirst(List list);
Node getStaticNext(List list, Node node);
Node getStaticLast(List list);
Node getStaticPrevious(List list, Node node);
Data getStaticData(Node node);
Node getStaticNode(List list, size_t index);
size_t getNx(char* streamId);
size_t getStaticSize(List list);
size_t getStaticVisits(List list);
void geoToStaticList(List list, char* streamId);
void printStaticList(List list);
void delStaticNode(List list, Node node);
void freeStaticList(List list);

#endif