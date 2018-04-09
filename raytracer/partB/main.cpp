/***********************************************************
	
	Starter code for Assignment 3

***********************************************************/

#include <cstdlib>
#include "raytracer.h"

int main(int argc, char* argv[])
{
	// Build your scene and setup your camera here, by calling 
	// functions from Raytracer.  The code here sets up an example
	// scene and renders it from two different view points, DO NOT
	// change this if you're just implementing part one of the 
	// assignment.  
	Raytracer raytracer;
	LightList light_list;
	Scene scene;
	int width = 640;
	int height = 480;
    int option = -1;
    int i;

	if (argc == 4) {
		width = atoi(argv[1]);
		height = atoi(argv[2]);
        option = atoi(argv[3]);
	}
    
    
    switch (option) {
        case 0:
            i = reflection(width,height);
            break;
        
        case 4:
            i = softshadow(width,height);
            break;
            
        case 3:
            i = anti(width,height);
            break;
            
        case 2:
            i = environment(width,height);
            break;
            
        case 1:
            i = hardshadow(width,height);
            break;
            
        case 5:
            i = glossy(width,height);
            break;

//        case "reflection":
//            i = reflection(width,height);
//            break;
            
        default:
            i = full(width,height);
            break;
    }
	return 0;
}

int reflection(int width,int height) {
    Raytracer raytracer;
    LightList light_list;
    Scene scene;
    
    // Define materials for shading.
    Material gold(Color(0.3, 0.3, 0.3), Color(0.75164,0.60648,0.22648),
                  Color(0.628281, 0.555802, 0.366065),
                  51.2);
    Material jade(Color(0, 0, 0), Color(0.54,0.89,0.63),
                  Color(0.316228,0.316228,0.316228),
                  12.8);
    
    // Defines a point light source.
    PointLight* pLight = new PointLight(Point3D(0,0,5), Color(0.9,0.9,0.9));
    light_list.push_back(pLight);
    
    // Add a unit square into the scene with material mat.
    SceneNode* sphere = new SceneNode(new UnitSphere(), &gold);
    scene.push_back(sphere);
    SceneNode* plane = new SceneNode(new UnitSquare(), &jade);
    scene.push_back(plane);
    
    // Apply some transformations to the sphere and unit square.
    double factor1[3] = { 1.0, 2.0, 1.0 };
    sphere->translate(Vector3D(0, 0, -5));
    sphere->rotate('x', -45);
    sphere->rotate('z', 45);
    sphere->scale(Point3D(0, 0, 0), factor1);
    
    double factor2[3] = { 6.0, 6.0, 6.0 };
    plane->translate(Vector3D(0, 0, -7));
    plane->rotate('z', 45);
    plane->scale(Point3D(0, 0, 0), factor2);
    
    // Render the scene, feel free to make the image smaller for
    // testing purposes.
    Camera camera1(Point3D(0, 0, 1), Vector3D(0, 0, -1), Vector3D(0, 1, 0), 60.0);
    Image image1(width, height);
    raytracer.render_reflection(camera1, scene, light_list, image1); //render 3D scene to image
    image1.flushPixelBuffer("reflection1.bmp"); //save rendered image to file
    
    // Render it from a different point of view.
    Camera camera2(Point3D(4, 2, 1), Vector3D(-4, -2, -6), Vector3D(0, 1, 0), 60.0);
    Image image2(width, height);
    raytracer.render_reflection(camera2, scene, light_list, image2);
    image2.flushPixelBuffer("reflection2.bmp");
    
    // Free memory
    for (size_t i = 0; i < scene.size(); ++i) {
        delete scene[i];
    }
    
    for (size_t i = 0; i < light_list.size(); ++i) {
        delete light_list[i];
    }
    
    return 0;
}

