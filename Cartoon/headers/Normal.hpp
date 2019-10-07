#ifndef NORMAL_HPP_
#define NORMAL_HPP_

#include <list>
#include <cmath>
#include <vector>

#include "Regions.hpp"
#include "defs.hpp"

typedef enum{NORMALIN, NORMALOUT, NORMALINANDOUT} DIRECTION;

/*
 * A
 */
typedef struct tuple
{
	int _index;           //posição do ponto em uma curva "list<Point2D>".
	int _index_matrix;    //posição/linha/identificar da curva a qual o ponto pertence em uma matrix de curvas.
	Point2D _point;	      //coordenada do ponto.
	bool itsProjection;  //tag: true  => ponto de projeção válido
	      							  //    false => não é ponto de projeção
	bool itsCEP;
	long int KxK; //resultado da multiplicação do produto vetorial CExCE_next com o produto vetoraial CPxCP_next.
							//K > 0  => eliminação no sentido projeção ao fim da lista da curva.
							//K <= 0 => eliminação no sentido começo da lista da curva a projeção.

	void init(int index, int index_matrix, Point2D point)
	{
		_index = index;
		_index_matrix = index_matrix;
		_point = point;
	}

	/* dados de extremidade de curva que me gerou (assimindo-me como uma projeção). */
	int index_extreme;
	int index_matrix_extreme;
	Point2D point_extreme;

} point_tuple;

const double PI=3.14159265;

using namespace std;

class Normal {
private:

	int index (int i, int j, int width) {return (i+j*width);}

	bool orientation(list<Point2D> *listPoint);	

	void updateDistance(Point2D p,list<Curve>::iterator iterCurve,double &sumDi,vector<double> *di);
	void updateDistanceUsingHier(Point2D p,Vectorization *v,double &sumDi,vector<double> *di,int label);
	void updateNpxNpyUsingHier(Point2D p,double *Npx,double *Npy,Vectorization *v,vector<double>::iterator iterWi,int label);
	void updateNpxNpy(Point2D p,double *Npx,double *Npy,list<Curve>::iterator iterCurve,vector<double>::iterator iterWi);   		

	void calculateDistance(Point2D p,list<Curves>::iterator iterCurves,Vectorization *v,double &sumDi,vector<double> *di,list<Label>::iterator iterLabel);   		   	
	void calculateWi(vector<double> *di,double sumDi,vector<double> *wi);   	
	void calculateNpxNpy(Point2D p,double *Npx,double *Npy,list<Curves>::iterator iterCurves,Vectorization *vet,vector<double> *wi,list<Label>::iterator iterLabel,unsigned int i);
	void calculateNormal(unsigned int i,list<Interior*>::iterator iterListRegion,list<Label>::iterator iterHierLabel,list<Curves>::iterator listCurves);

public:
	Regions	*regions; //receber no parametro da funcao
	int w,h;

	vector<Point3D> normalBuffer;  //normal to region interior points
	vector<Point3D>	normalBuffer_new;  //(by Ailton) new normals 3D to region interior points

	normalToAll	normalToBound; //normal to boundary points (vectorization)
	normalToRegion regionNormalToBound;	//(by Ailton) new normals to boundary points (vectorization)

	Normal(Regions *r);

	void extractBoundaryNormals();
	void smoothBoundaryNormals();    //suavisar as normais da borda
	void printBoundaryNormal(DIRECTION);
	void smoothXtremes(int n); //n is the number of interpolations

	void drawNormalVector(DIRECTION);

	void calculateAllInteriorNormal();
	void smoothInterior(double a,double b,double c);
	void drawInteriorNormalMap();

	void getLimitValues(int *iMin, int *iMax, int *jMin, int *jMax, list<Point2D> *curvePoints);
	void smoothAllCurves();
	void smoothCurve(list<Point2D> *curvePoints,int regionNum, int curveNum, int dist); //curvePoints - internal Curve, regionPoints - region containing the curve, dist - smooth raius
	void updateDistanceBuffer(int iC, int jC, int i, int j, int w, int h, int iMin, int jMin, vector<int> *distanceBuffer, vector<Boundary> *distanceList,  int pixelDistance, int dist);

	/** A
	 * Método que determina quais pontos da curva de bordo são visíveis por um ponto. Esse método retorna
	 * uma matriz onde cada linha possui um intervalo da curva visível por o ponto 'p'.
	 */
	list<Curve> setBoundarySegmentVisible(Point2D p, list<Point2D> curvePoints, int i, bool closedCurve, bool itsInternalBoundary = false);

	/** A
	 * Método que determina quais pontos da lista de curvas internas são visíveis por um ponto. Esse método retorna
	 * uma matriz onde cada linha possui um intervalo de cada curva visível por o ponto 'p'.
	 */
	list<Curve> setInternalCurveSegmentVisible(Point2D p, list<Curves>::iterator it_curves);

	/** A
	 * Detecta os intervalos de crescimento e decrescimento do angulo da curva em relação ao vetor
	 * <1,0> que parte do ponto 'p'. Na verdade o produto vetorial é usado para tal, onde o que
	 * se identifica é a mudança do sentido do vetor resultante para definir se o ângulo cresce ou
	 * decresce.
	 */
	list<Boundary> angleRangesGrowth(Point2D p, list<Point2D> curvePoints, int i, bool itsInternalBoundary);

