#include "../headers/Normal.hpp"
#include "../headers/UtilityFunctions.hpp"
#include "../headers/LightingModel.hpp"
#include <cstdlib>
#include <iomanip>
#include <limits>

#define vec regions->listBoundary

//const double CURVATURE = 0.237; //3ª opção
//const double CURVATURE = 0.200; //2ª opção
const double CURVATURE = 0.100; //1ª opção

///////////////////////////////////////////////////////////////////////////////

Normal::Normal(Regions *r){
	Point3D p;
	regions = r;
	w=r->imageThinning->width;
	h=r->imageThinning->height;
	
	normalBuffer.resize(w*h,p);
}
///////////////////////////////////////////////////////////////////////////////

/**
 * Verifies the curve orientation. Returns TRUE if clockwise or FALSE if the 
 * curve is counter clockwise.
 */
bool Normal::orientation(list<Point2D> *listPoint){
	list<Point2D>::iterator iter1=listPoint->begin();
	list<Point2D>::iterator iter2=listPoint->begin();
	iter2++;
	double anti_horario=0;
	unsigned int pointsSize=listPoint->size();
	for(unsigned int i=2; i<pointsSize; i++, iter1++, iter2++){
		anti_horario+=(*iter1)^(*iter2);
	}
	if(anti_horario>0){
		return true;
	}
	else{
		return false;
	}
}

///////////////////////////////////////////////////////////////////////////////

