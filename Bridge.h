
#ifndef BRIDGE_H
#define	BRIDGE_H

#include "Node.h"
#include "Edge.h"
#include "Level.h"

#include <math.h>
#include <iostream>

class Bridge {
public:

	Node* nodes;
	unsigned int nnodes;
	unsigned int nodesArrayLength;

	Edge* edges;
	unsigned int nedges;
	unsigned int edgesArrayLength;

	Node* tfrom;
	Node* tto;		// temporary to while drawing
	Edge* tEdge;	// temporary edge while drawing

	Bridge();
	~Bridge();

public:
	void addEdge(float width, float fromX, float fromY, float toX, float toY);
	void oneTimeStep(double time, Level* level);
	void click(unsigned int cx, unsigned int cy);
	void click3(unsigned int cx, unsigned int cy);
	void stopClicking();
	void liveRemove(int fx, int fy, int tx, int ty);
private:
	void addEdge(float width, Node* n1, Node* n2);
//	Node* findOrCreateNode(float x, float y);
//	void findOrCreateNode1(Node** n, float x, float y);
	void findOrCreateNode2(Node** n1, float x1, float y1, Node** n2, float x2, float y2);
//	Node* addNewNode(float x, float y);
	void removeEdge(unsigned int e);
	void removeNode(Node* n);
};

#endif

