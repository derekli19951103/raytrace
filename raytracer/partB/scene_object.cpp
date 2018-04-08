/***********************************************************
	
	Starter code for Assignment 3
	
	Implements scene_object.h

***********************************************************/

#include <cmath>
#include "scene_object.h"

bool UnitCube::intersect(Ray3D& ray, const Matrix4x4& worldToModel,
                           const Matrix4x4& modelToWorld) {
    Point3D transform_origin = worldToModel*ray.origin;
    Vector3D transform_dir = worldToModel*ray.dir;
    // lb is the corner of cube with minimal coordinates - left bottom, rt is maximal corner
    // r.org is origin of ray
    Point3D lb(-0.5,-0.5,-0.5);
    Point3D rt(0.5,0.5,0.5);
    double t1 = (lb[0] - transform_origin[0])/transform_dir[0];
    double t2 = (rt[0] - transform_origin[0])/transform_dir[0];
    double t3 = (lb[1] - transform_origin[1])/transform_dir[1];
    double t4 = (rt[1] - transform_origin[1])/transform_dir[1];
    double t5 = (lb[2] - transform_origin[2])/transform_dir[2];
    double t6 = (rt[2] - transform_origin[2])/transform_dir[2];
    
    double tmin = fmax(fmax(fmin(t1, t2), fmin(t3, t4)), fmin(t5, t6));
    double tmax = fmin(fmin(fmax(t1, t2), fmax(t3, t4)), fmax(t5, t6));
    double t;
    // if tmax < 0, ray is intersecting cube, but the whole cube is behind us
    // if tmin > tmax, ray doesn't intersect cube
    if (tmax < 0)
    {
        t = tmax;
        return false;
    }
    
    // if tmin > tmax, ray doesn't intersect cube
    if (tmin >= tmax)
    {
        t = tmax;
        return false;
    }

    t = tmin;
    double x = transform_origin[0] + t*transform_dir[0];
    double y = transform_origin[1] + t*transform_dir[1];
    double z = transform_origin[2] + t*transform_dir[2];
    Vector3D normal(0.0,0.0,0.0);
    // xyz > 0.49 && xyz < 0.51 has taken errors in account
    if(x >= -0.5 && x <= 0.5 && y >= -0.5 && y <= 0.5 && z > 0.49 && z < 0.51){
        normal=Vector3D(x,y,1.5);
    }
    if(x >= -0.5 && x <= 0.5 && y >= -0.5 && y <= 0.5 && z < -0.49 && z > -0.51){
        normal=Vector3D(x,y,-1.5);
    }
    if(x >= -0.5 && x <= 0.5 && z >= -0.5 && z <= 0.5 && y > 0.49 && y < 0.51){
        normal=Vector3D(x,1.5,z);
    }
    if(x >= -0.5 && x <= 0.5 && z >= -0.5 && z <= 0.5 && y < -0.49 && y > -0.51){
        normal=Vector3D(x,-1.5,z);
    }
    if(z >= -0.5 && z <= 0.5 && y >= -0.5 && y <= 0.5 && x > 0.49 && x < 0.51){
        normal=Vector3D(1.5,y,z);
    }
    if(z >= -0.5 && z <= 0.5 && y >= -0.5 && y <= 0.5 && x < -0.49 && x > -0.51){
        normal=Vector3D(-1.5,y,z);
    }
    if (ray.intersection.none || t < ray.intersection.t_value)
    {
        //If so, then get normal and intersection point
        ray.intersection.t_value = t;
        Point3D inter=Point3D(x,y,z);
        //Transform back to world coordinates
        ray.intersection.point = modelToWorld * inter;
        ray.intersection.normal = worldToModel.transpose()*normal;
        //Normalize normal
        ray.intersection.normal.normalize();
        ray.intersection.none = false;
        return true;
    }
    return false;

}

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
    Point3D transform_origin = worldToModel*ray.origin;
    Vector3D transform_dir = worldToModel*ray.dir;
    double t = -(transform_origin[2])/(transform_dir[2]); //Computing intersection location
    if (t < 0)
    {
        return false;
    }
    //Getting the point of intersection
    double x = transform_origin[0] + t*transform_dir[0];
    double y = transform_origin[1] + t*transform_dir[1];
    if (x >= -0.5 && x <= 0.5 && y >= -0.5 && y <= 0.5) //Checking if intersection point is in cube
    {
        if (ray.intersection.none || t < ray.intersection.t_value)
        {
            //If so, then get normal and intersection point
            ray.intersection.t_value = t;
            Point3D inter=Point3D(x,y,0.0);
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
    
    Point3D transform_origin = worldToModel*ray.origin;
    Vector3D transform_dir = worldToModel*ray.dir;
    Vector3D origin_vector(transform_origin[0],transform_origin[1],transform_origin[2]);
    //Computing step variables to help us compute intersection
    double A = transform_dir.dot(transform_dir);
    double B = 2.0*origin_vector.dot(transform_dir);
    double C = origin_vector.dot(origin_vector) - 1.0;
    double Delta = B*B - 4.0*A*C;
    
    double t0 = (- B - sqrt(Delta)) / (2.0*A);
    double t1 = (- B + sqrt(Delta)) / (2.0*A);

    double t;
    if (t0 < 0 && t1 < 0) {
        return false; // Neither are intersections
    } else if (t0 < 0) {
        t = t1;
    } else if (t1 < 0) {
        t = t0;
    } else {
        t = fmin(t0,t1);
    }
    
    //Get point of intersection
    double x = transform_origin[0] + t*transform_dir[0];
    double y = transform_origin[1] + t*transform_dir[1];
    double z = transform_origin[2] + t*transform_dir[2];
    
    if (x*x + y*y  + z*z < 1.1){
        if (ray.intersection.none || t < ray.intersection.t_value)
        {
            //Set point of intersection and normal
            ray.intersection.t_value = t;
            Point3D intersection=Point3D(x,y,z);
            Vector3D normal=Vector3D(x,y,z);
            //Transform back to world coordinates
            ray.intersection.point = modelToWorld * intersection;
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


