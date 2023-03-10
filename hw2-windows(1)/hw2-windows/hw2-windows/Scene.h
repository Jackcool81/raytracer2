#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <glm/glm.hpp>
#include "variables.h"

#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "ray.h"
using namespace glm;
using namespace std;
class Scene {
public:
    Scene() {}

    float intersection(ray r) {
        
        return 0;
    };
    vector<Scene*> objectz;
    vector<string> types;
    vector<vec3> vertexs;
    
    
    //Calculate the length of your dynamic array.

//Allocate the dynamic array as a pointer to a pointer to Technics - this is like
//Making an array of pointers each holding some Technics heirarchy object.
 

    

    

private:
    int test = 0;
};



class Sphere : public Scene {
public:
    Sphere() {}
    Sphere(const vec3& center, const float& radius)
        : xyz(center), rad(radius)
    {}
    //do we need to multiple our orignal center by the modelview then the transform matrix? 

    vec3 center() const { return xyz; }
    float radius() const { return rad; }
    mat4 trans; //hold the transfoomr values, center point its at
    float intersection(ray r) {
        vec3 newxyz = xyz*mat3(modelview);
        float a = dot(r.dir, r.dir);
        
        float b = dot(vec3(r.dir.x, r.dir.y, r.dir.z), (r.orig - newxyz)) * 2;

        float c = dot((r.orig - newxyz), (r.orig - newxyz)) - (rad * rad);
        float determine = (b * b) - (4 * a * c);

        if (determine < 0) {
            return 0;
        }

        float plust = (-b + sqrt(determine))/(2*a);
        float minust = (-b - sqrt(determine)) / (2 * a);

        //2 real positive
        if (plust > 0 && minust > 0) {
            if (plust < minust) {
                return plust;
            }
            else {
                return minust;
            }
        }

        //if both equal to eachother
        if (plust == minust) {
            return plust;
        }

        //One positive one negative
        if (plust > 0 && minust < 0) {
            return plust;
        }
        if (minust > 0 && plust < 0) {
            return minust;
        }

        return 0;
    }
   
private:
    vec3 xyz;
    float rad;
};

class Triangle : public Scene {
public:
    Triangle(const vec3& verts, const vec3& verts2, const vec3& verts3)
        : A(verts), B(verts2), C(verts3)
    {}


   
    float intersection(ray r) { 
        vec3 normal = cross((C - A), (B - A));
        normal = normal / sqrt((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));
        
        if (dot(r.dir, normal) == 0) {
            return 0;
        }
        float t = dot(normal, (A - r.orig))/ dot(r.dir,normal);

        vec3 P = r.orig + (t * r.dir);

        //is point on triangle
        
        vec3 ABcrossAP = glm::normalize(cross(B - A, P - A));
        
        float Numerator = dot(ABcrossAP, normal);
        
        
        vec3 inter;

        // edge 0
        vec3 edge0 = B - A;
        vec3 vp0 = P - A;

        inter = cross(edge0, vp0); // edge0.crossProduct(vp0);
        if (dot(normal, inter) < 0) return 0; // P is on the right side

        /*
        // edge 1
        vec3 edge1 = C - B;
        vec3 vp1 = P - B;
        inter = cross(edge1, vp1); // edge1.crossProduct(vp1);
        if (dot(normal, inter) < 0)  return 0; // P is on the right side

        // edge 2
        vec3 edge2 = A - C;
        vec3 vp2 = P - C;
        inter = cross(edge2, vp2); // edge2.crossProduct(vp2);
        if (dot(normal, inter) < 0) return 0; // P is on the right side;
        */
        float Denom = dot(normal, normal);

        float v = Numerator / Denom;


        return t; 
    }


private:
    vec3 A;
    vec3 B;
    vec3 C;
};

class Quad : public Scene {
public:
    Quad() {}

    float intersection(ray r) { return 0; }

private:

};

#endif
#pragma once
