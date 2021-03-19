#ifndef COLLECTDATA_H
#define COLLECTDATA_H

#include "types.h"

void newTempFile(char* path);
void updateTempFile(char* path, List list, bool usesStaticList);

#endif