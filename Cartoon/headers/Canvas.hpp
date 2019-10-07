#ifndef CANVAS_HPP_
#define CANVAS_HPP_

#include "idef.hpp"
#include "defs.hpp"
#include "Image.hpp"
#include "Regions.hpp"
#include "Normal.hpp"
#include "LightingModel.hpp"
#include "RegionProperties.hpp"
#include "Controller.hpp"

class Canvas : public Fl_Gl_Window {
private:
	Thinning *thin;
	Regions *region;
	Normal *normal;
	char eS;
	
	LightingModel *ligMod;
//	GLuint image_id;
    char exibitionMode;
    
public:
	Image *image;
	Canvas *canvas2;
	RegionProperties *regProp;
    int sr, sc, slc, x,y;
    bool object;
    
    Canvas(int X, int Y, int W, int H);			//ok
     
	void loadImage(const char*);		 		//ok
	void load(Image*);							//ok
	void setExibitionMode(char);
	void setSelectedCurve(int*,int*);			//ok
	void selectCurve();							//ok
	void setSelectedRegion(int*);				//ok
	void selectRegion(int);						//ok
	void selectRegion();						//ok
	void undoSelect();							//ok
	void discardSelection(bool);				//ok
	void cleanSelectedArea(int);				//ok
	void addBackGround(int);					//ok
	void bgColor(Color*);						//ok
	void removeFromBackGround(int);				//ok
	void changePointColor(int,int, int, int);	//ok
	void changeKa(int,double);					//ok
	void changeKs(int,double);					//ok
	void changeKd(int,double);					//ok
	void changeE(int,double);					//ok
	double getKa(int);							//ok
	double getKs(int);							//ok
	double getKd(int);							//ok
	double getE(int);							//ok
	bool getBackgroundValue(int);				//ok
	void doIt();								//ok
	void lightMode();
	void objectMode();							//ok
	void lightPosition(int, int);				//ok
	void lightPosition(int);					//ok
	void changelightColor(double, double,double);
	int handle(int event);
	
	void draw();

	// Ailton
//	void newLoad();

	~Canvas();
};

#endif /*CANVAS_HPP_*/
