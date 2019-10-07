#ifndef THINNING_HPP_
#define THINNING_HPP_
#include "../headers/Image.hpp"


class Thinning
{		
public:
	Image *imNew;
	
	Thinning(Image *image);
    bool isContourPoint(unsigned char* img,int i, int j, int *p,int *flag,int *flag1);	//points of boundary ,flags(number of neighbours and transitions)
    bool firstCondicional(int p);//condicion 2<=N(p1)<=6
	bool secondCondicional(int p);//condicion S(p1)=1
    bool thirdCondicional(int *p);//condicion p2*p4*p6=0
	bool fourthCondicional(int *p);//condicion p4*p6*p8=0
    bool seventhCondicional(int *p);//condicion p2*p4*p8=0
    bool eighthCondicional(int *p); //condicion p2*p6*p8=0
    void extractNeighboors(unsigned char* image,int i, int j,int *p);
	void deleteAllPixels(vector<int> *v,unsigned char* img);
	int  index(int i, int j);
	void thinningAlgorithm();  
	void draw();
	virtual ~Thinning();
};

#endif /*THINNING_HPP_*/
