#include "../headers/Controller.hpp"

Fl_Menu_Item Controller:: menu_items[] = {
{ "&File", 0, 0, 0, FL_SUBMENU },
{ "&Open...", FL_ALT + 'o', (Fl_Callback *)cb_open, 0, 0 },
{ "&Save", FL_ALT + 's', 0},
{ "Save &As...", FL_ALT + FL_SHIFT + 's', 0, 0, 0 },
{ "&Quit", FL_ALT + 'q', (Fl_Callback *)cb_exit},
{ 0 },
{ "&View", 0, 0, 0, FL_SUBMENU },
{ "&Thinning", FL_ALT + 't', (Fl_Callback *)cb_lb_thin, 0, 0 },
{ "Curves", FL_ALT + 'c', (Fl_Callback *)cb_lb_curv},
{ "&Regions", FL_ALT + 'r', (Fl_Callback *)cb_lb_reg},
{ "&Normals", FL_ALT + 'n', (Fl_Callback *)cb_lb_norm},
{ "Normal &Map", FL_ALT + 'm', (Fl_Callback *)cb_lb_normMap},
{ "&Illumination", FL_ALT + 'i', (Fl_Callback *)cb_lb_illum},
{ "New NormalMap", FL_ALT + 'z', (Fl_Callback *)cb_lb_newNormalMap},
{ "New Illumination", FL_ALT + 'x', (Fl_Callback *)cb_lb_newIllum},
{ 0 },
{ "&Help", 0, 0, 0, FL_SUBMENU },
{ "&About", FL_ALT + 'a', 0},
{ 0 },
{ 0 }
};

/*A
 * Método que executa a função da sequência File->Open. Retornando o path da
 * imagem escolhida pelo usuário.
 */
char* Controller :: cb_openI() {
	char *novo;
	fileChooser = new Fl_File_Chooser("Open File", "Image file (*.{bmp,jpg,jpeg,gif,png})",
											 Fl_File_Chooser::SINGLE, "Choose image file"); 
	fileChooser->preview(0);
	fileChooser->show();
	
	while(fileChooser->visible())
		Fl::wait();
				
	novo = (char*)fileChooser->value();
	//cout << novo;
	return novo;
}

void Controller :: cb_open(Fl_Menu_* o, void*){
	char* fileName = ((Controller*)o->parent())->cb_openI();
	if(fileName){
		try	{
			((Controller*)o->parent())->cb_loadImage(fileName);
		}catch(exception &e){
			cerr << e.what() << endl;
		}
	}
}

void Controller :: cb_loadImage(char* l){
	initial();
	canvas->loadImage(l);
	bt_run->activate();
	bt_doIt->deactivate();
	if(lb_lig->value() == 1){
		lb_lig->value(0);
		lb_lig->deactivate();
		lb_obj->value(1);
	}
}

void Controller :: initial(){
	box_exib->deactivate();   lb_obj->deactivate();     lb_lig->deactivate();
	box_curves->deactivate(); sl_dist->deactivate();    sl_scale->deactivate();
	box_reg->deactivate();    color->deactivate();      box_tex->deactivate();
	bt_bumpM->deactivate();   bt_proc->deactivate();    bt_n->deactivate();
    bt_n1->deactivate();      box_illum->deactivate();  sl_ka->deactivate();
	sl_ks->deactivate();      sl_kd->deactivate();      sl_e->deactivate();
	box_viewOp->deactivate(); lb_thin->deactivate();    lb_reg->deactivate();
	lb_curv->deactivate();    lb_norm->deactivate();    lb_normMap->deactivate();
	lb_illum->deactivate();	  background->deactivate();	background->deactivate();
	bLight->hide();		
}

/**
 * Ailton - Botao Do Something
 */
//void Controller :: cb_doThings(Fl_Button* o, void*){
//	((Controller*)(o->parent()))->cb_doThingsI();
//}
//void Controller :: cb_doThingsI()
//{
//	canvasResult->newLoad();
//  redraw();
//}

