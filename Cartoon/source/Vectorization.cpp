#include "../headers/Image.hpp"
#include "../headers/Vectorization.hpp"

Vectorization::Vectorization(){
}

Vectorization::Vectorization(list<Boundary*> *listScatBound, int wid, int hei ){
	scatteredBoundary = listScatBound;
	this->w=wid;
	this->h=hei;
	
	matrixBuffer = new char[w*h];
}

/*** Chain Code 
 *   counter-clockwise
 *    3 | 2 | 1
 *    4 | P | 0
 *    5 | 6 | 7
 * 
 ***/ 
 /*** MatrixBuffer
 *   the matrixBuffer is with: 
 *    0 - where pixel == false
 *    1 - where pixel == true
 ***/ 


//this function initialize the matrixBuffer (0 - pixel == false   /   1 - pixel == true)
void Vectorization::initializeMatrix(int listPosition){     	       
	int size = w*h;
	for(int i = 0; i < size; i++){ 
		matrixBuffer[i] = '0';
	}
	list<Boundary*>::iterator iterB = scatteredBoundary->begin();
	iterB++; 
	if(scatteredBoundary->size() > 1 && listPosition > 1){ 
		unsigned int i = 1;
		while(i != (unsigned)listPosition){
			iterB++;
			i++;
		}	
	}
	list<Point2D>::iterator iterP;
	Point2D p;
	iterP = (*iterB)->begin();
	size=(*iterB)->size();
	for(unsigned int t = 0; t < (unsigned int)size; t++){
		p = (*iterP);
		matrixBuffer[this->index(p.x, p.y)] = '1';
		iterP++;
	}
}

//this function indicate if the pixel is "black"(== 1) or not(== 0)
bool Vectorization::isBlack(int i, int j){                          //verificada depois do x e y invertidos
	int indice = this->index(i,j);
	if(matrixBuffer[indice] == '1'){
		return true; 
	}
	return false;
}

PointDirection Vectorization::nextPoint(PointDirection point){
	PointDirection newPoint;
	switch(point.direction){
	case 0:
		newPoint.x = point.x+1;
		newPoint.y = point.y;
		break;
	case 1:
		newPoint.x = point.x + 1;
		newPoint.y = point.y + 1;
		break;
	case 2:
		newPoint.x = point.x;
		newPoint.y = point.y + 1;
		break;
	case 3:
		newPoint.x = point.x - 1;
		newPoint.y = point.y + 1;
		break;
	case 4:
		newPoint.x = point.x - 1;
		newPoint.y = point.y;
		break;
	case 5:
		newPoint.x = point.x - 1;
		newPoint.y = point.y - 1;
		break;
	case 6:
		newPoint.x = point.x;
		newPoint.y = point.y - 1;
		break;
	case 7:
		newPoint.x = point.x + 1;
		newPoint.y = point.y - 1;
		break;
	}
	newPoint.direction = 0;
	return newPoint;
}

int Vectorization :: previewPoint(PointDirection v){
	if(v.direction < 4)
		return (v.direction + 4);
	else return (v.direction - 4);
}

list<PointDirection> Vectorization::findNeighbor(Point2D p){
	list<PointDirection> listpd;
	PointDirection pd;
	pd.x = p.x;
	pd.y = p.y;
	if(matrixBuffer[this->index(p.x+1, p.y)] == '1'){	
		pd.direction = 0;
		listpd.push_back(pd);	
	}
	if(matrixBuffer[this->index(p.x+1, p.y+1)] == '1'){	
		pd.direction = 1;
		listpd.push_back(pd);
	}
	if(matrixBuffer[this->index(p.x, p.y+1)] == '1'){
		pd.direction = 2;
		listpd.push_back(pd);	
	}
	if(matrixBuffer[this->index(p.x-1, p.y+1)] == '1'){
		pd.direction = 3;
		listpd.push_back(pd);
	}
	if(matrixBuffer[this->index(p.x-1, p.y)] == '1'){	
		pd.direction = 4;
		listpd.push_back(pd);
	}
	if(matrixBuffer[this->index(p.x-1, p.y-1)] == '1'){	
		pd.direction = 5;
		listpd.push_back(pd);
	}
	if(matrixBuffer[this->index(p.x, p.y-1)] == '1'){	
		pd.direction = 6;
		listpd.push_back(pd);
	}
	if(matrixBuffer[this->index(p.x+1, p.y-1)] == '1'){
		pd.direction = 7;
		listpd.push_back(pd);
	}
	return listpd;
}

