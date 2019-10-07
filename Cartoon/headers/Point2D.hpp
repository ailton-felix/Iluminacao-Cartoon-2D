#ifndef POINT2D_HPP_
#define POINT2D_HPP_

#include<iostream>
using namespace std;

class Point2D
{
public:
	int x ,y ;

	Point2D();
	Point2D(int x, int y);

	void setPoint(int a,int b);

	void getPoint(int *a,int *b);

	int operator ^(Point2D);
	bool operator!=(Point2D a);
	bool operator==(Point2D a);
	
	friend ostream& operator<<(ostream& o,const Point2D& p);

	~Point2D();
};



#endif /*POINT2D_HPP_*/
