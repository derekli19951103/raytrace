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

void Raytracer::computeShading(Ray3D& ray, LightList& light_list, Scene& scene) {
    Color c(0.0,0.0,0.0);
	for (size_t  i = 0; i < light_list.size(); ++i) {
		LightSource* light = light_list[i];
		
		// Each lightSource provides its own shading function.
		// Implement shadows here if needed.
		light->shade(ray);
        
        Point3D origin = ray.intersection.point;
        Vector3D dir = light->get_position() - origin;
        dir.normalize();
        
        Ray3D shadow(origin + 0.001 * dir ,dir);
        traverseScene(scene,shadow);
        
        if(!shadow.intersection.none){
            ray.col = 0.3 * ray.col;
        }
        c = c + ray.col;
	}
    ray.col = (1.0/light_list.size()) * c;
}

Color Raytracer::shadeRay(Ray3D& ray, Scene& scene, LightList& light_list, CubeEnv& cube, int reflect_times = 0) {
	Color col(0.0, 0.0, 0.0); 
	traverseScene(scene, ray);
	// Don't bother shading if the ray didn't hit 
	// anything.
	if (!ray.intersection.none) {
		computeShading(ray, light_list, scene);
		col = ray.col;  
	
	// You'll want to call shadeRay recursively (with a different ray, 
	// of course) here to implement reflection/refraction effects.
        
        //secondary reflection
        if(reflect_times < 2){
            
            Vector3D N = ray.intersection.normal;
            Vector3D I = ray.dir;
            
            //reflection
            Ray3D reflection;
            reflection.dir = I - (2 * N.dot(I) * N);
            reflection.dir.normalize();
            // Avoid intersecting with original object
            reflection.origin = ray.intersection.point + 0.001 * reflection.dir;
            Color reflect_col = shadeRay(reflection, scene, light_list, cube, reflect_times + 1);
            // Add new color with a small scalar multiple
            col = col + 0.2 * reflect_col;
            
        }
        col.clamp();
    }
    else{
        col = cube.get_color(ray.dir);
    }
	return col; 
}

void Raytracer::render(Camera& camera, Scene& scene, LightList& light_list, Image& image) {
	computeTransforms(scene);

	Matrix4x4 viewToWorld;
	double factor = (double(image.height)/2)/tan(camera.fov*M_PI/360.0);
    
	viewToWorld = camera.initInvViewMatrix();
    
    CubeEnv cube;
    cube.load_cube();
    
	// Construct a ray for each pixel.
	for (int i = 0; i < image.height; i++) {
		for (int j = 0; j < image.width; j++) {
            // Sets up ray origin and direction in view space,
            // image plane is at z = -1.
            Point3D origin(0, 0, 0);
            Point3D imagePlane;

            // 16X Supersampling for ANTI-aliasing
            Color avg_out(0.0, 0.0, 0.0);
            for (float i_patch = i; i_patch < i + 1; i_patch += 0.25f) {
                for (float j_patch = j; j_patch < j + 1; j_patch += 0.25f) {
                    // 4 rays per pixel, by adding 0.5f to the planar directions
                    imagePlane[0] = (-double(image.width)/2 + 0.5 + j_patch)/factor;
                    imagePlane[1] = (-double(image.height)/2 + 0.5 + i_patch)/factor;
                    imagePlane[2] = -1;

                    Vector3D direction=Vector3D(imagePlane[0],imagePlane[1],imagePlane[2]);

                    Ray3D ray;

                    ray.origin = viewToWorld*origin;
                    ray.dir = viewToWorld*direction;
                    Color col=shadeRay(ray,scene,light_list,cube);
                    col = (1.0/16.0) * col;
                    avg_out = avg_out + col;
                    avg_out.clamp();
                }
            }


            image.setColorAtPixel(i, j, avg_out);
            
		}
	}
}

void Texture::load(const char * filename){
    bmp_read(filename, &x, &y, &rarray, &garray, &barray);
}

Color Texture::col(Point3D uv) {
    int i = int(floor(uv[0] * x));
    int j = int(floor(uv[1] * y));
    //to get color in [0.0,1.0]
    Color col(rarray[i *x + j]/255.0,garray[i *x + j]/255.0,barray[i* x + j]/255.0);
    return col;
}

Color CubeEnv::get_color(Vector3D direction) {
    
    // Used the wikipedia article for cube mapping as a guide
    // As well as the textbook
    // Use the convention from the wikipedia article since in the book, +z is the up-direction
    // In the cube mapping code I assume that positive y is the up-direction, as given in main.cpp
    Texture * txt;
    double u;
    double v;
    double x = direction[0];
    double y = direction[1];
    double z = direction[2];
    
    double abs_x = fabs(x);
    double abs_y = fabs(y);
    double abs_z = fabs(z);
    
    double max = fmax(fmax(abs_x,abs_y),fmax(abs_y,abs_z));
    
    if (max==abs_x)
    {
        if (x > 0)
        {
            u = (-z + x) / (2 * x);
            v = (-y + x) / (2 * x);
            txt = posx;
        }
        else {
            u = (-z + x) / (2 * x);
            v = (y + x) / (2 * x);
            txt = negx;
        }
    }
    else if (max==abs_y)
    {
        if (y > 0)
        {
            u = (x + y) / (2 * y);
            v = (z + y) / (2 * y);
            txt = posy;
        }
        else {
            u = (-x + y) / (2 * y);
            v = (z + y) / (2 * y);
            txt = negy;
        }
    }
    else if (max==abs_z) {
        if (z > 0)
        {
            u = (x + z) / (2 * z);
            v = (-y + z) / (2 * z);
            txt = posz;
        }
        else {
            u = (x + z) / (2 * z);
            v = (y + z) / (2 * z);
            txt = negz;
        }
        
    }
    
    // We do this flipping because
    // The .bmp file coordinates
    // are flipped and stored as Y,X
    
    //otherwise we would do Point3D(u,v,0)
    Point3D p(1-v, 1-u, 0);
    return txt->col(p);
}

void CubeEnv::load_cube() {
    // we store the cube map
    // as six square .bmp images
    posx = new Texture();
    posx->load("posx.bmp"); // pos_x
    negx = new Texture();
    negx->load("negx.bmp"); // neg_x
    posy = new Texture();
    posy->load("posy.bmp"); // pos_y
    negy = new Texture();
    negy->load("negy.bmp"); // neg_y
    posz = new Texture();
    posz->load("posz.bmp"); // pos_z
    negz = new Texture();
    negz->load("negz.bmp"); // neg_z
}





