#include "../headers/Canvas.hpp"
#include "../headers/timer.hpp"

#define COR (canvas2->regProp->colors[ind])

Canvas :: Canvas(int X,int Y,int W,int H) : Fl_Gl_Window(X,Y,W,H){
	sr=-1; sc=-1; slc=-1; x = -1; y = -1; exibitionMode = 0;
	
	object = 1; //view mode - object == 1 / light == 0
	
	canvas2 = NULL;			thin = NULL; 
	region  = NULL;			normal = NULL;
	ligMod  = NULL;
}
void Canvas :: loadImage(const char* _image){
	//cout <<_image<< endl;
	sr=-1; 		sc=-1; 
	slc=-1; 	x = -1; 	y = -1; 
	if(thin) 	delete thin; 		
	if(region) 	delete region; 
	if(ligMod) 	delete ligMod;
	if(normal) 	delete normal;
	if(regProp) delete regProp;
	
	image = new Image(_image);
	setExibitionMode(0);
	//if(eS != 'o') objectMode();
	redraw();
}

void Canvas :: load(Image* i){
	image = i;

	thin = new Thinning(image);
	thin->thinningAlgorithm(); 
	 
	region = new Regions(thin->imNew);
	region->computeRegions();
	
	normal = new Normal(region);
	normal->extractBoundaryNormals(); //editando normalToBound ok
	normal->smoothBoundaryNormals();  //editando normalToBound ok
	normal->calculateAllInteriorNormal(); //ok
	normal->smoothInterior(2.0,3.0,1.5); //aplicando uma máscara para tirar a intensidade da descontinuidade do mapa de normais 3D.
	normal->smoothInterior(2.0,3.0,1.5);
	normal->smoothInterior(2.0,3.0,1.5);

	//	id da região.
//	normal->i = 1; //	#circle.PNG, coracao.jpg (Região interna) # girl.jpg (sapato do lado esquerda), # homer.png (ombro do lado esquerdo) #zegotinha # haltere
//	normal->i = 2; // #girl.jpg (Pedaço do cabelo) # homer.png (Camisa, parte do troco) #flor3.png (petalas)
//	normal->i = 3; // #girl.jpg (Perna do lado esquerdo) # homer.png (Bigode) # coyote.PNG (boca)
//	normal->i = 6; //	#homer.png (Cabeça) #piupiu.PNG (cabeça + tronco)
//	normal->i = 7; //	##scooby (barriga)

	regProp = new RegionProperties(region, image);
	ligMod = new LightingModel(region,image,normal, regProp);
	ligMod->imageIllumination();

	//by Ailton
	timer t;
	t.start();
	normal->calculateAllInteriorNormal_new();
	normal->smoothInterior_new(2.0,3.0,1.5);
	normal->smoothInterior_new(2.0,3.0,1.5);
	normal->smoothInterior_new(2.0,3.0,1.5);
	ligMod->imageIllumination_new();
	t.stop();
	t.check();

	setExibitionMode('x');
}

void Canvas ::  setExibitionMode(char a){
	exibitionMode = a;
	redraw();
}

void Canvas :: setSelectedCurve(int *c,int *lpc){
	sc=*c; slc=*lpc;
	if(sc != -1) selectCurve();
	
}
void Canvas :: selectCurve(){
	unsigned char* idata = (unsigned char*)image->im->data[0];
	list<Curves> :: iterator iter = canvas2->region->listBoundary->regionCurves.begin();
	for(int i = 0; i < sc; i++)
		iter++;
	list<Curve>::iterator iter2 = iter->begin();
	for(int i = 0; i < slc; i++)
		iter2++;
	list<Point2D>::iterator iterP = iter2->listPoints.begin();
	while(iterP != iter2->listPoints.end()){
		int i = image->index(iterP->x,iterP->y);
		idata[i] = (unsigned char)0;
		i = i + image->im->width*image->im->height;
		idata[i] = (unsigned char)255;
		i = i + image->im->width*image->im->height;
		idata[i] = (unsigned char)0;
		iterP++;
		if(iterP != iter2->listPoints.end()) iterP++;
		if(iterP != iter2->listPoints.end()) iterP++;
  	}
  	redraw();
}
void Canvas :: setSelectedRegion(int *r){
	sr=*r;
	if(sr != -1)selectRegion();
}

