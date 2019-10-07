#ifndef COLOR_HPP_
#define COLOR_HPP_

#include <list>

class Color{
public:
	double r, g, b;
	Color();
	Color(double,double,double);
	
	friend Color operator/(Color c,double e);
};



#endif /*COLOR_HPP_*/
