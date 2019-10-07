#include "../headers/Color.hpp"

Color::Color(double r,double g,double b){
	this->r = r;
	this->g = g;
	this->b = b;
}

Color::Color(){
	r = 1.0;
	g = 1.0;
	b = 1.0;
}

Color operator/(Color c,double e){
	Color C;
	C.r = c.r/e;
	C.g = c.g/e;
	C.b = c.b/e;
	return C;
};
