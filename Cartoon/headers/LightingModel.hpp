#ifndef LIGHTINGMODEL_HPP_
#define LIGHTINGMODEL_HPP_

#include "Regions.hpp"
#include "Point3D.hpp"
#include "Normal.hpp"
#include "RegionProperties.hpp"
#include <cmath>

#include "UtilityFunctions.hpp"

/**
 * by Ailton
 *
 */
typedef struct {
	GLubyte r, g, b;
}Pixel;

class LightingModel{
private:
	Regions *regions;

	Image *image;

	RegionProperties *regProp;

	Normal *normal;

	/** A
	 * Variável contém os resultados do método de iluminação da galera.
	 * */
	float *data;

	Point3D lightVec, observVec; 					//normalized

	void lightVector(int, int); 					//compute the light vector
	void viewVector(int, int);						//compute the view vector
	Point3D reflectionVector (int, int);			//compute the reflection vector
	double innerProduct(Point3D*, int, int);		//compute the inner product between  two vector
	double innerProduct(Point3D*,Point3D*);
	double diffuseReflection(Point3D*,int,int,int);
	double specularReflection(Point3D*,int ,int ,int);
	double ambientReflection(int);

	void regionIllumination(int);

public:
	Point3D lightPosition, observPosition;
	double iL, iS, iA;
	Color lightColor;
	LightingModel(Regions*,Image*, Normal*, RegionProperties*);
	
	void imageIlluminationWithoutBack();
	void imageIllumination();
	void computeRegionIllumination(int);
	void drawPoints();
	
	void backGoundIllumination();
	void curvesIllumination();
	void regionIllumination();
	void changeLightPositions(int, int);
	void changeLightPositions(int);
	void changeLightColor(double, double, double);
	
	/**
	 * (Ailton) novos métodos e variáveis
	 */
	float *dataNew;
	static Point2D clickPoint;
	void drawNewPoints();
	Point3D reflectionVector_new(int p1x, int p1y);
	double innerProduct_new(Point3D *p,int p1x, int p1y);
	double diffuseReflection_new(Point3D *lightVec,int p1x, int p1y, int regPos);
	double specularReflection_new(Point3D *o,int p1x, int p1y,int regPos);
	void regionIllumination_new();
	void regionIllumination_new(int region);
	void imageIllumination_new();

	~LightingModel();
};


#endif /*LIGHTINGMODEL_HPP_*/
