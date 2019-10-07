#ifndef _POINT3D_H
#define _POINT3D_H

#include <iostream>
#include <cmath>

using namespace std;

class Point3D{
	public:
		double x ,y ,z;
		
		
		Point3D();
		Point3D(double, double, double);

		void setPoint(double, double, double);

		void getPoint(double *a, double *b , double *c);


//		Point3D operator ^(Point3D);
		bool operator!=(Point3D a);
		bool operator==(Point3D a);
		
		friend Point3D operator+(Point3D p1, Point3D p2);
		friend Point3D operator-(Point3D p1,Point3D p2);
		friend Point3D operator/(Point3D p1,double e);
		friend Point3D operator/(double e,Point3D p1);
		friend Point3D operator*(Point3D& p, double e);
		friend Point3D operator*(double e,Point3D& p);
	
		friend ostream& operator<<(ostream& o,const Point3D& p);
		
		void normalize(); //normaliza um ponto qualquer


		~Point3D();

};

#endif