void Canvas :: selectRegion(int region){
	unsigned char* idata = (unsigned char*)image->im->data[0];
	list<Interior*>::iterator iterR = canvas2->region->listInterior.begin();
	for(int t = 0; t < region; t++)
  		iterR++;
  	list<Point2D>::iterator iterP = (*iterR)->begin();
  	while(iterP != (*iterR)->end()){
  		int ind = image->index(iterP->x,iterP->y);
		if((COR.r == 0 && COR.g == 0 && COR.b == 0)  // nao modifica  se for da borda
			|| (iterP->x == x && iterP->y == y)){    //ou o ponto escolhido pelo usuario
				iterP++;
		}else{
			idata[ind] = (unsigned char)82;
			ind = ind + image->im->width*image->im->height;
			idata[ind] = (unsigned char)82;
			ind = ind + image->im->width*image->im->height;
			idata[ind] = (unsigned char)82;
			iterP++;
		}
		if(iterP != (*iterR)->end()) iterP++;
		if(iterP != (*iterR)->end()) iterP++;
		if(iterP != (*iterR)->end()) iterP++;
		if(iterP != (*iterR)->end()) iterP++;
  	}
}

void Canvas :: selectRegion(){
	list<Interior*>::iterator iterR = canvas2->region->listInterior.begin();
	int j;
  	for(j=0;j < sr;j++)
  		iterR++;
  	if(canvas2->region->flag_setAsBackground[j] == 1){
  		selectRegion(0);
  		iterR = canvas2->region->listInterior.begin();
  		j = 1;
  		iterR++;
  		while(iterR != canvas2->region->listInterior.end()){
  			if(canvas2->region->flag_setAsBackground[j] == 1) selectRegion(j);
  			iterR++; j++;
  		}
  	}else selectRegion(j);
  	redraw();
}

void Canvas :: undoSelect(){ //unselect
	if(sc != -1){ 
		discardSelection(0);
		sc = -1;	slc = -1;
	}
	if(sr != -1){
		discardSelection(1);
		sr = -1;
	}
}

void Canvas :: discardSelection(bool o){ //0(false) == curve; 1(true) == region
	unsigned char* idata = (unsigned char*)image->im->data[0];
	if(o == 1){
		list<Interior*>::iterator iterR = canvas2->region->listInterior.begin();
		int j;
	  	for(j = 0; j < sr; j++)
	  		iterR++;
	  	if(canvas2->region->flag_setAsBackground[j] == 1){
	  		iterR = canvas2->region->listInterior.begin();
	  		j = 0;
	  		while(iterR != canvas2->region->listInterior.end()){
	  			if(canvas2->region->flag_setAsBackground[j] == 1){
	  				cleanSelectedArea(j);
				}iterR++; j++;
	  		}
	  	}else{
	  		cleanSelectedArea(j);
	  	}
		redraw();
		
	}else{ // for curves
		list<Curves> :: iterator iter = canvas2->region->listBoundary->regionCurves.begin();
	  	for(int j = 0; j < sc;j++)
	  		iter++;
	    list<Curve>::iterator iter2 = iter->begin();
		for(int j = 0; j < slc; j++)
			iter2++;
	  	list<Point2D>::iterator iterP = iter2->listPoints.begin();
		iterP = iter2->listPoints.begin();
		while(iterP != iter2->listPoints.end()){
			int i = image->index(iterP->x,iterP->y);
			idata[i] = (unsigned char)0;
			i = i + image->im->width*image->im->height;
			idata[i] = (unsigned char)0;
			i = i + image->im->width*image->im->height;
			idata[i] = (unsigned char)0;
			iterP++;
	  	}
	  	redraw();
	}
}