	/** A
	 * Remove os trechos que a própria curva esconde.
	 */
	void removeAutoIntersection(Point2D p, list<Boundary> &matrix);

	/** A
	 * Detecta os trechos escondidos do ponto 'p' nos intervalos visíveis que estão em 'matrix'.
	 */
	void removeHiddenStretch(Point2D p, list<Boundary> &matrix);

	/** A
	 * Detecta os trechos escondidos do ponto 'p' nos intervalos visíveis que estão em 'stretchBoundary'(1) e
	 * 'stretchInternalCurves'(2), removendo os trechos que (1) esconde de (2) e vice-versa.
	 */
	//void removeHiddenStrech_step2(Point2D p, list<Curve> &stretchBoundary, list<Curve> &stretchInternalCurves);

	/** A
	 * Remove da matriz curvas com tamanho menor ou igual que 'size'.
	 */
	void removeNoiseCurves(list<Boundary> &matrix, unsigned int size);

	/** A
	 * Armazena uma amostra da curva e suas respectivas posições na lista 'points'.
	 */
	vector<point_tuple> getSampleCurve(list<Point2D>::iterator begin,
			list<Point2D>::iterator end,
			int i, int count_k);

	/** A
	 * Busca na curva 'curve' por o ponto que pertence a mesma reta que passa por 'p' e 'p2'.
	 */
	point_tuple findingProjectionPoint(Point2D p, list<Point2D> curve, Point2D p2);
//	point_tuple findingProjectionPoint(Point2D p, list<Boundary>::iterator curve, Point2D p2);

	/** A
	 * Procura na curva 'curve' por uma projeção E.P.C., sendo E = 'extreme' e C = 'central'.
	 * Retorna a posição do ponto na lista.
	 */
	int getAutoProjection(Point2D projection, list<Point2D> curve, Point2D central);
//	int getValidExtreme(Point2D extreme, list<Boundary>::iterator curve, Point2D central);

	/** A
	 * Verifica se o ponto 'projection' é um ponto de corte C.E.P..
	 */
	bool its_CEP(Point2D central, Point2D projection, Point2D extreme);

	/** A
	 * Verifica se o ponto 'projection' é um ponto E.C.P..
	 */
	bool its_ECP(Point2D extreme, Point2D central, Point2D projection);

	/** A
	 * Verifica se o ponto 'projection' é um ponto E.P.C..
	 */
	bool its_EPC(Point2D extreme, Point2D projection, Point2D central);

	/** A
	 * Verifica se dados dois pontos, os mesmos são da mesma curva.
	 */
	bool pointsOfTheSameCurve(vector<point_tuple> pts);

	static unsigned int CURVE_MIN_SIZE;

	vector<point_tuple> samplePointsCurve;
	vector<point_tuple> samplePointsInternalCurve;

	void extractBoundaryNormals_new(list<Curves> visibleCurve);
	void smoothBoundaryNormals_new(list<Curves> visibleCurve);
	normalToRegion extractInternalBoundaryNormals(Curves curves);
	void smoothInternalBoundaryNormals(Curves curves, normalToRegion &nr);
	void calculateAllInteriorNormal_new();
	void calculateNormal_new(int i, list<Interior*>::iterator iterListRegion, list<Label>::iterator iterHierLabel, list<Curves>::iterator listCurves);
	void calculateDistance_new(Point2D p, list<Curves> visibleCurve, Vectorization *v, double &sumDi, vector<double> *di, list<Label>::iterator iterLabel);

	void calculateNpxNpy_2(Point2D p,double *Npx,double *Npy,list<Curves>::iterator iterCurves,Vectorization *vet,vector<double> *wi,list<Label>::iterator iterLabel,unsigned int i);
	void calculateNpxNpy_new(Point2D p, double *Npx, double *Npy, list<Curves> visibleCurve, Vectorization *vet, vector<double> *wi, list<Label>::iterator iterLabel, int i);

//	void updateDistance_new(Point2D p,Curve curve,double &sumDi,vector<double> *di);

//	void drawInteriorNormalMap_2();
	void drawInteriorNormalMap_new();

	void smoothInterior_new(double a, double b, double c);
	~Normal();
};

//class Normal{
//private:
//	


//
//	void buildNormalMap(int width, int height);
//	void getLimitValues(int *iMin, int *iMax, int *jMin, int *jMax, list<Point2D> *curvePoints);
//	void smoothInterior(int width, int height, double a, double b, double c);
//		
//	//ir pra classe de curvas
//	void brokenCurve(int xB, int yB, int xE, int yE, bool *brokenBegin, bool *brokenEnd, list<Curve> *listCurves);
//	//-------------------------------------------
//		
//public:

//	

//	
//	

//	
//	

//	
//	void drawBorderNormalMap();
//	void drawInteriorNormalMap();
//	void drawNormalMap();
//	void normalize();
//	void xScale(Regions *r, Vectorization *_regionNormals, double scale);
//	void yScale(Regions *r, Vectorization *_regionNormals, double scale);
//	void zScale(Regions *r, Vectorization *_regionNormals, double scale);
//	void scale(Regions *r, Vectorization *_regionNormals, double scale);//escala uniforme
//	
//	
//	void upDownCurve(list<Point2D> *curvePoints, list<Point2D> *regionPoints, unsigned int dist); //curvePoints - internal Curve, regionPoints - region containing the curve, dist - smooth raius

//
//	
//};

#endif /*NORMAL_HPP_*/
