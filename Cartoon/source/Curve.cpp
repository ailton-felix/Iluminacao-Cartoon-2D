#include "../headers/Curve.hpp"

Curve::Curve(){
	flagFechado = 0;
	list <Point2D>l;
}

Curve::~Curve(){
	listPoints.clear();	
}
