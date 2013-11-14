
#ifndef NODE_H
#define NODE_H

#include <iostream>

class Node {
public:
	struct {
		float x;
		float y;
	} p;	// position
	struct {
		float x;
		float y;
	} s;	// speed
	struct {
		float x;
		float y;
	} f;	// force
	struct {
		float x;
		float y;
	} n;	// new position
	struct {
		float x;
		float y;
	} b;	// where it was initially built
	float weight;

	Node();
	~Node();

	void set(float tx, float ty);
	void update(); // apply new position
	void move(double time); // calculate new position
	void reset();
};

#endif // NODE_H
