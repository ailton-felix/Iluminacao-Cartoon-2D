#include<exception>

using namespace std;

#ifndef _IMAGEEXCEPTION_H
#define _IMAGEEXCEPTION_H


class ImageException_:public exception{
	private:
	public: 
		virtual const char *what() const throw();
};

#endif