void Vectorization::extract(Point2D vo, Curve *curve){
	int a, previewCode; Point2D current; 
	PointDirection v, preview; 
	Curves lCurve; 
	list<PointDirection> listNeighbor;
	
	v.x = -1; v.y = -1;
	curve->listPoints.push_back(vo);
	listNeighbor = findNeighbor(vo);
	if(listNeighbor.size() > 0){
		list<PointDirection>::iterator neighbor = listNeighbor.begin();
		v = nextPoint((*neighbor));
		current.x = v.x; current.y = v.y;
		listNeighbor = findNeighbor(current);
		matrixBuffer[this->index(current.x, current.y)] = '0';
		listNeighbor.pop_back();
		if(listNeighbor.size() > 0){
			do{
				neighbor = listNeighbor.begin();
				v = (*neighbor);
				neighbor++;
				while(neighbor != listNeighbor.end()){//nao entrou
					previewCode = previewPoint(preview);
					a = (*neighbor).direction;
					if(v.direction < previewCode && previewCode < a){
						v = (*neighbor);
					}
					neighbor++;
				}
				current.x = v.x; current.y = v.y;
				curve->listPoints.push_back(current);
				matrixBuffer[this->index(current.x, current.y)] = '0';
				preview = v;
				v = nextPoint(v);
				current.x = v.x; current.y = v.y;
				listNeighbor = findNeighbor(current);
			}while(listNeighbor.size() != 0 && v != vo);
		}
		if (v == vo){
			current.x = v.x; current.y = v.y;
			curve->listPoints.push_back(current);
			curve->flagFechado = 1;
			matrixBuffer[this->index(current.x, current.y)] = '0';
		} 
		else{
			current.x = v.x; current.y = v.y;
			curve->listPoints.push_back(current);
			matrixBuffer[this->index(current.x, current.y)] = '0';
			current.x = vo.x; current.y = vo.y;
			curve->flagFechado = 0;
		}
	}
	matrixBuffer[this->index(vo.x, vo.y)] = '0';
}

void Vectorization::extract2(int regionPosition){
	list<Curves>:: iterator iterRegion;
	Curve temp; Curves lCurve; Point2D v(0,0); int f = 0, region = 0;
	while(f != 1){
		if(!isBlack(v.x,v.y)){
			if(v.x == w-1 && v.y == h-1){
				f = 1;
			}
			else v.x++;
			if(v.x == w-1 && v.y != h-1){
				v.x = 0; v.y++;
			}
		}else{
			if (isBlack(v.x,v.y)){
				temp.listPoints.clear();
				extract(v, &temp);
				if(temp.listPoints.size() >= 5){
					region = 0;
					iterRegion = regionCurves.begin();
					while(region < regionPosition){
						region++; iterRegion++;
					}
					iterRegion->push_back(temp);//iterRegion->listCurve.push_back(temp);
				}
				temp.listPoints.clear();
				lCurve.clear();//lCurve.listCurve.clear();
				v.x = 0; v.y = 0;
			}else{ 
				if(v.x == w-1 && v.y == h-1){
					f = 1;
				}
			}
		}
	}
}

void Vectorization::initializeBackGround(HierarchyRegions *aCurves){
	HierarchyRegions *curves = aCurves; int son, count = 0;
	list<int>::iterator iter1;
	list<Boundary*>::iterator iter; list<Point2D>::iterator iterP;      
	list<Curves>::iterator iterRC; list<Curve>::iterator iterC;
	list<Point2D>::iterator iterPP; list<Label>::iterator iterLabel;
	unsigned int size = w*h, size1 = 0;
	for(unsigned int i = 0; i < size; i++){ 
		matrixBuffer[i] = '0';
	}
	iter = scatteredBoundary->begin();
	iterP = (*iter)->begin();

	unsigned int i = (*iter)->size();	
	for(unsigned t = 0; t < i; t++){
		matrixBuffer[index((*iterP).x,(*iterP).y)] = '1';
		iterP++;
	}
	iterRC = regionCurves.begin();
	iterC = iterRC->begin();//iterC = iterRC->listCurve.begin();
	iterPP = iterC->listPoints.begin();
	i = iterC->listPoints.size();
	for(unsigned t = 0; t < i; t++){
		matrixBuffer[index((*iterPP).x,(*iterPP).y)] = '0';
		iterPP++;
	}
	iterLabel = curves->descendentList.begin();
	iter1 = iterLabel->begin(); son = 0;
	iterLabel = curves->descendentList.begin();
	while(size1 < iterLabel->size()){
		iterRC = regionCurves.begin();
		son = (*iter1);
		if(son != -1){
			while(count != son){
				iterRC++; count++;
			} 
			iterC = iterRC->begin();//iterC = iterRC->listCurve.begin();
			iterPP = iterC->listPoints.begin();
			size =  iterC->listPoints.size(); 
			count = 0;
			for(unsigned int t = 0; t < size-1; t++){
				matrixBuffer[this->index((*iterPP).x,(*iterPP).y)] = '0';
				iterPP++;
			}
		}iter1++; size1++;
	}
}

