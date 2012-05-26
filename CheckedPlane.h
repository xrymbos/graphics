#include "Plane.h"
#include <cstdio>

class CheckedPlane : public Plane
{
protected:
	Color color2;
	int xDivisions;
	int yDivisions;
public:
	CheckedPlane(void):Plane()
	{
	};

	CheckedPlane(Vector pa, Vector pb, Vector pc, Vector pd, Color col):Plane(pa, pb, pc, pd, col)
	{
	};

	CheckedPlane(Vector pa, Vector pb, Vector pc, Vector pd, Color col, Color col2, int xDiv, int yDiv):Plane(pa, pb, pc, pd, col)
	{
		xDivisions = xDiv;
		yDivisions = yDiv;
		color2 = col2;
	};

	Color getColor(Vector pos);
};
