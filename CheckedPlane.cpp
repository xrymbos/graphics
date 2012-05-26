/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The chequered plane class
*  This is a subclass of Plane
-------------------------------------------------------------*/

#include "CheckedPlane.h"
#include <cstdio>

Color CheckedPlane::getColor(Vector pos){
	double xParam = (pos - a).dot(b - a) / (b - a).dot(b - a); //how far we are along one side
	double yParam = (pos - a).dot(d - a) / (d - a).dot(d - a); //how far we are along the other
	int xCol = (xParam * xDivisions);
	int yCol = (yParam * yDivisions);
	if(xCol % 2 == yCol % 2)
	{
		return color;
	}
	else
	{
		return color2;
	}
}
