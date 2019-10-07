#ifndef POINTDIRECTION_HPP_
#define POINTDIRECTION_HPP_

#include<iostream>
#include "Point2D.hpp"

class PointDirection:public Point2D{
	public: 
		int direction;
		PointDirection();
		PointDirection(int i, int j, int direction);	
	
};

#endif /*POINTDIRECTION_HPP_*/
