#include "../headers/PointDirection.hpp"

PointDirection::PointDirection(){}

PointDirection::PointDirection(int i, int j, int direction){
	x = i;
	y = j;
	this->direction = direction;
}