void Controller :: cb_run(Fl_Button* o, void*){
	((Controller*)(o->parent()))->cb_runI();
}
void Controller :: cb_runI(){
	canvasResult->object = 1;   ///imagem pronta para abrir
	canvasResult->load(canvas->image); 	canvasResult->show();
	canvas->canvas2 = canvasResult; 	canvasResult->canvas2 = canvasResult; 
	/******************          screen           ******************/
	cb_objectModeI();	  
	lb_obj->activate();     lb_lig->activate();
	lb_illum->value(0);     lb_illum->activate();   lb_obj->value(1);	     lb_obj->activate();
	lb_thin->value(0);      lb_thin->activate();    lb_lig->value(0);      lb_lig->activate();
	lb_reg->value(0);       lb_reg->activate();     lb_curv->value(0);     lb_curv->activate();
	lb_norm->value(0);      lb_norm->activate();    lb_normMap->value(0);  lb_normMap->activate();
	sl_dist->activate();    sl_scale->activate();   color->activate();
	sl_ka->activate();      sl_ks->activate();      sl_kd->activate();     sl_e->activate();
//	bt_bumpM->activate();   bt_proc->activate();    bt_n->activate();      bt_n1->activate(); ///texture
	box_prop->activate();   box_exib->activate();   box_illum->activate();
	box_tex->activate();    box_viewOp->activate(); box_reg->activate();    box_curves->activate();
	bt_run->deactivate();   bt_doIt->activate();    background->activate(); //bt_doThings->activate();
	lb_new_normalMap->value(0); lb_new_normalMap->activate(); lb_lb_newIllum->value(1); lb_lb_newIllum->activate();
//	canvasResult->regionsColor(); 
	cout << "Ran\n";
}
void Controller :: refresh(){
	if(lb_obj->value() == 1){
		if(canvas->sr != -1){
			sl_ka->value(canvasResult->getKa(canvas->sr));
			sl_ks->value(canvasResult->getKs(canvas->sr));
			sl_kd->value(canvasResult->getKd(canvas->sr));
			sl_e->value(canvasResult->getE(canvas->sr));
			background->value(canvasResult->getBackgroundValue(canvas->sr));
		}
	}
}

/****************              light              ******************/
void Controller :: cb_lightMode(Fl_Button* o, void*){
	((Controller*)(o->parent()))->cb_lightModeI();
}
void Controller :: cb_lightModeI(){
	canvas->lightMode();
	canvasResult->hide();
	lb_lig->value(1);
	lb_obj->value(0);		bt_doIt->deactivate();	cC->show();
	ok->show();				bLight->show();			b->hide();
	box_exib->activate();	box_curves->hide();		box_illum->hide();
	box_tex->hide();        box_viewOp->hide();  	lb_illum->hide();
	lb_lig->activate();     lb_thin->hide();     	lb_reg->hide();
	lb_curv->hide();        lb_norm->hide();     	lb_normMap->hide();
	sl_dist->hide();        sl_scale->hide();    	bt_bumpM->hide();
	bt_proc->hide();        bt_n->hide();        	bt_n1->hide();
	sl_ka->hide();			sl_ks->hide(); 			sl_kd->hide();
	sl_e->hide(); 			background->hide();		box_reg->hide();
	color->hide();	        lb_new_normalMap->hide();			lb_lb_newIllum->hide();
	
	if(lb_lig->value() == 1){
		cC->position((int)(wid*0.2), (int)(hei*0.86));
		ok->position((int)(int)(wid*0.378), (int)(hei*0.96));
	}					
}
/*******************************************************************/
/****************          light/object           ******************/

void Controller :: cb_okI(){
	int red, green, blue;
	red = (int)(cC->r()*255.0); green = (int)(cC->g()*255.0);blue = (int)(cC->b()*255.0);
	if(lb_obj->value()== 1){
		if(canvas->sr != -1){
			canvas->changePointColor(canvas->sr,red, green, blue);
			bt_doIt->activate();
			cC->hide(); 	ok->hide();		background->activate();
		}
	}else if(lb_lig->value() == 1){
		canvas->changelightColor(cC->r(), cC->g(),cC->b());
		}
	b->hide(); 				
	box_prop->activate();   box_exib->activate();   box_curves->activate();
	box_reg->activate();    box_tex->activate();    box_viewOp->activate();
	box_illum->activate();  lb_obj->activate();     lb_lig->activate();
	lb_thin->activate();    lb_reg->activate();     lb_curv->activate();
	lb_norm->activate();    lb_normMap->activate(); lb_illum->activate();
	sl_ka->activate();      sl_ks->activate();      sl_kd->activate();
	sl_e->activate();   	sl_dist->activate();    sl_scale->activate();
	lb_new_normalMap->activate();	lb_lb_newIllum->activate();
//	bt_bumpM->activate();	bt_proc->activate();    bt_n->activate();       bt_n1->activate()
}
void Controller :: cb_ok(Fl_Button* o, void*){
	((Controller*)(o->parent()))->cb_okI();
}
void Controller :: cb_colorI(){
	b->show();				cC->show();				  ok->show();
	box_prop->deactivate();	box_exib->deactivate();	  box_curves->deactivate();
	box_reg->deactivate();	box_tex->deactivate();	  box_viewOp->deactivate();
	lb_obj->deactivate();	lb_lig->deactivate(); 	  lb_thin->deactivate();
	lb_curv->deactivate();	lb_norm->deactivate();	  lb_normMap->deactivate();
	lb_illum->deactivate();	bt_run->deactivate();	  lb_reg->deactivate();
	sl_ka->deactivate();	sl_ks->deactivate();	  sl_kd->deactivate();
	sl_dist->deactivate();	sl_scale->deactivate();	  bt_bumpM->deactivate();
	bt_proc->deactivate();	bt_n->deactivate();		  bt_n1->deactivate();
	sl_e->deactivate();		background->deactivate(); lb_new_normalMap->deactivate();
	lb_lb_newIllum->deactivate();
}

