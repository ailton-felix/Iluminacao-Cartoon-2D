#ifndef HIERARCHYREGIONS_HPP_
#define HIERARCHYREGIONS_HPP_

#include "Vectorization.hpp"
#include "defs.hpp"


class Vectorization;

class HierarchyRegions
{
	public:
		
		list<Interior*> *listInterior;//input
		
		list<Label> ascendentList; //output
		list<Label> descendentList;//output
		
		HierarchyRegions();
		HierarchyRegions(list<Interior*> *listInt);
		
		int numIntersections(Point2D *p,Curve *c);
		Point2D choosePointOfRegion(Interior *r);
		
		bool findPoint(unsigned int position,int label);
		bool isInsideRegion(list<Interior*>::iterator Ri,  list<Curve>::iterator curvesRegion);
		bool thereAreIntersection(list<Point2D>::iterator,list<Point2D>::iterator,Curve *c);
		
		
		void buildPrimaryDescendentList();
		void buildDescendentList();
		void writeAscendentList();
		void writeDescendentList();
		
		void detectHierarchyRegions(Vectorization *setCurve);
		
		~HierarchyRegions();
		
	
};

#endif /*HIERARCHYREGIONS_HPP_*/