/*for(regiao 0 ate a ultima){
	criar uma imagem com cada regiao
	apagar a curva externa
	apagar as curvas internas que sao externas de outras regioes
	extrair as curvas abertas
}*/
//This function extract the other curves using the regions hierarchy.
void Vectorization::extractHierarchy(HierarchyRegions *hR){
	
	HierarchyRegions *aCurves = hR; int regionPosition = 0;
	list<Label>::iterator iterHierarchy; list<Curves>::iterator iterR; list<Curve>::iterator iterC;
	list<Point2D>::iterator iterPoint; list<int>::iterator iter1; int hierarchyListPosition, son,count; Point2D p;
	unsigned int i = regionCurves.size(), j = 0, size ; 
	while(j < i){
		iterHierarchy = aCurves->descendentList.begin();
		iter1 = (*iterHierarchy).begin();
		if(regionPosition != 0){
			initializeMatrix(regionPosition); //cria uma imagem com cada regiao
			hierarchyListPosition = 0;
			iterHierarchy = aCurves->descendentList.begin();
			iterR = regionCurves.begin();
			while(hierarchyListPosition != regionPosition){
				hierarchyListPosition++; iterHierarchy++; iterR++;
			} 
			iterC = iterR->begin();//iterC = iterR->listCurve.begin();
			iterPoint = iterC->listPoints.begin();
			size =  iterC->listPoints.size(); 
			count = 0;
			for(unsigned int t = 0; t < size-1; t++){
				p.x = (*iterPoint).x; p.y = (*iterPoint).y;
				matrixBuffer[this->index(p.x, p.y)] = '0';
				iterPoint++;
			}
			/*nesse ponto aCurvesPosition == listPosition, ou seja  a posicao da lista da hierarquia � igual a da regiao
			 * agora falta apagar as curvas fechadas que sao as curvas externas das filhas 	 */
			iter1 = (*iterHierarchy).begin(); son = 0;
			while(iter1 != (*iterHierarchy).end()){
				iterR = regionCurves.begin();
				son = (*iter1);
				if(son != -1){
					while(count != son){
						iterR++; count++;
					} 
					iterC = iterR->begin();//iterC = iterR->listCurve.begin();
					iterPoint = iterC->listPoints.begin();
					size =  iterC->listPoints.size(); 
					count = 0;
					for(unsigned int t = 0; t < size-1; t++){
						p.x = (*iterPoint).x; p.y = (*iterPoint).y; 
						matrixBuffer[this->index(p.x, p.y)] = '0';
						iterPoint++;
					}
				}iter1++;
			}
		}
		else {
			initializeBackGround(aCurves);
		}///neste ponto o buffer ja esta pronto. Basta agora extrair as curvas abertas.
		extract2(regionPosition);
		regionPosition++; /*iterR++*/;  j++;
		}
	}


void Vectorization::extractBackGround(){
	Point2D v; Curve aux; Curves lCurveAux; 
	v.y = 0;
	for(int i = 0; i < w; i++){
		v.x = i;
		aux.listPoints.push_back(v);
	}
	for(int j = 1; j < h; j++){ //verificar se � j > 0 ou j >= 0
		v.y = j;
		aux.listPoints.push_back(v);
	}
	for(int i = w - 2; i >= 0 ; i--){
		v.x = i;
		aux.listPoints.push_back(v); 
	}
	for(int j = h - 2; j >= 0 ; j--){ //verificar se � j > 0 ou j >= 0
		v.y = j;
		aux.listPoints.push_back(v);
	}
	aux.flagFechado = 1;
	lCurveAux.push_back(aux);
	regionCurves.push_back(lCurveAux);
}