void Controller :: cb_color(Fl_Button* o, void*){
	((Controller*)(o->parent()))->cb_colorI();
}
/*******************************************************************/
/********************          object      *************************/
void Controller :: cb_objectMode(Fl_Button* o, void*){
	((Controller*)(o->parent()))->cb_objectModeI();
}
void Controller :: cb_objectModeI(){
	lb_lig->value(0);
	canvas->objectMode();
	canvasResult->show();
	bt_doIt->activate();
	color->show();
	
	box_reg->show();
	sl_ka->hide();				background->activate();
	sl_ka->show();				sl_ks->show();			sl_kd->show();
	sl_e->show();				box_exib->activate();  	box_curves->show();		
	box_illum->show();			box_tex->show();		box_viewOp->show(); 	
	lb_illum->show();			lb_lig->activate();    	lb_thin->show();    	
	lb_reg->show();				lb_curv->show();       	lb_norm->show();    	
	lb_normMap->show();			sl_dist->show();       	sl_scale->show();   	
	bt_bumpM->show();			bt_proc->show();       	bt_n->show();
	bt_n1->show();				background->show();		bLight->hide();
	ok->hide();					b->hide();				cC->hide();
	lb_new_normalMap->show();	lb_lb_newIllum->show();

	if(lb_obj->value() == 1){
		cC->position((int)(wid*0.415), (int)(hei*0.837));
		ok->position((int)(wid*0.563), (int)(hei*0.967));
	}
}

