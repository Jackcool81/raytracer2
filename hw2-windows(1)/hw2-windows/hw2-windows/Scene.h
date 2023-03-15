
#pragma once
#ifndef SCENE_H
#define SCENE_H
#include <glm/glm.hpp>
#include "variables.h"
#include <cmath>

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
    vector<float> lightposn;
    vector<float> lightcol;
    int numlights = 0;
    
    //Calculate the length of your dynamic array.

//Allocate the dynamic array as a pointer to a pointer to Technics - this is like
//Making an array of pointers each holding some Technics heirarchy object.
 

    

    

private:
    int test = 0;
};



class Sphere : public Scene {
public:

    vec2 uv ;

    Sphere() {}
    Sphere(const vec3& center, const float& radius, const mat4& transformation, mat4& inverseTrans, vec3 invOrigin,
        float amb[4], float dif[4], float emissn[4], float specula[4], float shinines)
        : xyz(center), rad(radius), trans(transformation), invTrans(inverseTrans), rayorigin(invOrigin)
    {
        for (int i = 0; i < 4; i++) {
            ambi[i] = amb[i];
            diffu[i] = dif[i];
            emiss[i] = emissn[i];
            specul[i] = specula[i];
            shini = shinines;
        }

    }
    //do we need to multiple our orignal center by the modelview then the transform matrix? 

    vec3 center() const { return xyz; }
    float radius() const { return rad; }

    void textureColor(vec3 point) {
       /* float theta = atan2(-(point.z - xyz.z), point.x - xyz.x);
        const double pi = 3.14159265358979323846;
        float u = (theta + pi)/2*pi;
        float phi = acos(-(point.y - xyz.y) / rad);
        float v = phi / pi;
        uv.x = u;
        uv.y = v; */
        const double pi = 3.14159265358979323846;
        double v = asin(point.z / rad);
        double oneEighty = 180;
        double vDeg = v * (oneEighty / pi);
        double u = acos(point.x / rad * cos(v));
        u = u * (oneEighty / pi);
        double ninety = 90;
  
        double threeSixty = 360;
        uv.x = (u + oneEighty) / threeSixty;
        uv.y = (vDeg + ninety) / oneEighty;
    }

    float intersection(ray r) {


        //vec3 rayorigin = vec3(inverse(trans) * vec4(r.orig, 1));
        vec3 raydirection = glm::normalize(vec3(invTrans * vec4(r.dir, 0)));


        //compute the hit
        vec3 newxyz = xyz;
        float a = dot(raydirection, raydirection);

        float b = dot(vec3(raydirection.x, raydirection.y, raydirection.z), (rayorigin - newxyz)) * 2;

        float c = dot((rayorigin - newxyz), (rayorigin - newxyz)) - (rad * rad);
        float determine = (b * b) - (4 * a * c);

        if (determine < 0) {
            return 0;
        }

        float plust = (-b + sqrt(determine)) / (2 * a);
        float minust = (-b - sqrt(determine)) / (2 * a);

        //find t then



        //2 real positive
        if (plust > 0 && minust > 0) {
            if (plust < minust) {

                r.inter = vec3(trans * vec4(r.pos(rayorigin, raydirection, plust), 1));
                plust = glm::distance(r.orig, r.inter);
                textureColor(r.inter);
                return plust;
            }
            else {
                r.inter = vec3(trans * vec4(r.pos(rayorigin, raydirection, minust), 1));
                minust = glm::distance(r.orig, r.inter);
                textureColor(r.inter);
                return minust;
            }
        }

        //if both equal to eachother
        if (plust == minust) {

            r.inter = vec3(trans * vec4(r.pos(rayorigin, raydirection, plust), 1));
            plust = glm::distance(r.orig, r.inter);
            textureColor(r.inter);
            return plust;
        }

        //One positive one negative
        if (plust > 0 && minust < 0) {
            r.inter = vec3(trans * vec4(r.pos(rayorigin, raydirection, plust), 1));
            plust = glm::distance(r.orig, r.inter);
            textureColor(r.inter);
            return plust;
        }
        if (minust > 0 && plust < 0) {
            r.inter = vec3(trans * vec4(r.pos(rayorigin, raydirection, minust), 1));
            minust = glm::distance(r.orig, r.inter);
            textureColor(r.inter);
            return minust;
        }

        return 0;
    }

private:
    vec3 xyz;
    float rad;
    mat4 trans;
    mat4 invTrans;
    vec3 rayorigin;
    float ambi[4], diffu[4], specul[4], emiss[4], shini;
};


class Triangle : public Scene {
public:

    float ambient[4], diffuse[4], specular[4], emission[4], shininess;

    Triangle(const vec3& verts, const vec3& verts2, const vec3& verts3, const mat4& inverseTrans, vec3& origin,float amb[4], float dif[4],
        float emissn[4], float specula[4], float shinines)
        : A(verts), B(verts2), C(verts3), trans(inverseTrans), rayorigin(origin)
    {
        for (int i = 0; i < 4; i++) {
            ambi[i] = amb[i];
            diffu[i] = dif[i];
            emiss[i] = emissn[i];
            specul[i] = specula[i];
            shini = shinines;
        }
        
    }

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


        vec3 raydirection = glm::normalize(vec3(trans * vec4(r.dir, 0)));
        
        float t = dot(normal, (A - rayorigin)) / dot(raydirection, normal);
      

        vec3 P = rayorigin + (t * raydirection);

        float beta = SolveBary(normal, C - B, A - C, C, P);

        float gamma = SolveBary(normal, A - C, B - A, A, P);

        float alpha = 1 - beta - gamma;

        if (alpha >= 0 && beta >= 0 && gamma >= 0) {
            r.inter = r.orig + (t * r.dir);
            t = glm::distance(r.orig, r.inter);
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
    mat4 trans;
    vec3 rayorigin;
    float ambi[4], diffu[4], specul[4], emiss[4], shini;
};

class Quad : public Scene {
public:
    Quad() {}

    float intersection(ray r) { return 0; }

private:

};

#endif
#pragma once
