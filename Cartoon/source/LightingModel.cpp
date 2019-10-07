#include "../headers/LightingModel.hpp"

Point2D LightingModel::clickPoint(0,0);

LightingModel::LightingModel(Regions *r,Image *i, Normal *n, RegionProperties *rP){
	regions = r;	
	image = i;		
	regProp = rP;
	normal = n;
	data = new float[r->imageThinning->width*r->imageThinning->height*3];
	dataNew = new float[r->imageThinning->width*r->imageThinning->height*3]; //(by Ailton)

	for(int i = 0; i<r->imageThinning->width*r->imageThinning->height*3; i++ ){
		data[i] = 0.0;
		dataNew[i] = 0.0; //(by Ailton)
	}
	//light intensity
	iL = 0.9; iS = 0.6; iA = 1.0;
	
	//view position
	observPosition.x = (double)((r->imageThinning->im->width) /2);
	observPosition.y = (double)((r->imageThinning->im->height) /2);
	observPosition.z = 50.0; 
	
	//light position
	lightPosition.x = (double)((r->imageThinning->im->width))/2;
	lightPosition.y = (double)((r->imageThinning->im->height) - 5);
	lightPosition.z = 1.0;
}

void LightingModel::lightVector(int p1x, int p1y){
	Point3D p(p1x,p1y,0);
	lightVec = lightPosition - p;
	double norma = (double)(sqrt(lightVec.x*lightVec.x+lightVec.y*lightVec.y+lightVec.z*lightVec.z));
	lightVec = lightVec/norma;
}

void LightingModel::viewVector(int p1x, int p1y){
	Point3D p(p1x,p1y,0);
	observVec = observPosition - p;
	double norma = (double)(sqrt(observVec.x*observVec.x+observVec.y*observVec.y+observVec.z*observVec.z));
	observVec = observVec/norma;
}

Point3D LightingModel::reflectionVector(int p1x, int p1y){
	int ind = regions->index(p1x, p1y);
	double res = 2*(innerProduct(&lightVec, p1x,p1y));
	return ((normal->normalBuffer[ind])*res - lightVec);
}
 
double LightingModel::innerProduct(Point3D *p,Point3D *nP){
	return (p->x*nP->x + p->y*nP->y + p->z*nP->z);
}
double LightingModel::innerProduct(Point3D *p,int p1x, int p1y){
	int ind = regions->index(p1x, p1y);

	return (p->x*(normal->normalBuffer[ind]).x
		   +p->y*(normal->normalBuffer[ind]).y
		   +p->z*(normal->normalBuffer[ind]).z);
}

double LightingModel::diffuseReflection(Point3D *lightVec,int p1x, int p1y, int regPos){
	return (iL*regProp->diffuseCoef[regPos]*innerProduct(lightVec,p1x,p1y));
}

double LightingModel::specularReflection(Point3D *o,int p1x, int p1y,int regPos){
	Point3D reflec;
	reflec = reflectionVector(p1x,p1y);						//compute reflect vector normalized
	viewVector(p1x,p1y);									//compute view vector normalized
	return(iS*regProp->specularCoef[regPos]*pow(innerProduct(o,&reflec),regProp->specularExp[regPos]));
}

double LightingModel::ambientReflection(int regPos){
//	cout << "regProp->ambientCoef[regPos] " << regProp->ambientCoef[regPos] << " reg = " << regPos << " .2. - "<<regProp->ambientCoef[2] << endl;
	return (iA*regProp->ambientCoef[regPos]);
}

void LightingModel::backGoundIllumination(){
	list<Interior*>::iterator iterR = regions->listInterior.begin();
	int i = 0;
	regions->flag_setAsBackground[0] = 1;
	while(iterR != regions->listInterior.end()){
		list<Point2D>::iterator iterP = (*iterR)->begin();
		if(regions->flag_setAsBackground[i] == 1){
			while(iterP != (*iterR)->end()){
				int ind = regions->index(iterP->x, iterP->y);
				Color oC;
				oC = regProp->colors[ind]/255;
				ind = regions->index(iterP->x,iterP->y)*3;
	
				data[ind] = oC.r*lightColor.r;
				dataNew[ind] = oC.r*lightColor.r; //(by Ailton)
				ind = ++ind;
				data[ind] = oC.g*lightColor.g;
				dataNew[ind] = oC.g*lightColor.g; //(by Ailton)
				ind = ++ind;
				data[ind] = oC.b*lightColor.b;
				dataNew[ind] = oC.b*lightColor.b; //(by Ailton)
		
				iterP++;
			}
		} iterR++; i++;
	}
}