void Controller :: cb_ka(Fl_Slider* o, void*){
	((Controller*)(o->parent()))->cb_kaI();
}
void Controller :: cb_kaI(){
	if(lb_obj->value() == 1){
		if(canvas->sr != -1){
			canvasResult->changeKa(canvas->sr, sl_ka->value());
		}
	}
}
void Controller :: cb_ks(Fl_Slider* o, void*){
	((Controller*)(o->parent()))->cb_ksI();
}
void Controller :: cb_ksI(){
	if(lb_obj->value() == 1){
		if(canvas->sr != -1){
			canvasResult->changeKs(canvas->sr,sl_ks->value());
		}
	}
}
void Controller :: cb_kd(Fl_Slider* o, void*){
	((Controller*)(o->parent()))->cb_kdI();
}
void Controller :: cb_kdI(){
	if(lb_obj->value() == 1){
		if(canvas->sr != -1){
			canvasResult->changeKd(canvas->sr,sl_kd->value());
		}
	}
}
void Controller :: cb_e(Fl_Slider* o, void*){
	((Controller*)(o->parent()))->cb_eI();
}
void Controller :: cb_eI(){
	if(lb_obj->value() == 1){
		if(canvas->sr != -1){
			canvasResult->changeE(canvas->sr,sl_e->value());
		}
	}
}
void Controller :: cb_doIt(Fl_Button* o, void*){
	((Controller*)(o->parent()))->cb_doItI();
}
void Controller :: cb_doItI(){
	canvas->undoSelect();
	canvasResult->doIt();
}
void Controller :: cb_lb_thin(Fl_Button* o, void*){
	((Controller*)(o->parent()))->cb_lb_thinI();
}
void Controller :: cb_lb_thinI(){
	canvasResult->setExibitionMode('t');
	lb_thin->value(1); lb_obj->value(1);
	lb_lig->value(0);lb_reg->value(0);lb_curv->value(0);
	lb_norm->value(0);lb_normMap->value(0);lb_illum->value(0);lb_new_normalMap->value(0);
	lb_lb_newIllum->value(0);
}
void Controller :: cb_lb_reg(Fl_Button* o, void*){
	((Controller*)(o->parent()))->cb_lb_regI();
}
void Controller :: cb_lb_regI(){
	canvasResult->setExibitionMode('r');
	lb_thin->value(0); lb_obj->value(1);
	lb_lig->value(0);lb_reg->value(1);lb_curv->value(0);
	lb_norm->value(0);lb_normMap->value(0);lb_illum->value(0);lb_new_normalMap->value(0);
	lb_lb_newIllum->value(0);
}
void Controller :: cb_lb_curv(Fl_Button* o, void*){
	((Controller*)(o->parent()))->cb_lb_curvI();
}
void Controller :: cb_lb_curvI(){
	canvasResult->setExibitionMode('c');
	lb_thin->value(0); lb_obj->value(1);
	lb_lig->value(0);lb_reg->value(0);lb_curv->value(1);
	lb_norm->value(0);lb_normMap->value(0);lb_illum->value(0);lb_new_normalMap->value(0);
	lb_lb_newIllum->value(0);
}
void Controller :: cb_lb_norm(Fl_Button* o, void*){
	((Controller*)(o->parent()))->cb_lb_normI();
}
void Controller :: cb_lb_normI(){
	canvasResult->setExibitionMode('n');
	lb_thin->value(0); lb_obj->value(1);
	lb_lig->value(0);lb_reg->value(0);lb_curv->value(0);
	lb_norm->value(1);lb_normMap->value(0);lb_illum->value(0);lb_new_normalMap->value(0);
	lb_lb_newIllum->value(0);
}
void Controller :: cb_lb_normMap(Fl_Button* o, void*){
	((Controller*)(o->parent()))->cb_lb_normMapI();
}
void Controller :: cb_lb_normMapI(){
	canvasResult->setExibitionMode('m');
	lb_thin->value(0); lb_obj->value(1);
	lb_lig->value(0);lb_reg->value(0);lb_curv->value(0);
	lb_norm->value(0);lb_normMap->value(1);lb_illum->value(0);lb_new_normalMap->value(0);
	lb_lb_newIllum->value(0);
}
void Controller :: cb_lb_illum(Fl_Button* o, void*){
	((Controller*)(o->parent()))->cb_lb_illumI();
}
void Controller :: cb_lb_illumI(){
	canvasResult->setExibitionMode('i');
	lb_thin->value(0); lb_obj->value(1);
	lb_lig->value(0);lb_reg->value(0);lb_curv->value(0);
	lb_norm->value(0);lb_normMap->value(0);lb_illum->value(1);lb_new_normalMap->value(0);
	lb_lb_newIllum->value(0);
}
void Controller :: cb_lb_newIllum(Fl_Button* o, void*){
	((Controller*)(o->parent()))->cb_lb_newIllumI();
}
void Controller :: cb_lb_newIllumI(){
	canvasResult->setExibitionMode('x');
	lb_thin->value(0); lb_obj->value(1);
	lb_lig->value(0);lb_reg->value(0);lb_curv->value(0);
	lb_norm->value(0);lb_normMap->value(0);lb_illum->value(0);lb_new_normalMap->value(0);
	lb_lb_newIllum->value(1);
}
void Controller :: cb_lb_newNormalMap(Fl_Button* o, void*){
	((Controller*)(o->parent()))->cb_lb_newNormalMapI();
}
void Controller :: cb_lb_newNormalMapI(){
	canvasResult->setExibitionMode('z');
	lb_thin->value(0); lb_obj->value(1);
	lb_lig->value(0);lb_reg->value(0);lb_curv->value(0);
	lb_norm->value(0);lb_normMap->value(0);lb_illum->value(0);lb_new_normalMap->value(1);
	lb_lb_newIllum->value(0);
}
void Controller :: cb_addBG(Fl_Check_Button* o, void*){
	((Controller*)(o->parent()))->cb_addBGI();
}
void Controller :: cb_addBGI(){
	if((int)background->value() == 0)
		canvas->removeFromBackGround(canvas->sr);
	else canvas->addBackGround(canvas->sr);
}
/*******************************************************************/
void Controller :: cb_exit(Fl_Widget* w, void*){
	exit(0);
}

