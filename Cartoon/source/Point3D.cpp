#include<iostream>
#include "../headers/Point3D.hpp"
using namespace std;


Point3D::Point3D(){
	this->x=0;
	this->y=0;
	this->z=0;
}

Point3D::Point3D(double _x, double _y, double _z){
	this->x=_x;
	this->y=_y;
	this->z=_z;
}

void Point3D::getPoint(double *a, double *b , double *c){
	*a = this->x;
	*b = this->y;
	*c = this->z;
}

void Point3D::setPoint(double x, double y, double z){
	this->x=x;
	this->y=y;
	this->z=z;
}

void Point3D::normalize(){
	double norma=sqrt((this->x*this->x)+(this->y*this->y)+(this->z*this->z));
    this->setPoint(this->x/norma, this->y/norma, this->z/norma);
}

//int Point3D :: operator ^ (Point3D b){
//	return ((this->x*b.y) - (this->y*b.x));
//}

bool Point3D::operator!=(Point3D a){
	return(!(this->x == a.x and this->y == a.y and this->z == a.z));
}

bool Point3D::operator==(Point3D a){
	return(this->x == a.x and this->y == a.y and this->z == a.z);
}

Point3D operator+(Point3D p1,Point3D p2){
	Point3D pR;
	pR.x = p1.x+p2.x;
	pR.y = p1.y+p2.y;
	pR.z = p1.z+p2.z;
	return pR;
}

Point3D operator-(Point3D p1,Point3D p2){
	Point3D pR;
	pR.x = p1.x-p2.x;
	pR.y = p1.y-p2.y;
	pR.z = p1.z-p2.z;
	return pR;
}

Point3D operator/(Point3D p1,double e){
	Point3D pR;
	pR.x = p1.x/e;
	pR.y = p1.y/e;
	pR.z = p1.z/e;
	return pR;
}

Point3D operator/(double e,Point3D p1){
	Point3D pR;
	pR.x = p1.x/e;
	pR.y = p1.y/e;
	pR.z = p1.z/e;
	return pR;
}

Point3D operator*(Point3D& p, double e){
	Point3D pR;
	pR.x = p.x*e;	pR.y = p.y*e;	pR.z = p.z*e;
	return pR; 	
}

Point3D operator*(double e,Point3D& p){
	Point3D pR;
	pR.x = p.x*e;	pR.y = p.y*e;	pR.z = p.z*e;
	return pR; 	
}


ostream& operator<<(ostream& o,const Point3D& p){
	o<<"("<< p.x <<"," << p.y << "," << p.z<<")";
	return o;
}

Point3D::~Point3D()
{
}
