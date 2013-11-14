#include "Node.h"

Node::Node() {
	s.x =
	s.y =
	f.x =
	f.y =
	weight = 0;
}

Node::~Node() {
}

void Node::set(float tx, float ty) {
	p.x = b.x = tx; p.y = b.y = ty;
}

void Node::update() {
//	std::cout<<"\np.x="<<p.x<<" n.x="<<n.x<<" p.y="<<p.y<<" n.y="<<n.y<<" ";
	p.x = n.x; p.y = n.y;
}

void Node::move(double time) {
	if(p.x==24||p.x==-24||p.y==-12.5) {
		n.x=p.x; n.y=p.y;
		return;
	}
	f.x/=weight; f.x*=time;
	f.y/=weight; f.y*=time;
	
	float friction = 1-time/4.0;
	s.x*=friction; s.y*=friction;
	
	s.x+=f.x; s.y+=f.y;

	n.x=p.x+s.x*time; n.y=p.y+s.y*time;
	if(n.y<-12.5) {
//		n.y=-4;
//		s.y=-s.y;
		f.x=0; f.y=9.81*weight;
	} else {
		f.x=0; f.y=-9.81*weight;
	}
}

void Node::reset() {
	p.x = b.x; p.y = b.y;
	s.x = 0; s.y = 0;
	f.x = 0; f.y = -9.81*weight;
}

