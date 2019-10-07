#include "../headers/Point2D.hpp"

#include<iostream>
using namespace std;

Point2D::Point2D(){
	this->x=0;
	this->y=0;
}

Point2D::Point2D(int x, int y){
	this->x=x;
	this->y=y;
}

void Point2D::getPoint(int *a, int *b ){
	*a = this->x;
	*b = this->y;
}

int Point2D :: operator ^ (Point2D b){
	return ((this->x*b.y) - (this->y*b.x));
}

void Point2D :: setPoint(int a, int b){
	this->x = a;
	this->y = b;
}

bool Point2D::operator!=(Point2D a){
	return(!(this->x == a.x and this->y == a.y));
}

bool Point2D::operator==(Point2D a){
	return(this->x == a.x and this->y == a.y);
}

ostream& operator<<(ostream& o,const Point2D& p){
	o<<"("<<p.x<<","<< p.y<<")";
	return o; 
}

Point2D::~Point2D(){
}