void Canvas :: cleanSelectedArea(int reg){
	unsigned char* idata = (unsigned char*)image->im->data[0];
	list<Interior*>::iterator iterR = canvas2->region->listInterior.begin();
	for(int t = 0; t < reg; t++)
  		iterR++;
  	list<Point2D>::iterator iterP = (*iterR)->begin();
	while(iterP != (*iterR)->end()){	
  		int ind = image->index(iterP->x,iterP->y);
  		if((COR.r == 0 && COR.g == 0 && COR.b == 0)  // nao modifica  se for da borda
			|| (iterP->x == x && iterP->y == y)){    //ou o ponto escolhido pelo usuario
				iterP++;
		}else{
			int i = ind;
			idata[i] = (unsigned int)COR.r;
			i = i + image->im->width*image->im->height;
			idata[i] = (unsigned int)COR.g;
			i = i + image->im->width*image->im->height;
			idata[i] = (unsigned int)COR.b;
			iterP++;
		}
	}
}
/*********************************************************************************/
void Canvas :: bgColor(Color *cl){
	list<Interior*>::iterator iterR = canvas2->region->listInterior.begin();
	list<Point2D>::iterator iterP = (*iterR)->begin();
	bool c = 0;
  	while(iterP != (*iterR)->end() && c == 0){
  		int ind = image->index(iterP->x,iterP->y);
		if(COR.r == 0 && COR.g == 0 && COR.b == 0)
			iterP++;
		else{
			cl->r = (double)(((unsigned char*)(image->im->data[0]))[ind]); /* aqui eh atribuida a cor para cada regiao*/
  			cl->g = (double)(((unsigned char*)(image->im->data[1]))[ind]); /* que foi selecinada*/
  			cl->b = (double)(((unsigned char*)(image->im->data[2]))[ind]); 
		    
		    cout << "cor (" << cl->r << ", " << cl->g << ", " << cl->r << ")\n"; 
		    c = 1;
		}
  	} 
	if(c == 0){
		cl->r = 0;	cl->g = 0;	cl->b = 0;
	}
}
/*********************************************************************************/
void Canvas :: changePointColor(int reg,int r, int g, int b){ //for regions
	unsigned char* idata = (unsigned char*)image->im->data[0];
	list<Interior*>::iterator iterR = canvas2->region->listInterior.begin();
	if(canvas2->region->flag_setAsBackground[reg] == 1){
		int t = 0;
		while(iterR != canvas2->region->listInterior.end()){
			if(canvas2->region->flag_setAsBackground[t] == 1){
				list<Point2D>::iterator iterP = (*iterR)->begin();
	  			while(iterP != (*iterR)->end()){
		  			int ind = image->index(iterP->x,iterP->y);	  		
		  			if(COR.r == 0 && COR.g == 0 && COR.b == 0) iterP++;
					else{
						COR.r = (double)r;	COR.g = (double)g;	COR.b = (double)b;
						idata[ind] = (unsigned char)r;
						ind = ind + image->im->width*image->im->height;
						idata[ind] = (unsigned char)g;
						ind = ind + image->im->width*image->im->height;
						idata[ind] = (unsigned char)b;
						iterP++;
					}
		  		}	
			}iterR++;	t++; 
		}
	}else{
	  	for(int j=0;j < reg;j++)
	  		iterR++;
	  	list<Point2D>::iterator iterP = (*iterR)->begin();
	  	while(iterP != (*iterR)->end()){
	  		int ind = image->index(iterP->x,iterP->y);
	  		if(COR.r == 0 && COR.g == 0 && COR.b == 0) iterP++;
			else{
				COR.r = (double)r;	COR.g = (double)g;	COR.b = (double)b;
				idata[ind] = (unsigned char)r;
				ind = ind + image->im->width*image->im->height;
				idata[ind] = (unsigned char)g;
				ind = ind + image->im->width*image->im->height;
				idata[ind] = (unsigned char)b;
				iterP++;
			}
	  	}
	}
  	setExibitionMode(0);
}

void Canvas :: addBackGround(int reg){
	list<Interior*>::iterator iterR = canvas2->region->listInterior.begin();
	Color cl;
	bgColor(&cl);
	cout << "cor (" << cl.r << ", " << cl.g << ", " << cl.r << ")\n"; 
  	for(int j=0;j < reg;j++)
  		iterR++;
  	canvas2->region->flag_setAsBackground[reg] = 1;
  	unsigned char* idata = (unsigned char*)image->im->data[0];
  	list<Point2D>::iterator iterP = (*iterR)->begin();
	while(iterP != (*iterR)->end()){
		int ind = image->index(iterP->x,iterP->y);	  		
		if(COR.r == 0 && COR.g == 0 && COR.b == 0) iterP++;
		else{
			COR.r = cl.r;	COR.g = cl.g;	COR.b = cl.b;
			idata[ind] = (unsigned char)cl.r;
			ind = ind + image->im->width*image->im->height;
			idata[ind] = (unsigned char)cl.g;
			ind = ind + image->im->width*image->im->height;
			idata[ind] = (unsigned char)cl.b;
			iterP++;
		}
	}
  	undoSelect();
  	redraw();
}

void Canvas :: removeFromBackGround(int reg){
	list<Interior*>::iterator iterR = canvas2->region->listInterior.begin();
  	for(int j=0;j < reg;j++)
  		iterR++;
  	discardSelection(1);
  	canvas2->region->flag_setAsBackground[reg] = 0;
  	redraw();
}

