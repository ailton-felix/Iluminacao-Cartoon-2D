#ifndef _IMAGE_H
#define _IMAGE_H

#ifdef __cplusplus
#include<iostream>
#include<GL/gl.h>
#include<IM/im.h>
#include<fstream>
#include<vector>

#include <IM/im.h>
#include <IM/im_image.h>
#include <IM/im_convert.h>
#include <IM/im_process_loc.h>
#include <IM/im_process_pon.h>
#include "ImageException.hpp"
#include "MyDrawable.hpp"


class Image:public MyDrawable{
	protected:
		const char *nameImage;
		imFile *imfile;		
		ImageException_ ImageException;

	public:
	    imImage *im;
		int width,height,colorMode, dataType,depht;

		Image();        		
		Image(const char *fn);
		
		void showDataImage();
		bool searchDepthOnePixel(int color,int *a,int *b,unsigned char*);
		int  index(int i, int j){return (i + j*im->width);}
		void putDataFile(char *fileName);

		/* A
		 * Método que preenche as bordas da imagem (do quadro da imagem) com a
		 * cor definida pela variável 'color'.
		 * */
		void setBorder(imImage *imNew, unsigned char color);

		void saveImage(char *nameImage,char *typeImage);
		imImage *imageToThinning();
		void draw();		
		
		virtual ~Image();
};

#endif
#endif
