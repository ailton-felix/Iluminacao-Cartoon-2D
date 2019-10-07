#include "../headers/HierarchyRegions.hpp"

HierarchyRegions::HierarchyRegions(list<Interior*> *listInt)
{
	listInterior = listInt;
}

void HierarchyRegions::buildDescendentList(){
	list<Label>::iterator iter1;
	list<int>::iterator iter2;
	Label l;
	for(unsigned int i=0; i<ascendentList.size();i++){
		iter1=ascendentList.begin();
		for(unsigned int j=0;j<ascendentList.size();j++, iter1++){
			iter2=(*iter1).begin();
			for(unsigned int k=0;k<(*iter1).size(); k++, iter2++){
				if((*iter2)==(int)i){
					l.push_back(j);
				}
			}
		}
		if(l.size()==0){
			l.push_back(-1);
		}
		descendentList.push_back(l);
		l.clear();
	}
}

bool HierarchyRegions::thereAreIntersection(list<Point2D>::iterator iterFirst,list<Point2D>::iterator iterLast/*,int flag*/,Curve *c){
	list<Point2D>::iterator iterF;
	list<Point2D>::iterator iterL;
	
	list<Point2D>::iterator verifyBegin = c->listPoints.begin();
	list<Point2D>::iterator verifyEnd   = c->listPoints.end(); verifyEnd--;
	verifyEnd--;
	
	iterF = iterFirst;
	iterL = iterLast;
	
	if(iterF==verifyBegin){
		iterF = verifyEnd;
	}else{
		iterF--;
	}
	if(iterL==verifyEnd){
		iterL = verifyBegin;
	}else{
		iterL++;
	}
	if(iterF->y==iterL->y){ 
		return false;
	}else{
		return true;
	}
}
Point2D HierarchyRegions::choosePointOfRegion(Interior *r){
    list<Point2D>::iterator  iterPoint = r->begin();
	return (*iterPoint);
}

int HierarchyRegions::numIntersections(Point2D*p,Curve *c){
	 int num,flagTangent; 
	 num = flagTangent = 0;
	 
	 list<Point2D>::iterator iteratorPoint = c->listPoints.begin();
	 list<Point2D>::iterator iteratorAux = c->listPoints.end(); iteratorAux--; //problem to resolv

	 list<Point2D>::iterator iteratorFirstPoint;
	 list<Point2D>::iterator iteratorLastPoint;

	 for(unsigned int i=0;i<c->listPoints.size();i++){ 
		 if((*iteratorPoint).x>p->x){
			 if((*iteratorPoint).y==p->y){
				 if(flagTangent==0){
					 flagTangent=1;
					 iteratorFirstPoint = iteratorPoint; //store the pointer to the first point at tangency
				 }
			 }else{
				 if(flagTangent==1){
					 flagTangent=0;
					 iteratorLastPoint = iteratorAux;
					 if(thereAreIntersection(iteratorFirstPoint,iteratorLastPoint,c)){
						 num++;
					 }
				 }
			 }
		 }
		 iteratorAux = iteratorPoint;
		 iteratorPoint++;
	 }
	 
	 return num;
}



/*This method verifies if one region is inside of another.
 *the arguments of method are pointers. 
 **/
bool HierarchyRegions::isInsideRegion(list<Interior*>::iterator Ri, list<Curve>::iterator curvesRegion){
	Point2D  pixel;
	pixel = choosePointOfRegion((*Ri));
	if((numIntersections(&pixel,&(*curvesRegion)))%2!=0)
	    return true;
	else
	    return false;
}


void HierarchyRegions::detectHierarchyRegions(Vectorization *setCurve){
	Label rotulo;
	
	list<Interior*>::iterator regionIterator = listInterior->begin();
	list<Curves>::iterator curvesIterator = setCurve->regionCurves.begin();

	list<Curve>::iterator  curveIterator = curvesIterator->begin();
	/*this iterator was incremented because the first region is background's image
	 *and the curves list don't store this region.	
	 **/   
	
	for(unsigned int i=0;i<listInterior->size();i++){
		for(unsigned int j=0;j<setCurve->regionCurves.size();j++){
			if(i!=j){
				if(isInsideRegion(regionIterator,curveIterator)){
					rotulo.push_back(j);
				}
			}
			curvesIterator++;
			curveIterator = curvesIterator->begin();
		}
		ascendentList.push_back(rotulo);
		rotulo.clear();	
		curvesIterator = setCurve->regionCurves.begin();
        curveIterator = curvesIterator->begin();
		regionIterator++;
	}
	buildDescendentList();
}

void HierarchyRegions::writeAscendentList(){
	list<Label>::iterator iterLabel = ascendentList.begin();
	list<int>::iterator iterInteger = iterLabel->begin();
	
	for(unsigned int i=0;i<ascendentList.size();i++){
		cout << "Regiao["<<i<<"]->";
		iterInteger = iterLabel->begin();
		
		if(!iterLabel->empty()){
			for(unsigned int j=0;j<iterLabel->size();j++){
				cout << (*iterInteger)<<",";
				iterInteger++;
			}
			cout << endl;
			iterLabel++;
		}else{
			cout << "sem pai"<<endl;
			iterLabel++;
		}
	}
}

void HierarchyRegions::writeDescendentList(){
	list<Label>::iterator iterLabel = descendentList.begin();
	list<int>::iterator iterInteger = iterLabel->begin();

	for(unsigned int i=0;i<descendentList.size();i++){
		cout << "Regiao["<<i<<"]->";
		iterInteger = iterLabel->begin();
		if(!iterLabel->empty()){
			for(unsigned int j=0;j<iterLabel->size();j++){
				cout <<"["<< (*iterInteger)<<"]";
				iterInteger++;
			}
			cout << endl;
			iterLabel++;
		}
	}
}
bool HierarchyRegions::findPoint(unsigned int position,int label){
	list<Label>::iterator auxList = descendentList.begin();
	list<int>::iterator auxPoint;
	
	for(unsigned int i=0;i<descendentList.size();i++){
		auxPoint = auxList->begin();
		if(i!=position && ((*auxPoint)!=-1)){
			for(unsigned int j=0;j<auxList->size();j++){				
				if(label==(*auxPoint)){
					//cout <<"Ponto = " << (*auxPoint)<<endl;
					return true;
				}
				auxPoint++;
			}
		}
		auxList++;	
	}
	return false;
}


void HierarchyRegions::buildPrimaryDescendentList(){
	list<Label>::iterator currentList = descendentList.begin();
	list<int>::iterator currentPoint;
	
	for(unsigned int i=0;i<descendentList.size();i++){
		currentPoint = currentList->begin();
		if(currentList->size()!=1){
			for(unsigned int j=0;j<currentList->size();j++){
				if(findPoint(i,(*currentPoint))){
					currentPoint = currentList->erase(currentPoint);
					j--;
				}else{
					currentPoint++;
				}
			}
		}
		currentList++;
	}
}


HierarchyRegions::~HierarchyRegions()
{	
	ascendentList.clear();
	descendentList.clear();	
}
