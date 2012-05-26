/*----------------------------------------------------------
 * COSC363  Ray Tracer
 *
 *  The Cube class
 *  This is a subclass of Object, and hence implements the
 *  methods intersect() and normal().
 -------------------------------------------------------------*/

#include "Vector.h"
#include "Object.h"
#include "Plane.h"
#include <vector>
using namespace std;

class Cube : public Object
{
	private:
		vector<Plane> sides;

	public:
		Cube(void);

		Cube(Vector centre, double size, Color col)
		{
			color = col;

			vector<Vector> points;
			//We find the vertices of the cube and push them into the points array
			for(int i=-1;i<=2;i+=2)
			{
				for(int j=-1;j<=2;j+=2)
				{
					for(int k=-1;k<=2;k+=2)
					{
						points.push_back(Vector(centre.x + size * i, centre.y + size * j, centre.z + size * k));
					}
				}
			}
			//We build the sides of the cube, and push them into the sides array
			sides.push_back(Plane(points[0], points[2], points[3], points[1], col));
			sides.push_back(Plane(points[4], points[5], points[7], points[6], col));
			sides.push_back(Plane(points[0], points[4], points[6], points[2], col));
			sides.push_back(Plane(points[1], points[3], points[7], points[5], col));
			sides.push_back(Plane(points[0], points[1], points[5], points[4], col));
			sides.push_back(Plane(points[2], points[6], points[7], points[3], col));
		};

		float intersect(Vector pos, Vector dir);

		Vector normal(Vector pos);
};
