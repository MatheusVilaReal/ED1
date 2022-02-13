#ifndef COLONY_H
#define COLONY_H

#include "types.h"

Colony buildColony();

Tree getRectangles(Colony colony);

Tree getCircles(Colony colony);

double colony_getRadiation(Colony colony);

void updateSheltered(Colony colony);

void updateRadiation(FILE* txt, Colony colony, double radiation);

void updateFg(Colony colony);

void getShadows(Colony colony, double x, double y);

bool getFg(Colony colony);

void destroyColony(Colony colony);

#endif