int softshadow(int width,int height) {
    Raytracer raytracer;
    LightList light_list;
    Scene scene;

    
    // Define materials for shading.
    Material gold(Color(0.3, 0.3, 0.3), Color(0.75164,0.60648,0.22648),
                  Color(0.628281, 0.555802, 0.366065),
                  51.2);
    Material jade(Color(0, 0, 0), Color(0.54,0.89,0.63),
                  Color(0.316228,0.316228,0.316228),
                  12.8);
    
    // multipule light to produce area light
    // soft shadow
    PointLight* pLight = new PointLight(Point3D(0,0,5), Color(0.9,0.9,0.9));
    light_list.push_back(pLight);
    pLight = new PointLight(Point3D(0.4,0,5), Color(0.9,0.9,0.9));
    light_list.push_back(pLight);
    pLight = new PointLight(Point3D(-0.4,0,5), Color(0.9,0.9,0.9));
    light_list.push_back(pLight);
    pLight = new PointLight(Point3D(0,0.4,5), Color(0.9,0.9,0.9));
    light_list.push_back(pLight);
    pLight = new PointLight(Point3D(0,-0.4,5), Color(0.9,0.9,0.9));
    light_list.push_back(pLight);
    pLight = new PointLight(Point3D(0.4,-0.4,5), Color(0.9,0.9,0.9));
    light_list.push_back(pLight);
    
    // Add a unit square into the scene with material mat.
    SceneNode* sphere = new SceneNode(new UnitSphere(), &gold);
    scene.push_back(sphere);
    SceneNode* plane = new SceneNode(new UnitSquare(), &jade);
    scene.push_back(plane);
    
    // Apply some transformations to the sphere and unit square.
    double factor1[3] = { 1.0, 2.0, 1.0 };
    sphere->translate(Vector3D(0, 0, -5));
    sphere->rotate('x', -45);
    sphere->rotate('z', 45);
    sphere->scale(Point3D(0, 0, 0), factor1);
    
    double factor2[3] = { 6.0, 6.0, 6.0 };
    plane->translate(Vector3D(0, 0, -7));
    plane->rotate('z', 45);
    plane->scale(Point3D(0, 0, 0), factor2);
    
    // Render the scene, feel free to make the image smaller for
    // testing purposes.
    Camera camera1(Point3D(0, 0, 1), Vector3D(0, 0, -1), Vector3D(0, 1, 0), 60.0);
    Image image1(width, height);
    raytracer.render_softshadow(camera1, scene, light_list, image1); //render 3D scene to image
    image1.flushPixelBuffer("softshadow1.bmp"); //save rendered image to file
    
    // Render it from a different point of view.
    Camera camera2(Point3D(4, 2, 1), Vector3D(-4, -2, -6), Vector3D(0, 1, 0), 60.0);
    Image image2(width, height);
    raytracer.render_softshadow(camera2, scene, light_list, image2);
    image2.flushPixelBuffer("softshadow2.bmp");
    
    // Free memory
    for (size_t i = 0; i < scene.size(); ++i) {
        delete scene[i];
    }
    
    for (size_t i = 0; i < light_list.size(); ++i) {
        delete light_list[i];
    }
    
    return 0;
}

int anti(int width,int height) {
    Raytracer raytracer;
    LightList light_list;
    Scene scene;

    
    // Define materials for shading.
    Material gold(Color(0.3, 0.3, 0.3), Color(0.75164,0.60648,0.22648),
                  Color(0.628281, 0.555802, 0.366065),
                  51.2);
    Material jade(Color(0, 0, 0), Color(0.54,0.89,0.63),
                  Color(0.316228,0.316228,0.316228),
                  12.8);
    
    // Defines a point light source.
    PointLight* pLight = new PointLight(Point3D(0,0,5), Color(0.9,0.9,0.9));
    light_list.push_back(pLight);
    
    // Add a unit square into the scene with material mat.
    SceneNode* sphere = new SceneNode(new UnitSphere(), &gold);
    scene.push_back(sphere);
    SceneNode* plane = new SceneNode(new UnitSquare(), &jade);
    scene.push_back(plane);
    
    // Apply some transformations to the sphere and unit square.
    double factor1[3] = { 1.0, 2.0, 1.0 };
    sphere->translate(Vector3D(0, 0, -5));
    sphere->rotate('x', -45);
    sphere->rotate('z', 45);
    sphere->scale(Point3D(0, 0, 0), factor1);
    
    double factor2[3] = { 6.0, 6.0, 6.0 };
    plane->translate(Vector3D(0, 0, -7));
    plane->rotate('z', 45);
    plane->scale(Point3D(0, 0, 0), factor2);
    
    // Render the scene, feel free to make the image smaller for
    // testing purposes.
    Camera camera1(Point3D(0, 0, 1), Vector3D(0, 0, -1), Vector3D(0, 1, 0), 60.0);
    Image image1(width, height);
    raytracer.render_anti(camera1, scene, light_list, image1); //render 3D scene to image
    image1.flushPixelBuffer("antialiasing1.bmp"); //save rendered image to file
    
    // Render it from a different point of view.
    Camera camera2(Point3D(4, 2, 1), Vector3D(-4, -2, -6), Vector3D(0, 1, 0), 60.0);
    Image image2(width, height);
    raytracer.render_anti(camera2, scene, light_list, image2);
    image2.flushPixelBuffer("antialiasing2.bmp");
    
    // Free memory
    for (size_t i = 0; i < scene.size(); ++i) {
        delete scene[i];
    }
    
    for (size_t i = 0; i < light_list.size(); ++i) {
        delete light_list[i];
    }
    
    return 0;
}

