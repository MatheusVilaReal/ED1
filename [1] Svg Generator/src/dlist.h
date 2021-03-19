#ifndef DLIST_H
#define DLIST_H

#include "types.h"

List newList();
Node getNode(List list, size_t index);
Node getFirst(List list);
Node getNext(List list, Node node);
Node getLast(List list);
Node getPrevious(List list, Node node);
Node insertAfter(List list, Data data, Node node);
Node push(List list, Data data);
Node insertBefore(List list, Data data, Node node);
Data getData(Node node);
size_t getListSize(List list);
size_t getVisits(List list);
void geoToList(List list, char* source);
void printList(List list);
void delNode(List list, Node node);
void freeList(List list);

#endif