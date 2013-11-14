#include "Edge.h"

Edge::Edge() {

}

Edge::~Edge() {

}

//Edge::Edge(float swidth, unsigned int& fromNode, unsigned int& toNode) {
//	this->set(swidth,fromNode,toNode);
//}

void Edge::set(float swidth, Node* fromNode, Node* toNode) {
	from=fromNode;
	to=toNode;
	width=swidth;
	initialLength=sqrt(pow(fromNode->p.x-toNode->p.x,2)+pow(fromNode->p.y-toNode->p.y,2));
}

/*float Edge::getLengthDiff() {
	return sqrt(pow(from->p.x-to->p.x,2)+pow(from->p.y-to->p.y,2))-initialLength;
}*/

/*float Edge::getRelativeLengthDiff() {
	return (sqrt(pow(from->p.x-to->p.x,2)+pow(from->p.y-to->p.y,2))-initialLength)/initialLength;
}*/

/*float Edge::getSpringConstant() {
	float E=210; // Elastizitaetsmodul ferritischer Stahl
	float A=pow(width,2); // Querschnittsflaeche
	float L0=initialLength;
	return E*A/L0; // this is D
	// later: F=D * \delta L
}*/

/*float Edge::getTotalForce() {
	return 210*pow(width,2)/initialLength * (sqrt(pow(from->p.x-to->p.x,2)+pow(from->p.y-to->p.y,2))-initialLength);
}*/

/*void Edge::getNormalizedDirection() {
	float x=from->p.x-to->p.x;
	float y=from->p.y-to->p.y;
	float length=sqrt(pow(x,2)+pow(y,2));
	x/=length;
	y/=length;
}*/

/*Vec2 Edge::getForces() {
	Vec2 f;
	f.x = from->p.x-to->p.x;
	f.y = from->p.y-to->p.y;
	float length = sqrt(pow(f.x,2)+pow(f.y,2));
	float totalForce = 210 * pow(width,2) / initialLength * (length - initialLength);
	length/=totalForce;
	f.x/=length;
	f.y/=length;
//	f.x*=totalForce;
//	f.y*=totalForce;
	return f;
}*/

float Edge::setForces() {
    float x = from->p.x-to->p.x;
    float y = from->p.y-to->p.y;
//	std::cout<<"\nx="<<x<<" "<<from->p.x<< " "<<to->p.x;
    float length = sqrt(pow(x,2)+pow(y,2));
    float totalForce = 210000000000 * pow(width,2) / initialLength * (length - initialLength);
	stress = (210000000000 / initialLength * (length - initialLength)) / 355000000; // percentOfMaxF
	if(stress<0)
		stress=-stress;
//	std::cout<<"\n"<<stress<<std::endl;
	length/=totalForce;
	x/=length;
	y/=length;
	from->f.x-=x;
	from->f.y-=y;
	to->f.x+=x;
	to->f.y+=y;
	return stress;
//	std::cout<<"\nx="<<x<<" y="<<y<<" length="<<length<<" totalForce="<<totalForce;
}

