#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gmath.h"
#include "matrix.h"
#include "ml6.h"

//lighting functions
color get_lighting( double *normal, double *view, color alight, double light[2][3], double *areflect, double *dreflect, double *sreflect) {
  color i;
	
  normalize(normal);
  normalize(light[LOCATION]);
  normalize(view);

  color a = calculate_ambient(alight, areflect);
  color d = calculate_diffuse(light, dreflect, normal);
  color s = calculate_specular(light, sreflect, view, normal);
	
	i.red = a.red + d.red + s.red;
	i.green = a.green + d.green + s.green;
	i.blue = a.blue + d.blue + s.blue;
	
	limit_color(&i);
	
  return i;
}

color calculate_ambient(color alight, double *areflect ) {
  color a;

  a.red = areflect[RED] * alight.red;
  a.green = areflect[GREEN] * alight.green;
  a.blue = areflect[BLUE] * alight.blue;
  
  return a;
}

color calculate_diffuse(double light[2][3], double *dreflect, double *normal ) {
  color d;

  d.red = light[COLOR][RED]*dreflect[RED]*dot_product(light[LOCATION], normal);
  d.green = light[COLOR][GREEN]*dreflect[GREEN]*dot_product(light[LOCATION], normal);
  d.blue = light[COLOR][BLUE]*dreflect[BLUE]*dot_product(light[LOCATION], normal);
  
  return d;
}

color calculate_specular(double light[2][3], double *sreflect, double *view, double *normal ) {
  color s;
	int expt = 10;
	double tmp[3];
	
  tmp[0] = (2*dot_product(normal, light[LOCATION]))*normal[0] - light[LOCATION][0];
  tmp[1] = (2*dot_product(normal, light[LOCATION]))*normal[1] - light[LOCATION][1];
  tmp[2] = (2*dot_product(normal, light[LOCATION]))*normal[2] - light[LOCATION][2];

	s.red = light[COLOR][RED]* *sreflect*pow(dot_product(tmp, view), expt);
	s.green = light[COLOR][GREEN]* *sreflect*pow(dot_product(tmp, view), expt);
	s.blue = light[COLOR][BLUE]* *sreflect*pow(dot_product(tmp, view), expt);
		
  return s;
}


//limit each component of c to a max of 255
void limit_color( color * c ) {
	if (c->red > 255) {
		c->red = 255;
	}
	if (c->green > 255) {
		c->green = 255;
	}
	if (c->blue > 255) {
		c->blue = 255;
	}
	if (c->red < 0) {
		c->red = 0;
	}
	if (c->green < 0) {
		c->green = 0;
	}
	if (c->blue < 0) {
		c->blue = 0;
	}
}

//vector functions
//normalize vetor, should modify the parameter
void normalize( double *vector ) {
  int i;
  double magnitude = 0;
  for (i = 0; i < 3; i++) {
      magnitude += vector[i] * vector[i];
  }
  magnitude = sqrt(magnitude);
  for (i = 0; i < 3; i++) {
      vector[i] = vector[i] / magnitude;
  }
}

//Return the dot porduct of a . b
double dot_product( double *a, double *b ) {
	double sum = a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
  return sum;
}

double *calculate_normal(struct matrix *polygons, int i) {

  double A[3];
  double B[3];
  double *N = (double *)malloc(3 * sizeof(double));

  A[0] = polygons->m[0][i+1] - polygons->m[0][i];
  A[1] = polygons->m[1][i+1] - polygons->m[1][i];
  A[2] = polygons->m[2][i+1] - polygons->m[2][i];

  B[0] = polygons->m[0][i+2] - polygons->m[0][i];
  B[1] = polygons->m[1][i+2] - polygons->m[1][i];
  B[2] = polygons->m[2][i+2] - polygons->m[2][i];

  N[0] = A[1] * B[2] - A[2] * B[1];
  N[1] = A[2] * B[0] - A[0] * B[2];
  N[2] = A[0] * B[1] - A[1] * B[0];

  return N;
}
