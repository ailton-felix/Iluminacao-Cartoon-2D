#ifndef _IMAGE_
#define _IMAGE_

#include"../headers/MyDrawable.hpp"
#include"../headers/ImageException.hpp"
#include"../headers/Image.hpp"

using namespace std;

Image::Image(){
	im = NULL;
	imfile = NULL;
}

Image::Image(const char *fn){
	this->nameImage = fn;
	int error;
	im = NULL;
	imfile = imFileOpen(fn, &error);
	if (error){
		throw (new ImageException_);
	}
	im = imFileLoadImage (imfile, 0, &error);
	imFileClose(imfile);

	this->width = im->width;
	this->height = im->height;
}

void Image::showDataImage(){
    cout << "name: "<<nameImage<<endl;
	cout << "width: "<< im->width << endl;
	cout << "height: "<< im->height << endl;
	cout << "colorSpace: "<< im->color_space << endl;
	cout << "depth: "<< im->depth << endl;	
	cout << "dataType: "<< im->data_type << endl;
}

void Image::setBorder(imImage *imNew, unsigned char color){
	unsigned char *imgAux = (unsigned char*)imNew->data[0];
	for(int i=0;i<imNew->height;i++){
		imgAux[index(0,i)]=color;
		imgAux[index(imNew->width-1,i)]=color;
	}
	for(int i=1;i<imNew->width-1;i++){
		imgAux[index(i,0)]=color;
		imgAux[index(i,imNew->height-1)]=color;
	}	
}

bool Image::searchDepthOnePixel(int color,int *a,int *b,unsigned char *pImage){
	//unsigned char *pImage = (unsigned char*)im->data[0];
	for(int i=0;i<im->width;i++){
		for(int j=0;j<im->height;j++){
			if(pImage[index(i,j)]==(unsigned char)color){
				 *a=i;
				 *b=j;
				 return true; 
			}			
		}
	}
	return false;
}

void Image::putDataFile(char *fileName){
    ofstream outfile;
    outfile.open (fileName);
    int j=1;
    for(int i=0;i<im->height*im->width;i++){
            if(i%im->width==0){
                outfile<<"Linha: "<<j<<endl;
                j++;
            }         
            outfile << (int)((unsigned char*)(im->data[0]))[i]<<","  
                    << (int)((unsigned char*)(im->data[1]))[i]<<","
                    << (int)((unsigned char*)(im->data[2]))[i]<<endl;                     
    }
    outfile.close(); 
}
void Image::saveImage(char *nameImage,char *typeImage){
    int error;
    imFile* ifile = imFileNew(nameImage,typeImage, &error);
    error = imFileSaveImage(ifile, im);
    imFileClose(ifile);
}

imImage *Image::imageToThinning(){
	imImage *imageBinary=NULL;
	imageBinary = imImageCreate(im->width,im->height,IM_BINARY,im->data_type);
	//fazer o tratamento de erro depois ja que se nao ocorrer a conversao h� o retorno de um numero(error)
	imConvertColorSpace(im,imageBinary);
	setBorder(imageBinary,1);
	return imageBinary;
}

//by Allan
//imImage *Image::imageToThinning2(){
//	imImage *imageBinary=NULL;
//    int j=1;
//    for(int i=0;i<im->height*im->width;i++){     
//    	if((int)((unsigned char*)(im->data[0]))[i]!=0 && (int)((unsigned char*)(im->data[2]))[i]!=0 && (int)((unsigned char*)(im->data[3]))[i]){
//           (int)((unsigned char*)(im->data[0]))[i]=1;  
//           (int)((unsigned char*)(im->data[1]))[i]=1;
//           (int)((unsigned char*)(im->data[2]))[i]=1;                     
//    }
//}

void Image::draw(){
	glRasterPos2f(0.0f,0.0f);
    char *gl_data;  
    gl_data = NULL;
    gl_data = new char[im->width*im->height*im->depth];
        
    imConvertPacking(im->data[0], gl_data, im->width, im->height, im->depth, im->data_type, 0); /*we convert to unpacked because here is 
                                                                                              has packed data*/                                                                                                                                                                                 
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); /* data alignment must be 1 */
    glDrawPixels(im->width,im->height,GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)gl_data);    
}

Image::~Image(){
	delete []im->data;
}

#endif