void LightingModel::curvesIllumination(){
	list<Curves>::iterator iterV = regions->listBoundary->regionCurves.begin();
	int i = 0;
	while(iterV != regions->listBoundary->regionCurves.end()){
		list<Curve>::iterator iterC = iterV->begin();
		while(iterC != iterV->end()){
			list<Point2D>::iterator iterP = iterC->listPoints.begin();
			while(iterP != iterC->listPoints.end()){
				int ind = regions->index(iterP->x, iterP->y);
				Color oC;
				oC = regProp->colors[ind]/255;
				
				ind = regions->index(iterP->x,iterP->y)*3;
				data[ind] = oC.r;
				dataNew[ind] = oC.r; //(by Ailton)
				ind = ++ind;
				data[ind] = oC.g;
				dataNew[ind] = oC.g;	//(by Ailton)
				ind = ++ind;
				data[ind] = oC.b;
				dataNew[ind] = oC.b;	//(by Ailton)
				iterP++;
			}iterC++;
		}iterV++; i++;
	}	
}

void LightingModel::regionIllumination(){
	list<Interior*>::iterator iterR = regions->listInterior.begin();
	iterR++; 
	int i = 1;
	
	while(iterR != regions->listInterior.end()){
		double Iamb = ambientReflection(i);
		list<Point2D>::iterator iterP = (*iterR)->begin();
		double Idif,Ispec, idR,idG,idB,IR,IG,IB,isR,isG,isB,iaR,iaG,iaB;
		if(regions->flag_setAsBackground[i] !=  1){
			while(iterP != (*iterR)->end()){
				int ind = regions->index(iterP->x, iterP->y);
				Color oC;
				oC = regProp->colors[ind]/255;
		
				Idif=Ispec=idR=idG=idB =isR = 0.0;
				isG=isB=iaR=iaG=iaB= IR = IG = IB = 0.0;
						
				lightVector(iterP->x, iterP->y);
				
				Idif = diffuseReflection(&lightVec,iterP->x, iterP->y,i);
				Ispec = specularReflection(&observVec,iterP->x, iterP->y,i);
				idR = Idif*oC.r*lightColor.r; idG = Idif*oC.g*lightColor.g; idB = Idif*oC.b*lightColor.b;	//diffuse component
				isR = Ispec*lightColor.r; 	isG = Ispec*lightColor.g; 	isB = Ispec*lightColor.b;	//specular component
				iaR = Iamb*oC.r*lightColor.r; iaG = Iamb*oC.g*lightColor.g; iaB = Iamb*oC.b*lightColor.b;	//ambient component
				IR = idR+isR+iaR;
				IG = idG+isG+iaG;
				IB = idB+isB+iaB;
				
				ind = regions->index(iterP->x,iterP->y)*3;
				data[ind] = IR;
				ind = ++ind;
				data[ind] = IG;
				ind = ++ind;
				data[ind] = IB;
			
				iterP++;
			}
		}iterR++; i++;
	}
}
void LightingModel::regionIllumination(int region){
	int i = 0;
	list<Interior*>::iterator iterR = regions->listInterior.begin();
	while(i!=region){
		iterR++; 
		i++;;
	}
//	cout << "i = " << i << endl;
	double Iamb = ambientReflection(i);
	list<Point2D>::iterator iterP = (*iterR)->begin();
	double Idif,Ispec, idR,idG,idB,IR,IG,IB,isR,isG,isB,iaR,iaG,iaB;
	if(regions->flag_setAsBackground[i] !=  1){
//		cout << "passar aki pois n � bg \n";
		while(iterP != (*iterR)->end()){
			int ind = regions->index(iterP->x, iterP->y);
			Color oC;
			oC = regProp->colors[ind]/255;
	
			Idif=Ispec=idR=idG=idB=isR=0.0;
			isG=isB=iaR=iaG=iaB=IR=IG=IB=0.0;
					
			lightVector(iterP->x, iterP->y);
			
			Idif = diffuseReflection(&lightVec,iterP->x, iterP->y,i);
			Ispec = specularReflection(&observVec,iterP->x, iterP->y,i);
			
			idR = Idif*oC.r*lightColor.r;	idG = Idif*oC.g*lightColor.g;	idB = Idif*oC.b*lightColor.b;	//diffuse component
			isR = Ispec*lightColor.r;		isG = Ispec*lightColor.g;		isB = Ispec*lightColor.b;	//specular component
			iaR = Iamb*oC.r*lightColor.r;	iaG = Iamb*oC.g*lightColor.g;	iaB = Iamb*oC.b*lightColor.b;	//ambient component
			IR = idR+isR+iaR;
			IG = idG+isG+iaG;
			IB = idB+isB+iaB;
			
			ind = regions->index(iterP->x,iterP->y)*3;
			data[ind] = IR;
			ind = ++ind;
			data[ind] = IG;
			ind = ++ind;
			data[ind] = IB;
			iterP++;
		}
	}//cout << "i " << i << endl;
}
void LightingModel :: changeLightPositions(int x, int y){
	lightPosition.x = x;
	lightPosition.y = y;	
}
void LightingModel :: changeLightPositions(int z){
	lightPosition.z = (lightPosition.z)+z;
}
void LightingModel::imageIlluminationWithoutBack(){
	curvesIllumination();
	regionIllumination();
	regionIllumination_new(); //by Ailton
}
void LightingModel :: changeLightColor(double r, double g, double b){
	lightColor.r = r;
	lightColor.g = g;
	lightColor.b = b;
}

