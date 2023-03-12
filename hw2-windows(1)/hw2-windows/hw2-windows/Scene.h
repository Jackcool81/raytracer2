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
    Sphere(const vec3& center, const float& radius, const mat4& inverseTransform, const vec3& invertedRayOrigin)
        : xyz(center), rad(radius), trans(inverseTransform), rayorigin(invertedRayOrigin)
    {}

    //do we need to multiple our orignal center by the modelview then the transform matrix? 

    vec3 center() const { return xyz; }
    float radius() const { return rad; }
    float intersection(ray r) {

       
        vec3 raydirection = glm::normalize(vec3(trans * vec4(r.dir, 0)));

        
        //compute the hit
        vec3 newxyz = xyz;
        float a = dot(raydirection, raydirection);
        float b = dot(vec3(raydirection.x, raydirection.y, raydirection.z), (rayorigin - newxyz)) * 2;
        float c = dot((rayorigin - newxyz), (rayorigin - newxyz)) - (rad * rad);
        float determine = (b * b) - (4 * a * c);

        if (determine < 0) {
            return 0;
        }

        float plust = (-b + sqrt(determine))/(2*a);
        float minust = (-b - sqrt(determine)) / (2 * a);

        //find t then
   
        

        //2 real positive
        if (plust > 0 && minust > 0) {
            if (plust < minust) {
               

                r.inter = vec3(trans * vec4(r.pos(rayorigin, raydirection, plust), 1));
                plust = glm::distance(r.orig, r.inter);
                return plust;
            }
            else {

                r.inter = vec3(trans * vec4(r.pos(rayorigin, raydirection, minust), 1));
                minust = glm::distance(r.orig, r.inter);
                return minust;
            }
        }

        //if both equal to eachother
        if (plust == minust) {

            r.inter = vec3(trans * vec4(r.pos(rayorigin, raydirection, plust), 1));
            plust = glm::distance(r.orig, r.inter);

            return plust;
        }

        //One positive one negative
        if (plust > 0 && minust < 0) {
            r.inter = vec3(trans * vec4(r.pos(rayorigin, raydirection, plust), 1));
            plust = glm::distance(r.orig, r.inter);
            return plust;
        }
        if (minust > 0 && plust < 0) {
            r.inter = vec3(trans * vec4(r.pos(rayorigin, raydirection, minust), 1));
            minust = glm::distance(r.orig, r.inter);
            return minust;
        }

        return 0;
    }
   
private:
    vec3 xyz;
    float rad;
    mat4 trans;
    vec3 rayorigin;
};

class Triangle : public Scene {
public:
    Triangle(const vec3& verts, const vec3& verts2, const vec3& verts3)
        : A(verts), B(verts2), C(verts3) 
    {}

    float SolveBary(vec3 normal, vec3 Triedge1, vec3 Triedge2, vec3 P, vec3 intersec) {
        vec3 normXEdge1 = cross(normal, Triedge1);
        vec3 newNormal = -(normXEdge1 / dot(normXEdge1, Triedge2));
        return dot(newNormal, intersec) + dot(newNormal, P);
    }
        
   
    float intersection(ray r) { 
      
        vec3 normal = glm::normalize(cross((C - A), (B - A)));

        if (dot(r.dir, normal) == 0) {
            return 0;
        }

        //vec3 rayorigin = vec3(inverseTrans * vec4(r.orig, 1));
        //vec3 raydirection = glm::normalize(vec3(inverseTrans * vec4(r.dir, 0)));
        
        float t = dot(normal, (A - r.orig)) / dot(r.dir, normal);
      

        vec3 P = r.pos(r.orig,r.dir,t);

        float beta = SolveBary(normal, C - B, A - C, C, P);

        float gamma = SolveBary(normal, A - C, B - A, A, P);

        float alpha = 1 - beta - gamma;

        if (alpha >= 0 && beta >= 0 && gamma >= 0) {
           
            t = glm::distance(r.orig, P);
            return t;
        }
        else {
            return 0;
        }
       

    }


private:
    vec3 A;
    vec3 B;
    vec3 C;
    mat4 inverseTrans;
    vec3 rayorigin;
};

class Quad : public Scene {
public:
    Quad() {}

    float intersection(ray r) { return 0; }

private:

};

#endif
#pragma once
