/***********************************************************
	
	Starter code for Assignment 3
	
	Implements scene_object.h

***********************************************************/

#include <cmath>
#include "scene_object.h"

bool UnitSquare::intersect(Ray3D& ray, const Matrix4x4& worldToModel,
		const Matrix4x4& modelToWorld) {
	// TODO: implement intersection code for UnitSquare, which is
	// defined on the xy-plane, with vertices (0.5, 0.5, 0), 
	// (-0.5, 0.5, 0), (-0.5, -0.5, 0), (0.5, -0.5, 0), and normal
	// (0, 0, 1).
	//
	// Your goal here is to fill ray.intersection with correct values
	// should an intersection occur.  This includes intersection.point, 
	// intersection.normal, intersection.none, intersection.t_value.   
	//
	// HINT: Remember to first transform the ray into object space  
	// to simplify the intersection test.
    Ray3D r_h;
    //Transforming the world to model coordinates
    r_h.origin = worldToModel*ray.origin;
    r_h.dir = worldToModel*ray.dir;
    double t = -(r_h.origin[2])/(r_h.dir[2]); //Computing intersection location
    if (t <= 0)
    {
        return false;
    }
    //Getting the point of intersection
    double x_check = r_h.origin[0] + t*r_h.dir[0];
    double y_check = r_h.origin[1] + t*r_h.dir[1];
    //std::cout<<"x_check=" << x_check<< "\n";
    //std::cout<<"y_check=" << y_check << "\n";
    //std::cout<<"Hi";
    if (x_check >= -0.5 && x_check <= 0.5 && y_check >= -0.5 && y_check <= 0.5) //Checking if intersection point is in cube
    {
        //std::cout<<"x_check=" << x_check<< "\n";
        //std::cout<<"y_check=" << y_check << "\n";
        if (ray.intersection.none || t < ray.intersection.t_value)
        {
            //If so, then get normal and intersection point
            ray.intersection.t_value = t;
            Point3D inter=Point3D(x_check,y_check,0.0);
            Vector3D normal=Vector3D(0.0,0.0,1.0);
            //Transform back to world coordinates
            ray.intersection.point = modelToWorld * inter;
            ray.intersection.normal = worldToModel.transpose()*normal;
            //Normalize normal
            ray.intersection.normal.normalize();
            ray.intersection.none = false;
            return true;
        }
    }
    
	return false;
}

bool UnitSphere::intersect(Ray3D& ray, const Matrix4x4& worldToModel,
		const Matrix4x4& modelToWorld) {
	// TODO: implement intersection code for UnitSphere, which is centred 
	// on the origin.  
	//
	// Your goal here is to fill ray.intersection with correct values
	// should an intersection occur.  This includes intersection.point, 
	// intersection.normal, intersection.none, intersection.t_value.   
	//
	// HINT: Remember to first transform the ray into object space  
	// to simplify the intersection test.
    
    Ray3D r_h;
    r_h.origin = worldToModel*ray.origin;
    r_h.dir = worldToModel*ray.dir;
    Color r=Color(1.0,0.0,0.0);
    
    //Computing step variables to help us compute intersection
    double A = r_h.dir.dot(r_h.dir);
    double B = r_h.origin[0]*r_h.dir[0] + r_h.origin[1]*r_h.dir[1] + r_h.origin[2]*r_h.dir[2];
    double C = r_h.origin[0]*r_h.origin[0] + r_h.origin[1]*r_h.origin[1] + r_h.origin[2]*r_h.origin[2] - 1;
    double D = pow(B,2) - A*C;
    
    double t = -(B/A) - (sqrt(D)/A);
    if (t < 0)
    {
        return false;
    }
    //Get point of intersection
    double x_check = r_h.origin[0] + t*r_h.dir[0];
    double y_check = r_h.origin[1] + t*r_h.dir[1];
    double z_check = r_h.origin[2] + t*r_h.dir[2];
    if (pow(x_check,2) + pow(y_check,2)  + pow(z_check,2) <= 1.1) //Checking if point is in unit sphere
    {
        if (ray.intersection.none || t < ray.intersection.t_value)
        {
            
            //Set point of intersection and normal
            ray.intersection.t_value = t;
            Point3D inter=Point3D(x_check,y_check,z_check);
            Vector3D normal=Vector3D(x_check,y_check,z_check);
            
            
            //Transform back to world coordinates
            ray.intersection.point = modelToWorld * inter;
            ray.intersection.normal = worldToModel.transpose()*normal;
            
            //Normalize normal
            ray.intersection.normal.normalize();
            ray.intersection.none = false;
            return true;
        }
    }
    
	return false;
}

void SceneNode::rotate(char axis, double angle) {
	Matrix4x4 rotation;
	double toRadian = 2*M_PI/360.0;
	int i;
	
	for (i = 0; i < 2; i++) {
		switch(axis) {
			case 'x':
				rotation[0][0] = 1;
				rotation[1][1] = cos(angle*toRadian);
				rotation[1][2] = -sin(angle*toRadian);
				rotation[2][1] = sin(angle*toRadian);
				rotation[2][2] = cos(angle*toRadian);
				rotation[3][3] = 1;
			break;
			case 'y':
				rotation[0][0] = cos(angle*toRadian);
				rotation[0][2] = sin(angle*toRadian);
				rotation[1][1] = 1;
				rotation[2][0] = -sin(angle*toRadian);
				rotation[2][2] = cos(angle*toRadian);
				rotation[3][3] = 1;
			break;
			case 'z':
				rotation[0][0] = cos(angle*toRadian);
				rotation[0][1] = -sin(angle*toRadian);
				rotation[1][0] = sin(angle*toRadian);
				rotation[1][1] = cos(angle*toRadian);
				rotation[2][2] = 1;
				rotation[3][3] = 1;
			break;
		}
		if (i == 0) {
			this->trans = this->trans*rotation; 	
			angle = -angle;
		} 
		else {
			this->invtrans = rotation*this->invtrans; 
		}	
	}
}

void SceneNode::translate(Vector3D trans) {
	Matrix4x4 translation;
	
	translation[0][3] = trans[0];
	translation[1][3] = trans[1];
	translation[2][3] = trans[2];
	this->trans = this->trans*translation; 	
	translation[0][3] = -trans[0];
	translation[1][3] = -trans[1];
	translation[2][3] = -trans[2];
	this->invtrans = translation*this->invtrans; 
}

void SceneNode::scale(Point3D origin, double factor[3] ) {
	Matrix4x4 scale;
	
	scale[0][0] = factor[0];
	scale[0][3] = origin[0] - factor[0] * origin[0];
	scale[1][1] = factor[1];
	scale[1][3] = origin[1] - factor[1] * origin[1];
	scale[2][2] = factor[2];
	scale[2][3] = origin[2] - factor[2] * origin[2];
	this->trans = this->trans*scale; 	
	scale[0][0] = 1/factor[0];
	scale[0][3] = origin[0] - 1/factor[0] * origin[0];
	scale[1][1] = 1/factor[1];
	scale[1][3] = origin[1] - 1/factor[1] * origin[1];
	scale[2][2] = 1/factor[2];
	scale[2][3] = origin[2] - 1/factor[2] * origin[2];
	this->invtrans = scale*this->invtrans; 
}


