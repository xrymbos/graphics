/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The color class
*  A simple colour class with a set of operations including
*    phong lighting.
-------------------------------------------------------------*/
#include "Color.h"
#include <cmath>

void Color::scaleColor(float scaleFactor) {
    r = r * scaleFactor;
    g = g * scaleFactor;
    b = b * scaleFactor;
 }

void Color::combineColor(Color col) {
    r *= col.r;
    g *= col.g;
    b *= col.b;
 }

void Color::combineColor(Color col, float scaleFactor) {
    r +=  scaleFactor * col.r;
    g +=  scaleFactor * col.g;
    b +=  scaleFactor * col.b;
 }

float Color::dist(Color col){
	double dr = r - col.r;
	double dg = g - col.g;
	double db = b - col.b;
	return sqrt(dr*dr + dg*dg + db*db);
}

//Phong lighting equations:
// Input:  Light's ambient color, l.n,  (r.v)^f
// Assumptions:
//   Material ambient = Material diffuse = current color
//   Material specular = white
//   Light diffuse = Light specular = white

Color Color::phongLight(Color ambientCol, float diffTerm, float specTerm) {
	Color col;
    col.r = (ambientCol.r) * r + diffTerm * r + specTerm;
    col.g = (ambientCol.g) * g + diffTerm * g + specTerm;
    col.b = (ambientCol.b) * b + diffTerm * b + specTerm;
	return col;
 }

const Color Color::WHITE = Color(1, 1, 1);
const Color Color::BLACK = Color(0, 0, 0);
const Color Color::RED = Color(1, 0, 0);
const Color Color::GREEN = Color(0, 1, 0);
const Color Color::BLUE = Color(0, 0, 1);
const Color Color::GRAY = Color(0.2f, 0.2f, 0.2f);
