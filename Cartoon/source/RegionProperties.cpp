#include "../headers/RegionProperties.hpp"

RegionProperties :: RegionProperties(Regions *r, Image *image){
	unsigned int size = r->imageThinning->width*r->imageThinning->height;
	colors.resize(size);
	
	for(int i = 0; i<r->imageThinning->width;i++){
		for(int j = 0; j<r->imageThinning->height;j++){
			int ind = r->index(i,j);
			(colors[ind]).r = ((double)((unsigned char*)(image->im->data[0]))[ind]);
			(colors[ind]).g = ((double)((unsigned char*)(image->im->data[1]))[ind]);
			(colors[ind]).b = ((double)((unsigned char*)(image->im->data[2]))[ind]);
		}
	}
	size = r->listInterior.size();
	
	ambientCoef.resize(size);				diffuseCoef.resize(size);
	specularCoef.resize(size);				specularExp.resize(size);
	
	for(unsigned int i = 0; i < size; i++) 	ambientCoef[i] = 0.6;
	for(unsigned int i = 0; i < size; i++)	diffuseCoef[i] = 1.0;
	for(unsigned int i = 0; i < size; i++)	specularCoef[i] = 0.2;
	for(unsigned int i = 0; i < size; i++)	specularExp[i] = 100.0;
}

RegionProperties :: ~RegionProperties(){}
