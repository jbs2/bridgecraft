
#ifndef LEVEL_H
#define LEVEL_H

#include "Node.h"

class Level {
	public:
		Level();
		~Level();

	    typedef struct {
	        int x;
	        int y;
	    } Intpos;    // integer position type

		Intpos* fixPositions;
		int amountOfFix;

		bool contains(Node* n);
};

#endif

