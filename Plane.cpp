/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The Plane class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#include "Plane.h"
#include "Vector.h"
#include <math.h>

//Function to test if an input point is within the quad.
const float eps = 10e-6;

bool Plane::isInside(Vector pos)
{
	Vector n = normal(pos);
	Vector ua = b - a;
	Vector ub = c - b;
	Vector uc = d - c;
	Vector ud = a - d;
	Vector va = pos - a;
	Vector vb = pos - b;
	Vector vc = pos - c;
	Vector vd = pos - d;
	bool pos1 = ua.cross(va).dot(n) > 0;
	bool pos2 = ub.cross(vb).dot(n) > 0;
	bool pos3 = uc.cross(vc).dot(n) > 0;
	bool pos4 = ud.cross(vd).dot(n) > 0;
	return pos1 && pos2 && pos3 && pos4;
}

//Function to compute the paramter t at the point of intersection.

float Plane::intersect(Vector pos, Vector dir)
{
	Vector n = normal(pos);
	if(fabs(dir.dot(n)) < eps){
		return -1;
	}
	float t = ((a - pos).dot(n)) / dir.dot(n);
	Vector inter = pos + (dir * t);
	if(isInside(inter) && t > eps){
		return t;
	}
	else{
		return -1;
	}
    //Implement this function
}

// Function to compute the unit normal vector

Vector Plane::normal(Vector pos)
{
	Vector n = ((b-a).cross(c-a));
	n.normalise();
	return n;
}



