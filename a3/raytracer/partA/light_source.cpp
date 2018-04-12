/***********************************************************
	
	Starter code for Assignment 3

	Implements light_source.h

***********************************************************/

#include <cmath>
#include "light_source.h"

void PointLight::shade(Ray3D& ray) {
	// TODO: implement this function to fill in values for ray.col 
	// using phong shading.  Make sure your vectors are normalized, and
	// clamp Color values to 1.0.
	//
	// It is assumed at this point that the intersection information in ray 
	// is available.  So be sure that traverseScene() is called on the ray 
	// before this function.
    
    //Retrieving the variables
    //coefficients
    Color Ka = (*ray.intersection.mat).ambient;
    Color Kd = (*ray.intersection.mat).diffuse;
    Color Ks = (*ray.intersection.mat).specular;
    double alpha = (*ray.intersection.mat).specular_exp;
    //incident light
    Color Ia = col_ambient;
    Color Id = col_diffuse;
    Color Is = col_specular;
    //N,L,B,R
    Vector3D N = ray.intersection.normal;
    Vector3D L = pos-ray.intersection.point;
    Vector3D B = -ray.dir;
    L.normalize();
    B.normalize();
    double LN = N.dot(L);
    Vector3D R = 2*(LN)*N - L;
    R.normalize();
    double BR = B.dot(R);
    //phong
    //Remove diffuse here to ge diffuse.bmp
    ray.col = Ka*Ia + fmax(0.0,LN)*Kd*Id + fmax(0.0,pow(BR,alpha))*Ks*Is;
    ray.col.clamp();

}

