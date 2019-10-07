#ifndef DEFS_HPP_
#define DEFS_HPP_

#include <vector>

#include "Color.hpp"
#include "Point2D.hpp"
#include "Curve.hpp"
#include "Point3D.hpp"

typedef list<Point2D> Boundary;
typedef list<Point2D> Interior;
typedef list<Curve> Curves;
typedef list<int>     Label;
typedef list<Point2D>::iterator PointsIterator;

typedef vector<Point3D> normalArray;
typedef vector<normalArray> normalToRegion;
typedef vector<normalToRegion> normalToAll;


#endif /*DEFS_HPP_*/
