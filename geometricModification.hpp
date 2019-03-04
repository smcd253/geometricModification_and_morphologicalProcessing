#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>

int width = 512;
int height = 512;

struct cornerData 
{
	int corner_startCoords[4][2];
	int corner_endCoords[4][2];
};

