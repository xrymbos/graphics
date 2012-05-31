/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The Cube class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#include "Cube.h"
#include "Plane.h"

float Cube::intersect(Vector pos, Vector dir)
{
	float answer = -1;
	for(int i=0;i<(int)sides.size();i++)
        {
		float t = sides[i].intersect(pos, dir);
		if(t == -1)
                {
			continue;
		}
		if(answer == -1)
                {
			answer = t;
		}
		if(t < answer)
                {
			answer = t;
		}
	}
	return answer;
}

Vector Cube::normal(Vector pos)
{
	for(int i=0;i<(int)sides.size();i++)
        {
		if(sides[i].isInside(pos))
                {
			return sides[i].normal(pos);
		}
	}
	return Vector(-1,-1,-1);
}
