#include "../headers/Thinning.hpp"

Thinning::Thinning(Image *image)
{    
	  imNew = image;	
	  imNew = new Image();
	  imNew->im = imImageCreate(image->im->width,image->im->height,IM_BINARY,image->im->data_type); 
	  imConvertColorSpace(image->im,imNew->im);
	  imNew->width = imNew->im->width;
	  imNew->height = imNew->im->height;
	  
}

int Thinning::index(int i, int j){
      return (i + j*(imNew->im->width)) ;
}

void  Thinning::extractNeighboors(unsigned char* image,int i, int j,int *p){      
      p[0]=(int)image[index(i,j+1)];
      p[1]=(int)image[index(i+1,j+1)];
      p[2]=(int)image[index(i+1,j)]; 
      p[3]=(int)image[index(i+1,j-1)];   
      p[4]=(int)image[index(i,j-1)];
      p[5]=(int)image[index(i-1,j-1)]; 
      p[6]=(int)image[index(i-1,j)];                                                            
      p[7]=(int)image[index(i-1,j+1)];          
}

bool Thinning::isContourPoint(unsigned char* image,int i, int j, int *p,int *neib, int *trans){
     int cont=0;
     int indexAux = index(i,j);
   
     *neib=*trans=0;
     if((int)image[indexAux]==0){
     	   extractNeighboors(image,i,j,p);	                   
           while(cont<8){
                if(p[cont]==0){//compute number of neighboors
                      (*neib)++;         
                      cont++;
                }
                else{ //compute number of transitions
                	if((cont<7)&& p[cont+1]==0) 
                		(*trans)++;
                	cont++;   
                }                                              
           } 
           if(p[7]==1 && p[0]==0)
               (*trans)++; 
           if((*neib)<8) return true;
     }
     return false;     
}

bool Thinning::firstCondicional(int flag){
     if((flag>=2) && (flag<=6))
             return true;
     else
             return false;              
}

bool Thinning::secondCondicional(int flag){
     if(flag==1)
         return true;
     else
         return false;               
}

bool Thinning::thirdCondicional(int *p){		
     if(p[0]==1 || p[2]==1 || p[4]==1)
                return true; 
     else
                return false;                           
}

bool Thinning::fourthCondicional(int *p){
     if(p[2]==1 || p[4]==1 || p[6]==1)
                return true; 
     else
                return false;                                     
}

bool Thinning::seventhCondicional(int *p){
     if(p[0]==1 || p[2]==1 || p[6]==1)
                return true; 
     else
                return false;      
}

bool Thinning::eighthCondicional(int *p){
     if(p[0]==1 || p[4]==1 || p[6]==1)
                return true; 
     else
                return false;      
}

void  Thinning::deleteAllPixels(vector<int> *v,unsigned char* im){
      vector<int>::iterator iter=v->begin();     
      for(unsigned int i=0;i<v->size();i++){
           im[*iter]=(unsigned char)1;
           iter++;           
      }
      v->clear();
}

void Thinning::thinningAlgorithm(){ 
	int i,j,numNeighbors=0,numTransistion=0, p[8],endloop1=0;
	unsigned char *imgAux = (unsigned char*)imNew->im->data[0];
	imNew->setBorder(imNew->im,1);//tratamento de borda
	vector<int> storePos;	  
	while(endloop1==0) 
	{
		for(i=0; i<imNew->im->width; i++){          
			for(j=0;j<imNew->im->height;j++){
				if(isContourPoint(imgAux,i,j,p,&numNeighbors,&numTransistion)){ 
					if(firstCondicional(numNeighbors) && secondCondicional(numTransistion)&&
							thirdCondicional(p) && fourthCondicional(p))                                      
						storePos.push_back(index(i,j));
				}     
				numNeighbors=numTransistion=0;                                                                                                     
			}
		}
		if(storePos.size()==0) endloop1=1;  	
		deleteAllPixels(&storePos,imgAux);    
	
		for(i=0; i<imNew->im->width; i++){          
			for(j=0;j<imNew->im->height;j++){
				if(isContourPoint(imgAux,i,j,p,&numNeighbors,&numTransistion)){ 
					if(firstCondicional(numNeighbors) && secondCondicional(numTransistion)&&
							seventhCondicional(p) && eighthCondicional(p))                                      
						storePos.push_back(index(i,j));
				}     
				numNeighbors=numTransistion=0;                                                                                                     
			}
		}
		if(storePos.size()==0) endloop1=1;   	
		deleteAllPixels(&storePos,imgAux);     
	}    
}

void Thinning::draw(){
	glRasterPos2f(0.0f,0.0f);
    /*Image *imgGray= new Image();
    imgGray->im=imImageCreate(imNew->im->width,imNew->im->height,IM_GRAY,imNew->im->data_type);         
    
    imConvertColorSpace(imNew->im,imgGray->im);
	glRasterPos2f(0.0f,0.0f);
  
    char *gl_data;  
    gl_data = NULL;
    gl_data = new char[imgGray->im->width*imgGray->im->height*imgGray->im->depth];*/
    //imConvertPacking(imgGray->im->data[0], gl_data, imgGray->im->width, imgGray->im->height, imgGray->im->depth, imgGray->im->data_type, 0); /*we convert to unpacked because here is has packed data*/                                                                                                                                                                                   
    
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); /* data alignment must be 1 */
    GLfloat mapr[]={0.0f,1.0f};
    GLfloat mapg[]={0.0f,1.0f};
    GLfloat mapb[]={0.0f,1.0f};
    GLfloat mapa[]={0.0f,0.0f};
    glPixelMapfv(GL_PIXEL_MAP_I_TO_R,2,mapr);
    glPixelMapfv(GL_PIXEL_MAP_I_TO_G,2,mapg);
    glPixelMapfv(GL_PIXEL_MAP_I_TO_B,2,mapb);
    glPixelMapfv(GL_PIXEL_MAP_I_TO_A,2,mapa);
    glDrawPixels(imNew->im->width, imNew->im->height,GL_COLOR_INDEX, GL_UNSIGNED_BYTE, (GLvoid*)imNew->im->data[0]); 
}

Thinning::~Thinning()
{
}
