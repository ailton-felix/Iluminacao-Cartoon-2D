#ifndef REGIONPROPERTIES_HPP_
#define REGIONPROPERTIES_HPP_

#include <vector>
#include "Regions.hpp"

class RegionProperties{
public:
	vector<double> ambientCoef;
	vector<double> diffuseCoef;
	vector<double> specularCoef;
	vector<double> specularExp;
	vector<Color> colors;
	
	RegionProperties(Regions *r, Image *image);
	~RegionProperties();
};

#endif /*REGIONPROPERTIES_HPP_*/