int environment(int width,int height) {
    Raytracer raytracer;
    LightList light_list;
    Scene scene;

    
    // Define materials for shading.
    Material gold(Color(0.3, 0.3, 0.3), Color(0.75164,0.60648,0.22648),
                  Color(0.628281, 0.555802, 0.366065),
                  51.2);
    Material jade(Color(0, 0, 0), Color(0.54,0.89,0.63),
                  Color(0.316228,0.316228,0.316228),
                  12.8);
    
    // Defines a point light source.
    PointLight* pLight = new PointLight(Point3D(0,0,5), Color(0.9,0.9,0.9));
    light_list.push_back(pLight);
    
    // Add a unit square into the scene with material mat.
    SceneNode* sphere = new SceneNode(new UnitSphere(), &gold);
    scene.push_back(sphere);
    SceneNode* plane = new SceneNode(new UnitSquare(), &jade);
    scene.push_back(plane);
    
    // Apply some transformations to the sphere and unit square.
    double factor1[3] = { 1.0, 2.0, 1.0 };
    sphere->translate(Vector3D(0, 0, -5));
    sphere->rotate('x', -45);
    sphere->rotate('z', 45);
    sphere->scale(Point3D(0, 0, 0), factor1);
    
    double factor2[3] = { 6.0, 6.0, 6.0 };
    plane->translate(Vector3D(0, 0, -7));
    plane->rotate('z', 45);
    plane->scale(Point3D(0, 0, 0), factor2);
    
    // Render the scene, feel free to make the image smaller for
    // testing purposes.
    Camera camera1(Point3D(0, 0, 1), Vector3D(0, 0, -1), Vector3D(0, 1, 0), 60.0);
    Image image1(width, height);
    raytracer.render_cube(camera1, scene, light_list, image1); //render 3D scene to image
    image1.flushPixelBuffer("environmentmap1.bmp"); //save rendered image to file
    
    // Render it from a different point of view.
    Camera camera2(Point3D(4, 2, 1), Vector3D(-4, -2, -6), Vector3D(0, 1, 0), 60.0);
    Image image2(width, height);
    raytracer.render_cube(camera2, scene, light_list, image2);
    image2.flushPixelBuffer("environmentmap2.bmp");
    
    // Free memory
    for (size_t i = 0; i < scene.size(); ++i) {
        delete scene[i];
    }
    
    for (size_t i = 0; i < light_list.size(); ++i) {
        delete light_list[i];
    }
    
    return 0;
}

