#ifndef CURVE_HPP_
#define CURVE_HPP_

#include <list>
#include "Point2D.hpp"

using namespace std;

typedef list<Point2D>::iterator PointsIterator;

class Curve{		    	
	public:		
		//representation like list of points
		//{ (x,y), (x,y) , (x,y) ...}
	    list<Point2D> listPoints;		    
	    bool flagFechado; //diz se a curva é aberta ou fechada;
	    
//	    list<PointsIterator>  curveIterator;
	    
		Curve();
		~Curve();		
};



#endif /*CURVE_HPP_*/