bool Canvas :: getBackgroundValue(int reg){
	return(canvas2->region->flag_setAsBackground[reg]);
}

void Canvas :: changeKa(int reg, double a){
	list<Interior*>::iterator iterR = region->listInterior.begin();
	int j;
	for(j = 0; j < reg; j++)
		iterR++;
	regProp->ambientCoef[reg] = a;
	ligMod->computeRegionIllumination(j);
	setExibitionMode('i');
	
}
void Canvas :: changeKs(int reg,double a){
	list<Interior*>::iterator iterR = region->listInterior.begin();
	int j;
	for(j = 0; j < reg; j++)
		iterR++;
	regProp->specularCoef[reg] = a;
	ligMod->computeRegionIllumination(j);
	setExibitionMode('i');
}
void Canvas :: changeKd(int reg,double a){
	list<Interior*>::iterator iterR = region->listInterior.begin();
	int j;
	for(j = 0; j < reg; j++)
		iterR++;
	regProp->diffuseCoef[reg] = a;
	ligMod->computeRegionIllumination(j);
	setExibitionMode('i');
}
void Canvas :: changeE(int reg,double a){
	list<Interior*>::iterator iterR = region->listInterior.begin();
	int j;
	for(j = 0; j < reg; j++)
		iterR++;
	regProp->specularExp[reg] = a;
	ligMod->computeRegionIllumination(j);
	setExibitionMode('i');
}

double Canvas :: getKa(int reg){
	list<Interior*>::iterator iterR = canvas2->region->listInterior.begin();
	for(int j = 0; j < reg; j++)
		iterR++;
	return(canvas2->regProp->ambientCoef[reg]);
}
double Canvas :: getKs(int reg){
	list<Interior*>::iterator iterR = canvas2->region->listInterior.begin();
	for(int j = 0; j < reg; j++)
		iterR++;
	return(canvas2->regProp->specularCoef[reg]);
}
double Canvas :: getKd(int reg){
	list<Interior*>::iterator iterR = canvas2->region->listInterior.begin();
	for(int j = 0; j < reg; j++)
		iterR++;
	return(canvas2->regProp->diffuseCoef[reg]);
}
double Canvas :: getE(int reg){
	list<Interior*>::iterator iterR = canvas2->region->listInterior.begin();
	for(int j = 0; j < reg; j++)
		iterR++;
	return(canvas2->regProp->specularExp[reg]);
}

void Canvas :: doIt(){
	list<Interior*>::iterator iterR = region->listInterior.begin();
	iterR++;
	unsigned int j;
	for(j = 0; j < region->listInterior.size(); j++)
		ligMod->imageIllumination();
	redraw();
}
void Canvas :: lightMode(){
	ligMod = canvas2->ligMod;
	object = 0;
	eS = 'l';
//	setExibitionMode('i');
	setExibitionMode('x');
}
void Canvas :: objectMode(){
	ligMod = NULL;
	setExibitionMode(0);
	object = 1;
	eS = 'x';
}
void Canvas :: lightPosition(int x, int y){
	ligMod->changeLightPositions(x,y);
	ligMod->imageIlluminationWithoutBack();
	redraw();
}
void Canvas :: lightPosition(int z){
	ligMod->changeLightPositions(z);
	ligMod->imageIlluminationWithoutBack();
	redraw();
}
void Canvas :: changelightColor(double red, double green,double blue){
	ligMod->changeLightColor(red, green, blue);
	ligMod->imageIlluminationWithoutBack();
	redraw();
}

/*A
 * Método que desenha nas duas janelas de vizualizão.
 **/