void Vectorization::curveExtraction(){	
	Point2D vo(-1,-1);
	Curve temp; Curves lCurve;
	extractBackGround();
	int i, j = 0, f = 0, listPosition = 0;
	list<Boundary*>::iterator listBound = scatteredBoundary->begin();
	listBound++; 	listPosition = 1;
	while(listBound != scatteredBoundary->end()){
		/*inicializa  matrixBuffer com uma regiao de cada vez, onde
		 * listPosition eh a posicao (da regiao) na lista de regioes
		 */
		initializeMatrix(listPosition);
		j = 0;
		do{
			list<Point2D>:: iterator ponto = (*listBound)->begin();
			i = 0;
			do{
				if(isBlack(i, j)){
					vo.x = i; vo.y = j;
					f = 1; 
				}
				i++;
			}
			while(i < w && f == 0);
			j++;
		}
		while(j < h && f == 0); 
		extract(vo, &temp);
		//if(temp.listPoints.size() >= 5){
		lCurve.push_back(temp);//lCurve.listCurve.push_back(temp);
			regionCurves.push_back(lCurve);
		//}
		temp.listPoints.clear(); lCurve.clear(); //lCurve.listCurve.clear(); 
		listBound++; listPosition++; f = 0;
	}
}

//mostrar pontos das curvas de cada regiao
void Vectorization::showPoints(){
	list<Curves> :: iterator iter = regionCurves.begin();
	list<Curve>::iterator iter2;
	list<Point2D>::iterator iter3;
	int qtn = 0;
	unsigned int a = 0, b = 0, c = 0;
	while (a < regionCurves.size()){
		cout << "\nregiao [" << a << "] ";
		iter2 = iter->begin();//iter2 = iter->listCurve.begin();
		b = 0;
		while(b < iter->size()){//while(b < iter->listCurve.size()){
			cout << "\ncurva : " << b <<  " flag = " << (*iter2).flagFechado << endl;
			iter3 = iter2->listPoints.begin();
			c = 0;
			while(c < iter2->listPoints.size()){
				cout <<"(" << (*iter3).x << ", " << (*iter3).y << ")"<< "; ";
				iter3++; c++;
			}	
			iter2++; b++;
		}
		iter++; qtn++; a++;
	}
	cout << "\nquantidade de regioes = " << qtn << endl;
}

void Vectorization :: drawPoints(){
	double a = 0.0, b = 0.0, c = 0.5, dc;
	srand( time(NULL) );
	dc = (rand() % 50)/50.0;
	a = b = dc;	
	c += dc;
    list<Curves> :: iterator iter = regionCurves.begin();
    //A - Para cada região.
    while (iter != regionCurves.end()){
	   	glBegin(GL_POINTS);
	   	glColor3f(a,b,c);
		list<Curve>::iterator iter2 = iter->begin();//iter->listCurve.begin();
		//A - Para cada curva pertencenta a região.
		while(iter2 != iter->end()){//while(iter2 != iter->listCurve.end()){
			list<Point2D>::iterator iter3 = iter2->listPoints.begin();
			//A - Percorrendo a lista de pontos da curva.
			while(iter3 != iter2->listPoints.end()){
				glVertex2i((*iter3).x,(*iter3).y);
				iter3++;	
			}	
			iter2++;		
		}
		iter++;
		a = a + 0.3; b = b + 0.2; c = c + 0.1;
		if(a >= 1.0 && b >= 1.0 && c >= 1.0){		
			dc = (rand() % 50)/50.0;
			a = b = c = dc;		
		}
		glEnd();
	}
}

/****************************************************************/
void Vectorization :: findCurve(int *c, int *lpc, int x, int y){
	int ca = 0, cb = 0;
	char s = 0;
	list<Curves> :: iterator iter = regionCurves.begin();
    while (iter != regionCurves.end() ){
		list<Curve>::iterator iter2 = iter->begin();
		cb = 0;
		while(iter2 != iter->end() && s == 0){
			list<Point2D>::iterator iter3 = iter2->listPoints.begin();
			while(iter3 != iter2->listPoints.end() && s == 0){
				if((iter3->x >= x-2  && iter3->x <= x+2) && (iter3->y >= y-2  && iter3->y <= y+2)){
					*c = ca;  *lpc = cb; s = 's'; 	
//					cout << " curva " << ca << " " << cb << " " << *c << endl;
				}iter3++;
			}iter2++; cb++;
		}iter++; ca++;
    }
} 
Vectorization :: ~Vectorization(){
	delete[] matrixBuffer;
}
