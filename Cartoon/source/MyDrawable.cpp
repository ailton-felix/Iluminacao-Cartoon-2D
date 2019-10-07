//Superclasse para o desenho grafico das entidades
#ifndef _MYDRAWABLE_
#define _MYDRAWABLE_
#include"../headers/MyDrawable.hpp"

MyDrawable::MyDrawable(){
	alpha = 1.0f;
	fColor[0] = 1.0f;
	fColor[1] = 1.0f;
	fColor[2] = 1.0f;
	bColor[0] = 1.0f;
	bColor[1] = 1.0f;
	bColor[2] = 1.0f;
	hidden = false;
}
void MyDrawable::setAlpha(float a){
	alpha = a;
}
void MyDrawable::setFColor(float r, float g, float b){
	fColor[0] = r;
	fColor[1] = g;
	fColor[2] = b;
}
void MyDrawable::setBColor(float r, float g, float b){
	bColor[0] = r;
	bColor[1] = g;
	bColor[2] = b;
}


void MyDrawable::show(){}
void MyDrawable::hide(){}
void MyDrawable::clear(){}
void MyDrawable::update(){}
MyDrawable::~MyDrawable(){};

#endif
