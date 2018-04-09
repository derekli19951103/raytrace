/***********************************************************
	
	Starter code for Assignment 3
	
	This file contains the interface and datastructures of the raytracer.  
	Simple traversal and addition code to the datastructures are given to you.

***********************************************************/
#pragma once

#include "util.h"
#include "scene_object.h"
#include "light_source.h"

struct Texture {
    // define the size of the .bmp file
    
    unsigned long int x;
    long int y;
    unsigned char* rarray;
    unsigned char* garray;
    unsigned char* barray;
    
    // array of dimension (x,y) giving the heightmap value at that pixel
    void load(const char * filename);
    Color col(Point3D uv);
};

struct CubeEnv {
    Point3D coord_trans(Vector3D direction, int* face);
    
    // For the environment maps that use bmp files
    // We define an image for every face of the cube;
    void load_cube();
    Color get_color(Vector3D direction);
    
    Texture * posx;
    Texture * negx;
    Texture * posy;
    Texture * negy;
    Texture * posz;
    Texture * negz;
};

class Raytracer {
public:
	// Renders 3D scene to an image given camera and lights setup.
	void render(Camera& camera, Scene& scene, LightList& light_list, Image& image);
		
private:

	// Return the color of the ray after intersection and shading, call 
	// this function recursively for reflection and refraction.  
	Color shadeRay(Ray3D& ray, Scene& scene, LightList& light_list, CubeEnv& cube, int reflect_times);

	// Traversal code for the scene, the ray is transformed into 
	// the object space of each node where intersection is performed.
	void traverseScene(Scene& scene, Ray3D& ray);

	// After intersection, calculate the color of the ray by shading it
	// with all light sources in the scene.
	void computeShading(Ray3D& ray, LightList& light_list, Scene& scene);

	// Precompute the modelToWorld and worldToModel transformations for each
    // object in the scene.
	void computeTransforms(Scene& scene);

};
