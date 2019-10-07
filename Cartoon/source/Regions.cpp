#include<fstream>
#include<GL/gl.h>
#include<cmath>
#include<iostream>
#include "../headers/Regions.hpp"
#include "../headers/HierarchyRegions.hpp"


Regions::Regions(){}


Regions::Regions(Image *imgThin){	
	this->imageThinning = imgThin;
	imageThinning->setBorder(imageThinning->im,0);
	auxThin = new Image();
}

// i 0->DIREITA
// j 0->CIMA
int Regions::index(int i, int j){
      return (i + j*imageThinning->im->width) ;
}

/*Implementation algorithm Flood Fill recursive 4-connect
 *This is the most simple algorithm to make, but also the slowest.
 *Also, since it uses a recursive function, the recursion stack may
 *overflow making it crash when filling larger areas.*/   
 
void Regions::floodFillRecursive(int x,int y, int newColor,int oldColor){			
	if( (x>= 0) && (x < imageThinning->im->width) && (y>= 0) && (y < imageThinning->im->height) && (pImage[index(x,y)]==(unsigned char)oldColor) && (pImage[index(x,y)]!=(unsigned char)newColor)){		
		pImage[index(x,y)]= (unsigned char)newColor; //set color before starting recursion															
        floodFillRecursive(x + 1, y,     newColor,oldColor);        
        floodFillRecursive(x - 1, y,     newColor,oldColor);        
        floodFillRecursive(x,     y + 1, newColor,oldColor);        
        floodFillRecursive(x,     y - 1, newColor,oldColor);        
	}		
}
/*Implemntation algorithm Flood Fill queue 4-connect
 * this is the same that recursive algorithm but we 
 * have the control of queue and we can stop it for 
 * it doesn't have overflow.*/

/*This algorithm was changed, it store the points of region's boundary too*/

void Regions::floodFillQueue(int a,int b,int targetColor,int replacementColor){
	Point2D point;
	list<Point2D> queue;
	list<Point2D>::iterator front;
	Interior *interiorPoints;
	Boundary *borderPoints;
	int x=0,y=0;
	
	if(pImage[index(a,b)]!=(unsigned char)targetColor) return;
	
	interiorPoints=new Interior;
	borderPoints=new Boundary;
	
	point.setPoint(a,b);
	queue.push_back(point);
	interiorPoints->push_back(point);	
	
	while(!queue.empty()){
		front=queue.begin();
		front->getPoint(&x,&y);						
		pImage[index(x,y)]=(unsigned char) replacementColor;
		
		if(pImage[index(x,y-1)]==(unsigned char) targetColor){
			pImage[index(x,y-1)] = (unsigned char)replacementColor;
			point.setPoint(x,y-1);			
			interiorPoints->push_back(point);
			queue.push_back(point);
		}else{
			if(pImage[index(x,y-1)]==(unsigned char) 0){
				point.setPoint(x,y-1);
				borderPoints->push_back(point);	
			}
		}
		if(pImage[index(x,y+1)]==(unsigned char) targetColor){
			pImage[index(x,y+1)]= (unsigned char)replacementColor;			
			point.setPoint(x,y+1);
			interiorPoints->push_back(point);
			queue.push_back(point);
		}else{
			if(pImage[index(x,y+1)]==(unsigned char) 0){
				point.setPoint(x,y+1);
				borderPoints->push_back(point);
			}
		}
		if(pImage[index(x+1,y)]==(unsigned char) targetColor){
			pImage[index(x+1,y)]=(unsigned char)replacementColor;
			point.setPoint(x+1,y);
			interiorPoints->push_back(point);
			queue.push_back(point);
		}else{
			if(pImage[index(x+1,y)]==(unsigned char)0){
				point.setPoint(x+1,y);
				borderPoints->push_back(point);
			}
		}
		if(pImage[index(x-1,y)]==(unsigned char) targetColor){
			pImage[index(x-1,y)]=(unsigned char)replacementColor;
			point.setPoint(x-1,y);
			interiorPoints->push_back(point);
			queue.push_back(point);
		}else{
			if(pImage[index(x-1,y)]==(unsigned char)0){
				point.setPoint(x-1,y);
				borderPoints->push_back(point);
			}
		}	
		queue.pop_front();								
	}
	listInterior.push_back(interiorPoints);
	listScatteredBoundary.push_back(borderPoints);

}

////////////////////////////////////////////////////
void Regions::showBoundaryPoints(){
	list<Boundary*>::iterator iterC = listScatteredBoundary.begin();
	list<Point2D>::iterator iterP;
	int a,b;
	unsigned int listBoundarySize = listScatteredBoundary.size();
	for(unsigned int i=0;i<listBoundarySize;i++){
		unsigned int listPointsSize = (*iterC)->size();
		iterP = (*iterC)->begin();
		for(unsigned int j=0;j<listPointsSize;j++){
			iterP->getPoint(&a,&b);
			pImage[index(a,b)]=(unsigned char)255;
			iterP++;
		}
		cout <<endl;
		iterC++;
	}
}
bool Regions::searchPixel(int color,int *a,int *b){
	for(int i=0;i<imageThinning->im->width;i++){
		for(int j=0;j<imageThinning->im->height;j++){
			if(pImage[index(i,j)]==(unsigned char)color){
				 *a=i;
				 *b=j;
				 return true; 
			}			
		}
	}
	return false;
}