Controller :: Controller() : Fl_Double_Window(0,0,1,1, "Cartoon Project") {
	this->fullscreen();
//	this->fullscreen_off(0, 0, 1300, 700); //TODO Tamanho da janela do programa.
	wid = w(); hei = h()-25;
	this->resize(0,0,wid,hei);

	canvas = new Canvas(0,(int)(hei*0.035),(int)(wid*0.498), (int)(hei*0.8)); 
	canvasResult= new Canvas((int)(wid*0.5),(int)(hei*0.035),(int)(wid*0.498), (int)(hei*0.8));	canvasResult->hide();
	
	{Fl_Menu_Bar* menu_bar = new Fl_Menu_Bar(0, 0, wid, (int)(hei*0.035));
		menu_bar->box(FL_PLASTIC_THIN_UP_BOX);
  		menu_bar->color((Fl_Color)42);
        menu_bar->down_box(FL_PLASTIC_UP_BOX);
        menu_bar->selection_color((Fl_Color)35);
        menu_bar->menu(menu_items);
	}
	{ /*bt_doThings = new Fl_Button((int)(wid*0.263), 0, (int)(wid*0.080), (int)(hei*0.035), "Do Something");
	  bt_doThings->deactivate();
	  bt_doThings->box(FL_PLASTIC_THIN_UP_BOX);
	  bt_doThings->color((Fl_Color)35);
	  bt_doThings->callback((Fl_Callback*)cb_doThings);*/
	}
	{ bt_run = new Fl_Button((int)(wid*0.127), 0, (int)(wid*0.068), (int)(hei*0.035), "Run");
	  bt_run->deactivate();
	  bt_run->box(FL_PLASTIC_THIN_UP_BOX);
	  bt_run->color((Fl_Color)35);
	  bt_run->callback((Fl_Callback*)cb_run);
	}
	{ bt_doIt = new Fl_Button((int)(wid*0.195), 0, (int)(wid*0.068), (int)(hei*0.035), "Do It!");
	  bt_doIt->deactivate();
	  bt_doIt->box(FL_PLASTIC_THIN_UP_BOX);
	  bt_doIt->color((Fl_Color)35);
	  bt_doIt->callback((Fl_Callback*)cb_doIt);
	} 
	{ box_prop = new Fl_Box(0, (int)(hei*0.833), (int)(wid*1.01), (int)(hei*0.172));
	  box_prop->box(FL_PLASTIC_THIN_UP_BOX);
	  box_prop->labeltype(FL_NO_LABEL);
	} // Fl_Box* o
	{ box_exib = new Fl_Box((int)(wid*0.004), (int)(hei*0.837), (int)(wid*0.107), (int)(hei*0.162),"Exibition Mode");
	  box_exib->box(FL_THIN_DOWN_FRAME);
	  box_exib->color((Fl_Color)35);
	  box_exib->labelfont(1);
	  box_exib->labelsize(12);
	  box_exib->align(FL_ALIGN_TOP_LEFT|FL_ALIGN_INSIDE);
	  box_exib->deactivate();
	} // Fl_Box* o
	{ lb_obj = new Fl_Light_Button((int)(wid*0.014), (int)(hei*0.882), (int)(wid*0.073), (int)(hei*0.035), "Object");
	  lb_obj->labelsize(12);
	  lb_obj->box(FL_PLASTIC_THIN_UP_BOX);
	  lb_obj->color((Fl_Color)35);
	  lb_obj->callback((Fl_Callback*)cb_objectMode);
	  lb_obj->deactivate();
	} // Fl_Light_Button* o
	{ lb_lig = new Fl_Light_Button((int)(wid*0.014), (int)(hei*0.932), (int)(wid*0.073), (int)(hei*0.035), "Light");
	  lb_lig->labelsize(12);
	  lb_lig->box(FL_PLASTIC_THIN_UP_BOX);
	  lb_lig->color((Fl_Color)35);
	  lb_lig->callback((Fl_Callback*)cb_lightMode);
	  lb_lig->deactivate();
	} // Fl_Light_Button* o
	{ box_curves = new Fl_Box((int)(wid*0.122), (int)(hei*0.837), (int)(wid*0.185), (int)(hei*0.162), "Curves");
	  box_curves->box(FL_THIN_DOWN_FRAME);
	  box_curves->labelfont(1);
	  box_curves->labelsize(12);
	  box_curves->align(FL_ALIGN_TOP_LEFT|FL_ALIGN_INSIDE);
	  box_curves->deactivate();
	} // Fl_Box* o
	{ sl_dist = new Fl_Value_Slider((int)(wid*0.146),(int)(hei*0.896),(int)(wid*0.146),(int)(hei*0.021),"distance");
	  sl_dist->labelsize(12);
	  sl_dist->step(0.01);
	  sl_dist->box(FL_PLASTIC_UP_BOX);
	  sl_dist->type(FL_HOR_NICE_SLIDER);
	  sl_dist->maximum(1);
	  sl_dist->align(FL_ALIGN_TOP|FL_ALIGN_LEFT|FL_ALIGN_CLIP);
	  sl_dist->deactivate();
	} 
	{ sl_scale = new Fl_Value_Slider((int)(wid*0.146),(int)(hei*0.95),(int)(wid*0.146),(int)(hei*0.021),"Z-scale");
	  sl_scale->labelsize(12);
	  sl_scale->step(0.01);
	  sl_scale->box(FL_PLASTIC_UP_BOX);
	  sl_scale->type(FL_HOR_NICE_SLIDER);
	  sl_scale->maximum(1);
	  sl_scale->align(FL_ALIGN_TOP|FL_ALIGN_LEFT|FL_ALIGN_CLIP);
	  sl_scale->deactivate();
	}
	{ box_reg = new Fl_Box((int)(wid*0.317), (int)(hei*0.837), (int)(wid*0.433), (int)(hei*0.162), "Regions");
	  box_reg->box(FL_THIN_DOWN_FRAME);
	  box_reg->labelfont(1);
	  box_reg->labelsize(12);
	  box_reg->align(FL_ALIGN_TOP_LEFT|FL_ALIGN_INSIDE);
	  box_reg->deactivate();
	} // Fl_Box* o
	{ color = new Fl_Button((int)(wid*0.332), (int)(hei*0.889), (int)(wid*0.058), (int)(hei*0.07), "Color");
	  color->labelsize(12);
	  color->box(FL_PLASTIC_THIN_UP_BOX);
	  color->down_box(FL_PLASTIC_THIN_DOWN_BOX);
	  color->color((Fl_Color)35);
	  color->align(FL_ALIGN_TOP_LEFT);
	  color->callback((Fl_Callback*)cb_color);
	  color->deactivate();
	} // Fl_Button* o
	{ background = new Fl_Check_Button((int)(wid*0.325), (int)(hei*0.96), 25, 25, "Background");
  	  background->labelsize(12);
  	  background->callback((Fl_Callback*)cb_addBG);
  	  background->deactivate();
	}
	{ box_illum = new Fl_Box((int)(wid*0.41), (int)(hei*0.85), (int)(wid*0.185), (int)(hei*0.15), "Proprieties");
	  box_illum->box(FL_ENGRAVED_FRAME);
	  box_illum->labelfont(1);
	  box_illum->labelsize(12);
	  box_illum->align(FL_ALIGN_TOP_LEFT|FL_ALIGN_INSIDE);
	  box_illum->deactivate();
	}
	{ sl_ka = new Fl_Value_Slider((int)(wid*0.435), (int)(hei*0.88), (int)(wid*0.15), (int)(hei*0.021), "ka");
	  sl_ka->labelsize(12);
	  sl_ka->step(0.01);
	  sl_ka->box(FL_PLASTIC_UP_BOX);
	  sl_ka->type(FL_HOR_NICE_SLIDER);
	  sl_ka->maximum(1);
	  sl_ka->align(FL_ALIGN_LEFT);
	  sl_ka->callback((Fl_Callback*)cb_ka);
	  sl_ka->deactivate();
	} // Fl_Output* o
	{ sl_ks = new Fl_Value_Slider((int)(wid*0.435), (int)(hei*0.91), (int)(wid*0.15), (int)(hei*0.021), "ks");
	  sl_ks->labelsize(12);
	  sl_ks->step(0.01);
	  sl_ks->box(FL_PLASTIC_UP_BOX);
	  sl_ks->type(FL_HOR_NICE_SLIDER);
	  sl_ks->maximum(1);
	  sl_ks->align(FL_ALIGN_LEFT);
	  sl_ks->callback((Fl_Callback*)cb_ks);
	  sl_ks->deactivate();
	} 
	{ sl_kd = new Fl_Value_Slider((int)(wid*0.435), (int)(hei*0.937), (int)(wid*0.15), (int)(hei*0.021), "kd");
	  sl_kd->labelsize(12);
	  sl_kd->step(0.01);
	  sl_kd->box(FL_PLASTIC_UP_BOX);
	  sl_kd->type(FL_HOR_NICE_SLIDER);
	  sl_kd->maximum(1);
	  sl_kd->align(FL_ALIGN_LEFT);
	  sl_kd->callback((Fl_Callback*)cb_kd);
	  sl_kd->deactivate();
	} 
	{ sl_e = new Fl_Value_Slider((int)(wid*0.435), (int)(hei*0.965), (int)(wid*0.15), (int)(hei*0.021), "e");
	  sl_e->labelsize(12);
	  sl_e->step(10);
	  sl_e->box(FL_PLASTIC_UP_BOX);
	  sl_e->type(FL_HOR_NICE_SLIDER);
	  sl_e->maximum(300);
	  sl_e->align(FL_ALIGN_LEFT);
	  sl_e->callback((Fl_Callback*)cb_e);
	  sl_e->deactivate();
	}
	{ box_tex = new Fl_Box((int)(wid*0.6), (int)(hei*0.85)/*(int)(hei*0.857)*/, (int)(wid*0.14), (int)(hei*0.15), "Texture");
	  box_tex->box(FL_ENGRAVED_FRAME);
	  box_tex->labelfont(1);
	  box_tex->labelsize(12);
	  box_tex->align(FL_ALIGN_TOP_LEFT|FL_ALIGN_INSIDE);
	  box_tex->deactivate();
	} // Fl_Box* o
	{ bt_bumpM=new Fl_Round_Button((int)(wid*0.61),(int)(hei*0.88),(int)(wid*0.062),(int)(hei*0.021),"Bump Mapping");
	  bt_bumpM->labelsize(12);
	  bt_bumpM->down_box(FL_ROUND_DOWN_BOX);
	  bt_bumpM->deactivate();
	} // Fl_Round_Button* o
	{ bt_proc = new Fl_Round_Button((int)(wid*0.61), (int)(hei*0.91), (int)(wid*0.062),(int)(hei*0.021),"Procedural");
	  bt_proc->labelsize(12);
	  bt_proc->down_box(FL_ROUND_DOWN_BOX);
	  bt_proc->deactivate();
	} // Fl_Round_Button* o
	{ bt_n = new Fl_Round_Button((int)(wid*0.61), (int)(hei*0.937), (int)(wid*0.062), (int)(hei*0.021), "n");
	  bt_n->labelsize(12);
	  bt_n->down_box(FL_ROUND_DOWN_BOX);
	  bt_n->deactivate();
	} // Fl_Round_Button* o
	{ bt_n1 = new Fl_Round_Button((int)(wid*0.61), (int)(hei*0.965), (int)(wid*0.062), (int)(hei*0.021), "n1");
	  bt_n1->labelsize(12);
	  bt_n1->down_box(FL_ROUND_DOWN_BOX);
	  bt_n1->deactivate();
	} // Fl_Round_Button* o
	
	{ box_viewOp = new Fl_Box((int)(wid*0.761), (int)(hei*0.837), (int)(wid*0.234),(int)(hei*0.162),"View Options");
	  box_viewOp->box(FL_THIN_DOWN_FRAME);
	  box_viewOp->labelfont(1);
	  box_viewOp->labelsize(12);
	  box_viewOp->align(FL_ALIGN_TOP_LEFT|FL_ALIGN_INSIDE);
	  box_viewOp->deactivate();
	} // Fl_Box* o
	{ lb_thin = new Fl_Light_Button((int)(wid*0.781),(int)(hei*0.862),(int)(wid*0.083),(int)(hei*0.028),"Thinning");
	  lb_thin->labelsize(12);
	  lb_thin->box(FL_PLASTIC_THIN_UP_BOX);
	  lb_thin->color((Fl_Color)35);
	  lb_thin->callback((Fl_Callback*)cb_lb_thin);
	  lb_thin->deactivate();
	} // Fl_Light_Button* o
	{ lb_reg = new Fl_Light_Button((int)(wid*0.781), (int)(hei*0.898),(int)(wid*0.083),(int)(hei*0.028),"Regions");
	  lb_reg->labelsize(12);
	  lb_reg->box(FL_PLASTIC_THIN_UP_BOX);
	  lb_reg->color((Fl_Color)35);
	  lb_reg->callback((Fl_Callback*)cb_lb_reg);
	  lb_reg->deactivate();
	} // Fl_Light_Button* o
	{ lb_curv = new Fl_Light_Button((int)(wid*0.781), (int)(hei*0.933), (int)(wid*0.083), (int)(hei*0.028),"Curves");
	  lb_curv->labelsize(12);
	  lb_curv->box(FL_PLASTIC_THIN_UP_BOX);
	  lb_curv->color((Fl_Color)35);
	  lb_curv->callback((Fl_Callback*)cb_lb_curv);
	  lb_curv->deactivate();
	} // Fl_Light_Button* o
	{ lb_norm = new Fl_Light_Button((int)(wid*0.888), (int)(hei*0.862), (int)(wid*0.087),(int)(hei*0.028),"Normals");
	  lb_norm->labelsize(12);
	  lb_norm->box(FL_PLASTIC_THIN_UP_BOX);
	  lb_norm->color((Fl_Color)35);
	  lb_norm->callback((Fl_Callback*)cb_lb_norm);
	  lb_norm->deactivate();
	} // Fl_Light_Button* o
	{ lb_normMap=new Fl_Light_Button((int)(wid*0.888),(int)(hei*0.898),(int)(wid*0.087),(int)(hei*0.028),"NormalMap");
	  lb_normMap->labelsize(12);
	  lb_normMap->box(FL_PLASTIC_THIN_UP_BOX);
	  lb_normMap->color((Fl_Color)35);
	  lb_normMap->callback((Fl_Callback*)cb_lb_normMap);
	  lb_normMap->deactivate();
	} // Fl_Light_Button* o
	{ lb_illum=new Fl_Light_Button((int)(wid*0.888),(int)(hei*0.933),(int)(wid*0.087),(int)(hei*0.028),"Illumination");
	  lb_illum->labelsize(12);
	  lb_illum->box(FL_PLASTIC_THIN_UP_BOX);
	  lb_illum->color((Fl_Color)35);
	  lb_illum->callback((Fl_Callback*)cb_lb_illum);
	  lb_illum->deactivate();
	} // Fl_Light_Button* o
	{ lb_lb_newIllum=new Fl_Light_Button((int)(wid*0.888),(int)(hei*0.968),(int)(wid*0.087),(int)(hei*0.028),"New Illumination");
		lb_lb_newIllum->labelsize(12);
		lb_lb_newIllum->box(FL_PLASTIC_THIN_UP_BOX);
		lb_lb_newIllum->color((Fl_Color)35);
		lb_lb_newIllum->callback((Fl_Callback*)cb_lb_newIllum);
		lb_lb_newIllum->deactivate();
	} // Fl_Light_Button* o
	{ lb_new_normalMap=new Fl_Light_Button((int)(wid*0.781),(int)(hei*0.968),(int)(wid*0.083),(int)(hei*0.028),"New NormalMap");
	  lb_new_normalMap->labelsize(12);
	  lb_new_normalMap->box(FL_PLASTIC_THIN_UP_BOX);
	  lb_new_normalMap->color((Fl_Color)35);
	  lb_new_normalMap->callback((Fl_Callback*)cb_lb_newNormalMap);
	  lb_new_normalMap->deactivate();
	} // Fl_Light_Button* o
	{ bLight = new Fl_Box((int)(wid*0.122), (int)(hei*0.837), (int)(wid*0.377), (int)(hei*0.162), "Light");
	  bLight->box(FL_THIN_DOWN_FRAME);
	  bLight->labelfont(1);
	  bLight->labelsize(12);
	  bLight->align(FL_ALIGN_TOP_LEFT|FL_ALIGN_INSIDE);
	  bLight->show();
	  bLight->hide();
	}
	{ b = new Fl_Box((int)(wid*0.41), (int)(hei*0.834), (int)(wid*0.185), (int)(hei*0.165));
	  b->box(FL_THIN_DOWN_BOX);
	  b->hide();
	  cC = new Fl_Color_Chooser((int)(wid*0.415), (int)(hei*0.837),(int)(wid*0.175),(int)(hei*0.127));
/*	  cC = new Fl_Color_Chooser((int)(wid*0.2), (int)(hei*0.86),(int)(wid*0.175),(int)(hei*0.127));
	  ok = new Fl_Button((int)(wid*0.378), (int)(hei*0.96), (int)(wid*0.027), (int)(hei*0.028), "OK");*/
	  cC->hide();
	  ok = new Fl_Button((int)(wid*0.563), (int)(hei*0.967), (int)(wid*0.027), (int)(hei*0.028), "OK");
	  ok->labelfont(1);
	  ok->hide();
	  ok->callback((Fl_Callback*)cb_ok);
	  end(); 
	}


	
	
	end();
}