int hardshadow(int width,int height) {
    Raytracer raytracer;
    LightList light_list;
    Scene scene;

    
    // Define materials for shading.
    Material gold(Color(0.3, 0.3, 0.3), Color(0.75164,0.60648,0.22648),
                  Color(0.628281, 0.555802, 0.366065),
                  51.2);
    Material jade(Color(0, 0, 0), Color(0.54,0.89,0.63),
                  Color(0.316228,0.316228,0.316228),
                  12.8);
    
    // Defines a point light source.
    PointLight* pLight = new PointLight(Point3D(0,0,5), Color(0.9,0.9,0.9));
    light_list.push_back(pLight);
    
    // Add a unit square into the scene with material mat.
    SceneNode* sphere = new SceneNode(new UnitSphere(), &gold);
    scene.push_back(sphere);
    SceneNode* plane = new SceneNode(new UnitSquare(), &jade);
    scene.push_back(plane);
    
    // Apply some transformations to the sphere and unit square.
    double factor1[3] = { 1.0, 2.0, 1.0 };
    sphere->translate(Vector3D(0, 0, -5));
    sphere->rotate('x', -45);
    sphere->rotate('z', 45);
    sphere->scale(Point3D(0, 0, 0), factor1);
    
    double factor2[3] = { 6.0, 6.0, 6.0 };
    plane->translate(Vector3D(0, 0, -7));
    plane->rotate('z', 45);
    plane->scale(Point3D(0, 0, 0), factor2);
    
    // Render the scene, feel free to make the image smaller for
    // testing purposes.
    Camera camera1(Point3D(0, 0, 1), Vector3D(0, 0, -1), Vector3D(0, 1, 0), 60.0);
    Image image1(width, height);
    raytracer.render_hardshadow(camera1, scene, light_list, image1); //render 3D scene to image
    image1.flushPixelBuffer("hardshadow1.bmp"); //save rendered image to file
    
    // Render it from a different point of view.
    Camera camera2(Point3D(4, 2, 1), Vector3D(-4, -2, -6), Vector3D(0, 1, 0), 60.0);
    Image image2(width, height);
    raytracer.render_hardshadow(camera2, scene, light_list, image2);
    image2.flushPixelBuffer("hardshadow2.bmp");
    
    // Free memory
    for (size_t i = 0; i < scene.size(); ++i) {
        delete scene[i];
    }
    
    for (size_t i = 0; i < light_list.size(); ++i) {
        delete light_list[i];
    }
    
    return 0;
}

int glossy(int width,int height) {
    Raytracer raytracer;
    LightList light_list;
    Scene scene;
    
    
    // Define materials for shading.
    Material gold(Color(0.3, 0.3, 0.3), Color(0.75164,0.60648,0.22648),
                  Color(0.628281, 0.555802, 0.366065),
                  51.2);
    Material jade(Color(0, 0, 0), Color(0.54,0.89,0.63),
                  Color(0.316228,0.316228,0.316228),
                  12.8);
    
    // Defines a point light source.
    PointLight* pLight = new PointLight(Point3D(0,0,5), Color(0.9,0.9,0.9));
    light_list.push_back(pLight);
    
    // Add a unit square into the scene with material mat.
    SceneNode* sphere = new SceneNode(new UnitSphere_glossy(), &gold);
    scene.push_back(sphere);
    SceneNode* plane = new SceneNode(new UnitSquare(), &jade);
    scene.push_back(plane);
    
    // Apply some transformations to the sphere and unit square.
    double factor1[3] = { 1.0, 2.0, 1.0 };
    sphere->translate(Vector3D(0, 0, -5));
    sphere->rotate('x', -45);
    sphere->rotate('z', 45);
    sphere->scale(Point3D(0, 0, 0), factor1);
    
    double factor2[3] = { 6.0, 6.0, 6.0 };
    plane->translate(Vector3D(0, 0, -7));
    plane->rotate('z', 45);
    plane->scale(Point3D(0, 0, 0), factor2);
    
    // Render the scene, feel free to make the image smaller for
    // testing purposes.
    Camera camera1(Point3D(0, 0, 1), Vector3D(0, 0, -1), Vector3D(0, 1, 0), 60.0);
    Image image1(width, height);
    raytracer.render_glossy(camera1, scene, light_list, image1); //render 3D scene to image
    image1.flushPixelBuffer("glossy1.bmp"); //save rendered image to file
    
    // Render it from a different point of view.
    Camera camera2(Point3D(4, 2, 1), Vector3D(-4, -2, -6), Vector3D(0, 1, 0), 60.0);
    Image image2(width, height);
    raytracer.render_glossy(camera2, scene, light_list, image2);
    image2.flushPixelBuffer("glossy2.bmp");
    
    // Free memory
    for (size_t i = 0; i < scene.size(); ++i) {
        delete scene[i];
    }
    
    for (size_t i = 0; i < light_list.size(); ++i) {
        delete light_list[i];
    }
    
    return 0;
}