void Canvas::draw(){
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);	
	if(!valid()){
		glViewport(0,0, w(), h());
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, 100, 0, 100, -1, 1);
		glMatrixMode(GL_MODELVIEW); 
		glLoadIdentity();
	}
	if (image){
		int x = w()/2-(image->im->width)/2, y = h()/2-(image->im->height)/2;
		glViewport(x, y, image->im->width, image->im->height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-1, image->im->width,-1, image->im->height, -1.0, 1.0);
		glMatrixMode(GL_MODELVIEW);
		glShadeModel(GL_SMOOTH);
		glLoadIdentity();
		if(exibitionMode == 0){image->draw();}
		else{
			switch(exibitionMode){
				case 't': //thining
					thin->draw();  //Desenhando no canvas da direita o resultado do thinning.
					break;
				case 'r': //regions
					region->drawRegion(); //Desenhando no canvas da direita as regiões.
					break;
				case 'c': //curves
					region->listBoundary->drawPoints(); //Desenhando no canvas da direita as curvas internas e de bordo.
					break;
				case 'n': //normals
					normal->drawNormalVector(NORMALOUT); //Desenhando no canvas da direita as normais.
//					normal->drawNormalVector(NORMALIN); //Desenhando no canvas da direita as normais.
//					normal->drawNormalVector(NORMALINANDOUT); //Desenhando no canvas da direita as normais. ????????
					break;
				case 'm': //normalMap
					normal->drawInteriorNormalMap(); //Desenhando no canvas da direita o mapa de normais.
//					normal->drawInteriorNormalMap_2();
					break;
				case 'z': //Novo mapa de normais.
					normal->drawInteriorNormalMap_new();
					break;
				case 'x': //Teste de visualização da nova iluminação.
					ligMod->drawNewPoints();
					break;
				case 'i': //Desenhando no canvas da direita o resultado da iluminação.
					ligMod->drawPoints();
					break;
				default:
					ligMod->drawPoints();
					break;
			}//switch's end
		}//else's end
		
	}	glFlush();
}

int Canvas :: handle(int event){
	if(image && canvas2){
		int a = w()/2-(image->im->width)/2, b = h()/2-(image->im->height)/2;
		int c = -1,lpc = -1, r = -1;
		switch(event) {
			case FL_PUSH: 
				if(object){
					if(Fl::event_button() == 1){
						if(sc != -1 || sr != -1) undoSelect();
						x = Fl::event_x()-a; y = h()-Fl::event_y()-b;
						// Aqui eu pego a posição do clique do mouse
						LightingModel::clickPoint = Point2D(x,y);
						canvas2->region->listBoundary->findCurve(&c,&lpc,Fl::event_x()-a,h()-Fl::event_y()-b);
						if(c == -1) //nao selecionou nenhuma curva
							canvas2->region->findRegion(&r,Fl::event_x()-a,h()-Fl::event_y()-b);
						else{ 
							setSelectedCurve(&c,&lpc); 
							x = -1; y = -1; //selecionar curva
						}
						if(r != -1){
							setSelectedRegion(&r);
							((Controller*)this->parent())->refresh();
						}
					}else if(Fl::event_button() == 2){
						undoSelect();
					}
				}else{
					if (eS == 'l' && Fl::event_button() == 1) eS = 'x';
					else if (eS == 'x' && Fl::event_button() == 1) eS = 'l';
				}
				return 1;
			case FL_MOVE: 
			    if(eS == 'l')
			    	lightPosition(Fl::event_x()-a,h()-Fl::event_y()-b);
				return 1;
			case FL_MOUSEWHEEL:
				if(eS == 'l')
					lightPosition(Fl::event_dy());
				return 1;
		}
	}
	return Fl_Gl_Window::handle(event);
}
/*switch(event) {
		case FL_PUSH:
		if(Fl::event_button() == 1){
			if(!ligMod){
				if(sc != -1 || sr != -1) doIt();
				x = Fl::event_x()-a; y = h()-Fl::event_y()-b;
//					cout << " x " << x << "y " << y << endl;
				canvas2->vectorization->findCurve(&c,&lpc,Fl::event_x()-a,h()-Fl::event_y()-b);
				if(c == -1){
					canvas2->region->findRegion(&r,Fl::event_x()-a,h()-Fl::event_y()-b);
				}else{ 
					setSelectedCurve(&c,&lpc); x = -1; y = -1;
				}
				if(r != -1){ 
					setSelectedRegion(&r);
				}
				
			}else{
				if (eS == 'l') eS = 'x';
				else if (eS == 'x') eS = 'l';
			}
		}else{
		if(Fl::event_button() == 2){
					doIt();
				}
		}
		break;
      //return 1;
    
	

*/

//void Canvas::newLoad()
//{
//	cout << "Criando novas normais 3D..." << endl;
//	timer t;
//	t.start();
//	normal->calculateAllInteriorNormal_new(normal->i,normal->j);
//	normal->smoothInterior_new(2.0,3.0,1.5);
//	normal->smoothInterior_new(2.0,3.0,1.5);
//	normal->smoothInterior_new(2.0,3.0,1.5);
//	ligMod->imageIllumination_new();
//	t.stop();
//	t.check();
//	cout << "feito" << endl;
//}

Canvas :: ~Canvas(){
	delete thin;
	delete region;
	delete normal;
	delete ligMod;
	delete image;
	delete regProp;
	
}