void Controller ::changeViewMode(){
	if(lb_obj->value() == 1){
		lb_obj->value(0);
		cb_lightModeI();
		lb_lig->value(1);
	}
	else{
		lb_obj->value(1);
		cb_objectModeI();
		lb_lig->value(0);
	}
}

int Controller :: handle(int event){
	if(canvasResult){
		switch(event){
			case FL_PUSH:
				if(canvasResult->canvas2){
					if(Fl::event_clicks() == 1){
						if(Fl::event_button() == 3 && 0 < Fl::event_x() && Fl::event_x()<(int)(wid*0.498) 
									 && (int)(hei*0.035)< Fl::event_y() && Fl::event_y()<(int)(hei*0.8)){
							changeViewMode();
						}
					}
				}
				if(Fl::event_button() == 1){
					refresh();
					redraw();
				}break;
		}
	} 
	return Fl_Double_Window::handle(event);
}

Controller :: ~Controller(){
	
	delete canvas;	delete canvasResult;  delete fileChooser;  delete cC;
	delete color;   delete box_curves;    delete box_prop;     delete box_exib;
	delete ok; 		delete bt_run;        delete bt_doIt;      delete b;  
	delete lb_obj;  delete bt_bumpM;      delete box_reg;      delete box_tex; 
	delete lb_lig;  delete box_viewOp; 	  delete lb_thin;      delete lb_reg; 
	delete lb_curv; delete lb_norm;       delete lb_normMap;   delete lb_illum;
	delete sl_ka;   delete sl_ks;         delete sl_kd;        delete sl_e; 
	delete sl_dist; delete sl_scale;      delete bt_proc;      delete bt_n; 
	delete bt_n1;   delete lb_new_normalMap;
}
