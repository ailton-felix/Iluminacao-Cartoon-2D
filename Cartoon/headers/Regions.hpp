#ifndef REGIONIMAGE_HPP_
#define REGIONIMAGE_HPP_

#include <vector>
#include "Thinning.hpp"
#include "Vectorization.hpp"
#include "defs.hpp"

using namespace std;

//Cada elemento i das listas abaixo se referem a uma regiao i
class Regions{
			
    public:
    	//Input
    	unsigned char *pImage;
    	Image *imageThinning;
    	Image *auxThin;   
    	list<Boundary*> listScatteredBoundary; // initial pre processed boundary
    	
    	//Output
    	list<Interior*>  listInterior; //lista dos pontos internos
    	HierarchyRegions *hierarchy;
    	Vectorization    *listBoundary; //curve set that make up the region boundaries and inside curves
    	
		//Flags
    	vector<bool> flag_useNormalVisibility; 
    	vector<bool> flag_setAsBackground;
    	
    	Regions();
   		Regions(Image *imgThin);   	
   		int  index(int i, int j);
   		bool searchPixel(int color,int *a,int *b);
		void showBoundaryPoints();
		void showRegionsPoints();
   		void floodFillRecursive(int x,int y, int newColor,int oldColor);
   		void floodFillQueue(int a,int b,int targetColor,int replancementColor);	
   		void detectRegions();
   		void drawRegion();
   		
   		~Regions();
   		
   		////////////////////////////////////////
   		void computeInteriorAndScatteredBoundary(void);
   	   	void computeExternalVectorization(void);
		void computeInternalVectorization(void);
   	   	void computeHierarchy(void);
   		void computeRegions(void);
   		
   		////////////////////////////////////////
   		void findRegion(int*,int,int);//novo
   		
 
};

#endif /*Regions_HPP_*/