int full(int width,int height) {
    // Build your scene and setup your camera here, by calling
    // functions from Raytracer.  The code here sets up an example
    // scene and renders it from two different view points, DO NOT
    // change this if you're just implementing part one of the
    // assignment.
    Raytracer raytracer;
    LightList light_list;
    Scene scene;
    
    
    // Define materials for shading.
    Material gold(Color(0.3, 0.3, 0.3), Color(0.75164,0.60648,0.22648),
                  Color(0.628281, 0.555802, 0.366065),
                  51.2);
    Material jade(Color(0, 0, 0), Color(0.54,0.89,0.63),
                  Color(0.316228,0.316228,0.316228),
                  12.8);
    
    // Defines a point light source.
    // multipule light to produce area light
    // soft shadow
    PointLight* pLight = new PointLight(Point3D(0,0,5), Color(0.9,0.9,0.9));
    light_list.push_back(pLight);
    pLight = new PointLight(Point3D(0.4,0,5), Color(0.9,0.9,0.9));
    light_list.push_back(pLight);
    pLight = new PointLight(Point3D(-0.4,0,5), Color(0.9,0.9,0.9));
    light_list.push_back(pLight);
    pLight = new PointLight(Point3D(0,0.4,5), Color(0.9,0.9,0.9));
    light_list.push_back(pLight);
    pLight = new PointLight(Point3D(0,-0.4,5), Color(0.9,0.9,0.9));
    light_list.push_back(pLight);
    pLight = new PointLight(Point3D(0.4,-0.4,5), Color(0.9,0.9,0.9));
    light_list.push_back(pLight);
    
    // Add a unit square into the scene with material mat.
    SceneNode* sphere = new SceneNode(new UnitSphere_glossy(), &gold);
    scene.push_back(sphere);
    SceneNode* plane = new SceneNode(new UnitSquare(), &jade);
    scene.push_back(plane);
    SceneNode* cube = new SceneNode(new UnitCube(), &gold);
    scene.push_back(cube);
    SceneNode* sphere_nog = new SceneNode(new UnitSphere(), &gold);
    scene.push_back(sphere_nog);
    
    // Apply some transformations to the sphere and unit square.
    double factor1[3] = { 2.0, 2.0, 2.0 };
    sphere->translate(Vector3D(0, 0, -5));
    sphere->rotate('z', 45);
    sphere->scale(Point3D(0, 0, 0), factor1);
    
    double factor2[3] = { 6.0, 6.0, 6.0 };
    plane->translate(Vector3D(0, 0, -7));
    plane->rotate('x', -45);
    plane->scale(Point3D(0, 0, 0), factor2);
    
    double factor3[3] = { 1.0, 2.0, 1.0 };
    cube->translate(Vector3D(3, 0, -5));
    cube->scale(Point3D(0, 0, 0), factor3);
    
    double factor4[3] = { 1.0, 2.0, 1.0 };
    sphere_nog->translate(Vector3D(-3, 0, -5));
    sphere_nog->scale(Point3D(0, 0, 0), factor4);
    
    // Render the scene, feel free to make the image smaller for
    // testing purposes.
    Camera camera1(Point3D(0, 0, 1), Vector3D(0, 0, -1), Vector3D(0, 1, 0), 60.0);
    Image image1(width, height);
    raytracer.render(camera1, scene, light_list, image1); //render 3D scene to image
    image1.flushPixelBuffer("full1.bmp"); //save rendered image to file
    
    // Render it from a different point of view.
    Camera camera2(Point3D(4, 2, 1), Vector3D(-4, -2, -6), Vector3D(0, 1, 0), 60.0);
    Image image2(width, height);
    raytracer.render(camera2, scene, light_list, image2);
    image2.flushPixelBuffer("full2.bmp");
    
    // Free memory
    for (size_t i = 0; i < scene.size(); ++i) {
        delete scene[i];
    }
    
    for (size_t i = 0; i < light_list.size(); ++i) {
        delete light_list[i];
    }
    
    return 0;
}
