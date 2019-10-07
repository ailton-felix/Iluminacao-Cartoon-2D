#ifndef CONTROLLER_HPP_
#define CONTROLLER_HPP_

#include "Canvas.hpp"
class Canvas;
class Controller : public Fl_Double_Window {
public:
	int wid, hei;
	Controller();
	~Controller();
	Canvas *canvas, *canvasResult;
	Fl_File_Chooser *fileChooser;
	Fl_Color_Chooser* cC;
	Fl_Check_Button *background;
	Fl_Button *ok,*color,*bt_run, *bt_doIt, *bt_doThings;
	Fl_Box *box_prop, *box_exib, *box_curves,*box_reg,*box_tex,*box_viewOp,*box_illum,*b, *bLight;
	Fl_Light_Button *lb_obj,*lb_lig,*lb_thin,*lb_reg,*lb_curv,*lb_norm,*lb_normMap,*lb_illum,*lb_new_normalMap, *lb_lb_newIllum;
	Fl_Value_Slider *sl_ka,*sl_ks,*sl_kd,*sl_e,*sl_dist,*sl_scale;
	Fl_Round_Button *bt_bumpM ,*bt_proc,*bt_n,*bt_n1;
	int handle(int);
	void changeViewMode();
	void initial();
	void refresh();
 
private:
	static Fl_Menu_Item menu_items[];
	static void cb_open(Fl_Menu_* o, void*);
	char* cb_openI();
	static void cb_exit(Fl_Widget* w, void*);
	static void cb_color(Fl_Button* w, void*);
	void cb_colorI();
	void cb_loadImage(char*);
	static void cb_ok(Fl_Button* w, void*);
	void cb_okI();
//	static void cb_doThings(Fl_Button* w, void*);
//	void cb_doThingsI();
	static void cb_run(Fl_Button* w, void*);
	void cb_runI();
	static void cb_lb_thin(Fl_Button* o, void*);
	void cb_lb_thinI();
	static void cb_lb_reg(Fl_Button* o, void*);
	void cb_lb_regI();
	static void cb_lb_curv(Fl_Button* o, void*);
	void cb_lb_curvI();
	static void cb_lb_norm(Fl_Button* o, void*);
	void cb_lb_normI();
	static void cb_lb_normMap(Fl_Button* o, void*);
	void cb_lb_normMapI();
	static void cb_lb_illum(Fl_Button* o, void*);
	void cb_lb_illumI();
	static void cb_lb_newIllum(Fl_Button* o, void*);
	void cb_lb_newIllumI();
	static void cb_lb_newNormalMap(Fl_Button* o, void*);
	void cb_lb_newNormalMapI();
	static void cb_doIt(Fl_Button* o, void*);
	void cb_doItI();
	static void cb_ka(Fl_Slider* o, void*);
	void cb_kaI();
	static void cb_ks(Fl_Slider* o, void*);
	void cb_ksI();
	static void cb_kd(Fl_Slider* o, void*);
	void cb_kdI();
	static void cb_e(Fl_Slider* o, void*);
	void cb_eI();
	static void cb_lightMode(Fl_Button* o, void*);
	void cb_lightModeI();
	static void cb_objectMode(Fl_Button* o, void*);
	void  cb_objectModeI();
	static void cb_addBG(Fl_Check_Button* o, void*);
	void  cb_addBGI();	
};


#endif /*CONTROLLER_HPP_*/
