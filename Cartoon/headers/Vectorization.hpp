#ifndef VECTORIZATION_HPP_
#define VECTORIZATION_HPP_

#include <stdlib.h>
#include "PointDirection.hpp"
#include "Image.hpp"
#include "HierarchyRegions.hpp"
#include "defs.hpp"

class HierarchyRegions;

class Vectorization{
private:
	//atributos
	//Regions* regions;
	list<Boundary*> *scatteredBoundary; //from regions
	char			*matrixBuffer;
	int 			w,h;
	
		
	int index(int i, int j){return (i + j*w);}
	void initializeMatrix(int listPosition);
	void initializeMatrix();
	bool isBlack(int i, int j);
	PointDirection nextPoint(PointDirection point);
	int previewPoint(PointDirection v);
    list<PointDirection> findNeighbor(Point2D p);
  
	
    void extractBackGround();
    void extract(Point2D vo, Curve *curve);
    void extract2(int regionPosition);
    void initializeBackGround(HierarchyRegions *aCurves);
	
public:
	list<Curves> regionCurves;
	
    Vectorization();
    Vectorization(list<Boundary*> *listScatBound, int wid, int hei);
    ~Vectorization();
    
    void extractHierarchy(HierarchyRegions *hR);
    void curveExtraction();
    
    void drawPoints();
   	void showPoints();
   	
   	/////////////////////////////////
   	void findCurve(int*,int*,int,int);//novo
   	
	
};


#endif /*Vectorization_HPP_*/