#if 0
void Normal::extractBoundaryNormals(){
	Point3D n;
	Curve tCurve;
	Curves tCurves;
	list<Curves>::iterator iter = vec->regionCurves.begin();
	list<Curve>::iterator iter2;
	list<Point2D>::iterator iter3, iter4;
	normalArray na;
	
	unsigned int regionsSize=vec->regionCurves.size();
	for (unsigned int i = 0; i < regionsSize; i++, iter++) {
		iter2 = iter->begin();
		unsigned int curvesSize=iter->size();
		for (unsigned int j=0; j<curvesSize; j++, iter2++) {
//			cout<<this->orientation(&(iter2->listPoints))<<endl;
			iter3 = iter2->listPoints.begin();
			iter4=iter2->listPoints.begin();
			iter4++;
//			if((*iter2).flag==true){
			n.setPoint(iter4->y-iter3->y, iter3->x-iter4->x, 0.0);
			n.normalize();
			normalBuffer[index(iter3->x, iter3->y, w)]=n;
			iter3 = iter2->listPoints.end();
			iter3--;
			normalBuffer[index(iter3->x, iter3->y, w)]=n;
			iter3 = iter2->listPoints.begin();
			unsigned int size=iter2->listPoints.size();
			for(unsigned int k =1; k<size; k++, iter3++, iter4++){
				n.setPoint(iter4->y-iter3->y, iter3->x-iter4->x, 0.0);
				n.normalize();
				normalBuffer[index(iter3->x, iter3->y, w)]=n;			
			}
		}
}
#endif
	
/////////////////////////////////////////////////////////////////
	void Normal::extractBoundaryNormals(){
		Point3D n;
		Curve tCurve;
		Curves tCurves;
		list<Curves>::iterator iter = vec->regionCurves.begin();
		list<Curve>::iterator iter2;
		list<Point2D>::iterator iter3, iter4;
		unsigned int i,j,k,size;
		
		normalArray		na;
		normalToRegion	nr;
		
		unsigned int regionsSize=vec->regionCurves.size();
		for (i = 0; i < regionsSize; i++, iter++) {
			iter2 = iter->begin();
			unsigned int curvesSize=iter->size();
			for (j=0; j<curvesSize; j++, iter2++)
			{
				iter3 = iter4=iter2->listPoints.begin();
				iter4++;
				size=iter2->listPoints.size();
				Point3D first;
				first.setPoint(iter4->y-iter3->y, iter3->x-iter4->x, 0.0);
				for(k=1; k<size; k++, iter3++, iter4++){	
					n.setPoint(iter4->y-iter3->y, iter3->x-iter4->x, 0.0);
					n.normalize();
					na.push_back(n);				
				}
				if((*iter2).flagFechado==true){
					na.push_back(first);
				}
				else{
					na.push_back(n);
				}
				nr.push_back(na);
				na.clear();
			}
			normalToBound.push_back(nr);
			nr.clear();
	}
}
//	Point3D n;
//	list<Point2D>::iterator iter3, iter4;
//	
//	for (list<Curves>::iterator iter = vec->regionCurves.begin(); iter!= vec->regionCurves.end(); iter++) {
//		for (list<Curve>::iterator iter2 = iter->begin(); iter2!=iter->end(); iter2++) {
//			list<Point2D>::iterator iter3 = iter2->listPoints.begin();
//			list<Point2D>::iterator iter4 = iter2->listPoints.begin();
//			iter4++;
//			for(; iter4!=iter2->listPoints.end(); iter3++, iter4++){
//				n.setPoint(iter4->y-iter3->y, iter3->x-iter4->x, 0.0);
//				n.normalize();
////				cout<<(*iter3)<<"->"<<(*iter4)<<"="<<n<<endl;
//				normalBuffer[index(iter3->x, iter3->y, this->w)]=n;
//			}
//			if(iter2->flag==false){
//				normalBuffer[index(iter3->x, iter3->y, this->w)] = n;
//			}
//		}
//		cout<<endl;
//	}
//}

///////////////////////////////////////////////////////////////////////////////

void Normal::smoothBoundaryNormals(){
	list<Curves> :: iterator iter;
	list<Curve>::iterator iter2;
	list<Point2D>::iterator iter3;
	Point3D p;
	unsigned int i,j,k,regionsSize=vec->regionCurves.size(),curvesSize,curveSize;

	for(iter = vec->regionCurves.begin(), i=0; i<regionsSize; i++, iter++){
		curvesSize=iter->size();
		for(j=0,iter2=iter->begin(); j<curvesSize; j++, iter2++){
			curveSize=iter2->listPoints.size();
			for(k=2; k<curveSize-2; k++){
				p.x = normalToBound[i][j][k-2].x + 4*normalToBound[i][j][k-1].x + 6*normalToBound[i][j][k].x 
						+4*normalToBound[i][j][k+1].x + normalToBound[i][j][k+2].x;
				p.y = normalToBound[i][j][k-2].y + 4*normalToBound[i][j][k-1].y + 6*normalToBound[i][j][k].y 
						+4*normalToBound[i][j][k+1].y + normalToBound[i][j][k+2].y;
				p.setPoint(p.x,p.y,p.z);
				p.normalize();
				normalToBound[i][j][k]=p;
			}

		}

	}
}

//////////////////////////////////////////////////////////
#if 0
void Normal::smoothBoundaryNormals(){
	list<Curves> :: iterator iter = vec->regionCurves.begin();
	list<Curve>::iterator iter2;
	list<Point2D>::iterator iter3;
	Point2D antePenultimate, penultimate, last, first, second, third, fourth, prePrevious, previous, current, next, neNext;
	Point3D p;
	int i0, i1, i2, i3, i4;
	unsigned int regionsSize=vec->regionCurves.size();
	for(unsigned int i=0; i<regionsSize; i++, iter++){
		iter2=iter->begin();
		unsigned int curvesSize=iter->size();
		for(unsigned int j=0; j<curvesSize; j++, iter2++){
			iter3=iter2->listPoints.end();
			iter3--;//TO POINT TO THE LAST!!!
			iter3--;//LAST == FIRST, SO LAST = PENULTIMATE
			last=(*iter3);
			iter3--;
			penultimate=(*iter3);
			iter3--;
			antePenultimate=(*iter3);
			iter3=iter2->listPoints.begin();
			first=prePrevious=(*iter3);
			iter3++;
			second=previous=(*iter3);
			iter3++;
			third=current=(*iter3);
			iter3++;
			fourth=(*iter3);
			iter3--;
			unsigned int curveSize=iter2->listPoints.size();
			for(unsigned int k=2; k<curveSize-2; k++, iter3++){
				iter3++;
				next=(*iter3);
				iter3++;
				neNext=(*iter3);
				iter3--;
				iter3--;
				current=(*iter3);
				
				i0=index(prePrevious.x, prePrevious.y, w);
				i1=index(previous.x, previous.y, w);
				i2=index(iter3->x, iter3->y, w);
				i3=index(next.x, next.y, w);
				i4=index(neNext.x, neNext.y, w);
				p.setPoint(normalBuffer[i0].x + 4*normalBuffer[i1].x + 6*normalBuffer[i2].x + 4*normalBuffer[i3].x + normalBuffer[i4].x, 
						   normalBuffer[i0].y + 4*normalBuffer[i1].y + 6*normalBuffer[i2].y + 4*normalBuffer[i3].y + normalBuffer[i4].y, 0.0);
				p.normalize();
				normalBuffer[index(iter3->x,iter3->y,w)]=p;
				
				prePrevious=previous;
				previous=current;
			}
			if((*iter2).flagFechado==true){
				iter3=iter2->listPoints.begin();
				i0=index(penultimate.x, penultimate.y, w);
				i1=index(last.x, last.y, w);
				i2=index(iter3->x, iter3->y, w);
				i3=index(second.x, second.y, w);
				i4=index(third.x, third.y, w);
				p.setPoint(normalBuffer[i0].x + 4* normalBuffer[i1].x + 6*normalBuffer[i2].x + 4*normalBuffer[i3].x + normalBuffer[i4].x,
						   normalBuffer[i0].y + 4* normalBuffer[i1].y + 6*normalBuffer[i2].y + 4*normalBuffer[i3].y + normalBuffer[i4].y, 0.0);
				p.normalize();
				normalBuffer[index(current.x, current.y, w)]=p;
				normalBuffer[index(iter3->x, iter3->y, w)]=p;
				
				iter3++;
				
				i0=index(last.x, last.y, w);
				i1=index(first.x, first.y, w);
				i2=index(iter3->x, iter3->y, w);
				i3=index(third.x, third.y, w);
				i4=index(fourth.x, fourth.y, w);
				p.setPoint(normalBuffer[i0].x + 4* normalBuffer[i1].x + 6*normalBuffer[i2].x + 4*normalBuffer[i3].x + normalBuffer[i4].x,
						   normalBuffer[i0].y + 4* normalBuffer[i1].y + 6*normalBuffer[i2].y + 4*normalBuffer[i3].y + normalBuffer[i4].y, 0.0);
				p.normalize();
				normalBuffer[index(iter3->x, iter3->y, w)]=p;
				
				iter3=iter2->listPoints.end();
				iter3--;//TO POINT TO THE LAST!!!
				normalBuffer[index(iter3->x, iter3->y, w)]=normalBuffer[index(current.x, current.y, w)];
				
				iter3--;
				i0=index(antePenultimate.x, antePenultimate.y, w);
				i1=index(penultimate.x, penultimate.y, w);
				i2=index(iter3->x, iter3->y, w);
				i3=index(first.x, first.y, w);
				i4=index(second.x, second.y, w);
				
				p.setPoint(normalBuffer[i0].x + 4* normalBuffer[i1].x + 6*normalBuffer[i2].x + 4*normalBuffer[i3].x + normalBuffer[i4].x,
						   normalBuffer[i0].y + 4* normalBuffer[i1].y + 6*normalBuffer[i2].y + 4*normalBuffer[i3].y + normalBuffer[i4].y, 0.0);
				p.normalize();
				normalBuffer[index(iter3->x, iter3->y, w)]=p;
				
			}
			else{
				iter3=iter2->listPoints.begin();
				i0=index(iter3->x, iter3->y, w);
				i1=index(second.x, second.y, w);
				i2=index(third.x, third.y, w);
				p.setPoint(6*normalBuffer[i0].x + 4*normalBuffer[i1].x + normalBuffer[i2].x, 
						   6*normalBuffer[i0].y + 4*normalBuffer[i1].y + normalBuffer[i2].y, 0.0);
				p.normalize();
				normalBuffer[index(iter3->x, iter3->y, w)]=p;
				iter3++;
				
				i0=index(first.x, first.y, w);
				i1=index(iter3->x, iter3->y, w);
				i2=index(third.x, third.y, w);
				i3=index(fourth.x, fourth.y, w);
				p.setPoint(4*normalBuffer[i0].x + 6*normalBuffer[i1].x + 4*normalBuffer[i2].x + normalBuffer[i3].x, 
						   4*normalBuffer[i0].y + 6*normalBuffer[i1].y + 4*normalBuffer[i2].y + normalBuffer[i3].y, 0.0);
				p.normalize();
				normalBuffer[index(iter3->x, iter3->y, w)]=p;
				iter3=iter2->listPoints.end();
				iter3--;
				iter3--;
				iter3--;
				last=penultimate;
				penultimate=antePenultimate;
				antePenultimate=(*iter3);
				iter3=iter2->listPoints.end();
				iter3--;
				i0=index(iter3->x, iter3->y, w);
				i1=index(penultimate.x, penultimate.y, w);
				i2=index(antePenultimate.x, antePenultimate.y, w);
				p.setPoint(6*normalBuffer[i0].x + 4*normalBuffer[i1].x + normalBuffer[i2].x, 
						   6*normalBuffer[i0].y + 4*normalBuffer[i1].y + normalBuffer[i2].y, 0.0);
				p.normalize();
				normalBuffer[index(iter3->x, iter3->y, w)]=p;
				
				iter3--;
				i0=index(last.x, last.y, w);
				i1=index(iter3->x, iter3->y, w);
				i2=index(antePenultimate.x, antePenultimate.y, w);
				p.setPoint(4*normalBuffer[i0].x + 6*normalBuffer[i1].x + 4*normalBuffer[i2].x, 
						   4*normalBuffer[i0].y + 6*normalBuffer[i1].y + 4*normalBuffer[i2].y, 0.0);
				p.normalize();
				normalBuffer[index(iter3->x, iter3->y, w)];
			}
		}
	}
}
#endif

///////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////
////////////////////////////////////////////////////
////////////////////////////////////////////////////


//
//void Normal::extractCurveNormals(){
//	Point3D n;
//	list<Point>::iterator iter3, iter4;
//	for (list<Curves>::iterator iter = regionNormals->regionCurves.begin(); iter!= regionNormals->regionCurves.end(); iter++) { 
//		for (list<Curve>::iterator iter2 = iter->listCurve.begin(); iter2!=iter->listCurve.end(); iter2++) {
//			list<Point>::iterator iter3 = iter2->listPoints.begin();
//			list<Point>::iterator iter4 = iter2->listPoints.begin();
//			iter4++;
//			for(; iter4!=iter2->listPoints.end(); iter3++, iter4++){
//				n.setPoint(iter4->y-iter3->y, iter3->x-iter4->x, 0.0);
//				normalMap[index(iter3->x, iter3->y, this->width)] = n;
//			}
//			if(iter2->flag==false){
//				normalMap[index(iter4->x, iter4->y, this->width)] = n;
//			}
//		}
//	}
//}
//
//

////suavisa a curva
//void Normal::smooth(){
//	list<Curves> :: iterator iter = this->regionNormals->regionCurves.begin();
//	list<Curve>::iterator iter2;
//	list<Point>::iterator iter3;
//	Point antePenultimate, penultimate, last, first, second, third, fourth, prePrevious, previous, current, next, neNext;
//	unsigned int regionsSize=this->regionNormals->regionCurves.size();
//	for(unsigned int i=0; i<regionsSize; i++, iter++){
//		iter2=iter->listCurve.begin();
//		unsigned int curvesSize=iter->listCurve.size();
//		for(unsigned int j=0; j<curvesSize; j++, iter2++){
//			iter3=iter2->listPoints.end();
//			iter3--;//TO POINT TO THE LAST!!!
////			cout<<"Este eh o ultimo da lista: "<<(*iter3)<<endl;
//			iter3--;//LAST == FIRST, SO LAST = PENULTIMATE
//			last=(*iter3);
//			iter3--;
//			penultimate=(*iter3);
//			iter3--;
//			antePenultimate=(*iter3);
//			iter3=iter2->listPoints.begin();
////			cout<<"Este eh o primeiro da lista: "<<(*iter3)<<endl;
//			first=prePrevious=(*iter3);
//			iter3++;
//			second=previous=(*iter3);
//			iter3++;
//			third=current=(*iter3);
//			iter3++;
//			fourth=(*iter3);
//			iter3--;
//			unsigned int curveSize=iter2->listPoints.size();
//			for(unsigned int k=2; k<curveSize-2; k++, iter3++){
//				iter3++;
//				next=(*iter3);
//				iter3++;
//				neNext=(*iter3);
//				iter3--;
//				iter3--;
//				current=(*iter3);
////				cout<<"k: "<<k<<endl;
////				cout<<"prePrevious: "<<"("<<prePrevious.nOutX<<","<<prePrevious.nOutY<<","<<prePrevious.nOutZ<<")"<<endl;
////				cout<<"previous: "<<"("<<previous.nOutX<<","<<previous.nOutY<<","<<previous.nOutZ<<")"<<endl;
////				cout<<"current: "<<"("<<iter3->nOutX<<","<<iter3->nOutY<<","<<iter3->nOutZ<<")"<<endl;
////				cout<<"next: "<<"("<<next.nOutX<<","<<next.nOutY<<","<<next.nOutZ<<")"<<endl;
////				cout<<"neNext: "<<"("<<neNext.nOutX<<","<<neNext.nOutY<<","<<neNext.nOutZ<<")"<<endl;
//				
//				iter3->setNormal(prePrevious.nOutX+4*previous.nOutX+6*iter3->nOutX+4*next.nOutX+neNext.nOutX, prePrevious.nOutY+4*previous.nOutY+6*iter3->nOutY+4*next.nOutY+neNext.nOutY, prePrevious.nOutZ+4*previous.nOutZ+6*iter3->nOutZ+4*next.nOutZ+neNext.nOutZ, 
//						         prePrevious.nInX+ 4*previous.nInX+ 6*iter3->nInX+ 4*next.nInX+neNext.nInX,   prePrevious.nInY+ 4*previous.nInY+ 6*iter3->nOutY+4*next.nInY+neNext.nInY,   prePrevious.nInZ+ 4*previous.nInZ+ 6*iter3->nOutZ+4* next.nInZ+neNext.nInZ);
//				
////				cout<<"depois do calculo: "<<"("<<iter3->nOutX<<","<<iter3->nOutY<<","<<iter3->nOutZ<<")"<<endl<<endl;
////				system("pause");
//				prePrevious=previous;
//				previous=current;
//			}
//			if((*iter2).flag==true){
//				iter3=iter2->listPoints.begin();
//				current.setNormal(penultimate.nOutX+4*last.nOutX+6*iter3->nOutX+4*second.nOutX+third.nOutX, penultimate.nOutY+4*last.nOutY+6*iter3->nOutY+4*second.nOutY+third.nOutY, penultimate.nOutZ+4*last.nOutZ+6*iter3->nOutZ+4*second.nOutZ+third.nOutZ,
//						          penultimate.nInX+ 4*last.nInX+ 6*iter3->nInX+ 4*second.nInX+third.nInX,   penultimate.nInY+ 4*last.nInY+ 6*iter3->nInY+ 4*second.nInY+third.nInY,   penultimate.nInZ+ 4*last.nInZ+ 6*iter3->nInZ+ 4*second.nInZ+third.nInZ);
//				iter3->setNormal(current.nOutX, current.nOutY, current.nOutZ, current.nInX, current.nInY, current.nInZ);
//				iter3++;
//				iter3->setNormal(last.nOutX+4*first.nOutX+6*iter3->nOutX+4*third.nOutX+fourth.nOutX, last.nOutY+4*first.nOutY+6*iter3->nOutY+4*third.nOutY+fourth.nOutY, last.nOutZ+4*first.nOutZ+6*iter3->nOutZ+4*third.nOutZ+fourth.nOutZ,
//						         last.nInX+ 4*first.nInX+ 6*iter3->nInX+ 4*third.nInX+fourth.nInX,   last.nInY+ 4*first.nInY+ 6*iter3->nInY+ 4*third.nInY+ fourth.nInY,  last.nInZ+ 4*first.nInZ+ 6*iter3->nInZ+ 4*third.nInZ+ fourth.nInZ);
//				iter3=iter2->listPoints.end();
//				iter3--;//TO POINT TO THE LAST!!!
//				iter3->setNormal(current.nOutX, current.nOutY, current.nOutZ, current.nInX, current.nInY, current.nInZ);
//				iter3--;
//				iter3->setNormal(antePenultimate.nOutX+4*penultimate.nOutX+6*iter3->nOutX+4*first.nOutX+second.nOutX, antePenultimate.nOutY+4*penultimate.nOutY+6*iter3->nOutY+4*first.nOutY+second.nOutY, antePenultimate.nOutZ+4*penultimate.nOutZ+6*iter3->nOutZ+4*first.nOutZ+second.nOutZ,
//						         antePenultimate.nInX+ 4*penultimate.nInX+ 6*iter3->nInX+ 4*first.nInX+second.nInX,   antePenultimate.nInY+ 4*penultimate.nInY+ 6*iter3->nInY+ 4*first.nInY+ second.nInY, antePenultimate.nInZ+  4*penultimate.nInZ+ 6*iter3->nInZ+ 4*first.nInZ+second.nInZ);
//				
//			}
//			else{
//				iter3=iter2->listPoints.begin();
//				iter3->setNormal(6*iter3->nOutX+4*second.nOutX+third.nOutX, 6*iter3->nOutY+4*second.nOutY+third.nOutY, 6*iter3->nOutZ+4*second.nOutZ+third.nOutZ, 6*iter3->nInX+4*second.nInX+third.nInX, 6*iter3->nInY+4*second.nInY+third.nInY, 6*iter3->nInZ+4*second.nInZ+third.nInZ);
//				iter3++;
//				iter3->setNormal(4*first.nOutX+6*iter3->nOutX+4*third.nOutX+fourth.nOutX, 4*first.nOutY+6*iter3->nOutY+4*third.nOutY+fourth.nOutY, 4*first.nOutZ+6*iter3->nOutZ+4*third.nOutZ+fourth.nOutZ, 
//						         4*first.nInX+ 6*iter3->nInX+ 4*third.nInX+ fourth.nInX,  4*first.nInY+ 6*iter3->nInY+ 4*third.nInY+ fourth.nInY,  3*first.nInZ+ 6*iter3->nInZ+ 4*third.nInZ+ fourth.nInZ);
//				iter3=iter2->listPoints.end();
//				iter3--;
//				iter3--;
//				iter3--;
//				last=penultimate;
//				penultimate=antePenultimate;
//				antePenultimate=(*iter3);
//				iter3=iter2->listPoints.end();
//				iter3--;
//				iter3->setNormal(6*iter3->nOutX+4*penultimate.nOutX+antePenultimate.nOutX, 6*iter3->nOutY+4*penultimate.nOutY+antePenultimate.nOutY, 6*iter3->nOutZ+4*penultimate.nOutZ+antePenultimate.nOutZ, 
//						         6*iter3->nInX+ 4*penultimate.nInX+ antePenultimate.nInX,  6*iter3->nInY+ 4*penultimate.nInY+antePenultimate.nInY,   6*iter3->nInZ+ 4*penultimate.nInZ+ antePenultimate.nInZ);
//				iter3--;
//				iter3->setNormal(4*last.nOutX+6*iter3->nOutX+4*antePenultimate.nOutX, 4*last.nOutY+6*iter3->nOutY+4*antePenultimate.nOutY, 4*last.nOutZ+6*iter3->nOutZ+4*antePenultimate.nOutZ, 
//						         4*last.nInX+ 6*iter3->nInX+ 4*antePenultimate.nInX,  4*last.nInY+ 6*iter3->nInY+ 4*antePenultimate.nInY,  4*last.nInZ+ 6*iter3->nInZ+ 4*antePenultimate.nInZ);
//			}
//		}
//	}
//}

////////////////////////////////////////////////////////////////////////////////

//void Normal::smoothXtremes(int n){ //n is the number of interpolations
//	for(list<Curves> :: iterator iter = this->regionNormals->regionCurves.begin(); iter!=this->regionNormals->regionCurves.end(); iter++){
//		list<Curve>::iterator iter2=iter->listCurve.begin();
//		iter2++; //Open Curves only
//		for(; iter2!=iter->listCurve.end(); iter2++){
//			list<Point>::iterator iter3=iter2->listPoints.begin();
//			int xB, yB, xE, yE;
//			xB=iter3->x; yB=iter3->y;
//			iter3=iter2->listPoints.end();
//			iter3--;//PARA APONTAR PAR O FIM, SEN�O BABAU!
//			xE=iter3->x; yE=iter3->y;
//			bool brokenBegin, brokenEnd;
//			brokenCurve(xB, yB, xE, yE, &brokenBegin, &brokenEnd, &(iter->listCurve));
//			if(brokenBegin==false){
//				Point p1, p2, p3;
//				iter3=iter2->listPoints.begin();
//				iter3++; //Point to the second element
//				p2.setPoint(iter3->x, iter3->y);
//				p2.setNormalOut(iter3->nOutX, iter3->nOutY, iter3->nOutZ);
//				iter3=iter2->listPoints.begin();
//				iter3->setNormalOut(iter3->x-p2.x, iter3->y-p2.y, 0);
//				double norma=sqrt((iter3->nOutX*iter3->nOutX)+(iter3->nOutY*iter3->nOutY)+(iter3->nOutZ*iter3->nOutZ));
//				iter3->setNormal(iter3->nOutX/norma,iter3->nOutY/norma, iter3->nOutZ/norma, iter3->nInX/norma, iter3->nInY/norma, iter3->nInZ/norma);
//				p1.setPoint(iter3->x, iter3->y, 0);
//				p1.setNormalOut(iter3->nOutX, iter3->nOutY, iter3->nOutZ);
//				double dT=1.0/(double)(n+1);
//				p3.setPoint(p2.x, p2.y, 0);
//				p3.setNormalOut(-p2.nOutX, -p2.nOutY, 0);
//				iter2->listPoints.push_front(p3);
//				p3.setPoint(iter3->x, iter3->y, 0);
//				double t=dT;
//				for(int i=0; i<n; i++){
//					p3.setNormalOut((1-t)*p1.nOutX + t*p2.nOutX, (1-t)*p1.nOutY + t*p2.nOutY, 0);
//					norma=sqrt((p3.nOutX*p3.nOutX)+(p3.nOutY*p3.nOutY)+(p3.nOutZ*p3.nOutZ));
//					p3.setNormal(p3.nOutX/norma,p3.nOutY/norma, p3.nOutZ/norma, p3.nInX/norma, p3.nInY/norma, p3.nInZ/norma);
//					iter2->listPoints.push_front(p3);
//					p3.setNormalOut((1-t)*p1.nOutX - t*p2.nOutX, (1-t)*p1.nOutY - t*p2.nOutY, 0);
//					norma=sqrt((p3.nOutX*p3.nOutX)+(p3.nOutY*p3.nOutY)+(p3.nOutZ*p3.nOutZ));
//					p3.setNormal(p3.nOutX/norma,p3.nOutY/norma, p3.nOutZ/norma, p3.nInX/norma, p3.nInY/norma, p3.nInZ/norma);
//					norma=sqrt((p3.nOutX*p3.nOutX)+(p3.nOutY*p3.nOutY)+(p3.nOutZ*p3.nOutZ));
//					p3.setNormal(p3.nOutX/norma,p3.nOutY/norma, p3.nOutZ/norma, p3.nInX/norma, p3.nInY/norma, p3.nInZ/norma);
//					iter2->listPoints.push_front(p3);					
//					p3.setNormalOut((t-1)*p1.nOutX - t*p2.nOutX, (t-1)*p1.nOutY - t*p2.nOutY, 0);
//					norma=sqrt((p3.nOutX*p3.nOutX)+(p3.nOutY*p3.nOutY)+(p3.nOutZ*p3.nOutZ));
//					p3.setNormal(p3.nOutX/norma,p3.nOutY/norma, p3.nOutZ/norma, p3.nInX/norma, p3.nInY/norma, p3.nInZ/norma);
//					iter2->listPoints.push_front(p3);
//					t+=dT;
//				}
//			}
//			
//			if(brokenEnd==false){
//				Point p1, p2, p3;
//				iter3=iter2->listPoints.end();
//				iter3--; //Point to the last element
//				iter3--; //Point to the penultimate element
//				p2.setPoint(iter3->x, iter3->y);
//				p2.setNormalOut(iter3->nOutX, iter3->nOutY, iter3->nOutZ);
//				iter3=iter2->listPoints.end();
//				iter3--; //Point to the last element
//				iter3->setNormalOut(iter3->x-p2.x, iter3->y-p2.y, 0);
//				double norma=sqrt((iter3->nOutX*iter3->nOutX)+(iter3->nOutY*iter3->nOutY)+(iter3->nOutZ*iter3->nOutZ));
//				iter3->setNormal(iter3->nOutX/norma,iter3->nOutY/norma, iter3->nOutZ/norma, iter3->nInX/norma, iter3->nInY/norma, iter3->nInZ/norma);
//				p1.setPoint(iter3->x, iter3->y, 0);
//				p1.setNormalOut(iter3->nOutX, iter3->nOutY, iter3->nOutZ);
//				double dT=1.0/(double)(n+1);
//				p3.setPoint(p2.x, p2.y, 0);
//				p3.setNormalOut(-p2.nOutX, -p2.nOutY, 0);
//				iter2->listPoints.push_back(p3);
//				p3.setPoint(iter3->x, iter3->y, 0);
//				double t=dT;
//				for(int i=0; i<n; i++){
//					p3.setNormalOut((1-t)*p1.nOutX + t*p2.nOutX, (1-t)*p1.nOutY + t*p2.nOutY, 0);
//					norma=sqrt((p3.nOutX*p3.nOutX)+(p3.nOutY*p3.nOutY)+(p3.nOutZ*p3.nOutZ));
//					p3.setNormal(p3.nOutX/norma,p3.nOutY/norma, p3.nOutZ/norma, p3.nInX/norma, p3.nInY/norma, p3.nInZ/norma);
//					iter2->listPoints.push_back(p3);
//					p3.setNormalOut((1-t)*p1.nOutX - t*p2.nOutX, (1-t)*p1.nOutY - t*p2.nOutY, 0);
//					iter2->listPoints.push_back(p3);
//					t+=dT;
//				}
//			}
//			
//		}
//	}
//}

////////////////////////////////////////////////////////////////////////////////

//void Normal::brokenCurve(int xB, int yB, int xE, int yE, bool *brokenBegin, bool *brokenEnd, list<Curve> *listCurves){
//	(*brokenBegin)=(*brokenEnd)=false;
//	list<Curve> :: iterator iter = listCurves->begin();
//	iter++; //Open Curves only
//	for(; iter!=listCurves->end(); iter++){
//		list<Point>::iterator iter2=iter->listPoints.begin();
//		if(iter2->x==xB && iter2->y==yB+1){
//			(*brokenBegin)=true;
//		}
//		else if(iter2->x==xB+1 && iter2->y==yB+1){
//			(*brokenBegin)=true;
//		}
//		else if(iter2->x==xB+1 && iter2->y==yB){
//			(*brokenBegin)=true;
//		}
//		else if(iter2->x==xB+1 && iter2->y==yB-1){
//			(*brokenBegin)=true;
//		}
//		else if(iter2->x==xB && iter2->y==yB-1){
//			(*brokenBegin)=true;
//		}
//		else if(iter2->x==xB-1 && iter2->y==yB-1){
//			(*brokenBegin)=true;
//		}
//		else if(iter2->x==xB-1 && iter2->y==yB){
//			(*brokenBegin)=true;
//		}
//		else if(iter2->x==xB-1 && iter2->y==yB+1){
//			(*brokenBegin)=true;
//		}
//		
//		iter2=iter->listPoints.end();
//		if(iter2->x==xE &&iter2->y==yE+1){
//			(*brokenEnd)=true;
//		}
//		else if(iter2->x==xE+1 &&iter2->y==yE+1){
//			(*brokenEnd)=true;
//		}
//		else if(iter2->x==xE+1 &&iter2->y==yE){
//			(*brokenEnd)=true;
//		}
//		else if(iter2->x==xE+1 && iter2->y==yE-1){
//			(*brokenEnd)=true;
//		}
//		else if(iter2->x==xE &&iter2->y==yE-1){
//			(*brokenEnd)=true;
//		}
//		else if(iter2->x==xE-1 &&iter2->y==yE-1){
//			(*brokenEnd)=true;
//		}
//		else if(iter2->x==xE-1 &&iter2->y==yE){
//			(*brokenEnd)=true;
//		}
//		else if(iter2->x==xE-1 &&iter2->y==yE+1){
//			(*brokenEnd)=true;
//		}
//		
//		if((*brokenBegin)==(*brokenEnd)==true){
//			break;
//		}
//	}
//}

////////////////////////////////////////////////////////////////////////////////

//void Normal::normalize(){
//	list<Curves> :: iterator iter = this->regionNormals->regionCurves.begin();
//	list<Curve>::iterator iter2;
//	list<Point>::iterator iter3;
//	double norma;
//	unsigned int regionsSize=this->regionNormals->regionCurves.size();
//	for(unsigned int i=0; i<regionsSize; i++, iter++){
//		iter2=iter->listCurve.begin();
//		unsigned int curvesSize=iter->listCurve.size();
//		for(unsigned int j=0; j<curvesSize; j++, iter2++){
//			iter3=iter2->listPoints.begin();
//			unsigned int curveSize=iter2->listPoints.size();
//			for(unsigned int k=0; k<curveSize; k++, iter3++){
//				norma=sqrt((iter3->nOutX*iter3->nOutX)+(iter3->nOutY*iter3->nOutY)+(iter3->nOutZ*iter3->nOutZ));
//				iter3->setNormal(iter3->nOutX/norma,iter3->nOutY/norma, iter3->nOutZ/norma, iter3->nInX/norma, iter3->nInY/norma, iter3->nInZ/norma);
//			}
//		}
//	}
//}

////////////////////////////////////////////////////////////////////////////////

void Normal::printBoundaryNormal(DIRECTION d){
	list<Curves> :: iterator iter = vec->regionCurves.begin();
	list<Curve>::iterator iter2;
	list<Point2D>::iterator iter3;
	unsigned int regionsSize=vec->regionCurves.size();
	for(unsigned int i=0; i<regionsSize; i++, iter++){
		iter2=iter->begin();
		unsigned int curvesSize=iter->size();
		for(unsigned int j=0; j<curvesSize; j++, iter2++){
			iter3=iter2->listPoints.begin();
			if(d==NORMALOUT){
				cout<<"-----------NORMAIS PARA FORA---------"<<endl;
				cout<<"Ponto-->NormalOut"<<endl;
				unsigned int curveSize=iter2->listPoints.size();
				for(unsigned int k=0; k<curveSize; k++, iter3++){
					cout<<(*iter3)<<"-->";
					cout<<normalToBound[i][j][k]<<endl;
				}
			}
			else if(d==NORMALIN){
				cout<<"-----------NORMAIS PARA DENTRO---------"<<endl;
				cout<<"Ponto-->NormalIn"<<endl;
				unsigned int curveSize=iter2->listPoints.size();
				for(unsigned int k=0; k<curveSize; k++, iter3++){
					cout<<(*iter3)<<"-->";
					Point3D p;
					p.setPoint(-normalToBound[i][j][k].x,-normalToBound[i][j][k].y,-normalToBound[i][j][k].z);
					cout<<p<<endl;
				}
			}
			else{
				cout<<"---NORMAIS PARA DENTRO e PARA FORA---------"<<endl;
				cout<<"Ponto-->NormalOut-->NormalIn"<<endl;
				unsigned int curveSize=iter2->listPoints.size();
				for(unsigned int k=0; k<curveSize; k++, iter3++){
					cout<<(*iter3)<<"-->";
					cout<<normalToBound[i][j][k]<<"-->";
					Point3D p;
  					p.setPoint(-normalToBound[i][j][k].x,-normalToBound[i][j][k].y,-normalToBound[i][j][k].z);
  					cout<<p<<endl;
				}
			}
			cout<<"regiao: "<<i<<endl;
			cout<<"curva: "<<j<<endl;
			cout<<"flag: "<<(*iter2).flagFechado<<endl;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
#if 0
void Normal::NormalVector(DIRECTION d){
	glBegin(GL_LINES);
	glColor3f(1.0,0.0,0.0);
	glPointSize(1.0);
	int i=0;
	for (list<Curves>::iterator iter = vec->regionCurves.begin(); iter!= vec->regionCurves.end();i++, iter++) {
//		if(i==3){
				for (list<Curve>::iterator iter2 = iter->begin(); iter2!=iter->end(); iter2++) {
					for(list<Point2D>::iterator iter3 = iter2->listPoints.begin(); iter3!=iter2->listPoints.end(); iter3++){
						if(d==NORMALOUT){
							Point3D p=normalBuffer[index(iter3->x, iter3->y, w)];
							glVertex3d((double)iter3->x, (double)iter3->y, 0.0);
							glVertex3d(iter3->x+(10*p.x),iter3->y+(10*p.y), 0.0);
						}
						else if(d==NORMALIN){
							Point3D p=normalBuffer[index(iter3->x, iter3->y, w)];
							glVertex3d((double)iter3->x, (double)iter3->y, 0.0);
							glVertex3d(iter3->x-(10*p.x),iter3->y-(10*p.y), 0.0);
						}
					}
				}
//		}

	}
	glEnd();
}
#endif////////////////////////////////////////////////////////////////////////////////

void Normal::drawNormalVector(DIRECTION d){
	glBegin(GL_LINES);
	glColor3f(1.0,0.0,0.0);
	glPointSize(1.0);
	unsigned int i,j,k;
	list<Curves>::iterator iter;
	list<Curve>::iterator iter2;
	list<Point2D>::iterator iter3;
	
	//A - Para cada região.
	for (iter = vec->regionCurves.begin(),i=0; iter!= vec->regionCurves.end();i++, iter++) {
//		if(i==3){
		        //A - Para cada curva pertencenta a região.
				for (iter2 = iter->begin(),j=0; iter2!=iter->end(); j++,iter2++) {
					//A - Percorrendo a lista de pontos da curva.
					for(iter3 = iter2->listPoints.begin(),k=0; iter3!=iter2->listPoints.end(); k++,iter3++){
						if(d==NORMALOUT){
							//Point3D p=normalBuffer[index(iter3->x, iter3->y, w)];
							Point3D p=normalToBound[i][j][k];
							glVertex3d((double)iter3->x, (double)iter3->y, 0.0);
							glVertex3d(iter3->x+(10*p.x),iter3->y+(10*p.y), 0.0);
						}
						else if(d==NORMALIN){
							//Point3D p=normalBuffer[index(iter3->x, iter3->y, w)];
							Point3D p=normalToBound[i][j][k];
							glVertex3d((double)iter3->x, (double)iter3->y, 0.0);
							glVertex3d(iter3->x-(10*p.x),iter3->y-(10*p.y), 0.0);
						}
					}
				}
//		}

	}
	glEnd();
}
//
//void Normal::paintBorder(){
//	list<Curves>::iterator iterV = regionNormals->regionCurves.begin();
//	glClearColor(0.0f,0.0f,0.0f,0.0f);
//	glClear(GL_COLOR_BUFFER_BIT);
//	glBegin(GL_POINTS);
//	iterV++;
//	while(iterV != regionNormals->regionCurves.end()){
//		list<Curve>::iterator iterC = iterV->listCurve.begin();
//		while(iterC != iterV->listCurve.end()){
//			list<Point>::iterator iterP = iterC->listPoints.begin();
//			Point p;	 
//			while(iterP != iterC->listPoints.end()){
//				p = (*iterP);
//				p.nOutX = (p.nOutX + 1)/2; 
//				p.nOutY = (p.nOutY + 1)/2; 
//				glColor3f(p.nOutX,p.nOutY,p.nOutZ);
//				glVertex2i((*iterP).x,(*iterP).y);
//				iterP++;
//			}iterC++;
//		}iterV++;
//	}
//	glEnd();
//}
//
void Normal :: drawInteriorNormalMap(){
	list<Curves>::iterator iterV = regions->listBoundary->regionCurves.begin();
	iterV++;
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POINTS);

	while(iterV != regions->listBoundary->regionCurves.end()){
		list<Curve>::iterator iterC = iterV->begin();
		while(iterC != iterV->end()){
			list<Point2D>::iterator iterP = iterC->listPoints.begin();
			Point3D p;	 
			while(iterP != iterC->listPoints.end()){
				int ind = regions->index(iterP->x, iterP->y);
				p.x = (normalBuffer[ind].x + 1)/2;
				p.y = (normalBuffer[ind].y + 1)/2;
				p.z = 0.5;
				
				glColor3f(p.x,p.y,p.z);
				glVertex2i((*iterP).x,(*iterP).y);
				iterP++;
			}iterC++;
		}iterV++;
	}
	list<Interior*>::iterator iterR = regions->listInterior.begin();
	iterR++;
//	cout << "B - " << normalBuffer.size() << endl;
	while(iterR != regions->listInterior.end()){
		list<Point2D>::iterator iterP = (*iterR)->begin();
		Point3D p;	
		while(iterP != (*iterR)->end()){
			int ind = regions->index(iterP->x, iterP->y);
			p.x = (normalBuffer[ind].x + 1)/2;
			p.y = (normalBuffer[ind].y + 1)/2;
			p.z = (normalBuffer[ind].z + 1)/2;
			glColor3f(p.x,p.y,p.z);
			glVertex2i((*iterP).x,(*iterP).y);
			iterP++;
		}iterR++;
	}
	glEnd();
}
//
//void Normal :: paint(){
//	list<Curves>::iterator iterV = regionNormals->regionCurves.begin();
//	iterV++;
//	//glClearColor(1.0f,1.0f,1.0f,1.0f);
//	glClearColor(0.0f,0.0f,0.0f,0.0f);
//	glClear(GL_COLOR_BUFFER_BIT);
//	//glPointSize(2.6);
//	glBegin(GL_POINTS);
//	while(iterV != regionNormals->regionCurves.end()){
//		list<Curve>::iterator iterC = iterV->listCurve.begin();
//		while(iterC != iterV->listCurve.end()){
//			list<Point>::iterator iterP = iterC->listPoints.begin();
//			Point p;	 
//			while(iterP != iterC->listPoints.end()){
//				p = (*iterP);
//				p.nOutX = (p.nOutX + 1)/2; 
//				p.nOutY = (p.nOutY + 1)/2; 
//				//cout << p.x << " " << p.nOutX << p.nOutY << p.nOutZ << endl;
//				glColor3f(p.nOutX,p.nOutY,p.nOutZ);
//				glVertex2i((*iterP).x,(*iterP).y);
//				iterP++;
//			}iterC++;
//		}iterV++;
//	}
//	list<Region>::iterator iterR = regions->listRegion.begin();
//	iterR++;
//	while(iterR != regions->listRegion.end()){
//		list<Point>::iterator iterP = iterR->listPoint.begin();
//		Point p;	
//		while(iterP != iterR->listPoint.end()){
//			p = (*iterP);
//			//cout << p.x << " " << p.nOutX << p.nOutY << p.nOutZ << endl;
//			p.nOutX = (p.nOutX + 1)/2; 
//			p.nOutY = (p.nOutY + 1)/2; 
//			p.nOutZ = (p.nOutZ + 1)/2; 
//			//cout << p.x << " " << p.nOutX << p.nOutY << p.nOutZ << endl;
//			glColor3f(p.nOutX,p.nOutY,p.nOutZ);
//			glVertex2i((*iterP).x,(*iterP).y);
//			iterP++;
//		}iterR++;
//	}
//	glEnd();
//}
//
//void Normal::xScale(Regions *r, Vectorization *_regionNormals, double scale){
//	regionNormals=_regionNormals;
//	regions = r;
//	
//	list<Curves>::iterator listCurves = regionNormals->regionCurves.begin();
//	listCurves++;
//	list<Curve>::iterator listCurve = listCurves->listCurve.begin();	
//	
//	list<Region>::iterator iterListRegion= regions->listRegion.begin();
//	iterListRegion++;
//	
//	while(iterListRegion != regions->listRegion.end()){
//		for(list<Point>::iterator  iterPointRegion = iterListRegion->listPoint.begin();iterPointRegion!=iterListRegion->listPoint.end();iterPointRegion++){	        
//	        iterPointRegion->setNormalOut(scale*iterPointRegion->nOutX, iterPointRegion->nOutY, iterPointRegion->nOutZ);
//		}
//		listCurves++;
//		iterListRegion++;
//	}
//	
//	list<Curves> :: iterator iter = this->regionNormals->regionCurves.begin();
//	list<Curve>::iterator iter2;
//	list<Point>::iterator iter3;
//	unsigned int regionsSize=this->regionNormals->regionCurves.size();
//	for(unsigned int i=0; i<regionsSize; i++, iter++){
//		iter2=iter->listCurve.begin();
//		unsigned int curvesSize=iter->listCurve.size();
//		for(unsigned int j=0; j<curvesSize; j++, iter2++){
//			iter3=iter2->listPoints.begin();
//			unsigned int curveSize=iter2->listPoints.size();
//			for(unsigned int k=0; k<curveSize; k++, iter3++){
//				iter3->setNormalOut(scale*iter3->nOutX, iter3->nOutY, iter3->nOutZ);
//			}
//		}
//	}
//}
//
//void Normal::yScale(Regions *r, Vectorization *_regionNormals, double scale){
//	regionNormals=_regionNormals;
//	regions = r;
//	
//	list<Curves>::iterator listCurves = regionNormals->regionCurves.begin();
//	listCurves++;
//	list<Curve>::iterator listCurve = listCurves->listCurve.begin();	
//	
//	list<Region>::iterator iterListRegion= regions->listRegion.begin();
//	iterListRegion++;
//	
//	while(iterListRegion != regions->listRegion.end()){
//		for(list<Point>::iterator  iterPointRegion = iterListRegion->listPoint.begin();iterPointRegion!=iterListRegion->listPoint.end();iterPointRegion++){	        
//	        iterPointRegion->setNormalOut(iterPointRegion->nOutX, scale*iterPointRegion->nOutY, iterPointRegion->nOutZ);
//		}
//		listCurves++;
//		iterListRegion++;
//	}
//	
//	list<Curves> :: iterator iter = this->regionNormals->regionCurves.begin();
//	list<Curve>::iterator iter2;
//	list<Point>::iterator iter3;
//	unsigned int regionsSize=this->regionNormals->regionCurves.size();
//	for(unsigned int i=0; i<regionsSize; i++, iter++){
//		iter2=iter->listCurve.begin();
//		unsigned int curvesSize=iter->listCurve.size();
//		for(unsigned int j=0; j<curvesSize; j++, iter2++){
//			iter3=iter2->listPoints.begin();
//			unsigned int curveSize=iter2->listPoints.size();
//			for(unsigned int k=0; k<curveSize; k++, iter3++){
//				iter3->setNormalOut(iter3->nOutX, scale*iter3->nOutY, iter3->nOutZ);
//			}
//		}
//	}
//}
//
//void Normal::zScale(Regions *r, Vectorization *_regionNormals, double scale){
//	regionNormals=_regionNormals;
//	regions = r;
//	
//	list<Curves>::iterator listCurves = regionNormals->regionCurves.begin();
//	listCurves++;
//	list<Curve>::iterator listCurve = listCurves->listCurve.begin();	
//	
//	list<Region>::iterator iterListRegion= regions->listRegion.begin();
//	iterListRegion++;
//	
//	while(iterListRegion != regions->listRegion.end()){
//		for(list<Point>::iterator  iterPointRegion = iterListRegion->listPoint.begin();iterPointRegion!=iterListRegion->listPoint.end();iterPointRegion++){	        
//	        iterPointRegion->setNormalOut(iterPointRegion->nOutX, iterPointRegion->nOutY, scale*iterPointRegion->nOutZ);
//		}
//		listCurves++;
//		iterListRegion++;
//	}
//	
//	list<Curves> :: iterator iter = this->regionNormals->regionCurves.begin();
//	list<Curve>::iterator iter2;
//	list<Point>::iterator iter3;
//	unsigned int regionsSize=this->regionNormals->regionCurves.size();
//	for(unsigned int i=0; i<regionsSize; i++, iter++){
//		iter2=iter->listCurve.begin();
//		unsigned int curvesSize=iter->listCurve.size();
//		for(unsigned int j=0; j<curvesSize; j++, iter2++){
//			iter3=iter2->listPoints.begin();
//			unsigned int curveSize=iter2->listPoints.size();
//			for(unsigned int k=0; k<curveSize; k++, iter3++){
//				iter3->setNormalOut(iter3->nOutX, iter3->nOutY, scale*iter3->nOutZ);
//			}
//		}
//	}
//}
//
//void Normal::scale(Regions *r, Vectorization *_regionNormals, double scale){
//	this->regionNormals=_regionNormals;
//	this->regions = r;
//	
//	list<Curves>::iterator listCurves = regionNormals->regionCurves.begin();
//	listCurves++;
//	list<Curve>::iterator listCurve = listCurves->listCurve.begin();	
//	
//	list<Region>::iterator iterListRegion= regions->listRegion.begin();
//	iterListRegion++;
//	
//	while(iterListRegion != regions->listRegion.end()){
//		for(list<Point>::iterator  iterPointRegion = iterListRegion->listPoint.begin();iterPointRegion!=iterListRegion->listPoint.end();iterPointRegion++){	        
//	        iterPointRegion->setNormalOut(scale*iterPointRegion->nOutX, scale*iterPointRegion->nOutY, scale*iterPointRegion->nOutZ);
//		}
//		listCurves++;
//		iterListRegion++;
//	}
//	
//	list<Curves> :: iterator iter = this->regionNormals->regionCurves.begin();
//	list<Curve>::iterator iter2;
//	list<Point>::iterator iter3;
//	unsigned int regionsSize=this->regionNormals->regionCurves.size();
//	for(unsigned int i=0; i<regionsSize; i++, iter++){
//		iter2=iter->listCurve.begin();
//		unsigned int curvesSize=iter->listCurve.size();
//		for(unsigned int j=0; j<curvesSize; j++, iter2++){
//			iter3=iter2->listPoints.begin();
//			unsigned int curveSize=iter2->listPoints.size();
//			for(unsigned int k=0; k<curveSize; k++, iter3++){
//				iter3->setNormalOut(scale*iter3->nOutX, scale*iter3->nOutY, scale*iter3->nOutZ);
////				cout<<"("<<iter3->nOutX<<","<<iter3->nOutY<<","<<iter3->nOutZ<<")"<<endl;
//			}
//		}
//	}
//}
//
//void Normal::buildNormalMap(int width, int height){
//	this->normalMap.resize(width*height);
//	Point p;
//	for(int i=0; i<width*height; i++){
//		normalMap[i]=p;
//	}
//	list<Curves>::iterator iterV = regionNormals->regionCurves.begin();
//	while(iterV != regionNormals->regionCurves.end()){
//		list<Curve>::iterator iterC = iterV->listCurve.begin();
//		while(iterC != iterV->listCurve.end()){
//			list<Point>::iterator iterP = iterC->listPoints.begin();	 
//			while(iterP != iterC->listPoints.end()){
//				normalMap[index(iterP->x, iterP->y, width)]=(*iterP);
//				iterP++;
//			}iterC++;
//		}iterV++;
//	}
//
//	list<Region>::iterator iterR = regions->listRegion.begin();
//	while(iterR != regions->listRegion.end()){
//		list<Point>::iterator iterP = iterR->listPoint.begin();
//		while(iterP != iterR->listPoint.end()){
//			normalMap[index(iterP->x, iterP->y, width)]=(*iterP);
//			iterP++;
//		}iterR++;
//	}
//	
////    ofstream outfile;
////    outfile.open ("normalMap.txt");
////	cout<<"Tamanho: "<<normalMap.size()<<endl;
////	outfile<<"Tamanho: "<<normalMap.size()<<endl;
////	for(unsigned int i=0; i<normalMap.size(); i++){
////		cout<<"Em buildNormalMap: "<<normalMap[i]<<"--> "<<normalMap[i].nOutX<<","<<normalMap[i].nOutY<<","<<normalMap[i].nOutZ<<endl;
////		outfile<<"Em buildNormalMap: "<<normalMap[i]<<"--> "<<normalMap[i].nOutX<<","<<normalMap[i].nOutY<<","<<normalMap[i].nOutZ<<endl;
////	}
////    outfile.close(); 
//}
//
//void Normal::upDownCurve(list<Point> *curvePoints, list<Point> *regionPoints, unsigned int dist){
//	int iMin, iMax, jMin, jMax, m, n;
//	iMin=iMax=jMin=jMax=m=n=0;
//	this->getLimitValues(&iMin, &iMax, &jMin, &jMax, curvePoints);
////	cout<<"Limites "<<iMin<<","<<iMax<<","<<jMin<<","<<jMax<<endl;
//	m=(iMax-iMin+1)+2*dist;
//	n=(jMax-jMin+1)+2*dist;
////	cout<<"m="<<m<<endl;
////	cout<<"n="<<n<<endl;
//	vector<int> distanceBuffer;
//	for(int i=0; i<m*n; i++){
//		distanceBuffer.push_back(-1);
//	}
////	cout<<"tamanho do vetor: "<<distanceBuffer.size()<<", m.n: "<<m*n<<endl;
//	list<Point>::iterator iterCurve = curvePoints->begin();
//	for(; iterCurve!=curvePoints->end(); iterCurve++){
//		int iB=iterCurve->x - iMin + dist;
//		int jB=iterCurve->y - jMin + dist;
//		distanceBuffer[index(iB,jB,n)]=0;
//	}
//	vector<Curve> distanceList;
//	Curve c;
//	c.listPoints=(*curvePoints);
//	distanceList.push_back(c);
//	c.listPoints.clear();
////	cout<<"tamanho: "<<c.listPoints.size();
//	for(unsigned int i=0; i<dist; i++){
//		distanceList.push_back(c);
//	}
//	for(unsigned int i=0; i<=dist-1; i++){
//		for(list<Point>::iterator iterP=distanceList[i].listPoints.begin(); iterP!=distanceList[i].listPoints.end(); iterP++){
//			Point p;
//			int iB=iterP->x - iMin + dist;
//			int jB=iterP->y - jMin + dist;
//			if(iB>=0 && iB<m && jB+1>=0 && jB+1<n){
//				if(distanceBuffer[index(iB,jB+1,n)]==-1){
//					distanceBuffer[index(iB,jB+1,n)]=distanceBuffer[index(iB,jB,n)]+1;
//					p.setPoint(iterP->x,jB=iterP->y+1);
//					distanceList[i+1].listPoints.push_back(p);
//				}
//				else{
//					if(distanceBuffer[index(iB,jB+1,n)]+1<distanceBuffer[index(iB,jB+1,n)]){
//						distanceBuffer[index(iB,jB+1,n)]=distanceBuffer[index(iB,jB,n)]+1;
//						if(distanceBuffer[index(iB,jB+1,n)]<=dist){//Remove from list[i], add to list[i+1]
////							remover da lista
////							inserir na outra lista
//						}
//					}
//				}
//			}
//
//			
//		}
//	}
//}
//
//
//
//void Normal::getLimitValues(int *iMin, int *iMax, int *jMin, int *jMax, list<Point> *curvePoints){
//	list<Point>::iterator iterCurve = curvePoints->begin();
//	(*iMin)=(*iMax)=iterCurve->x;
//	(*jMin)=(*jMax)=iterCurve->y;
////	cout<<"iterCurve "<<iterCurve->x<<","<<iterCurve->y<<endl;
//	iterCurve++;
//	for(; iterCurve!=curvePoints->end(); iterCurve++){
////		cout<<"iterCurve "<<iterCurve->x<<","<<iterCurve->y<<endl;
//		if(iterCurve->x<(*iMin)) {
//			(*iMin)=iterCurve->x;
//		}
//		if(iterCurve->x>(*iMax)){
//			(*iMax)=iterCurve->x;
//		}
//		if(iterCurve->y<(*jMin)){
//			(*jMin)=iterCurve->y;
//		}
//		if(iterCurve->y>(*jMax)){
//			(*jMax)=iterCurve->y;
//		}
//	}
//}
//
void Normal::smoothAllCurves(){
	list<Curves> :: iterator iter = this->vec->regionCurves.begin();
	list<Interior*>::iterator iterR = this->regions->listInterior.begin();
	unsigned int regionsSize=this->vec->regionCurves.size();
	iter++; //N�o trata o background
	iterR++; //N�o trata o background
//	cout<<"antes do primeiro for"<<endl;
	for(unsigned int i=1; i<regionsSize; i++, iter++, iterR++){
		list<Curve>::iterator iter2=iter->begin();
		iter2++;//Trata apenas as curvas abertas
//		cout<<"antes do segundo for"<<endl;
		unsigned int curvesSize=iter->size();
		for(unsigned int j=1 ; j<curvesSize; j++, iter2++){
			this->smoothCurve(&(iter2->listPoints), i, j, 10);
		}
	}
}
//
////void Normal::saveImage(char *nameImage,char *typeImage, int width, int height){
////	glPixelStorei(GL_PACK_ALIGNMENT, 1); /* data alignment must be 1 */
////	void *gl_data;
////	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)gl_data);
////	int error;
////	imFile *ifile = imFileNew(nameImage, typeImage, &error);
////	error = imFileWriteImageInfo(ifile, width, height, IM_RGB|IM_PACKED, IM_BYTE);
////	error = imFileWriteImageData(ifile, gl_data);
////	imFileClose(ifile); 
////}
//
//int Normal::index(int i, int j, int width){
//	return (i+j*width);
//}
//
///*** Mask 
// *   clockwise
// *    8 |  1   | 2
// *    7 | P(0) | 3
// *    6 |  5   | 4
// * 
// ***/ 
//
///*** Mask
// *   clockwise
// *    c | b | c
// *    b | A | b
// *    c | b | c
// * 
// ***/ 
//
//void Normal::smoothInterior(int width, int height, double a, double b, double c){
//	double sum=a+4*b+4*c;
//	buildNormalMap(width, height);
//	for(list<Region>::iterator iterR = regions->listRegion.begin(); iterR!=regions->listRegion.end(); iterR++){
//	    ofstream outfile;
//	    outfile.open ("smoothInterior.txt");
//		for(list<Point>::iterator iterP = iterR->listPoint.begin(); iterP!=iterR->listPoint.end(); iterP++){
//			double nX, nY, nZ;
//			nX=(a*normalMap[index(iterP->x, iterP->y, width)].nOutX + b*normalMap[index(iterP->x, iterP->y+1, width)].nOutX + c*normalMap[index(iterP->x+1, iterP->y+1, width)].nOutX + b*normalMap[index(iterP->x+1, iterP->y, width)].nOutX + c*normalMap[index(iterP->x+1, iterP->y-1, width)].nOutX + b*normalMap[index(iterP->x, iterP->y-1, width)].nOutX + c*normalMap[index(iterP->x-1, iterP->y-1, width)].nOutX + b*normalMap[index(iterP->x-1, iterP->y, width)].nOutX + c*normalMap[index(iterP->x-1, iterP->y+1, width)].nOutX)/sum;
//			nY=(a*normalMap[index(iterP->x, iterP->y, width)].nOutY + b*normalMap[index(iterP->x, iterP->y+1, width)].nOutY + c*normalMap[index(iterP->x+1, iterP->y+1, width)].nOutY + b*normalMap[index(iterP->x+1, iterP->y, width)].nOutY + c*normalMap[index(iterP->x+1, iterP->y-1, width)].nOutY + b*normalMap[index(iterP->x, iterP->y-1, width)].nOutY + c*normalMap[index(iterP->x-1, iterP->y-1, width)].nOutY + b*normalMap[index(iterP->x-1, iterP->y, width)].nOutY + c*normalMap[index(iterP->x-1, iterP->y+1, width)].nOutY)/sum;
//			nZ=(a*normalMap[index(iterP->x, iterP->y, width)].nOutZ + b*normalMap[index(iterP->x, iterP->y+1, width)].nOutZ + c*normalMap[index(iterP->x+1, iterP->y+1, width)].nOutZ + b*normalMap[index(iterP->x+1, iterP->y, width)].nOutZ + c*normalMap[index(iterP->x+1, iterP->y-1, width)].nOutZ + b*normalMap[index(iterP->x, iterP->y-1, width)].nOutZ + c*normalMap[index(iterP->x-1, iterP->y-1, width)].nOutZ + b*normalMap[index(iterP->x-1, iterP->y, width)].nOutZ + c*normalMap[index(iterP->x-1, iterP->y+1, width)].nOutZ)/sum;
//			outfile<<"("<<(*iterP)<<")->("<<iterP->nOutX<<","<<iterP->nOutY<<","<<iterP->nOutZ<<") ->";
//			outfile<<"norma: "<<sqrt((iterP->nOutX*iterP->nOutX)+(iterP->nOutY*iterP->nOutY)+(iterP->nOutZ*iterP->nOutZ))<<endl;
//			iterP->setNormalOut(nX,nY,nZ);
//			(*iterP)=normalize((*iterP));
////			cout<<"norma: "<<sqrt((iterP->nOutX*iterP->nOutX)+(iterP->nOutY*iterP->nOutY)+(iterP->nOutZ*iterP->nOutZ))<<endl;
//			outfile<<"("<<(*iterP)<<")->("<<iterP->nOutX<<","<<iterP->nOutY<<","<<iterP->nOutZ<<") ->";
//			outfile<<"norma: "<<sqrt((iterP->nOutX*iterP->nOutX)+(iterP->nOutY*iterP->nOutY)+(iterP->nOutZ*iterP->nOutZ))<<endl;
//			outfile<<endl;
//		}
//	    outfile.close(); 
//	}
//}

/*** Mask 
 *   clockwise
 *    8 |  1   | 2
 *    7 | P(0) | 3
 *    6 |  5   | 4
 * 
 ***/ 

/*** Mask
 *   clockwise
 *    c | b | c
 *    b | A | b
 *    c | b | c
 * 
 ***/ 

void Normal::smoothInterior(double a, double b, double c){
	double sum=a+4*b+4*c;
	double nX, nY, nZ;
	vector<Point3D> auxBuffer;
	Point3D p;
	int lenght=w*h;
	auxBuffer.resize(lenght,p);
	for(int i=0; i<lenght; i++){
		auxBuffer[i]=normalBuffer[i];
       // cout<<auxBuffer[i]<<"->"<<normalBuffer[i]<<endl;
	}
	list<Interior*>::iterator iterListRegion= regions->listInterior.begin();
	//pulando o background ja que o mesmo nao foi calculado
	iterListRegion++;
	
	for(; iterListRegion!=regions->listInterior.end(); iterListRegion++){
		list<Point2D>::iterator  iterPointRegion = (*iterListRegion)->begin();
		for(; iterPointRegion!=(*iterListRegion)->end(); iterPointRegion++){
			int i=index(iterPointRegion->x, iterPointRegion->y, w);
			nX=(a*auxBuffer[i].x + b*auxBuffer[i+w].x + c*auxBuffer[i+1+w].x + b*auxBuffer[i+1].x + c*auxBuffer[i+1-w].x + b*auxBuffer[i-w].x + c*auxBuffer[i-1-w].x + b*auxBuffer[i-1].x + c*auxBuffer[i-1+w].x)/sum;
			nY=(a*auxBuffer[i].y + b*auxBuffer[i+w].y + c*auxBuffer[i+1+w].y + b*auxBuffer[i+1].y + c*auxBuffer[i+1-w].y + b*auxBuffer[i-w].y + c*auxBuffer[i-1-w].y + b*auxBuffer[i-1].y + c*auxBuffer[i-1+w].y)/sum;
			nZ=(a*auxBuffer[i].z + b*auxBuffer[i+w].z + c*auxBuffer[i+1+w].z + b*auxBuffer[i+1].z + c*auxBuffer[i+1-w].z + b*auxBuffer[i-w].z + c*auxBuffer[i-1-w].z + b*auxBuffer[i-1].z + c*auxBuffer[i-1+w].z)/sum;
			p.setPoint(nX,nY,nZ);
			p.normalize();
			
			//cout << "Point3D : " << p <<endl;
			normalBuffer[index(iterPointRegion->x, iterPointRegion->y, w)]=p;
			//cout << normalBuffer[index(iterPointRegion->x, iterPointRegion->y, w)] <<endl;
		}
	}
	auxBuffer.clear();
}

void Normal::updateDistanceUsingHier(Point2D p,Vectorization *v,double &sumDi,vector<double> *di,int label){	
	list<Curves>::iterator iterCurvesAux = regions->listBoundary->regionCurves.begin();
	
	for(int i=0;i<label;i++){
		iterCurvesAux++;
	}
	list<Curve>::iterator iterCurveAux = iterCurvesAux->begin();	
	updateDistance(p,iterCurveAux,sumDi,di);
}

//----------------------------------------------------------------------------------------------------------
/* A
 * Calcula as distâncias do ponto interior aos pontos da curva.
 */
void Normal::calculateDistance(Point2D p,list<Curves>::iterator iterCurves,Vectorization *v,double &sumDi,vector<double> *di,list<Label>::iterator iterLabel){ 
	//A - para calcular normal 3D com a curva de bordo e os rabiscos/curvas internas da regiao
	for(list<Curve>::iterator iterCurve=iterCurves->begin(); iterCurve!=iterCurves->end();iterCurve++)
	{
//		list<Curve>::iterator iterCurve = iterCurves->begin();
		updateDistance(p,iterCurve,sumDi,di);
	}

	//para calcular usando a estruta de hierarquia dos filhos descendentes de primeiro grau
	for(list<int>::iterator iterInteger = iterLabel->begin();iterInteger!=iterLabel->end();iterInteger++)
	{
		if((*iterInteger)!=-1){
//			updateDistanceUsingHier(p,v,sumDi,di,(*iterInteger));
			list<Curves>::iterator iterCurvesAux = regions->listBoundary->regionCurves.begin();
			for(int i=0;i<(*iterInteger);i++){
				iterCurvesAux++;
			}
			list<Curve>::iterator iterCurveAux = iterCurvesAux->begin();	
			//updateDistance(p,iterCurveAux,sumDi,di);
			double distance=0;
			for(list<Point2D>::iterator iterPoint=iterCurveAux->listPoints.begin(); iterPoint!=iterCurveAux->listPoints.end();iterPoint++){
				distance  = (double)1.0/((p.x - iterPoint->x)*(p.x - iterPoint->x) + (p.y - iterPoint->y)*(p.y - iterPoint->y));			
				sumDi = sumDi + distance;
				di->push_back(distance);
			}
			
		}
	}
}

void Normal::updateNpxNpy(Point2D p,double *Npx,double *Npy,list<Curve>::iterator iterCurve,vector<double>::iterator iterWi){
	for(list<Point2D>::iterator iterPoint= iterCurve->listPoints.begin(); iterPoint!=iterCurve->listPoints.end();iterPoint++){
        	*Npx = *Npx - normalBuffer[index(iterPoint->x,iterPoint->y,w)].x *(*iterWi);
        	*Npy = *Npy - normalBuffer[index(iterPoint->x,iterPoint->y,w)].y *(*iterWi);
	        iterWi++;	    	        
	}
}

void Normal::updateNpxNpyUsingHier(Point2D p,double *Npx,double *Npy,Vectorization *v,vector<double>::iterator iterWi,int label){	
	list<Curves>::iterator iterCurves=v->regionCurves.begin();
	for(int i=0;i<label;i++){
		iterCurves++;
	}
	list<Curve>::iterator iterCurveAux=iterCurves->begin();;
	updateNpxNpy(p,Npx,Npy,iterCurveAux,iterWi);
	 
}

void Normal::calculateNpxNpy(Point2D p,double *Npx,double *Npy,list<Curves>::iterator iterCurves,Vectorization *vet,vector<double> *wi,list<Label>::iterator iterLabel,unsigned int i){
	bool flag = 0;
	vector<double>::iterator iterWi = wi->begin();
	unsigned int j = 0;
	unsigned int k = 0;

	//Para cada curva do conjunto de curvas de cada região
	for(list<Curve>::iterator iterCurve=iterCurves->begin(); iterCurve!=iterCurves->end();iterCurve++)
	{
		if(flag==0){//Analisando a curva de borda, curva fechada
			//Para cada ponto da curva
			for(list<Point2D>::iterator iterPoint= iterCurve->listPoints.begin(); iterPoint!=iterCurve->listPoints.end();iterPoint++)
			{
				*Npx = *Npx + normalToBound[i][j][k] .x*(*iterWi);
				*Npy = *Npy + normalToBound[i][j][k] .y*(*iterWi);
				k++;
				iterWi++;
			}
			flag = 1;
		}
		else{ //Analisando as curvas internas, curvas abertas
			k=0;
			for(list<Point2D>::iterator iterPoint= iterCurve->listPoints.begin(); iterPoint!=iterCurve->listPoints.end();iterPoint++)
			{
				if(((p.x-(*iterPoint).x)* normalToBound[i][j][k].x  + (p.y-(*iterPoint).y)*normalToBound[i][j][k].y)>0)
				{
					*Npx = *Npx - normalToBound[i][j][k] .x*(*iterWi);
					*Npy = *Npy - normalToBound[i][j][k] .y*(*iterWi);
					iterWi++;
				}
				else
				{
					*Npx = *Npx + normalToBound[i][j][k] .x*(*iterWi);
					*Npy = *Npy + normalToBound[i][j][k] .y*(*iterWi);
					iterWi++;					
				}
				k++;
			}
		}
		j++;		
	}
	
	//calculo da interpolacao com os filhos
	unsigned int a,b,c ;
	a = b = c = 0;
	for(list<int>::iterator iterInteger = iterLabel->begin();iterInteger!=iterLabel->end();iterInteger++)
	{
		if((*iterInteger)!=-1){
			a=0;
			list<Curves>::iterator iterCurves=vet->regionCurves.begin();
			for(int i=0;i<(*iterInteger);i++){
				iterCurves++;
				a++;
			}
			list<Curve>::iterator iterCurveAux=iterCurves->begin();
			//updateNpxNpy(p,Npx,Npy,iterCurveAux,iterWi);
			c=0;
			for(list<Point2D>::iterator iterPoint= iterCurveAux->listPoints.begin(); iterPoint!=iterCurveAux->listPoints.end();iterPoint++){
	               *Npx = *Npx - normalToBound[a][b][c] .x*(*iterWi);
	               *Npy = *Npy - normalToBound[a][b][c] .y*(*iterWi);
			        iterWi++;	    	        
			        c++;
			}
		}
	}
}

void Normal::calculateNormal(unsigned int i,list<Interior*>::iterator iterListRegion,list<Label>::iterator iterHierLabel,list<Curves>::iterator listCurves)
{
	/*Para cada ponto interior a região*/
	for(list<Point2D>::iterator  iterPointRegion = (*iterListRegion)->begin();iterPointRegion!=(*iterListRegion)->end();iterPointRegion++)
	{
		//poderia ser usado com as variaveis fora
		vector<double> di;
		vector<double> wi;
		
		
		double Npx,Npy,Npz;Npx=Npy=Npz = 0;
		double sumDi=0;	

		calculateDistance((*iterPointRegion),listCurves,regions->listBoundary,sumDi,&di,iterHierLabel);
		calculateWi(&di,sumDi,&wi);
		calculateNpxNpy((*iterPointRegion),&Npx,&Npy,listCurves,regions->listBoundary,&wi,iterHierLabel,i);
//		calculateNpxNpy_2((*iterPointRegion),&Npx,&Npy,listCurves,regions->listBoundary,&wi,iterHierLabel,i); //(by Ailton)
		Npz = sqrt(1 - Npx*Npx - Npy*Npy);
		normalBuffer[index(iterPointRegion->x,iterPointRegion->y,w)].x = Npx;
		normalBuffer[index(iterPointRegion->x,iterPointRegion->y,w)].y = Npy;
		normalBuffer[index(iterPointRegion->x,iterPointRegion->y,w)].z = Npz;
	}
}



void Normal::calculateAllInteriorNormal(){	
	list<Curves>::iterator listCurves = regions->listBoundary->regionCurves.begin();
	list<Interior*>::iterator iterListRegion= regions->listInterior.begin();
	list<Label>::iterator iterHierLabel = regions->hierarchy->descendentList.begin();
	unsigned int i=0;
	
	//isto esta sendo feito pois o background nao esta sendo processador no momento
	listCurves++;
	iterListRegion++;
	iterHierLabel++;	
	i++;

	//Para cada região
	while(iterListRegion != regions->listInterior.end()){				  
		calculateNormal(i,iterListRegion,iterHierLabel,listCurves);    
		listCurves++;
		iterListRegion++;
		iterHierLabel++;
		i++;
	}
}

void Normal::updateDistance(Point2D p,list<Curve>::iterator iterCurve,double &sumDi,vector<double> *di){
	double distance=0;
	for(list<Point2D>::iterator iterPoint=iterCurve->listPoints.begin(); iterPoint!=iterCurve->listPoints.end();iterPoint++){
		distance  = (double)1.0/((p.x - iterPoint->x)*(p.x - iterPoint->x) + (p.y - iterPoint->y)*(p.y - iterPoint->y));			
		sumDi = sumDi + distance;
		di->push_back(distance);
	}
}
void Normal::calculateWi(vector<double> *di,double sumDi,vector<double> *wi){
	vector<double>::iterator iterDi = di->begin();
	for(unsigned int i=0;i<di->size();i++){
		wi->push_back((*iterDi)/sumDi);
		iterDi++;
	}	
}
	
//    ofstream outfile;
//    outfile.open ("normalMap.txt");
//	cout<<"Tamanho: "<<normalMap.size()<<endl;
//	outfile<<"Tamanho: "<<normalMap.size()<<endl;
//	for(unsigned int i=0; i<normalMap.size(); i++){
//		cout<<"Em buildNormalMap: "<<normalMap[i]<<"--> "<<normalMap[i].nOutX<<","<<normalMap[i].nOutY<<","<<normalMap[i].nOutZ<<endl;
//		outfile<<"Em buildNormalMap: "<<normalMap[i]<<"--> "<<normalMap[i].nOutX<<","<<normalMap[i].nOutY<<","<<normalMap[i].nOutZ<<endl;
//	}
//    outfile.close(); 


void Normal::getLimitValues(int *iMin, int *iMax, int *jMin, int *jMax, list<Point2D> *curvePoints){
	list<Point2D>::iterator iterCurve = curvePoints->begin();
	(*iMin)=(*iMax)=iterCurve->x;
	(*jMin)=(*jMax)=iterCurve->y;
//	cout<<"iterCurve "<<iterCurve->x<<","<<iterCurve->y<<endl;
	iterCurve++;
	for(; iterCurve!=curvePoints->end(); iterCurve++){
//		cout<<"iterCurve "<<iterCurve->x<<","<<iterCurve->y<<endl;
		if(iterCurve->x<(*iMin)) {
			(*iMin)=iterCurve->x;
		}
		if(iterCurve->x>(*iMax)){
			(*iMax)=iterCurve->x;
		}
		if(iterCurve->y<(*jMin)){
			(*jMin)=iterCurve->y;
		}
		if(iterCurve->y>(*jMax)){
			(*jMax)=iterCurve->y;
		}
	}
}

void Normal::smoothCurve(list<Point2D> *curvePoints,int regionNum, int curveNum, int dist){
	int iMin, iMax, jMin, jMax, m, n;
	iMin=iMax=jMin=jMax=m=n=0;
	this->getLimitValues(&iMin, &iMax, &jMin, &jMax, curvePoints);
//	cout<<"Limites "<<iMin<<","<<iMax<<","<<jMin<<","<<jMax<<endl;
	m=(iMax-iMin+1)+2*dist;
	n=(jMax-jMin+1)+2*dist;
//	cout<<"m="<<m<<endl;
//	cout<<"n="<<n<<endl;
	vector<int> distanceBuffer;
	distanceBuffer.resize(m*n, -1);
//	cout<<"tamanho do vetor: "<<distanceBuffer.size()<<", m.n: "<<m*n<<endl;
	list<Point2D>::iterator iterCurve = curvePoints->begin();
	for(; iterCurve!=curvePoints->end(); iterCurve++){
//		cout<<"Pontos na curva: "<<(*iterCurve)<<endl;
		int iB=iterCurve->x - iMin + dist;
		int jB=iterCurve->y - jMin + dist;
		distanceBuffer[index(iB,jB,m)]=0;
	}
	
	vector<Boundary> distanceList;
	Boundary c;
	c=(*curvePoints);
	distanceList.push_back(c);
	c.clear();
//	cout<<"tamanho: "<<c.listPoints.size();
	for(int i=0; i<dist; i++){ //alocando as listas de distancia i
		distanceList.push_back(c);
	}
	for(int i=1; i<=dist-1; i++){ //inserindo os pontos nas listas de distancia
		for(list<Point2D>::iterator iterP=distanceList[i].begin(); iterP!=distanceList[i].end(); iterP++){
			Point2D p;
			int iB=iterP->x - iMin + dist;
			int jB=iterP->y - jMin + dist;
			
			//i,j+1 ->1
//			cout<<"Pontos na distance list: "<<(*iterP)<<endl;
			updateDistanceBuffer(iB,jB,iB,jB+1,m,n,iMin,jMin,&distanceBuffer,&distanceList, 1, dist);
//			//i+1,j+1 ->2
			updateDistanceBuffer(iB,jB,iB+1,jB+1,m,n,iMin,jMin,&distanceBuffer,&distanceList, 2, dist);
//			//i+1,j ->1
			updateDistanceBuffer(iB,jB,iB+1,jB,m,n,iMin,jMin,&distanceBuffer,&distanceList, 1, dist);
//			//i+1,j-1 ->2
			updateDistanceBuffer(iB,jB,iB+1,jB-1,m,n,iMin,jMin,&distanceBuffer,&distanceList, 2, dist);
//			//i,j-1 ->1
			updateDistanceBuffer(iB,jB,iB,jB-1,m,n,iMin,jMin,&distanceBuffer,&distanceList, 1, dist);
//			//i-1,j-1 ->2
			updateDistanceBuffer(iB,jB,iB-1,jB-1,m,n,iMin,jMin,&distanceBuffer,&distanceList, 2, dist);
//			//i-1,j ->1
			updateDistanceBuffer(iB,jB,iB-1,jB,m,n,iMin,jMin,&distanceBuffer,&distanceList, 1, dist);
//			//i-1,j+1 ->2
			updateDistanceBuffer(iB,jB,iB-1,jB+1,m,n,iMin,jMin,&distanceBuffer,&distanceList, 2, dist);
	
		} //aqui termina o for da lista de pontos
		
	} //aqui termina o for das distancias
	Point3D k(0.0,0.0,1.0);
	Point3D p3;
	Point3D a;
	unsigned int boundarySize=normalToBound[regionNum][curveNum].size();
	for(unsigned int i=0; i<boundarySize; i++){
		normalToBound[regionNum][curveNum][i].setPoint(0,0,1);
	}
	
	for(iterCurve = curvePoints->begin(); iterCurve!=curvePoints->end(); iterCurve++){
		normalBuffer[index(iterCurve->x, iterCurve->y, w)].setPoint(0,0,1);
	}
	
	for(int i=1; i<=dist-1; i++){ //atualizando as normais com distancia i no buffer
		cout<<"DIST: "<<dist<<endl;
		cout<<"tamanho"<<distanceList[i].size()<<endl;
		for(list<Point2D>::iterator iterP=distanceList[i].begin(); iterP!=distanceList[i].end(); iterP++){
			double x=i/dist;
			double fiX=x*x;
			double f1=1-fiX;
			int ind=index(iterP->x, iterP->y, w);
			Point3D f2=k-normalBuffer[ind];
			Point3D result=(f1*f2)+normalBuffer[ind];
			normalBuffer[ind]=result;
			cout<<"Teste: i:"<<i<<endl;
//			cout<<"w;ha�fv�fbv�fbv�sjfbvfnlkdnb"<<endl;
		}
	}
	
    ofstream outfile;
    outfile.open ("distanceBuffer.txt");
	for(int j=(n-1); j>=0; j--){
		for(int i=0; i<m ; i++){
			if(distanceBuffer[index(i,j,m)]==-1){
				cout<<distanceBuffer[index(i,j,m)]<<" ";
				outfile<<distanceBuffer[index(i,j,m)]<<" ";
			}
			else{
				cout<<" "<<distanceBuffer[index(i,j,m)]<<" ";
				outfile<<distanceBuffer[index(i,j,m)]<<" ";
			}
			
		}
		cout<<endl;
		outfile<<endl;
	}
    outfile.close(); 
}


	void Normal::updateDistanceBuffer(int iC, int jC, int i, int j, int m, int n, int iMin,
																		int jMin, vector<int> *distanceBuffer,
																		vector<Boundary> *distanceList,  int pixelDistance, int dist)
	{
		Point2D p;
		int indDist;
		//i,j ->pixelDistanace
		if(i>=0 && i<m && j>=0 && j<n){
			if((*distanceBuffer)[index(i,j,m)] == -1){
				(*distanceBuffer)[index(i,j,m)]=(*distanceBuffer)[index(iC,jC,m)]+pixelDistance;
				cout<<"distancia"<<(*distanceBuffer)[index(iC,jC,m)]<<endl;
				p.setPoint(i+iMin-dist,j+jMin-dist);//Convert to global image coordenates
				if((*distanceBuffer)[index(i,j,m)]<=dist){
					indDist = (*distanceBuffer)[index(i,j,m)];
//					((*distanceList)[ind].listPoints).push_back(p);
					(distanceList->at(indDist)).push_back(p);
				}
			}
			else{
				if((*distanceBuffer)[index(i,j,m)] > (*distanceBuffer)[index(iC,jC,m)]+pixelDistance){
					if((*distanceBuffer)[index(i,j,m)]+pixelDistance<=dist){
						p.setPoint(i+iMin-dist,j+jMin-dist);//Convert to global image coordenates
						indDist = (*distanceBuffer)[index(i,j,m)];
	//					((*distanceList)[ind].listPoints).push_back(p);
						(distanceList->at(indDist)).remove(p);
						(distanceList->at(indDist+pixelDistance)).push_back(p);
					}
					(*distanceBuffer)[index(i,j,m)]=(*distanceBuffer)[index(iC,jC,m)]+pixelDistance;
				}
			}
		}
	}

/*
#############################################################################
# 							             Novos métodos								   							  #
#############################################################################
*/

unsigned int Normal::CURVE_MIN_SIZE = 9;

/**
 * Método que determina quais pontos da curva de bordo são visíveis por um ponto. Esse método retorna
 * uma matriz onde cada linha possui um intervalo da curva visível por o ponto 'p'.
 */
list<Curve> Normal::setBoundarySegmentVisible(Point2D p, list<Point2D> curvePoints,
																							  int i, bool closedCurve, bool itsInternalBoundary)
{
	if(!closedCurve) cout << "CURVA ABERTA -> ERRO" << endl;//todo novo

	list<Boundary> matrix;
	list<Curve> curves;

  //1ª Etapa - Detectar os intervalos de crescimento e decrescimento do ângulo
  matrix = angleRangesGrowth(p, curvePoints, i, itsInternalBoundary);

  //2ª Etapa - Detectar os trechos escondidos nos intervalos vísiveis (resultantes da etapa 1).
  removeAutoIntersection(p, matrix);
  removeHiddenStretch(p, matrix);

//  cout << "Região " << i << " Número de curvas resultantes: " << matrix.size() << endl;
  int k = 0;
  for(list<Boundary>::iterator Ci = matrix.begin(); Ci != matrix.end(); ++Ci, ++k)
  {
  	Curve curve;
  	curve.listPoints = *Ci;
  	curve.flagFechado = false;//closedCurve;
  	curves.push_back(curve);

//  	cout << "Curva " << k << ": " << (*Ci).size() << " pontos" << endl;
  }

  return curves;
}

/** A
 * Método que determina quais pontos da lista de curvas internas são visíveis por um ponto. Esse método retorna
 * uma matriz onde cada linha possui um intervalo de cada curva visível por o ponto 'p'.
 */
list<Curve> Normal::setInternalCurveSegmentVisible(Point2D p, list<Curves>::iterator it_curves)
{
	list<Boundary> matrix;
	list<Curve> curves;

	list<Curve>::iterator it_curve = it_curves->begin();
	++it_curve; //passando a curva de bordo.

	for(; it_curve != it_curves->end(); ++it_curve) //colocando cada curva interna na matriz.
	{
		matrix.push_back(it_curve->listPoints);
	}

	//TODO problema com ruídos no uso desses métodos abaixo
//	removeAutoIntersection(p, matrix); //removendo autointerseção
//	removeHiddenStretch(p, matrix);	 //removendo os trechos escondidos.

  int k = 0;
  for(list<Boundary>::iterator Ci = matrix.begin(); Ci != matrix.end(); ++Ci, ++k)
  {
  	Curve curve;
  	curve.listPoints = *Ci;
  	curve.flagFechado = false;
  	curves.push_back(curve);

//  	cout << "Curva " << k << ": " << (*Ci).size() << " pontos" << endl;
  }

  return curves;
}

/*
 * Detecta os intervalos de crescimento e decrescimento do angulo da curva em relação ao vetor
 * <1,0> que parte do ponto 'p'. Na verdade o produto vetorial é usado para tal, onde o que
 * se identifica é a mudança do sentido do vetor resultante para definir se o ângulo cresce ou
 * decresce.
 */
list<Boundary> Normal::angleRangesGrowth(Point2D p, list<Point2D> curvePoints, int i, bool itsInternalBoundary)
{
	list<Boundary> matrix;
  list<Point2D> points;

	list<Point2D>::iterator itPoints = curvePoints.begin();
	Point2D v1, v2;
	Point2D p1, p2;
	int index = 0;

	vector<point_tuple> samplePointsCurve_temp;

	if(itsInternalBoundary)	samplePointsCurve_temp = samplePointsInternalCurve;
	else										samplePointsCurve_temp = samplePointsCurve;

	//APENAS PARA TESTE DE ELIMINAÇÃO DE CURVAS
//	samplePointsCurve_temp = getSampleCurve(curvePoints.begin(), curvePoints.end(), i, curvePoints.size()); //TODO comentar

	int size = samplePointsCurve_temp.size(); //todo novo

	for(int t = 0; t < size - 1; ++t)
	{
		p1 = samplePointsCurve_temp[t]._point;		p2 = samplePointsCurve_temp[t+1]._point;
		v1 = UtilityFunctions::getVector(p, p1);
		v2 = UtilityFunctions::getVector(p, p2);

		if(itsInternalBoundary) //analisando uma curva de bordo interna a alguma região.
		{
			if(UtilityFunctions::coefficientOfTheVectorProduct_K(v1, v2) < 0)
			{
				do
				{
					points.push_back(*itPoints++);
					index++;
				}	while(index < samplePointsCurve_temp[t+1]._index);//while(*itPoints != p2);
			}
			else
			{
//					if(points.size() >= CURVE_MIN_SIZE)		matrix.push_back(points); //adicionando à matriz as listas de curvas com nº de pontos maior que um certo valor.
				if(!points.empty())		matrix.push_back(points); //adicionando à matriz as listas de curvas //todo novo
				points.clear();
				do
				{
					itPoints++;
					index++;
				}while(index < samplePointsCurve_temp[t+1]._index);//while(*itPoints != p2);
			}
		}
		else 										//analisando a curva de bordo da própria região.
		{
			if(UtilityFunctions::coefficientOfTheVectorProduct_K(v1, v2) > 0)
			{
				do
				{
					points.push_back(*itPoints++);
					index++;
				} while(index < samplePointsCurve_temp[t+1]._index);//while(*itPoints != p2);
			}
			else
			{
//					if(points.size() >= CURVE_MIN_SIZE)		matrix.push_back(points); //adicionando à matriz as listas de curvas com nº de pontos maior que um certo valor.
				if(!points.empty())		matrix.push_back(points);//todo novo
				points.clear();
				do
				{
					itPoints++;
					index++;
				} while(index < samplePointsCurve_temp[t+1]._index);//while(*itPoints != p2);
			}
		}
	}

	//analisando o último ponto da curva com o primeiro, pois estamos tratando uma curva fechada.
	if(size > 2) //todo novo
	{
		p1 = samplePointsCurve_temp[size-1]._point;		p2 = samplePointsCurve_temp[0]._point;
		v1 = UtilityFunctions::getVector(p, p1);
		v2 = UtilityFunctions::getVector(p, p2);
//		int sizeOfCurve = curvePoints.size();

		if(itsInternalBoundary)  //analisando uma curva de bordo interna a alguma região.
		{
			if(UtilityFunctions::coefficientOfTheVectorProduct_K(v1, v2) < 0)
			{
				do
				{
					if(itPoints == curvePoints.end())	{ ++itPoints; index = -1; } ; //reiniciando a posição do iterador na lista.
					points.push_back(*itPoints++);
					index++;
				}	while(index < samplePointsCurve_temp[0]._index);//while(index < (sizeOfCurve + samplePointsCurve_temp[0]._index));
			}
			else
			{
//				if(points.size() >= CURVE_MIN_SIZE)		matrix.push_back(points); //adicionando à matriz as listas de curvas com nº de pontos maior que um certo valor.
				if(!points.empty())		matrix.push_back(points);
				points.clear();
				do
				{
					if(itPoints == curvePoints.end())	{ ++itPoints; index = -1; } //reiniciando a posição do iterador na lista.
					itPoints++;
					index++;
				}	while(index < samplePointsCurve_temp[0]._index);//while(index < (sizeOfCurve + samplePointsCurve_temp[0]._index));
			}
		}
		else 										//analisando a curva de bordo da própria região.
		{
			if(UtilityFunctions::coefficientOfTheVectorProduct_K(v1, v2) > 0)
			{
				do
				{
					if(itPoints == curvePoints.end())	{ ++itPoints; index = -1; } //reiniciando a posição do iterador na lista.
					points.push_back(*itPoints++);
					index++;
				}	while(index < samplePointsCurve_temp[0]._index);// while(index < (sizeOfCurve + samplePointsCurve_temp[0]._index));
			}
			else
			{
//				if(points.size() >= CURVE_MIN_SIZE)		matrix.push_back(points); //adicionando à matriz as listas de curvas com nº de pontos maior que um certo valor.
				if(!points.empty())		matrix.push_back(points);
				points.clear();
				do
				{
					if(itPoints == curvePoints.end())	{ ++itPoints; index = -1; } //reiniciando a posição do iterador na lista.
					itPoints++;
					index++;
				}	while(index < samplePointsCurve_temp[0]._index);// while(index < (sizeOfCurve + samplePointsCurve_temp[0]._index));
			}
		}
	}

	if(!points.empty())		matrix.push_back(points); //todo novo
//	if(points.size() >= CURVE_MIN_SIZE)		matrix.push_back(points); //adicionando à matriz as listas de curvas com nº de pontos maior que um certo valor.

	return matrix;
}

/* A
 * Remove os trechos que a própria curva esconde.
 */
void Normal::removeAutoIntersection(Point2D p, list<Boundary> &matrix)
{
	int c = 0;
	bool COMMENT = false;
	for(list<Boundary>::iterator it_curve = matrix.begin(); it_curve != matrix.end(); ++it_curve, ++c)
	{
		if(it_curve->size() > 3)
		{
			if(COMMENT) cout << "---------- Curva " << c << " ---------- " << endl;
			Point2D extremeFront = it_curve->front();
			Point2D extremeBack  = it_curve->back();

			int i = 0;
			int indexFront;
			int indexBack;

			Boundary::iterator it_point = it_curve->begin();
			++it_point;
			++i;

			bool eFrontIsCEP = false;
			bool eFrontIsEPC = false;
			bool eBackIsCEP  = false;
			bool eBackIsEPC  = false;

			long int p_value_eFront = UtilityFunctions::coefficientOfTheVectorProduct_K(UtilityFunctions::getVector(p,*it_point),
																																									UtilityFunctions::getVector(p, extremeFront));

			long int p_value_eBack = UtilityFunctions::coefficientOfTheVectorProduct_K(UtilityFunctions::getVector(p,*it_point),
																																								 UtilityFunctions::getVector(p, extremeBack));

			++i;
			indexFront = indexBack = i;
//			for(++it_point; (it_point != it_curve->end() && !eFrontItsCEP && !eBackItsCEP) ; ++it_point, ++i)
			for(++it_point; it_point != it_curve->end() ; ++it_point, ++i)
			{
				long int value_eFront = UtilityFunctions::coefficientOfTheVectorProduct_K(UtilityFunctions::getVector(p,*it_point),
														    																								  UtilityFunctions::getVector(p,extremeFront));

				long int value_eBack  = UtilityFunctions::coefficientOfTheVectorProduct_K(UtilityFunctions::getVector(p,*it_point),
																																									UtilityFunctions::getVector(p,extremeBack));

				if(p_value_eFront * value_eFront <= 0)
				{
					if(its_CEP(p, *it_point, extremeFront))
					{
						if(COMMENT) cout << "caso 1" << endl;
						eFrontIsCEP = true;
						indexFront = i;
					}
					else if(its_EPC(extremeFront, *it_point, p))
					{
						if(COMMENT) cout << "caso 2" << endl;
						eFrontIsEPC = true;
					}
				}
				else if(p_value_eBack * value_eBack <= 0)
				{
					if(its_CEP(p, *it_point, extremeBack))
					{
						if(COMMENT) cout << "caso 3" << endl;
						eBackIsCEP = true;
						indexBack = i;
					}
					else if(its_EPC(extremeBack, *it_point, p))
					{
						if(COMMENT) cout << "caso 4" << endl;
						eBackIsEPC = true;
					}
				}

				p_value_eFront = value_eFront;
				p_value_eBack  = value_eBack;
			}

			it_point = it_curve->begin();

			if(COMMENT) cout << "Front " << abs(indexFront - 0) << " ~= End " << abs(indexBack - (int(it_curve->size())-1)) << endl;

			if(eFrontIsCEP)
			{
				if(indexFront > 30 && indexFront < abs(int(it_curve->size()) - 31))
				{
					advance(it_point, indexFront);
					if(eBackIsEPC || eFrontIsEPC)	it_curve->erase(it_point, it_curve->end());
					else 						it_curve->erase(it_curve->begin(), it_point);
				}
			}
			else if(eBackIsCEP)
			{
				if(abs(indexBack - int(it_curve->size()) - 1) > 30)
				{
					advance(it_point, indexBack);
					if(eFrontIsEPC || eBackIsEPC) it_curve->erase(it_curve->begin(), it_point);
					else 						it_curve->erase(it_point, it_curve->end());
				}
			}

		}
		if(it_curve->empty()) it_curve = matrix.erase(it_curve);
	}

}

/*
 * Detecta os trechos escondidos do ponto 'p' nos intervalos visíveis que estão em 'matrix'.
 */
void Normal::removeHiddenStretch(Point2D p, list<Boundary> &matrix)
{
	int i = 0;
	bool COMMENT = false;

	if(COMMENT) cout << "numero de curvas inicial: " << matrix.size() << endl;

	//para cada curva
	for(list<Boundary>::iterator Ci = matrix.begin(); Ci != matrix.end() && i < matrix.size(); ++Ci, ++i) //Curva Ci
	{
		if(Ci->size() >= CURVE_MIN_SIZE)
		{
			int j = i + 1;
			list<Boundary>::iterator Bj = Ci;
			++Bj;

			while(Bj != matrix.end() && j < matrix.size()) //Curva Bj
			{
				bool finished = false;
				bool erase_Bj = false;
				if(Bj->size() >= CURVE_MIN_SIZE)
				{
					if(COMMENT) cout << i << " com " << j << endl;
					point_tuple extreme_c1, extreme_c2, extreme_b1, extreme_b2;

					extreme_c1.init(0, i, Ci->front());
					extreme_c2.init(Ci->size()-1, i, Ci->back());
					extreme_b1.init(0, j, Bj->front());
					extreme_b2.init(Bj->size()-1, j, Bj->back());

					point_tuple proj_c1_B, proj_c2_B, proj_b1_C, proj_b2_C;
					proj_c1_B = findingProjectionPoint(p, *Bj, extreme_c1._point); //calculando as projeções dos pontos de uma curva na outra.
					proj_c2_B = findingProjectionPoint(p, *Bj, extreme_c2._point);
					proj_b1_C = findingProjectionPoint(p, *Ci, extreme_b1._point);
					proj_b2_C = findingProjectionPoint(p, *Ci, extreme_b2._point);

					proj_c1_B._index_matrix = proj_c2_B._index_matrix = extreme_b1._index_matrix;//extreme_c1._index_matrix;
					proj_b1_C._index_matrix = proj_b2_C._index_matrix = extreme_c1._index_matrix;//extreme_b1._index_matrix;

					proj_c1_B.index_extreme = extreme_c1._index;	proj_c2_B.index_extreme = extreme_c2._index;
					proj_b1_C.index_extreme = extreme_b1._index;	proj_b2_C.index_extreme = extreme_b2._index;

					proj_c1_B.index_matrix_extreme = extreme_c1._index_matrix;	proj_c2_B.index_matrix_extreme = extreme_c2._index_matrix;
					proj_b1_C.index_matrix_extreme = extreme_b1._index_matrix;	proj_b2_C.index_matrix_extreme = extreme_b2._index_matrix;

					proj_c1_B.point_extreme = extreme_c1._point;	proj_c2_B.point_extreme = extreme_c2._point;
					proj_b1_C.point_extreme = extreme_b1._point;	proj_b2_C.point_extreme = extreme_b2._point;

					vector<point_tuple> pts; //vetor que conterá os pontos de projeção válidos.
					if(proj_c1_B.itsProjection) pts.push_back(proj_c1_B);
					if(proj_c2_B.itsProjection) pts.push_back(proj_c2_B);
					if(proj_b1_C.itsProjection) pts.push_back(proj_b1_C);
					if(proj_b2_C.itsProjection) pts.push_back(proj_b2_C);

					if(COMMENT) cout << "pts.size = " << pts.size() << endl;
					if(!pts.empty()) //se há interseção
					{
						vector<point_tuple> CEPs;

						for(unsigned int w = 0; w < pts.size(); ++w)
							if(pts[w].itsCEP)	CEPs.push_back(pts[w]); //verificando se as projeções válidas são CEPs

						if(COMMENT) cout << "CEPs.size = " << CEPs.size() << endl;
						//CASO 1 - Inicio -----------------------------------------------------------------------------
						if((CEPs.empty() && pts.size() == 2) && pointsOfTheSameCurve(pts)) //verificando se não há casos CEP, há duas projeções válidas e os pontos são da mesma curva
						{
							if(COMMENT) cout << "CASO NAO CEP" << endl;
							int index_matrix_extreme = pts[0].index_matrix_extreme == pts[1].index_matrix_extreme ? pts[0].index_matrix_extreme : pts[1].index_matrix_extreme;
							if(index_matrix_extreme == i)
							{
								Ci = matrix.erase(Ci); //elimine toda a curva Ci.
								Bj = Ci;
								j = i;
								if(COMMENT) cout << "Ci eliminada" << endl;
							}
							else
							{
								Bj = matrix.erase(Bj); //elimine toda a curva Bj.
								--j;
								erase_Bj = true;
								if(COMMENT) cout << "Bj eliminada" << endl;
							}
						}//CASO 1 - Fim -----------------------------------------------------------------------------
						else	//há casos CEP
						{
							if(COMMENT) cout << "CASO CEP" << endl;
							//projeções válidas e CEPS
							list<Point2D>::iterator it_proj;
							list<Point2D>::iterator it_proj_next;
							list<Point2D>::iterator it_extreme;
							list<Point2D>::reverse_iterator rit_extreme;

							point_tuple _p;
							long int K1, K2;
							Point2D v1, v2;
							int index;

							for(unsigned int k = 0; k < CEPs.size(); ++k) //analisando todas as projeções CEP
							{
								if(CEPs[k].index_matrix_extreme == j) //ponto de corte pertence a curva Ci.
								{
									it_proj = Ci->begin();

									advance(it_proj, CEPs[k]._index); //avançando o iterador da curva Ci até o ponto de projeção.

									it_proj_next  = it_proj;

									if(CEPs[k].index_extreme == 0) //extremo está no começa da lista da curva Bj.
									{
										it_extreme = Bj->begin();
										v1 = UtilityFunctions::getVector(p,*it_extreme);
										index =  getAutoProjection(*it_extreme, *Bj, p);
										if(index > 0) index = floor(0.7*index);
										else					index = floor(0.7*Bj->size());
										advance(it_extreme, index);
										v2 = UtilityFunctions::getVector(p,*it_extreme);//*(++it_extreme));
										if(COMMENT) cout << "AQUIo " << *it_extreme << endl;
									}
									else													//extremo está no fim da lista da curva Bj.
									{
										rit_extreme = Bj->rbegin();
										v1 = UtilityFunctions::getVector(p,*rit_extreme);
										index = getAutoProjection(*rit_extreme, *Bj, p);
										if(index > 0)	index = floor(0.7*(Bj->size() - index - 1));
										else					index = floor(0.7*Bj->size());
										advance(rit_extreme, index);
										v2 = UtilityFunctions::getVector(p,*rit_extreme);//*(++rit_extreme));
										if(COMMENT) cout << "AQUIf " << *rit_extreme << endl;
									}

									K1 = UtilityFunctions::coefficientOfTheVectorProduct_K(v1, v2);
									K1 = K1 / fabs(K1);

									v1 = UtilityFunctions::getVector(p, *it_proj_next);
									index = getAutoProjection(*it_proj_next, *Ci, p);

									if(index > 0) //caso ache alguma projeção
									{
										if( Ci->size() - index > (Ci->size()/2) ) index = index + floor(0.1*abs(CEPs[k]._index - index)); //se o ponto de projeção estiver na segunda metade da lista, adicione 10% da distância da posição do mesmo à sua projeção à posição de sua projeção.
										else																		  index = index - floor(0.1*abs(CEPs[k]._index - index)); //se o ponto de projeção estiver na primeira metade da lista, diminua 10% da distância da posição do mesmo à sua projeção à posição de sua projeção.
//										cout << "pDAp " << index << " CEP = " << CEPs[k]._index << endl;
									}
									else				 //caso não ache alguma projeção
									{
										if( Ci->size() - CEPs[k]._index > (Ci->size()/2) ) index = (Ci->size()-1) - floor(0.1*abs(CEPs[k]._index - int(Ci->size()-1)));//{P = Ci->back();  index = Ci->size()-1;}
										else																		  				 index = floor(0.1*CEPs[k]._index); //{P = Ci->front(); index = 0;}
//										cout << "NAO pDAp " << index << " CEP = " << CEPs[k]._index << endl;
									}
									it_proj_next = Ci->begin();
									advance(it_proj_next, index);
									v2 = UtilityFunctions::getVector(p, *it_proj_next);

									if(CEPs[k]._index < index)	K2 = UtilityFunctions::coefficientOfTheVectorProduct_K(v1,v2);
									else												K2 = UtilityFunctions::coefficientOfTheVectorProduct_K(v2,v1);

									K2 = K2 / fabs(K2);
									if(COMMENT) cout << "CEPs[" << k << "] = " << K1 << "*" << K2 << " = " << K1*K2 << endl;
									CEPs[k].KxK = K1 * K2;
								}
								else 																//ponto de corte pertence a curva Bj.
								{
									it_proj = Bj->begin();

									advance(it_proj, CEPs[k]._index); //avançando o iterador da curva Bj até o ponto de projeção.

									it_proj_next  = it_proj;

									if(CEPs[k].index_extreme == 0) //extremo está no começa da lista da curva Ci.
									{
										it_extreme = Ci->begin();
										v1 = UtilityFunctions::getVector(p,*it_extreme);
										index = getAutoProjection(*it_extreme, *Ci, p);
										if(index > 0)	index = floor(0.7*index);
										else					index = floor(0.7*Ci->size());
										advance(it_extreme, index);
										v2 = UtilityFunctions::getVector(p,*it_extreme);//*(++it_extreme));
										if(COMMENT) cout << "AQUIo " << *it_extreme << endl;
									}
									else													//extremo está no fim da lista da curva Ci.
									{
										rit_extreme = Ci->rbegin();
										v1 = UtilityFunctions::getVector(p, *rit_extreme);
										index = getAutoProjection(*rit_extreme, *Ci, p);
										if(index > 0)	index = floor(0.7*(Ci->size() - index - 1));
										else					index = floor(0.7*Ci->size());
										advance(rit_extreme, index);
										v2 = UtilityFunctions::getVector(p, *rit_extreme);
										if(COMMENT) cout << "AQUIf " << *rit_extreme << endl;
									}

									K1 = UtilityFunctions::coefficientOfTheVectorProduct_K(v1, v2);
									K1 = K1 / fabs(K1);

									v1 = UtilityFunctions::getVector(p, *it_proj_next);
									index = getAutoProjection(*it_proj_next, *Bj, p);

									if(index > 0) //caso ache alguma projeção
									{
										if( Bj->size() - index > (Bj->size()/2) )	index = index + floor(0.1*abs(CEPs[k]._index - index));
										else																			index = index - floor(0.1*abs(CEPs[k]._index - index));
									}
									else 					//caso não ache alguma projeção
									{
										if( Bj->size() - CEPs[k]._index > (Bj->size()/2) ) index = (Bj->size()-1) - floor(0.1*abs(CEPs[k]._index - int(Bj->size()-1)));//{ P = Bj->back();  index = Bj->size() - 1; }
										else																							 index = floor(0.1*CEPs[k]._index);//{ P = Bj->front(); index = 0; }
									}
									it_proj_next = Bj->begin();
									advance(it_proj_next, index);
									v2 = UtilityFunctions::getVector(p, *it_proj_next);

									if(CEPs[k]._index < index)	K2 = UtilityFunctions::coefficientOfTheVectorProduct_K(v1,v2);
									else 												K2 = UtilityFunctions::coefficientOfTheVectorProduct_K(v2,v1);

									K2 = K2 / fabs(K2);
									if(COMMENT) cout << "CEPs[" << k << "] = " << K1 << "*" << K2 << " = " << K1*K2 << endl;
									CEPs[k].KxK = K1 * K2;
								}
							}

//							bool finished = false;

							if(CEPs.size() > 1)	UtilityFunctions::quickSort(CEPs, 0, CEPs.size() - 1); //ordernando em ordem dececrescente em função do '_index' CEPs.

							//CASO 2 - Inicio -----------------------------------------------------------------------------
							if(CEPs.size() == 2)
							{
								if(pointsOfTheSameCurve(CEPs) && CEPs[0].KxK < 0 && CEPs[1].KxK > 0) //verificando se os dois pontos são da mesma curva e a eliminação é entre os extremos da mesma)
								{
									if(CEPs[0]._index_matrix == i) //ponto de corte pertence a curva Ci.
									{
										Boundary C1, C2;
//										C1 = C2 = *Ci;
										C1 = Boundary(*Ci); //TODO NOVO
										C2 = Boundary(*Ci); //TODO NOVO

										it_proj = C1.begin();//list<Point2D>::iterator it_c1 = C1.begin();
										it_proj_next = C2.begin();//list<Point2D>::iterator it_c2 = C2.begin();

										advance(it_proj, CEPs[1]._index);
										advance(it_proj_next, CEPs[0]._index);

										C1.erase(it_proj, C1.end());
//										C2.erase(C2.begin(), C2.end());
										C2.erase(C2.begin(), it_proj_next); //TODO NOVO

										if(!C1.empty()) matrix.push_back(C1); //adicionando os dois pedaços de curva restante da eliminação na matriz para análise.
										if(!C2.empty())	matrix.push_back(C2);

										Ci = matrix.erase(Ci); //eliminando a curva quebrada da análise
										Bj = Ci;
										j = i;

										if(COMMENT) cout << "Ci erase [it1,it2], Ci eliminada. C1 e C2 pushback" << endl;
										finished = true;
									}
									else 													 //ponto de corte pertence a curva Bj.
									{
										Boundary B1, B2;
//										B1 = B2 = *Bj;
										B1 = Boundary(*Bj); //TODO NOVO
										B2 = Boundary(*Bj); //TODO NOVO

										it_proj = B1.begin();
										it_proj_next = B2.begin();

										advance(it_proj, CEPs[1]._index);
										advance(it_proj_next, CEPs[0]._index);

										B1.erase(it_proj, B1.end());
										B2.erase(B2.begin(), it_proj_next);

										if(!B1.empty())	matrix.push_back(B1);
										if(!B2.empty())	matrix.push_back(B2);

										Bj = matrix.erase(Bj);
										--j;
										erase_Bj = true;

										if(COMMENT) cout << "Bj erase [it1,it2], Bj eliminada. B1 e B2 pushback" << endl;
										finished = true;
									}
								}
								else if(pointsOfTheSameCurve(CEPs)) //Projeções CEP's da mesma curva com eliminações nos extremos da mesma.
								{
									if(CEPs[0]._index_matrix == i) //pontos de corte pertence a curva Ci.
									{
										it_proj = Ci->begin();
										advance(it_proj, CEPs[0]._index);
										if(COMMENT) cout << "Ci erase [it,end]" << endl;
										Ci->erase(it_proj, Ci->end());

										it_proj = Ci->begin();
										advance(it_proj, CEPs[1]._index);
										if(COMMENT) cout << "Ci erase [begin,it]" << endl;
										Ci->erase(Ci->begin(), it_proj);

										if(Ci->empty()) //verificando se todos os pontos foram eliminados.
										{
											Ci = matrix.erase(Ci);
											Bj = Ci;
											j = i;
											if(COMMENT) cout << "Ci eliminada por estar vazia" << endl;
										}
									}
									else													//pontos de corte pertence a curva Bj.
									{
										it_proj = Bj->begin();
										advance(it_proj, CEPs[0]._index);
										if(COMMENT) cout << "Bj erase [it,end]" << endl;
										Bj->erase(it_proj, Bj->end());

										it_proj = Bj->begin();
										advance(it_proj, CEPs[1]._index);
										if(COMMENT) cout << "Bj erase [begin,it]" << endl;
										Bj->erase(Bj->begin(), it_proj);

										if(Bj->empty())
										{
											Bj = matrix.erase(Bj);
											--j;
											erase_Bj = true;
											if(COMMENT) cout << "Bj eliminada por estar vazia" << endl;
										}
									}
									finished = true;
								}
							}
							//CASO 2 - Fim -----------------------------------------------------------------------------

							//CASO 3 - Inicio --------------------------------------------------------------------------
							if(!finished) //Projeções CEP's de curvas diferentes.
							{
								for(unsigned int k = 0; k < CEPs.size(); ++k) //analisando todas as projeções CEP
								{
									if(CEPs[k]._index_matrix == i) //ponto de corte pertence a curva Ci.
									{
										it_proj = Ci->begin();
										advance(it_proj, CEPs[k]._index);
										if(COMMENT) cout << "KxK = " << CEPs[k].KxK << endl;
										if(CEPs[k].KxK > 0)
										{
											if(COMMENT) cout << "Ci erase [it,end]" << endl;
											Ci->erase(it_proj, Ci->end());
										}
										else
										{
											if(COMMENT) cout << "Ci erase [begin,it]" << endl;
											Ci->erase(Ci->begin(), it_proj);
										}

										if(Ci->empty()) //verificando se todos os pontos foram eliminados.
										{
											Ci = matrix.erase(Ci);
											Bj = Ci;
											j = i;
											if(COMMENT) cout << "Ci eliminada por estar vazia" << endl;
										}
									}
									else													 //ponto de corte pertence a curva Bj.
									{
										it_proj = Bj->begin();
										advance(it_proj, CEPs[k]._index);
										if(COMMENT) cout << "KxK = " << CEPs[k].KxK << endl;
										if(CEPs[k].KxK > 0)
										{
											if(COMMENT) cout << "Bj erase [it,end]" << endl;
											Bj->erase(it_proj, Bj->end());
										}
										else
										{
											if(COMMENT) cout << "Bj erase [begin,it]" << endl;
											Bj->erase(Bj->begin(), it_proj);
										}

										if(Bj->empty())
										{
											Bj = matrix.erase(Bj);
											--j;
											erase_Bj = true;
											if(COMMENT) cout << "Bj eliminada por estar vazia" << endl;
										}
									}
								}
							}
							//CASO 3 - Fim -----------------------------------------------------------------------------
						}
					}
				}
				++j;
				if(!erase_Bj)	++Bj;
			}
		}
	}
}

/* A
 * Detecta os trechos escondidos do ponto 'p' nos intervalos visíveis que estão em 'stretchBoundary'(1) e
 * 'stretchInternalCurves'(2), removendo os trechos que (1) esconde de (2) e vice-versa.
 */
/*void Normal::removeHiddenStrech_step2(Point2D p, list<Curve> &stretchBoundary, list<Curve> &stretchInternalCurves)
{
	bool COMMENT = true;

	//curva Ci id 0
	//curva Bj id 1
	for(list<Curve>::iterator Ci = stretchBoundary.begin(); Ci != stretchBoundary.end();) //para trecho de bordo
	{
		bool erase_Ci = false;
		if(Ci->listPoints.size() >= CURVE_MIN_SIZE)
		{
			for(list<Curve>::iterator Bj = stretchInternalCurves.begin(); Bj != stretchInternalCurves.end();) //para cada trecho de curva interna
			{
				bool finished = false;
//				bool erase_Ci = false;
				bool erase_Bj = false;
				if(Bj->listPoints.size() >= CURVE_MIN_SIZE && !Bj->flagFechado)
				{
					point_tuple extreme_c1, extreme_c2, extreme_b1, extreme_b2;

					extreme_c1.init(0, 0, Ci->listPoints.front());
					extreme_c2.init(Ci->listPoints.size()-1, 0, Ci->listPoints.back());
					extreme_b1.init(0, 1, Bj->listPoints.front());
					extreme_b2.init(Bj->listPoints.size()-1, 1, Bj->listPoints.back());

					point_tuple proj_c1_B, proj_c2_B, proj_b1_C, proj_b2_C;
					list<Point2D>::iterator it = Bj->listPoints.begin();
					proj_c1_B = findingProjectionPoint(p, Bj->listPoints, extreme_c1._point); //calculando as projeções dos pontos de uma curva na outra.
					proj_c2_B = findingProjectionPoint(p, Bj->listPoints, extreme_c2._point);
					proj_b1_C = findingProjectionPoint(p, Ci->listPoints, extreme_b1._point);
					proj_b2_C = findingProjectionPoint(p, Ci->listPoints, extreme_b2._point);

					proj_c1_B._index_matrix = proj_c2_B._index_matrix = extreme_b1._index_matrix;//extreme_c1._index_matrix;
					proj_b1_C._index_matrix = proj_b2_C._index_matrix = extreme_c1._index_matrix;//extreme_b1._index_matrix;

					proj_c1_B.index_extreme = extreme_c1._index;	proj_c2_B.index_extreme = extreme_c2._index;
					proj_b1_C.index_extreme = extreme_b1._index;	proj_b2_C.index_extreme = extreme_b2._index;

					proj_c1_B.index_matrix_extreme = extreme_c1._index_matrix;	proj_c2_B.index_matrix_extreme = extreme_c2._index_matrix;
					proj_b1_C.index_matrix_extreme = extreme_b1._index_matrix;	proj_b2_C.index_matrix_extreme = extreme_b2._index_matrix;

					proj_c1_B.point_extreme = extreme_c1._point;	proj_c2_B.point_extreme = extreme_c2._point;
					proj_b1_C.point_extreme = extreme_b1._point;	proj_b2_C.point_extreme = extreme_b2._point;

					vector<point_tuple> pts; //vetor que conterá os pontos de projeção válidos.
					if(proj_c1_B.itsProjection) pts.push_back(proj_c1_B);
					if(proj_c2_B.itsProjection) pts.push_back(proj_c2_B);
					if(proj_b1_C.itsProjection) pts.push_back(proj_b1_C);
					if(proj_b2_C.itsProjection) pts.push_back(proj_b2_C);

					if(COMMENT) cout << "pts.size = " << pts.size() << endl;
					if(!pts.empty())
					{
						vector<point_tuple> CEPs;

						for(unsigned int w = 0; w < pts.size(); ++w)
							if(pts[w].itsCEP)	CEPs.push_back(pts[w]); ////verificando se as projeções válidas são CEPs

						if(COMMENT) cout << "CEPs.size = " << CEPs.size() << endl;
						//CASO 1 - Inicio -----------------------------------------------------------------------------
						if((CEPs.empty() && pts.size() == 2) && pointsOfTheSameCurve(pts)) //verificando se não há casos CEP, há duas projeções válidas e os pontos são da mesma curva
						{
							if(COMMENT) cout << "CASO NAO CEP" << endl;
							int index_matrix_extreme = pts[0].index_matrix_extreme == pts[1].index_matrix_extreme ? pts[0].index_matrix_extreme : pts[1].index_matrix_extreme;
							if(index_matrix_extreme == 0)
							{
								Ci = stretchBoundary.erase(Ci); //elimine toda a curva Ci.
								erase_Ci = true;
								if(COMMENT) cout << "Ci eliminada" << endl;
							}
							else
							{
								Bj = stretchInternalCurves.erase(Bj); //elimine toda a curva Bj.
								erase_Bj = true;
								if(COMMENT) cout << "Bj eliminada" << endl;
							}
						}//CASO 1 - Fim -----------------------------------------------------------------------------
						else  //há casos CEP
						{
							if(COMMENT) cout << "CASO CEP" << endl;
							//projeções válidas e CEPS
							list<Point2D>::iterator it_proj;
							list<Point2D>::iterator it_proj_next;
							list<Point2D>::iterator it_extreme;
							list<Point2D>::reverse_iterator rit_extreme;

							point_tuple _p;
							long int K1, K2;
							Point2D v1, v2;
							int index;

							for(unsigned int k = 0; k < CEPs.size(); ++k) //analisando todas as projeções CEP
							{
								if(CEPs[k].index_matrix_extreme == 0) //ponto de corte pertence a curva Ci.
								{
									it_proj = Ci->listPoints.begin();

									advance(it_proj, CEPs[k]._index); //avançando o iterador da curva Ci até o ponto de projeção.

									it_proj_next  = it_proj;

									if(CEPs[k].index_extreme == 0) //extremo está no começa da lista da curva Bj.
									{
										it_extreme = Bj->listPoints.begin();
										v1 = UtilityFunctions::getVector(p,*it_extreme);
										index =  getAutoProjection(*it_extreme, Bj->listPoints, p);
										if(index > 0) index = floor(0.7*index);
										else					index = floor(0.7*Bj->listPoints.size());
										advance(it_extreme, index);
										v2 = UtilityFunctions::getVector(p,*it_extreme);
										if(COMMENT) cout << "AQUIo " << *it_extreme << endl;
									}
									else													//extremo está no fim da lista da curva Bj.
									{
										rit_extreme = Bj->listPoints.rbegin();
										v1 = UtilityFunctions::getVector(p,*rit_extreme);
										index = getAutoProjection(*rit_extreme, Bj->listPoints, p);
										if(index > 0)	index = floor(0.7*(Bj->listPoints.size() - index - 1));
										else					index = floor(0.7*Bj->listPoints.size());
										advance(rit_extreme, index);
										v2 = UtilityFunctions::getVector(p,*rit_extreme);
										if(COMMENT) cout << "AQUIf " << *rit_extreme << endl;
									}

									K1 = UtilityFunctions::coefficientOfTheVectorProduct_K(v1, v2);
									K1 = K1 / fabs(K1);

									v1 = UtilityFunctions::getVector(p, *it_proj_next);
									index = getAutoProjection(*it_proj_next, Ci->listPoints, p);

									if(index > 0) //caso ache alguma projeção
									{
										if( Ci->listPoints.size() - index > (Ci->listPoints.size()/2) ) index = index + floor(0.1*abs(CEPs[k]._index - index)); //se o ponto de projeção estiver na segunda metade da lista, adicione 10% da distância da posição do mesmo à sua projeção à posição de sua projeção.
										else																		  index = index - floor(0.1*abs(CEPs[k]._index - index)); //se o ponto de projeção estiver na primeira metade da lista, diminua 10% da distância da posição do mesmo à sua projeção à posição de sua projeção.
										//										cout << "pDAp " << index << " CEP = " << CEPs[k]._index << endl;
									}
									else				 //caso não ache alguma projeção
									{
										if( Ci->listPoints.size() - CEPs[k]._index > (Ci->listPoints.size()/2) ) index = (Ci->listPoints.size()-1) - floor(0.1*abs(CEPs[k]._index - int(Ci->listPoints.size()-1)));//{P = Ci->back();  index = Ci->size()-1;}
										else																		  				 index = floor(0.1*CEPs[k]._index); //{P = Ci->front(); index = 0;}
										//										cout << "NAO pDAp " << index << " CEP = " << CEPs[k]._index << endl;
									}
									it_proj_next = Ci->listPoints.begin();
									advance(it_proj_next, index);
									v2 = UtilityFunctions::getVector(p, *it_proj_next);

									if(CEPs[k]._index < index)	K2 = UtilityFunctions::coefficientOfTheVectorProduct_K(v1,v2);
									else												K2 = UtilityFunctions::coefficientOfTheVectorProduct_K(v2,v1);

									K2 = K2 / fabs(K2);
									if(COMMENT) cout << "CEPs[" << k << "] = " << K1 << "*" << K2 << " = " << K1*K2 << endl;
									CEPs[k].KxK = K1 * K2;
								}
								else 																//ponto de corte pertence a curva Bj.
								{
									it_proj = Bj->listPoints.begin();

									advance(it_proj, CEPs[k]._index); //avançando o iterador da curva Bj até o ponto de projeção.

									it_proj_next  = it_proj;

									if(CEPs[k].index_extreme == 0) //extremo está no começa da lista da curva Ci.
									{
										it_extreme = Ci->listPoints.begin();
										v1 = UtilityFunctions::getVector(p,*it_extreme);
										index = getAutoProjection(*it_extreme, Ci->listPoints, p);
										if(index > 0)	index = floor(0.7*index);
										else					index = floor(0.7*Ci->listPoints.size());
										advance(it_extreme, index);
										v2 = UtilityFunctions::getVector(p,*it_extreme);//*(++it_extreme));
										if(COMMENT) cout << "AQUIo " << *it_extreme << endl;
									}
									else													//extremo está no fim da lista da curva Ci.
									{
										rit_extreme = Ci->listPoints.rbegin();
										v1 = UtilityFunctions::getVector(p, *rit_extreme);
										index = getAutoProjection(*rit_extreme, Ci->listPoints, p);
										if(index > 0)	index = floor(0.7*(Ci->listPoints.size() - index - 1));
										else					index = floor(0.7*Ci->listPoints.size());
										advance(rit_extreme, index);
										v2 = UtilityFunctions::getVector(p, *rit_extreme);
										if(COMMENT) cout << "AQUIf " << *rit_extreme << endl;
									}

									K1 = UtilityFunctions::coefficientOfTheVectorProduct_K(v1, v2);
									K1 = K1 / fabs(K1);

									v1 = UtilityFunctions::getVector(p, *it_proj_next);
									index = getAutoProjection(*it_proj_next, Bj->listPoints, p);

									if(index > 0) //caso ache alguma projeção
									{
										if( Bj->listPoints.size() - index > (Bj->listPoints.size()/2) )	index = index + floor(0.1*abs(CEPs[k]._index - index));
										else																			index = index - floor(0.1*abs(CEPs[k]._index - index));
									}
									else 					//caso não ache alguma projeção
									{
										if( Bj->listPoints.size() - CEPs[k]._index > (Bj->listPoints.size()/2) ) index = (Bj->listPoints.size()-1) - floor(0.1*abs(CEPs[k]._index - int(Bj->listPoints.size()-1)));//{ P = Bj->back();  index = Bj->size() - 1; }
										else																							 index = floor(0.1*CEPs[k]._index);//{ P = Bj->front(); index = 0; }
									}
									it_proj_next = Bj->listPoints.begin();
									advance(it_proj_next, index);
									v2 = UtilityFunctions::getVector(p, *it_proj_next);

									if(CEPs[k]._index < index)	K2 = UtilityFunctions::coefficientOfTheVectorProduct_K(v1,v2);
									else 												K2 = UtilityFunctions::coefficientOfTheVectorProduct_K(v2,v1);

									K2 = K2 / fabs(K2);
									if(COMMENT) cout << "CEPs[" << k << "] = " << K1 << "*" << K2 << " = " << K1*K2 << endl;
									CEPs[k].KxK = K1 * K2;
								}
							}

							if(CEPs.size() > 1)	UtilityFunctions::quickSort(CEPs, 0, CEPs.size() - 1); //ordernando em ordem dececrescente em função do '_index' CEPs.

							//CASO 2 - Inicio -----------------------------------------------------------------------------
							if(CEPs.size() == 2)
							{
								if(pointsOfTheSameCurve(CEPs) && CEPs[0].KxK < 0 && CEPs[1].KxK > 0) //verificando se os dois pontos são da mesma curva e a eliminação é entre os extremos da mesma)
								{
									if(CEPs[0]._index_matrix == 0) //ponto de corte pertence a curva Ci.
									{
										Curve C1, C2;
										C1.flagFechado = C2.flagFechado = false;
										C1.listPoints = C2.listPoints = Ci->listPoints;

										it_proj = C1.listPoints.begin();
										it_proj_next = C2.listPoints.begin();

										advance(it_proj, CEPs[1]._index);
										advance(it_proj_next, CEPs[0]._index);

										C1.listPoints.erase(it_proj, C1.listPoints.end());
										C2.listPoints.erase(C2.listPoints.begin(), C2.listPoints.end());

										if(!C1.listPoints.empty()) stretchBoundary.push_back(C1); //adicionando os dois pedaços de curva restante da eliminação na matriz para análise.
										if(!C2.listPoints.empty())	stretchBoundary.push_back(C2);

										Ci = stretchBoundary.erase(Ci); //eliminando a curva quebrada da análise
										erase_Ci = true;

										if(COMMENT) cout << "Ci erase [it1,it2], Ci eliminada. C1 e C2 pushback" << endl;
										finished = true;
									}
									else 													 //ponto de corte pertence a curva Bj.
									{
										Curve B1, B2;
										B1.flagFechado = B2.flagFechado = false;
										B1.listPoints = B2.listPoints = Bj->listPoints;

										it_proj = B1.listPoints.begin();
										it_proj_next = B2.listPoints.begin();

										advance(it_proj, CEPs[1]._index);
										advance(it_proj_next, CEPs[0]._index);

										B1.listPoints.erase(it_proj, B1.listPoints.end());
										B2.listPoints.erase(B2.listPoints.begin(), it_proj_next);

										if(!B1.listPoints.empty())	stretchInternalCurves.push_back(B1);
										if(!B2.listPoints.empty())	stretchInternalCurves.push_back(B2);

										Bj = stretchInternalCurves.erase(Bj);
										erase_Bj = true;

										if(COMMENT) cout << "Bj erase [it1,it2], Bj eliminada. B1 e B2 pushback" << endl;
										finished = true;
									}
								}
								else if(pointsOfTheSameCurve(CEPs)) //Projeções CEP's da mesma curva com eliminações nos extremos da mesma.
								{
									if(CEPs[0]._index_matrix == 0) //pontos de corte pertence a curva Ci.
									{
										it_proj = Ci->listPoints.begin();
										advance(it_proj, CEPs[0]._index);
										if(COMMENT) cout << "Ci erase [it,end]" << endl;
										Ci->listPoints.erase(it_proj, Ci->listPoints.end());

										it_proj = Ci->listPoints.begin();
										advance(it_proj, CEPs[1]._index);
										if(COMMENT) cout << "Ci erase [begin,it]" << endl;
										Ci->listPoints.erase(Ci->listPoints.begin(), it_proj);

										if(Ci->listPoints.empty()) //verificando se todos os pontos foram eliminados.
										{
											Ci = stretchBoundary.erase(Ci);
											erase_Ci = true;
											if(COMMENT) cout << "Ci eliminada por estar vazia" << endl;
										}
									}
									else													//pontos de corte pertence a curva Bj.
									{
										it_proj = Bj->listPoints.begin();
										advance(it_proj, CEPs[0]._index);
										if(COMMENT) cout << "Bj erase [it,end]" << endl;
										Bj->listPoints.erase(it_proj, Bj->listPoints.end());

										it_proj = Bj->listPoints.begin();
										advance(it_proj, CEPs[1]._index);
										if(COMMENT) cout << "Bj erase [begin,it]" << endl;
										Bj->listPoints.erase(Bj->listPoints.begin(), it_proj);

										if(Bj->listPoints.empty())
										{
											Bj = stretchInternalCurves.erase(Bj);
											erase_Bj = true;
											if(COMMENT) cout << "Bj eliminada por estar vazia" << endl;
										}
									}
									finished = true;
								}
							}
							//CASO 2 - Fim -----------------------------------------------------------------------------

							//CASO 3 - Inicio --------------------------------------------------------------------------
							if(!finished) //Projeções CEP's de curvas diferentes.
							{
								for(unsigned int k = 0; k < CEPs.size(); ++k) //analisando todas as projeções CEP
								{
									if(CEPs[k]._index_matrix == 0) //ponto de corte pertence a curva Ci.
									{
										it_proj = Ci->listPoints.begin();
										advance(it_proj, CEPs[k]._index);
										if(COMMENT) cout << "KxK = " << CEPs[k].KxK << endl;
										if(CEPs[k].KxK > 0)
										{
											if(COMMENT) cout << "Ci erase [it,end]" << endl;
											Ci->listPoints.erase(it_proj, Ci->listPoints.end());
										}
										else
										{
											if(COMMENT) cout << "Ci erase [begin,it]" << endl;
											Ci->listPoints.erase(Ci->listPoints.begin(), it_proj);
										}

										if(Ci->listPoints.empty()) //verificando se todos os pontos foram eliminados.
										{
											Ci = stretchBoundary.erase(Ci);
											erase_Ci = true;
											if(COMMENT) cout << "Ci eliminada por estar vazia" << endl;
										}
									}
									else													 //ponto de corte pertence a curva Bj.
									{
										it_proj = Bj->listPoints.begin();
										advance(it_proj, CEPs[k]._index);
										if(COMMENT) cout << "KxK = " << CEPs[k].KxK << endl;
										if(CEPs[k].KxK > 0)
										{
											if(COMMENT) cout << "Bj erase [it,end]" << endl;
											Bj->listPoints.erase(it_proj, Bj->listPoints.end());
										}
										else
										{
											if(COMMENT) cout << "Bj erase [begin,it]" << endl;
											Bj->listPoints.erase(Bj->listPoints.begin(), it_proj);
										}

										if(Bj->listPoints.empty())
										{
											Bj = stretchInternalCurves.erase(Bj);
											erase_Bj = true;
											if(COMMENT) cout << "Bj eliminada por estar vazia" << endl;
										}
									}
								}
							}
							//CASO 3 - Fim -----------------------------------------------------------------------------
						}
					}
				}
				//coisas aqui
//				if(!erase_Ci) ++Ci;
				if(!erase_Bj)	++Bj;
			}
		}
		if(!erase_Ci) ++Ci;
	}
}*/

/*
 * Remove da matriz curvas com tamanho menor ou igual que 'size'.
 */
void Normal::removeNoiseCurves(list<Boundary> &matrix, unsigned int size)
{
	for(list<Boundary>::iterator it = matrix.begin(); it != matrix.end(); ++it)
	{
		if(it->size() <= size) it = matrix.erase(it);
	}
}

vector<point_tuple> Normal::getSampleCurve(list<Point2D>::iterator begin,
																			     list<Point2D>::iterator end,
																			     int i, int count_k)
{
	vector<point_tuple> pts;

	for(int k = 0; begin != end; *begin++, ++k)
	{
		Point2D p = *begin;
		Point3D p_temp = this->normalToBound[i][0][k]; //j = 0 porque trabalha-se aqui apenas com o bordo.
		Point3D n1(p_temp.x, p_temp.y, 0);

		if(k+1 < count_k)  p_temp = this->normalToBound[i][0][k+1]; //j = 0 porque trabalha-se aqui apenas com o bordo.
		else							 p_temp = this->normalToBound[i][0][0];

		Point3D n2(p_temp.x, p_temp.y, 0);

//		cout << p << " Curvatura = " << UtilityFunctions::Curvature(n1, n2) << endl;

		if(UtilityFunctions::Curvature(n1, n2) > CURVATURE)
		{
			point_tuple pt;
			pt._index = k;  pt._point = p;
			pts.push_back(pt);
		}
	}

//	if(pts.size() > 0) pts.push_back(pts[0]);//TODO novo: comentei

	return pts;
}

/*
 * Busca na curva 'curve' por o ponto que pertence a mesma reta que passa por 'p' e 'p2'.
 */
/*point_tuple Normal::findingProjectionPoint(Point2D p, list<Boundary>::iterator curve, Point2D p2)
{
	point_tuple pt;
	Point2D p_found;
	int i = 0;
	bool found = false;
	bool itsCEP = false;
	list<Point2D>::iterator Pi = curve->begin();
	long int p_value = UtilityFunctions::coefficientOfTheVectorProduct_K(UtilityFunctions::getVector(p,*Pi),
																																			 UtilityFunctions::getVector(p,p2));

	for(++Pi; Pi != curve->end() && !itsCEP; ++Pi, ++i)
	{
		long int value = UtilityFunctions::coefficientOfTheVectorProduct_K(UtilityFunctions::getVector(p,*Pi),
																																			 UtilityFunctions::getVector(p,p2));
		if(p_value * value <= 0)
		{
			if(!its_ECP(p2, p, *Pi)) //verificando se a projeção é válida, ou seja, se o ponto central não está entre o seu extremo e a sua projeção.
			{
				found = true;
				if(its_CEP(p, *Pi, p2))	itsCEP = true; //verificando se a projeção é CEP
				p_found = *Pi;
			}
		}
//		cout << "K' * K = " << p_value * value << endl;
		p_value = value;
	}
//	pt.init(i, -1, *Pi);
	pt.init(i, -1, p_found);
	if(found)  pt.itsProjection = true; //marcando o ponto de projeção encontrado como válido.
	else			 pt.itsProjection = false;

	pt.itsCEP = itsCEP;

	return pt;
}*/

/*
 * Busca na curva 'curve' por o ponto que pertence a mesma reta que passa por 'p' e 'p2'.
 */
point_tuple Normal::findingProjectionPoint(Point2D p, list<Point2D> curve, Point2D p2)
{
	{
		point_tuple pt;
		Point2D p_found;
		int i = 0;
		bool found = false;
		bool itsCEP = false;
		list<Point2D>::iterator Pi = curve.begin();
		long int p_value = UtilityFunctions::coefficientOfTheVectorProduct_K(UtilityFunctions::getVector(p,*Pi),
																																				 UtilityFunctions::getVector(p,p2));

		for(++Pi; Pi != curve.end() && !itsCEP; ++Pi, ++i)
		{
			long int value = UtilityFunctions::coefficientOfTheVectorProduct_K(UtilityFunctions::getVector(p,*Pi),
																																				 UtilityFunctions::getVector(p,p2));
			if(p_value * value <= 0)
			{
				if(!its_ECP(p2, p, *Pi)) //verificando se a projeção é válida, ou seja, se o ponto central não está entre o seu extremo e a sua projeção.
				{
					found = true;
					if(its_CEP(p, *Pi, p2))	itsCEP = true; //verificando se a projeção é CEP
					p_found = *Pi;
				}
			}
	//		cout << "K' * K = " << p_value * value << endl;
			p_value = value;
		}
	//	pt.init(i, -1, *Pi);
		pt.init(i, -1, p_found);
		if(found)  pt.itsProjection = true; //marcando o ponto de projeção encontrado como válido.
		else			 pt.itsProjection = false;

		pt.itsCEP = itsCEP;

		return pt;
	}
}

/** A 02
 */
/*int Normal::getValidExtreme(Point2D extreme, list<Boundary>::iterator curve, Point2D central)
{
	int i = 0;
//	int i = indexStart;
	int index = curve->size() - 1;
	bool found = false;
	list<Point2D>::iterator	Pi = curve->begin();
//	advance(Pi, indexStart);
	++Pi; ++i;
	long int p_value = UtilityFunctions::coefficientOfTheVectorProduct_K(UtilityFunctions::getVector(central,*Pi),
																																			 UtilityFunctions::getVector(central,extreme));
	for(++Pi; Pi != curve->end() && !found; ++Pi, ++i)
	{
		long int value = UtilityFunctions::coefficientOfTheVectorProduct_K(UtilityFunctions::getVector(central,*Pi),
																																			 UtilityFunctions::getVector(central,extreme));

		if(p_value * value <= 0 && its_ECP(extreme, central, *Pi))
		{
			index = i;
			found = true;
		}

		p_value = value;
	}

	if(!found)	index = -1;

	return index;
}*/

/*int Normal::getAutoProjection(Point2D projection, list<Boundary>::iterator curve, Point2D central)
{
	int i = 0;
	int index = curve->size() - 1;
	bool found = false;
	list<Point2D>::iterator	Pi = curve->begin();
	++Pi; ++i;

	long int p_value = UtilityFunctions::coefficientOfTheVectorProduct_K(UtilityFunctions::getVector(central,*Pi),
																																			 UtilityFunctions::getVector(central,projection));
	for(++Pi; Pi != curve->end() && !found; ++Pi, ++i)
	{
		long int value = UtilityFunctions::coefficientOfTheVectorProduct_K(UtilityFunctions::getVector(central,*Pi),
																																			 UtilityFunctions::getVector(central,projection));

		if(p_value * value <= 0 && its_ECP(projection, central, *Pi))
		{
			index = i;
			found = true;
		}

		p_value = value;
	}

	if(!found)	index = -1; //sentando index como -1 terei como saber se alguma projeção foi achada na chamada do método.x

	return index;
}*/

/** A
 * Procura na curva 'curve' por uma projeção E.P.C., sendo E = 'extreme' e C = 'central'.
 * Retorna a posição do ponto na lista.
 */
int Normal::getAutoProjection(Point2D projection, list<Point2D> curve, Point2D central)
{
	int i = 0;
	int index = curve.size() - 1;
	bool found = false;
	list<Point2D>::iterator	Pi = curve.begin();
	++Pi; ++i;

	long int p_value = UtilityFunctions::coefficientOfTheVectorProduct_K(UtilityFunctions::getVector(central,*Pi),
																																			 UtilityFunctions::getVector(central,projection));
	for(++Pi; Pi != curve.end() && !found; ++Pi, ++i)
	{
		long int value = UtilityFunctions::coefficientOfTheVectorProduct_K(UtilityFunctions::getVector(central,*Pi),
																																			 UtilityFunctions::getVector(central,projection));

		if(p_value * value <= 0 && its_ECP(projection, central, *Pi))
		{
			index = i;
			found = true;
		}

		p_value = value;
	}

	if(!found)	index = -1; //sentando index como -1 terei como saber se alguma projeção foi achada na chamada do método.x

	return index;
}

/** A 01
 * Procura na curva 'curve' por uma projeção E.P.C., sendo E = 'extreme' e C = 'central'.
 * Retorna a posição do ponto na lista.
 */
/*int Normal::getValidExtreme(Point2D extreme, list<Boundary>::iterator curve, Point2D central)
{
	int i = 0;
	int index = 0;
	bool found = false;
	list<Point2D>::iterator	Pi = curve->begin();
	++Pi; ++i;
	long int p_value = UtilityFunctions::coefficientOfTheVectorProduct_K(UtilityFunctions::getVector(central,*Pi),
																																			 UtilityFunctions::getVector(central,extreme));
	for(++Pi; Pi != curve->end() && !found; ++Pi, ++i)
	{
		long int value = UtilityFunctions::coefficientOfTheVectorProduct_K(UtilityFunctions::getVector(central,*Pi),
																																			 UtilityFunctions::getVector(central,extreme));

		if(p_value * value <= 0 && its_EPC(extreme, *Pi, central))
		{
			found = true;
			index = i;
			p_value = value;
		}

//		p_value = value;
	}

	return index;
}*/

/** A
 * Procura na curva 'curve' por uma projeção E.P.C., sendo E = 'extreme' e C = 'central'.
 * Retorna a posição do ponto na lista.
 */
/*int Normal::getValidExtreme(Point2D extreme, list<Boundary>::iterator curve, Point2D central)
{
	int i = 0;
	int index = 0;
//	vector<point_tuple> EPCs;
	double minDistance;
//	bool found = false;
	list<Point2D>::iterator	Pi = curve->begin();
	++Pi;

	minDistance = std::numeric_limits<int>::max();//MaxInt
	long int p_value = UtilityFunctions::coefficientOfTheVectorProduct_K(UtilityFunctions::getVector(central,*Pi),
																																			 UtilityFunctions::getVector(central,extreme));

//	for(++Pi; Pi != curve->end() && !found; ++Pi, ++i)
	for(++Pi; Pi != curve->end(); ++Pi, ++i)
	{
		long int value = UtilityFunctions::coefficientOfTheVectorProduct_K(UtilityFunctions::getVector(central,*Pi),
																																			 UtilityFunctions::getVector(central,extreme));

		if(p_value * value <= 0 && its_EPC(extreme, *Pi, central))
		{
//			point_tuple pt;
//			found = true;
//			pt._index = i;
//			pt._point = *Pi;
//			EPCs.push_back(pt);
//			extreme = *Pi;
			double tempDistance = UtilityFunctions::euclideanDistance(central, *Pi);
			if(tempDistance < minDistance)
			{
				index = i;
				minDistance = tempDistance;
				p_value = value;
			}
			extreme = *Pi;
		}

//		p_value = value;
	}

//	vector<double> distances;
//	for(unsigned int k = 0; k < EPCs.size(); ++k)	distances.push_back(UtilityFunctions::euclideanDistance(central, EPCs[k]._point));
//
//	if(!distances.empty()) { index = EPCs[UtilityFunctions::min(distances)]._index; cout << "min=" << index << " EPCs = " << EPCs.size() << endl; }
//	else {cout << "ñ" << endl;}

	return index;
}*/

/*
 * Verifica se o ponto 'projection' é um ponto de corte C.E.P..
 */
bool Normal::its_CEP(Point2D central, Point2D projection, Point2D extreme)
{
	bool sentence_1 = UtilityFunctions::euclideanDistance(central, extreme) <
									  UtilityFunctions::euclideanDistance(central, projection);

	bool sentence_2 = UtilityFunctions::euclideanDistance(extreme, projection) <
										UtilityFunctions::euclideanDistance(central, projection);

	return sentence_1 && sentence_2;
}

/*
 * Verifica se o ponto 'projection' é um ponto E.C.P..
 */
bool Normal::its_ECP(Point2D extreme, Point2D central, Point2D projection)
{
	bool sentence_1 = UtilityFunctions::euclideanDistance(extreme, central) <
									  UtilityFunctions::euclideanDistance(extreme, projection);

	bool sentence_2 = UtilityFunctions::euclideanDistance(central, projection) <
										UtilityFunctions::euclideanDistance(extreme, projection);

	return sentence_1 && sentence_2;
}

/*
 * Verifica se o ponto 'projection' é um ponto E.P.C..
 */
bool Normal::its_EPC(Point2D extreme, Point2D projection, Point2D central)
{
	bool sentence_1 = UtilityFunctions::euclideanDistance(extreme, projection) <
									  UtilityFunctions::euclideanDistance(extreme, central);

	bool sentence_2 = UtilityFunctions::euclideanDistance(projection, central) <
										UtilityFunctions::euclideanDistance(extreme, central);

	return sentence_1 && sentence_2;
}

/*
 * Verifica se dados dois pontos, os mesmos são da mesma curva.
 */
bool Normal::pointsOfTheSameCurve(vector<point_tuple> pts)
{
	bool sentence = false;

	if(pts.size() == 2)	sentence = pts[0]._index_matrix == pts[1]._index_matrix;
	else if(pts.size() == 3) sentence = true;

	return sentence;
}

//void Normal::extractBoundaryNormals_new(list<Curve> curves)
void Normal::extractBoundaryNormals_new(list<Curves> visibleCurve)
{
	Point3D n;

	list<Point2D>::iterator iter3, iter4;
	int k, size;

	normalArray		na;
	normalToRegion	nr;

	regionNormalToBound.clear();

	list<Curve> visibleCurveSegments = visibleCurve.front();
	list<Curve> visibleInternalCurveSegments = visibleCurve.back();
	visibleCurveSegments.splice(visibleCurveSegments.end(), visibleInternalCurveSegments); //juntando os segmentos visíveis do bordo com as curvas internas a região.

	//Para cada segmento de curva
	for(list<Curve>::iterator it_curve = visibleCurveSegments.begin(); it_curve != visibleCurveSegments.end(); ++it_curve)
	{
		iter3 = iter4 = it_curve->listPoints.begin();//curves.listPoints.begin();
		iter4++;
		size = it_curve->listPoints.size();

		Point3D first;
		first.setPoint(iter4->y-iter3->y, iter3->x-iter4->x, 0.0);
		for(k=1; k<size; k++, iter3++, iter4++)
		{
			n.setPoint(iter4->y-iter3->y, iter3->x-iter4->x, 0.0); //vetor normal ao vetor tangente ao bordo da curva.
			n.normalize();
			na.push_back(n);
		}
		if(it_curve->flagFechado==true)
		{
			na.push_back(first);
		}
		else
		{
			na.push_back(n);
		}
		nr.push_back(na);
		na.clear();
	}
//	normalToBound.push_back(nr);
	regionNormalToBound = nr;
//	regionNormalToBound.push_back(nr);
//	nr.clear();
}

void Normal::smoothBoundaryNormals_new(list<Curves> visibleCurve)
{
	list<Curve>::iterator it_curve;
	Point3D p;

	int j, k, curveSize;

	list<Curve> visibleCurveSegments = visibleCurve.front();
	list<Curve> visibleInternalCurveSegments = visibleCurve.back();
	visibleCurveSegments.splice(visibleCurveSegments.end(), visibleInternalCurveSegments); //juntando os segmentos visíveis do bordo com as curvas internas a região.

	//Para cada segmento de curva
	for(j = 0, it_curve = visibleCurveSegments.begin(); it_curve != visibleCurveSegments.end(); ++it_curve, ++j)
	{
		curveSize = it_curve->listPoints.size();//curves.listPoints.size();
		for(k = 2; k < curveSize-2; k++)
		{
			p.x = regionNormalToBound[j][k-2].x + 4*regionNormalToBound[j][k-1].x + 6*regionNormalToBound[j][k].x
					+4*regionNormalToBound[j][k+1].x + regionNormalToBound[j][k+2].x;
			p.y = regionNormalToBound[j][k-2].y + 4*regionNormalToBound[j][k-1].y + 6*regionNormalToBound[j][k].y
					+4*regionNormalToBound[j][k+1].y + regionNormalToBound[j][k+2].y;
			p.setPoint(p.x,p.y,p.z);
			p.normalize();
			//		normalToBound[i][j][k]=p;
			regionNormalToBound[j][k] = p;
		}
	}
}

normalToRegion Normal::extractInternalBoundaryNormals(Curves curves)
{
	Point3D n;
	list<Point2D>::iterator iter3, iter4;
	int k, size;

	normalArray		na;
	normalToRegion	nr;

	//Para cada segmento de curva
	for(list<Curve>::iterator it_curve = curves.begin(); it_curve != curves.end(); ++it_curve)
	{
		iter3 = iter4 = it_curve->listPoints.begin();//curves.listPoints.begin();
		iter4++;
		size = it_curve->listPoints.size();

		Point3D first;
		first.setPoint(iter4->y-iter3->y, iter3->x-iter4->x, 0.0);
		for(k=1; k<size; k++, iter3++, iter4++)
		{
			n.setPoint(iter4->y-iter3->y, iter3->x-iter4->x, 0.0); //vetor normal ao vetor tangente ao bordo da curva.
			n.normalize();
			na.push_back(n);
		}
		if(it_curve->flagFechado==true)
		{
			na.push_back(first);
		}
		else
		{
			na.push_back(n);
		}
		nr.push_back(na);
		na.clear();
	}

//	regionNormalToBound = nr;
	return nr;
}

void Normal::smoothInternalBoundaryNormals(Curves curves, normalToRegion &nr)
{
	list<Curve>::iterator it_curve;
	Point3D p;

	int j, k, curveSize;

	//Para cada segmento de curva
	for(j = 0, it_curve = curves.begin(); it_curve != curves.end(); ++it_curve, ++j)
	{
		curveSize = it_curve->listPoints.size();//curves.listPoints.size();
		for(k = 2; k < curveSize-2; k++)
		{
			p.x = nr[j][k-2].x + 4*nr[j][k-1].x + 6*nr[j][k].x
					+4*nr[j][k+1].x + nr[j][k+2].x;
			p.y = nr[j][k-2].y + 4*nr[j][k-1].y + 6*nr[j][k].y
					+4*nr[j][k+1].y + nr[j][k+2].y;
			p.setPoint(p.x,p.y,p.z);
			p.normalize();
			nr[j][k] = p;
		}
	}
}

void Normal::calculateAllInteriorNormal_new()
{
	list<Curves>::iterator listCurves = regions->listBoundary->regionCurves.begin();
	list<Interior*>::iterator iterListRegion= regions->listInterior.begin();
	list<Label>::iterator iterHierLabel = regions->hierarchy->descendentList.begin();
	int i = 0;

	//o background nao deve ser processado nesse momento
	listCurves++;
	iterListRegion++;
	iterHierLabel++;
	i++;

	normalBuffer_new.clear();
	normalBuffer_new.resize(w*h);

	while(iterListRegion != regions->listInterior.end()) //para cada região
	{
		calculateNormal_new(i, iterListRegion, iterHierLabel, listCurves);
		++listCurves;
		++iterListRegion;
		++iterHierLabel;
		++i;
	}
}

void Normal::calculateNormal_new(int i, list<Interior*>::iterator iterListRegion, list<Label>::iterator iterHierLabel, list<Curves>::iterator listCurves)
{
//	regionNormalBuffer.clear();
//	regionNormalBuffer.resize(w*h);
//	int count = 0;

	Curves visibleInternalCurveSegments;

	list<Curve>::iterator listCurve = listCurves->begin(); //iterador apontado para a curva de bordo.
	Curve boundaryCurve = *listCurve; //pegando a curva de bordo

	//colocando as curvas internas da região no vetor de curvas visíveis.
	for(++listCurve; listCurve != listCurves->end(); ++listCurve)	visibleInternalCurveSegments.push_back(*listCurve);

	samplePointsCurve = getSampleCurve(boundaryCurve.listPoints.begin(), boundaryCurve.listPoints.end(), i, boundaryCurve.listPoints.size());

	// Para cada ponto interior a região
	for(list<Point2D>::iterator  iterPointRegion = (*iterListRegion)->begin(); iterPointRegion!=(*iterListRegion)->end(); iterPointRegion++)
	{
		//poderia ser usado com as variaveis fora
		vector<double> di;
		vector<double> wi;


		double Npx,Npy,Npz;Npx=Npy=Npz = 0;
		double sumDi=0;

		//------------- Atualizando temporariamente regions->listBoundary->regionCurves -------------
		Curves visibleBoundarySegments;
		list<Curves> visibleCurve;

		//pegando apenas os pontos de BORDO visíveis por 'iterPointRegion'
		visibleBoundarySegments = setBoundarySegmentVisible(*iterPointRegion, boundaryCurve.listPoints, i, boundaryCurve.flagFechado);

		visibleCurve.push_back(visibleBoundarySegments);			//0 - trechos visíveis do bordo.
		visibleCurve.push_back(visibleInternalCurveSegments); //1 - curvas internas.

		// ----------------------- FIM da Atualização -----------------------

		extractBoundaryNormals_new(visibleCurve);
		smoothBoundaryNormals_new(visibleCurve);

		calculateDistance_new((*iterPointRegion),visibleCurve,regions->listBoundary,sumDi,&di,iterHierLabel);//calculateDistance_new((*iterPointRegion),visibleCurve,regions->listBoundary,sumDi,&di,iterHierLabel);
		calculateWi(&di,sumDi,&wi); // nao precisa analisar a lista de curva aqui
		calculateNpxNpy_new((*iterPointRegion),&Npx,&Npy,visibleCurve,regions->listBoundary,&wi,iterHierLabel,i);
		Npz = sqrt(1 - Npx*Npx - Npy*Npy);

		normalBuffer_new[index(iterPointRegion->x, iterPointRegion->y,w)].x = Npx;
		normalBuffer_new[index(iterPointRegion->x, iterPointRegion->y,w)].y = Npy;
		normalBuffer_new[index(iterPointRegion->x, iterPointRegion->y,w)].z = Npz;
	}
}

void Normal::calculateDistance_new(Point2D p,list<Curves> visibleCurve,Vectorization *v,double &sumDi,vector<double> *di,list<Label>::iterator iterLabel)
{
	list<Curve> visibleCurveSegments = visibleCurve.front();
	list<Curve> visibleInternalCurveSegments = visibleCurve.back();
	visibleCurveSegments.splice(visibleCurveSegments.end(), visibleInternalCurveSegments); //juntando os segmentos visíveis do bordo com as curvas internas a região.

	//para calcular com a curva de bordo e os rabiscos da regiao
	for(list<Curve>::iterator iterCurve=visibleCurveSegments.begin(); iterCurve != visibleCurveSegments.end(); iterCurve++)
	{
		updateDistance(p,iterCurve,sumDi,di);
	}
	//para calcular usando a estruta de hierarquia dos filhos descendentes de primeiro grau. Para cada filho...
	for(list<int>::iterator iterInteger = iterLabel->begin();iterInteger!=iterLabel->end();iterInteger++){
		if((*iterInteger)!=-1){
			//updateDistanceUsingHier(p,v,sumDi,di,(*iterInteger));
			list<Curves>::iterator iterCurvesAux = regions->listBoundary->regionCurves.begin();
			for(int i=0;i<(*iterInteger);i++){
				iterCurvesAux++;
			}

			list<Curve>::iterator iterCurveAux = iterCurvesAux->begin();
			//------------- atualizanado curvas com os pontos visíveis--------------
			Curve boundaryCurve = *iterCurveAux; //pegando a curva interna de bordo.
			samplePointsInternalCurve = getSampleCurve(boundaryCurve.listPoints.begin(), boundaryCurve.listPoints.end(), *iterInteger, boundaryCurve.listPoints.size());

			//pegando apenas os pontos de BORDO da região interna visíveis por 'p'
			Curves visibleBoundarySegments = setBoundarySegmentVisible(p, boundaryCurve.listPoints, *iterInteger, boundaryCurve.flagFechado, true);
			//----------------------- fim da atualização ---------------------------

			for(list<Curve>::iterator iterCurve = visibleBoundarySegments.begin(); iterCurve != visibleBoundarySegments.end(); ++iterCurve)
			{
				//updateDistance(p,iterCurveAux,sumDi,di);
				double distance=0;
				for(list<Point2D>::iterator iterPoint = iterCurve->listPoints.begin(); iterPoint!=iterCurve->listPoints.end();iterPoint++)
				{
					distance  = (double)1.0/((p.x - iterPoint->x)*(p.x - iterPoint->x) + (p.y - iterPoint->y)*(p.y - iterPoint->y));
					sumDi = sumDi + distance;
					di->push_back(distance);
				}
			}
		}
	}
}

/** A
 * Método que calcula as componentes x e y do vetor normal 3D usando apenas a influência das normais da curva de bordo.
 */
void Normal::calculateNpxNpy_2(Point2D p,double *Npx,double *Npy,list<Curves>::iterator iterCurves,Vectorization *vet,vector<double> *wi,list<Label>::iterator iterLabel,unsigned int i)
{
	vector<double>::iterator iterWi = wi->begin();
	unsigned int j = 0;
	unsigned int k = 0;

	list<Curve>::iterator iterCurve = iterCurves->begin();

	for(list<Point2D>::iterator iterPoint= iterCurve->listPoints.begin(); iterPoint!=iterCurve->listPoints.end();iterPoint++)
	{
		*Npx = *Npx + normalToBound[i][j][k] .x*(*iterWi);
		*Npy = *Npy + normalToBound[i][j][k] .y*(*iterWi);
		k++;
		iterWi++;
	}
}

/** A
 * Método que calcula as componentes x e y do vetor normal 3D usando apenas a influência das normais VISÍVEIS da curva de bordo.
 */
void Normal::calculateNpxNpy_new(Point2D p,double *Npx,double *Npy, list<Curves> visibleCurve,Vectorization *vet,vector<double> *wi,list<Label>::iterator iterLabel,int i)
{
	vector<double>::iterator iterWi = wi->begin();
	int j = 0;
	int k;

	list<Curve> visibleBoundarySegments = visibleCurve.front();
	list<Curve> visibleInternalCurveSegments = visibleCurve.back();

	//para cada segmento da curva de borda
	for(list<Curve>::iterator it_curve = visibleBoundarySegments.begin(); it_curve != visibleBoundarySegments.end(); ++it_curve)
	{
		k = 0;
		//Para cada ponto da curva
		for(list<Point2D>::iterator iterPoint = it_curve->listPoints.begin(); iterPoint != it_curve->listPoints.end(); ++iterPoint)
		{
			*Npx = *Npx + regionNormalToBound[j][k] .x*(*iterWi);//normalToBound[i][j][k] .x*(*iterWi);
			*Npy = *Npy + regionNormalToBound[j][k] .y*(*iterWi);//normalToBound[i][j][k] .y*(*iterWi);
			k++;
			iterWi++;
		}
		j++;
	}

	//para cada curva interna
	for(list<Curve>::iterator it_curve = visibleInternalCurveSegments.begin(); it_curve != visibleInternalCurveSegments.end(); ++it_curve)
	{
		k=0;
		for(list<Point2D>::iterator iterPoint= it_curve->listPoints.begin(); iterPoint != it_curve->listPoints.end(); iterPoint++)
		{
			if(((p.x-(*iterPoint).x)* regionNormalToBound[j][k].x  + (p.y-(*iterPoint).y)*regionNormalToBound[j][k].y) > 0)
			{
				*Npx = *Npx - regionNormalToBound[j][k] .x*(*iterWi);//normalToBound[i][j][k] .x*(*iterWi);
				*Npy = *Npy - regionNormalToBound[j][k] .y*(*iterWi);//normalToBound[i][j][k] .y*(*iterWi);
				iterWi++;
			}
			else
			{
				*Npx = *Npx + regionNormalToBound[j][k] .x*(*iterWi);//normalToBound[i][j][k] .x*(*iterWi);
				*Npy = *Npy + regionNormalToBound[j][k] .y*(*iterWi);//normalToBound[i][j][k] .y*(*iterWi);
				iterWi++;
			}
			k++;
		}
		++j;
	}

	//calculo da interpolacao com os filhos
	unsigned int a,b,c ;
//	a = b = c = 0;
	for(list<int>::iterator iterInteger = iterLabel->begin();iterInteger!=iterLabel->end();iterInteger++)
	{
		if((*iterInteger)!=-1)
		{
			a = 0;
			list<Curves>::iterator iterCurves=vet->regionCurves.begin();
			for(int i=0;i<(*iterInteger);i++)
			{
				iterCurves++;
				a++;
			}
			list<Curve>::iterator iterCurveAux=iterCurves->begin();
			//------------- atualizanado curvas com os pontos visíveis--------------
			Curve boundaryCurve = *iterCurveAux; //pegando a curva interna de bordo.
			samplePointsInternalCurve = getSampleCurve(boundaryCurve.listPoints.begin(), boundaryCurve.listPoints.end(), *iterInteger, boundaryCurve.listPoints.size());

			//pegando apenas os pontos de BORDO da região interna visíveis por 'p'
			Curves visibleBoundarySegments = setBoundarySegmentVisible(p, boundaryCurve.listPoints, *iterInteger, boundaryCurve.flagFechado, true);

			normalToRegion internalRegionNormalToBound = extractInternalBoundaryNormals(visibleBoundarySegments);
			smoothInternalBoundaryNormals(visibleBoundarySegments, internalRegionNormalToBound);
			//----------------------- fim da atualização ---------------------------

			//updateNpxNpy(p,Npx,Npy,iterCurveAux,iterWi);
			b = 0;
			for(list<Curve>::iterator iterCurve = visibleBoundarySegments.begin(); iterCurve != visibleBoundarySegments.end(); ++iterCurve)
			{
				c = 0;
				for(list<Point2D>::iterator iterPoint= iterCurve->listPoints.begin(); iterPoint!=iterCurve->listPoints.end();iterPoint++)
				{
					*Npx = *Npx - internalRegionNormalToBound[b][c].x*(*iterWi);//normalToBound[a][b][c] .x*(*iterWi);
					*Npy = *Npy - internalRegionNormalToBound[b][c].y*(*iterWi);//normalToBound[a][b][c] .y*(*iterWi);
					iterWi++;
					c++;
				}
				++b;
			}
		}
	}
}

/*void Normal::updateDistance_new(Point2D p,Curve curve,double &sumDi,vector<double> *di){
	double distance=0;
	for(list<Point2D>::iterator iterPoint=curve.listPoints.begin(); iterPoint!=curve.listPoints.end();iterPoint++){
		distance  = (double)1.0/((p.x - iterPoint->x)*(p.x - iterPoint->x) + (p.y - iterPoint->y)*(p.y - iterPoint->y));
		sumDi = sumDi + distance;
		di->push_back(distance);
	}
}*/

/** A
 * Método que desenha o mapa de normais de apenas uma região.
 */
/*void Normal::drawInteriorNormalMap_2()
{
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(1);
	glBegin(GL_POINTS);

	list<Curves>::iterator iterV = regions->listBoundary->regionCurves.begin();
	advance(iterV, this->i);
	list<Curve>::iterator iterC = iterV->begin();
	while(iterC != iterV->end())
	{
		list<Point2D>::iterator iterP = iterC->listPoints.begin();
		Point3D p;
		while(iterP != iterC->listPoints.end())
		{
			int ind = regions->index(iterP->x, iterP->y);
			p.x = (normalBuffer[ind].x + 1)/2;
			p.y = (normalBuffer[ind].y + 1)/2;
			p.z = 0.5;

			glColor3f(p.x,p.y,p.z);
			glVertex2i((*iterP).x,(*iterP).y);
			iterP++;
		}iterC++;
	}iterV++;

	list<Interior*>::iterator iterR = regions->listInterior.begin();
//	iterR++;
	advance(iterR, this->i);

	list<Point2D>::iterator iterP = (*iterR)->begin();
	Point3D p;
	while(iterP != (*iterR)->end())
	{
		int ind = regions->index(iterP->x, iterP->y);
		p.x = (normalBuffer[ind].x + 1)/2;
		p.y = (normalBuffer[ind].y + 1)/2;
		p.z = (normalBuffer[ind].z + 1)/2;
		glColor3f(p.x,p.y,p.z);
		glVertex2i((*iterP).x,(*iterP).y);
		iterP++;
	}//iterR++;

	glEnd();
}*/

void Normal::drawInteriorNormalMap_new()
{
	list<Curves>::iterator iterV = regions->listBoundary->regionCurves.begin();
	++iterV;

	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POINTS);
	glPointSize(1);

	while(iterV != regions->listBoundary->regionCurves.end())
	{
		list<Curve>::iterator iterC = iterV->begin();
		while(iterC != iterV->end())
		{
			list<Point2D>::iterator iterP = iterC->listPoints.begin();
			Point3D p;
			while(iterP != iterC->listPoints.end())
			{
				int ind = regions->index(iterP->x, iterP->y);
				p.x = (normalBuffer_new[ind].x + 1)/2;
				p.y = (normalBuffer_new[ind].y + 1)/2;
				p.z = 0.5;

				glColor3f(p.x,p.y,p.z);
				glVertex2i((*iterP).x,(*iterP).y);
				++iterP;
			}++iterC;
		}++iterV;
	}

	list<Interior*>::iterator iterR = regions->listInterior.begin();
	iterR++;
	while(iterR != regions->listInterior.end())
	{
		list<Point2D>::iterator iterP = (*iterR)->begin();
		Point3D p;
		int count = 0;
		//			cout << "A - " << normalBuffer.size() << endl;
		while(iterP != (*iterR)->end() && !normalBuffer_new.empty())
		{
			int ind = regions->index(iterP->x, iterP->y);
			p.x = (normalBuffer_new[ind].x + 1)/2;
			p.y = (normalBuffer_new[ind].y + 1)/2;
			p.z = (normalBuffer_new[ind].z + 1)/2;
			glColor3f(p.x,p.y,p.z);
			glVertex2i((*iterP).x,(*iterP).y);
			++iterP;
			++count;
		}++iterR;
	}
	glEnd();
}

void Normal::smoothInterior_new(double a, double b, double c)
{
	double sum=a+4*b+4*c;
	double nX, nY, nZ;
	vector<Point3D> auxBuffer;
	Point3D p;
	int lenght=w*h;
	auxBuffer.resize(lenght,p);
	for(int k=0; k<lenght; k++)
	{
		auxBuffer[k] = normalBuffer_new[k];//normalBuffer[i];
		// cout<<auxBuffer[i]<<"->"<<normalBuffer[i]<<endl;
	}
	list<Interior*>::iterator iterListRegion= regions->listInterior.begin();
	++iterListRegion; //pulando o background ja que o mesmo nao foi calculado
//	advance(iterListRegion, this->i);

	for(; iterListRegion != regions->listInterior.end(); ++iterListRegion)
	{
		list<Point2D>::iterator  iterPointRegion = (*iterListRegion)->begin();
		for(; iterPointRegion!=(*iterListRegion)->end(); iterPointRegion++)
		{
			int i = index(iterPointRegion->x, iterPointRegion->y, w);
			nX=(a*auxBuffer[i].x + b*auxBuffer[i+w].x + c*auxBuffer[i+1+w].x + b*auxBuffer[i+1].x + c*auxBuffer[i+1-w].x + b*auxBuffer[i-w].x + c*auxBuffer[i-1-w].x + b*auxBuffer[i-1].x + c*auxBuffer[i-1+w].x)/sum;
			nY=(a*auxBuffer[i].y + b*auxBuffer[i+w].y + c*auxBuffer[i+1+w].y + b*auxBuffer[i+1].y + c*auxBuffer[i+1-w].y + b*auxBuffer[i-w].y + c*auxBuffer[i-1-w].y + b*auxBuffer[i-1].y + c*auxBuffer[i-1+w].y)/sum;
			nZ=(a*auxBuffer[i].z + b*auxBuffer[i+w].z + c*auxBuffer[i+1+w].z + b*auxBuffer[i+1].z + c*auxBuffer[i+1-w].z + b*auxBuffer[i-w].z + c*auxBuffer[i-1-w].z + b*auxBuffer[i-1].z + c*auxBuffer[i-1+w].z)/sum;
			p.setPoint(nX,nY,nZ);
			p.normalize();

			normalBuffer_new[i] = p;
		}
	}
	auxBuffer.clear();
}

Normal::~Normal(){
	normalBuffer.clear();
}