void Regions::showRegionsPoints(){
	int a=0,b=0;
	list<Interior*>::iterator iterR = listInterior.begin();	
	list<Point2D>::iterator iterP = (*iterR)->begin();	 
	
	unsigned int listRegionSize = listInterior.size();
	
  	for(unsigned int j=0;j< listRegionSize;j++){
  		cout <<"Regiao["<<j<<"]->";  						
  		iterP = (*iterR)->begin();
  		unsigned int listPointSize = (*iterR)->size();
		for(unsigned int i=0;i<listPointSize;i++){
			iterP->getPoint(&a,&b);						
			cout <<"Pontos["<<a<<","<<b<<"]";	
			iterP++;	
		}
		cout << endl;
		iterR++;
  	}
}

void Regions::detectRegions(){	
	auxThin->im = imImageCreate(imageThinning->im->width,imageThinning->im->height,IM_GRAY,imageThinning->im->data_type);
	imConvertColorSpace(imageThinning->im,auxThin->im);
	pImage = (unsigned char*)auxThin->im->data[0];
	
	int a,b;a=b=0;	
//	int color = 128; Valor incial definido por eles.
	int color = 80;  //A - Meu valor inicial.
    while(imageThinning->searchDepthOnePixel(255,&a,&b,pImage)){	
    	floodFillQueue(a,b,255,color);
    	color=color+20;
    }	    
}





void Regions::drawRegion(){	
	glRasterPos2f(0.0f,0.0f);
  
	char *gl_data;  
    gl_data = NULL;
    gl_data = new char[auxThin->im->width*auxThin->im->height*auxThin->im->depth];
	
    imConvertPacking(auxThin->im->data[0], gl_data, auxThin->im->width, auxThin->im->height, auxThin->im->depth, auxThin->im->data_type, 0); /*we convert to unpacked because here is 
                                                                                                 no mapping for non IM_PACKED images and imImage structure         
                                                                                               has packed data*/                                                                                                                                                                                  
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); /* data alignment must be 1 */
    glDrawPixels(auxThin->im->width, auxThin->im->height,GL_LUMINANCE, GL_UNSIGNED_BYTE, (GLvoid*)gl_data); 
    delete[] gl_data;
}

Regions::~Regions(){
	for(list<Boundary*>::iterator iter= listScatteredBoundary.begin();
	iter!=listScatteredBoundary.end();iter++)
		(*iter)->clear();
	listScatteredBoundary.clear();
//	delete[] auxThin;	
	//listRegion.clear();
	//listBoundary.clear();
}

////////////////////////////////////////
void Regions::computeInteriorAndScatteredBoundary(void){	
	detectRegions();  			
}
///////////////////////////////////////
void Regions::computeExternalVectorization(void){
	
	listBoundary = new Vectorization(&listScatteredBoundary, imageThinning->width, imageThinning->height);
	listBoundary->curveExtraction();
}
////////////////////////////////////////
void Regions::computeInternalVectorization(void){	
	listBoundary->extractHierarchy(hierarchy);
}   		
////////////////////////////////////////
void Regions::computeHierarchy(void){	
	hierarchy = new HierarchyRegions(&listInterior);
	hierarchy->detectHierarchyRegions(listBoundary); 
	hierarchy->buildPrimaryDescendentList();
}

/////////////////Funcao Principal////////////////////////
void Regions::computeRegions(void){
	computeInteriorAndScatteredBoundary();
	computeExternalVectorization();
	computeHierarchy();
	computeInternalVectorization();
	
	flag_useNormalVisibility.resize(listInterior.size()); 
    flag_setAsBackground.resize(listInterior.size());
}


/*********************************************************/
void Regions :: findRegion(int *r,int x, int y){
	
	int a=0,b=0; char s = 0;
	list<Interior*>::iterator iterR = listInterior.begin();	
	list<Point2D>::iterator iterP = (*iterR)->begin();	 
	unsigned int listRegionSize = listInterior.size();
  	for(int j=0;j < (int)listRegionSize;j++){
  		if(s == 0){			
	  		iterP = (*iterR)->begin();
	  		unsigned int listPointSize = (*iterR)->size();
			for(int i=0;i < (int)listPointSize;i++){
				iterP->getPoint(&a,&b);	
				if(a == x && y == b){
//					cout << " regiao " << j << " " << i << endl;
					*r = j; s = 's';break;
					
				}iterP++;
			}iterR++;
  		}
  	}
}
