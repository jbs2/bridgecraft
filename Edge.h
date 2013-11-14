#ifndef EDGE_H
#define EDGE_H

#include "Node.h"
#include "Vec2.h"
#include <math.h>

class Edge {
	public:
		Node* from;
		Node* to;
		float width;
		float initialLength;
		float stress;

		Edge();
		~Edge();
		void set(float swidth, Node* fromNode, Node* toNode);
//		float getLengthDiff();
//		float getRelativeLengthDiff();
//		float getSpringConstant();
//		float getTotalForce();
//		void getNormalizedDirection();
//		Vec2 getForces();
		float setForces();
};

#endif	// EDGE_H

