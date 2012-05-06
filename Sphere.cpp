/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The sphere class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#include "Sphere.h"
#include "Vector.h"
#include <math.h>
#include <stdlib.h>

/**
* Sphere's intersection method.  The input is a ray (pos, dir). 
*/
float Sphere::intersect(Vector pos, Vector dir)
{
    Vector vdif = pos - center;
    float b = dir.dot(vdif);
    float len = vdif.length();
    float c = len*len - radius*radius;
    float delta = b*b - c;
   
	if(abs(delta) < 0.001) return -1.0; 
    if(delta < 0.0) return -1.0;

    float t1 = -b + sqrt(delta);
    float t2 = -b - sqrt(delta);
    if(fabs(t1) < 0.001) t1 = -1.0;
    if(fabs(t2) < 0.001 ) t2 = -1.0;

	return (t1 < t2)? t1: t2;
}

/**
* Returns the unit normal vector at a given point.
* Assumption: The input point p lies on the sphere.
*/
Vector Sphere::normal(Vector p)
{
    Vector n = p - center;
    n.normalise();
    return n;
}
