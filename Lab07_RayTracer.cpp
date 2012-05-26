/*========================================================================
 * COSC 363  Lab06
 * A simple ray tracer
 *========================================================================
 */
#include <iostream>
#include <cmath>
#include <cstdio>
#include <vector>
#include "Vector.h"
#include "Sphere.h"
#include "CheckedPlane.h"
#include "Plane.h"
#include "Cube.h"
#include "Color.h"
#include "Object.h"
#include <GL/glut.h>
using namespace std;

const float WIDTH = 20.0;  
const float HEIGHT = 20.0;
const float EDIST = 40.0;
const int PPU = 30; 
const int MAX_STEPS = 100;
const float XMIN = -WIDTH * 0.5;
const float XMAX =  WIDTH * 0.5;
const float YMIN = -HEIGHT * 0.5;
const float YMAX =  HEIGHT * 0.5;
const float reflCoeff = 0.5;
const float refrCoeff = 0.5;
const float glassRefr = 1.03;//refraction index of "glass"
const float airRefr = 1;//refraction index of "air"

vector<Object*> sceneObjects;

Vector light = Vector(10.0, 40.0, -5.0);
Color backgroundCol = Color::GRAY;

struct PointBundle
{
	Vector point;
	int index;
	float dist;
};

/*
 * This function compares the given ray with all objects in the scene
 * and computes the closest point  of intersection.
 */
PointBundle closestPt(Vector pos, Vector dir)
{
	Vector  point(0, 0, 0);
	float min = 10000.0;

	PointBundle out = {point, -1, 0.0};

	for(int i = 0;  i < (int)sceneObjects.size();  i++)
	{
		float t = sceneObjects[i]->intersect(pos, dir);
		if(t > 0)        //Intersects the object
		{
			point = pos + dir*t;
			if(t < min)
			{
				out.point = point;
				out.index = i;
				out.dist = t;
				min = t;
			}
		}
	}

	return out;
}

/*
 * Computes the colour value obtained by tracing a ray.
 * If reflections and refractions are to be included, then secondary rays will 
 * have to be traced from the point, by converting this method to a recursive
 * procedure.
 * The step parameter is the number of "bounces" the ray has gone through thus far.
 * The refraction parameter is the refraction index of the medium that the ray is travelling through.
 */

Color trace(Vector pos, Vector dir, int step, int refraction)
{
	PointBundle q = closestPt(pos, dir);

	if(q.index == -1) return backgroundCol;        //no intersection

	Color col = sceneObjects[q.index]->getColor(q.point); //Object's colour
	Vector n = sceneObjects[q.index]->normal(q.point);
	Vector l = light - q.point;
	l.normalise();
	double lDotn = l.dot(n);
	Vector shadowRay = light - q.point;
	double lightDist = shadowRay.length();
	shadowRay.normalise();
	PointBundle blocker = closestPt(q.point, shadowRay);
	double blockDist = blocker.point.dist(q.point);

	if(q.index != 5 && (lDotn <= 0 || (blocker.index != -1 && blockDist < lightDist))){
		/*if(blocker.index != -1){//easier to see which object is casting shadows
		  return sceneObjects[blocker.index]->getColor();
		  }*/
		return col.phongLight(backgroundCol, 0.0, 0.0);
	}

	Vector r = ((n * 2) * lDotn) - l;
	r.normalise();
	Vector v(-dir.x, -dir.y, -dir.z);
	float rDotv = r.dot(v);
	float spec;
	if(rDotv < 0) spec = 0.0;
	else spec = pow(rDotv, 10);

	Color colorSum = col.phongLight(backgroundCol, lDotn, spec);
	if(q.index == 1 && step < MAX_STEPS){//we trace a reflection ray
		Vector view = dir * -1;
		Vector reflectionVector = ((n*2)*(n.dot(view))) - view;
		reflectionVector.normalise();
		Color reflectionCol = trace(q.point, reflectionVector, step+1, airRefr);
		colorSum.combineColor(reflectionCol,reflCoeff);
	}
	if(q.index == 5 && step < MAX_STEPS){
		double newRefr = -1;
		if(refraction == airRefr){
			newRefr = glassRefr;
		}
		else{
			newRefr = airRefr;
		}
		double c1 = -n.dot(dir);
		double N = refraction / newRefr;
		double c2 = sqrt(1 - N * N * (1 - c1 * c1));
		Vector refractionVector = dir * N + n * (N * c1 - c2);
		if(refractionVector.dot(dir) < 0){
			if(newRefr == airRefr){
				newRefr = glassRefr;
			}
			else{
				newRefr = airRefr;
			}
		}
		Color refractionCol = trace(q.point, refractionVector, step+1, newRefr);
		colorSum.combineColor(refractionCol, refrCoeff);
	}

	return colorSum;

}

//---The main display module -----------------------------------------------------------
// In a ray tracing application, it just displays the ray traced image by drawing
// each pixel as quads.
//---------------------------------------------------------------------------------------
void display()
{
	int widthInPixels = (int)(WIDTH * PPU);
	int heightInPixels = (int)(HEIGHT * PPU);
	float pixelSize = 1.0/PPU;
	float halfPixelSize = pixelSize/2.0;
	float x1, y1, xc, yc;
	Vector eye(0., 0., 0.);

	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_QUADS);  //Each pixel is a quad.

	for(int i = 0; i < widthInPixels; i++)	//Scan every "pixel"
	{
		x1 = XMIN + i*pixelSize;
		xc = x1 + halfPixelSize;
		for(int j = 0; j < heightInPixels; j++)
		{
			y1 = YMIN + j*pixelSize;
			yc = y1 + halfPixelSize;

			Vector dir(xc, yc, -EDIST);	//direction of the primary ray

			dir.normalise();			//Normalise this direction

			Color col = trace (eye, dir, 1, airRefr); //Trace the primary ray and get the colour value
			glColor3f(col.r, col.g, col.b);
			glVertex2f(x1, y1);				//Draw each pixel with its color value
			glVertex2f(x1 + pixelSize, y1);
			glVertex2f(x1 + pixelSize, y1 + pixelSize);
			glVertex2f(x1, y1 + pixelSize);
		}
	}

	glEnd();
	glFlush();
}



void initialize()
{
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(XMIN, XMAX, YMIN, YMAX);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(0, 0, 0, 1);
	Sphere * sphere1 = new Sphere(Vector(5,6,-70), 3.0, Color::RED);
	Sphere * sphere2 = new Sphere(Vector(-7,0,-70), 10.0, Color(0,0.1,0.2));
	Sphere * sphere3 = new Sphere(Vector(9,7,-50), 4.0, Color::GREEN);
	CheckedPlane *plane = new CheckedPlane(Vector(-10, -10, -40), Vector(10, -10, -40),
			Vector(10., -10, -80), Vector(-10., -10, -80), Color(1, 0, 1), Color(0, 1, 0), 3, 4);
	Cube *cube = new Cube(Vector(-5, -8, -50), 2., Color(1, 0, 0));
	Sphere * refract = new Sphere(Vector(3,-5,-50), 4.0, Color::BLUE);


	sceneObjects.push_back(sphere1);
	sceneObjects.push_back(sphere2);
	sceneObjects.push_back(sphere3);
	sceneObjects.push_back(plane);
	sceneObjects.push_back(cube);
	sceneObjects.push_back(refract);
}


int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(20, 20);
	glutCreateWindow("Raytracing");

	glutDisplayFunc(display);
	initialize();

	glutMainLoop();
	return 0;
}
