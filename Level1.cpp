
#include "Level1.h"

Level1::Level1() {
	fixPositions = new Intpos[4];
	amountOfFix=4;

	fixPositions[0].x=-23;
	fixPositions[0].y=7;
	fixPositions[1].x=23;
	fixPositions[1].y=7;
	fixPositions[2].x=-21;
	fixPositions[2].y=3;
	fixPositions[3].x=21;
	fixPositions[3].y=3;
}