void LightingModel::imageIllumination(){
	backGoundIllumination();
	curvesIllumination();
	regionIllumination();
//	regionIllumination(normal->i);
}

void LightingModel :: computeRegionIllumination(int region){
	if(regions->flag_setAsBackground[region] == 1){
		//cout << "errado \n";
		backGoundIllumination();
	}
	else{//cout << "certo \n";
		regionIllumination(region);
		regionIllumination_new(region); //by Ailton
	}
}

void LightingModel :: drawPoints(){
	glRasterPos2f(0.0f,0.0f);
	glClearColor(0.0f,0.0f,0.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glDrawPixels(image->width,image->height,GL_RGB, GL_FLOAT, (GLvoid*)data);
}

/* A
 * Método usado apenas para testes.
 * */
void LightingModel::drawNewPoints()
{
	//#####################################################################################//
	//ALGORITMO QUE PREENCHE TODA A IMAGEM NO MODO region->drawRegion() EM Canvas.cpp COM UMA COR.
//	for(int j = 0; j < image->height; ++j) {
//		for(int i = 0; i < image->width; ++i) {
//			regions->pImage[regions->index(i,j)] = 126;
//			std::cout << "(i,j) = (" << i << "," << j << ")" << std::endl;
//		}
//	}

	//#####################################################################################//
    // Desenha apenas uma das regiões na tela usando seus respectivos pontos internos.
//	Interior *inte = regions->listInterior.back();
//	list<Interior*>::iterator interior = regions->listInterior.begin();
	int i, j, k;
	// Escolhendo a região para teste de acordo com as imagens de teste. O iterado 'curvesIt' em uma parte
	// de código mais abaixo, precisa ser incrementado o mesmo número de vezes
//	interior++; // circle.PNG, coracao.jpg (Região interna) # girl.jpg (sapato do lado esquerda), # homer.png (ombro do lado esquerdo)

//	interior++; // girl.jpg (Pedaço do cabelo) # homer.png (Camisa, parte do troco)

//	interior++; // girl.jpg (Perna do lado esquerdo) # homer.png (Bigode) # coyote.PNG (boca)

//	interior++; interior++; interior++; // #homer.png (Cabeça) #piupiu.PNG (cabeça + tronco)

//	interior++ //#scooby (barriga)

//	list<Point2D>::iterator it = (*interior)->begin();

//	Point2D p = clickPoint;
//	cout << "Ponto clicado: " << clickPoint << endl;

	//Preenchendo a região com cor.
//	glPointSize(1);
//	glColor3f(1.0, 1.0, 1.0);
//	glBegin(GL_POINTS);
//	while(it != (*interior)->end())
//	{
//		glVertex2f(it->x, it->y);
//		it++;
//	}
//	glEnd();

	//Desenhando na tela o ponto clicado da região.
//	glColor3f(0.0, 1.0, 0.0);
//	glPointSize(4);
//	glBegin(GL_POINTS);
//	  glVertex2f(p.x, p.y);
//	glEnd();

	//#####################################################################################//
	// Desenha apenas as curvas (internas e/ou de bordo) na tela de UMA região usando seus respectivos pontos.
	/*i = 2;
	list<Curves>::iterator it_curves = regions->listBoundary->regionCurves.begin();
	list<Label>::iterator iterHierLaber = regions->hierarchy->descendentList.begin();
	advance(it_curves, i);
	advance(iterHierLaber, i);

	//DESENHANDO TODAS AS CURVAS DE UMA REGIÃO
	glColor3f(0.0, 0.0, 1.0);
	glPointSize(2);
	glBegin(GL_POINTS);
	for(list<Curve>::iterator it_curve = it_curves->begin(); it_curve != it_curves->end(); ++it_curve)
	{
//		if((*it_curve).flagFechado) //Desenhando apenas as curvas fechadas (curvas internas)
		{
			for(list<Point2D>::iterator points = (*it_curve).listPoints.begin(); points != (*it_curve).listPoints.end(); ++points)
			{
				glVertex2f(points->x,points->y);
			}
		}
	}
	glEnd();*/

	//#####################################################################################//
	// DESENHANDO O RESULTADO DA ETAPA 1 e 2.
		/*list<Curve> stretchInternalCurves;
		list<Curve> stretchInternalBoundary;
		list<Curve> stretchVisible;
//		j = normal->j;//0;

		stretchVisible = normal->setBoundarySegmentVisible(p, it_curves->front().listPoints, i, it_curves->front().flagFechado);
		stretchInternalCurves = normal->setInternalCurveSegmentVisible(p, it_curves);
//		normal->removeHiddenStrech_step2(p, stretchVisible, stretchInternalCurves);

		//para calcular usando a estruta de hierarquia dos filhos descendentes de primeiro grau. Para cada filho...
		for(list<int>::iterator iterInteger = iterHierLaber->begin(); iterInteger != iterHierLaber->end(); ++iterInteger)
		{
			if((*iterInteger)!=-1)
			{
				list<Curves>::iterator iterCurvesAux = regions->listBoundary->regionCurves.begin();
				for(int i=0;i<(*iterInteger);i++)
				{
					iterCurvesAux++;
				}

				list<Curve>::iterator iterCurveAux = iterCurvesAux->begin(); //pegando a curva de bordo da região filha.

				Curve boundaryCurve = *iterCurveAux; //pegando a curva interna de bordo.

				//pegando apenas os pontos de BORDO da regição interna visíveis por 'p'
				Curves visibleBoundarySegments = normal->setBoundarySegmentVisible(p, boundaryCurve.listPoints, *iterInteger, boundaryCurve.flagFechado, true);

				glColor3f(0.0, 1.0, 0.0);
				glPointSize(2);
				glBegin(GL_POINTS);
				for(list<Curve>::iterator iterCurve = visibleBoundarySegments.begin(); iterCurve != visibleBoundarySegments.end(); ++iterCurve)
				{
					for(list<Point2D>::iterator points = iterCurve->listPoints.begin(); points != iterCurve->listPoints.end(); ++points)
					{
						glVertex2f(points->x,points->y);
					}
					glEnd();
				}
			}
		}

		stretchVisible.splice(stretchVisible.end(), stretchInternalCurves); //transferindo os trechos visíveis das curvas internas.

		glPointSize(2);
		glBegin(GL_POINTS);

		int c = 0;
		for(list<Curve>::iterator b = stretchVisible.begin(); b != stretchVisible.end(); ++b, ++c)
		{
//			cout << "Curva " << c << ": " << b->listPoints.size() << " pontos" << endl;
			glColor3f(UtilityFunctions::colors[c].r, UtilityFunctions::colors[c].g, UtilityFunctions::colors[c].b);
//			glColor3f(1,0,0);
			for(list<Point2D>::iterator points = (*b).listPoints.begin(); points != (*b).listPoints.end(); ++points)
			{
				glVertex2f(points->x,points->y);
			}
		}
		glEnd();
		cout << endl;*/

		//#####################################################################################//
	//TESTANDO A ILUMINAÇÃO COM OS NOVOS DADOS DE ENTRADA RESULTANTES DA ETAPA 1 E 2.
	glRasterPos2f(0.0f,0.0f);
	glClearColor(0.0f,0.0f,0.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glDrawPixels(image->width,image->height,GL_RGB, GL_FLOAT, (GLvoid*)dataNew);
	//--------------------------------------------------------------------------------------------
}

Point3D LightingModel::reflectionVector_new(int p1x, int p1y)
{
	int ind = regions->index(p1x, p1y);
	double res = 2*(innerProduct_new(&lightVec, p1x,p1y));
	return ((normal->normalBuffer_new[ind])*res - lightVec);
}

double LightingModel::innerProduct_new(Point3D *p,int p1x, int p1y)
{
	int ind = regions->index(p1x, p1y);
	return (p->x*(normal->normalBuffer_new[ind]).x
			   +p->y*(normal->normalBuffer_new[ind]).y
			   +p->z*(normal->normalBuffer_new[ind]).z);
}

double LightingModel::diffuseReflection_new(Point3D *lightVec,int p1x, int p1y, int regPos)
{
	return (iL*regProp->diffuseCoef[regPos]*innerProduct_new(lightVec,p1x,p1y));
}

double LightingModel::specularReflection_new(Point3D *o,int p1x, int p1y,int regPos)
{
	Point3D reflec;
	reflec = reflectionVector_new(p1x,p1y);						//compute reflect vector normalized
	viewVector(p1x,p1y);									//compute view vector normalized
	return(iS*regProp->specularCoef[regPos]*pow(innerProduct(o,&reflec),regProp->specularExp[regPos]));
}

void LightingModel::regionIllumination_new(){
	list<Interior*>::iterator iterR = regions->listInterior.begin();
	iterR++;
	int i = 1;

	while(iterR != regions->listInterior.end()){
		double Iamb = ambientReflection(i);
		list<Point2D>::iterator iterP = (*iterR)->begin();
		double Idif,Ispec, idR,idG,idB,IR,IG,IB,isR,isG,isB,iaR,iaG,iaB;
		if(regions->flag_setAsBackground[i] !=  1){
			while(iterP != (*iterR)->end()){
				int ind = regions->index(iterP->x, iterP->y);
				Color oC;
				oC = regProp->colors[ind]/255;

				Idif=Ispec=idR=idG=idB =isR = 0.0;
				isG=isB=iaR=iaG=iaB= IR = IG = IB = 0.0;

				lightVector(iterP->x, iterP->y);

				Idif = diffuseReflection_new(&lightVec,iterP->x, iterP->y,i);
				Ispec = specularReflection_new(&observVec,iterP->x, iterP->y,i);

				idR = Idif*oC.r*lightColor.r; idG = Idif*oC.g*lightColor.g; idB = Idif*oC.b*lightColor.b;	//diffuse component
				isR = Ispec*lightColor.r; 	isG = Ispec*lightColor.g; 	isB = Ispec*lightColor.b;	//specular component
				iaR = Iamb*oC.r*lightColor.r; iaG = Iamb*oC.g*lightColor.g; iaB = Iamb*oC.b*lightColor.b;	//ambient component
				IR = idR+isR+iaR;
				IG = idG+isG+iaG;
				IB = idB+isB+iaB;

				ind = regions->index(iterP->x,iterP->y)*3;
				dataNew[ind] = IR;
				ind = ++ind;
				dataNew[ind] = IG;
				ind = ++ind;
				dataNew[ind] = IB;

				iterP++;
			}
		}iterR++; i++;
	}
}

void LightingModel::regionIllumination_new(int region)
{
	int i = 0;
	list<Interior*>::iterator iterR = regions->listInterior.begin();
	while(i!=region){
		iterR++;
		i++;
	}
//	cout << "i = " << i << endl;
	double Iamb = ambientReflection(i);
	list<Point2D>::iterator iterP = (*iterR)->begin();
	double Idif,Ispec, idR,idG,idB,IR,IG,IB,isR,isG,isB,iaR,iaG,iaB;
	if(regions->flag_setAsBackground[i] !=  1){
//		cout << "passar aki pois n � bg \n";
		while(iterP != (*iterR)->end()){
			int ind = regions->index(iterP->x, iterP->y);
			Color oC;
			oC = regProp->colors[ind]/255;

			Idif=Ispec=idR=idG=idB=isR=0.0;
			isG=isB=iaR=iaG=iaB=IR=IG=IB=0.0;

			lightVector(iterP->x, iterP->y);

			Idif = diffuseReflection_new(&lightVec,iterP->x, iterP->y,i);
			Ispec = specularReflection_new(&observVec,iterP->x, iterP->y,i);

			idR = Idif*oC.r*lightColor.r;	idG = Idif*oC.g*lightColor.g;	idB = Idif*oC.b*lightColor.b;	//diffuse component
			isR = Ispec*lightColor.r;		isG = Ispec*lightColor.g;		isB = Ispec*lightColor.b;	//specular component
			iaR = Iamb*oC.r*lightColor.r;	iaG = Iamb*oC.g*lightColor.g;	iaB = Iamb*oC.b*lightColor.b;	//ambient component
			IR = idR+isR+iaR;
			IG = idG+isG+iaG;
			IB = idB+isB+iaB;

			ind = regions->index(iterP->x,iterP->y)*3;
			dataNew[ind] = IR;
			ind = ++ind;
			dataNew[ind] = IG;
			ind = ++ind;
			dataNew[ind] = IB;
			iterP++;
		}
	}//cout << "i " << i << endl;
}

void LightingModel::imageIllumination_new()
{
//	backGoundIllumination();
//	curvesIllumination();
	regionIllumination_new();
}

LightingModel :: ~LightingModel(){
	delete[] data;
	delete[] dataNew;
}
