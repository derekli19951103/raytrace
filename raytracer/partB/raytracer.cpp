/***********************************************************
	
	Starter code for Assignment 3

	Implementations of functions in raytracer.h, 
	and the main function which specifies the scene to be rendered.	

***********************************************************/


#include "raytracer.h"
#include <cmath>
#include <iostream>
#include <cstdlib>

void Raytracer::traverseScene(Scene& scene, Ray3D& ray)  {
	for (size_t i = 0; i < scene.size(); ++i) {
		SceneNode* node = scene[i];

		if (node->obj->intersect(ray, node->worldToModel, node->modelToWorld)) {
			ray.intersection.mat = node->mat;
		}
	}
}

void Raytracer::computeTransforms(Scene& scene) {
	// right now this method might seem redundant. But if you decide to implement 
	// scene graph this is where you would propagate transformations to child nodes
		
	for (size_t i = 0; i < scene.size(); ++i) {
		SceneNode* node = scene[i];

		node->modelToWorld = node->trans;
		node->worldToModel = node->invtrans; 
	}
}

void Raytracer::computeShading(Ray3D& ray, LightList& light_list) {
	for (size_t  i = 0; i < light_list.size(); ++i) {
		LightSource* light = light_list[i];
		
		// Each lightSource provides its own shading function.
		// Implement shadows here if needed.
		light->shade(ray);        
	}
}

Color Raytracer::shadeRay(Ray3D& ray, Scene& scene, LightList& light_list, int k_bounce=0) {
	Color col(0.0, 0.0, 0.0); 
	traverseScene(scene, ray); 

	// Don't bother shading if the ray didn't hit 
	// anything.
	if (!ray.intersection.none) {
		computeShading(ray, light_list); 
		col = ray.col;  
	
	// You'll want to call shadeRay recursively (with a different ray, 
	// of course) here to implement reflection/refraction effects.  

        if(k_bounce < 2){
            
            //normal at the intersection
            Vector3D ray_norm = ray.intersection.normal;
            //direction of incidence
            Vector3D I_dir = ray.dir;
            
            //if refractive then calculate the refraction.
            if(ray.intersection.mat->refractive){
                //direction of refraction, using snells law
                double cos_idx = ray_norm.dot(I_dir);
                double idx = ray.intersection.mat->index_of_refraction;
                Vector3D dir = idx * I_dir - (-cos_idx + idx*cos_idx) * ray_norm;
                
                //create new ray
                Ray3D new_ray;
                new_ray.dir = dir;
                new_ray.dir.normalize();
                new_ray.origin = ray.intersection.point + 0.01 * new_ray.dir;
                
                //shade ray
                Color refrCol = shadeRay(new_ray, scene, light_list, k_bounce+1);
                col = col + refrCol;
            }
            //for reflection
            Ray3D ray_new;
            ray_new.dir = ray.dir - (2 * (ray.intersection.normal.dot(ray.dir)) * ray.intersection.normal);
            ray_new.dir.normalize();
            // Avoid intersecting with original object
            ray_new.origin = ray.intersection.point + 0.01*ray_new.dir;
            Color new_col = shadeRay(ray_new, scene, light_list, k_bounce+1);
            // Add new color with a small scalar multiple
            col = col + 0.2*new_col;
            
        }
        col.clamp();
    }
	return col; 
}

void Raytracer::render(Camera& camera, Scene& scene, LightList& light_list, Image& image) {
	computeTransforms(scene);

	Matrix4x4 viewToWorld;
	double factor = (double(image.height)/2)/tan(camera.fov*M_PI/360.0);

	viewToWorld = camera.initInvViewMatrix();
    int N= 20;
    int M= 20;
	// Construct a ray for each pixel.
	for (int i = 0; i < image.height; i++) {
		for (int j = 0; j < image.width; j++) {
			// Sets up ray origin and direction in view space, 
			// image plane is at z = -1.
			Point3D origin(0, 0, 0);
			Point3D imagePlane;
			imagePlane[0] = (-double(image.width)/2 + 0.5 + j)/factor;
			imagePlane[1] = (-double(image.height)/2 + 0.5 + i)/factor;
			imagePlane[2] = -1;
            Vector3D direction=Vector3D(imagePlane[0],imagePlane[1],imagePlane[2]);
    
            Ray3D ray;
            // TODO: Convert ray to world space
            ray.origin = viewToWorld*origin;
            ray.dir = viewToWorld*direction;
            Color col=shadeRay(ray,scene,light_list);
			image.setColorAtPixel(i, j, col);
		}
	}
}

