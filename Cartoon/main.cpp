#include <cstdlib>
#include <iostream>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/gl.h>
#include <FL/Fl_Gl_Window.H>
#include "headers/Controller.hpp"

using namespace std;

int main(int argc, char *argv[]){
    Controller *ctr = new Controller();
    ctr->show();

    cerr << "Running v5..." << endl;
    return (Fl::run());
}
