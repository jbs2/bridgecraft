
#include "Level.h"

Level::Level() {
//	fixPositions = new Intpos[4];
	amountOfFix=0;
}

Level::~Level() {
	delete fixPositions;
}

bool Level::contains(Node* n) {
	if(amountOfFix==0)
		return false;
	for(int i=amountOfFix; i--;) {
		if((*n).p.x == fixPositions[i].x && (*n).p.y == fixPositions[i].y)
			return true;
	}
	return false;
}

