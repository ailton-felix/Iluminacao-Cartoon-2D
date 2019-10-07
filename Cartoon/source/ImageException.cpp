#include<iostream>
#include<exception>

using namespace std;

#ifndef _IMAGEEXCEPTION_
#define _IMAGEEXCEPTION_

#include"../headers/ImageException.hpp"

const char *ImageException_::what() const throw(){
			return "Error: Problem to open image file.";
}


#endif
