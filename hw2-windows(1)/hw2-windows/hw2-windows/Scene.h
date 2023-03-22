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
#include <utility>
using namespace std;
using namespace glm;
using namespace std;

struct light {
    vec3 dir;
    vec3 color;
    float atten;
    string type;
};

class Scene {
public:

    

    Scene() {}

    float intersection(ray r) {
        
        return 0;
    };
    vector<Scene*> objectz;
    vector<string> types;
    vector<vec3> vertexs;
    vector<light> lights;
    int numlights = 0;

    string output = "raytrace.png";
    int maxDepth = 5;
    
    

//Allocate the dynamic array as a pointer to a pointer to Technics - this is like
//Making an array of pointers each holding some Technics heirarchy object.
 

    

    

private:
    int test = 0;
};



class Sphere : public Scene {
public:



    Sphere() {}
    Sphere(const vec3& center, const float& radius, const mat4& transformation, mat4& inverseTrans, mat3& transposeInv, vec3 invOrigin,
        vec3 amb, vec3 dif, vec3 emissn, vec3 specula, float shinines)
        : xyz(center), rad(radius), trans(transformation), invTrans(inverseTrans), transposeInv(transposeInv),rayorigin(invOrigin)
    {
        for (int i = 0; i < 3; i++) {
            ambi[i] = amb[i];
            diffu[i] = dif[i];
            emiss[i] = emissn[i];
            specul[i] = specula[i];
            
        }
        shini = shinines;
    }
    

    

    vec3 center() const { return xyz; }

    vec3 ambient() const { return ambi; }
    vec3 diffuse() const { return diffu; }
    vec3 specular() const { return specul; }
    vec3 emission() const { return emiss; }

    float radius() const { return rad; }
    pair<float, vec3> intersection(ray r) {


      
        vec3 raydirection = vec3(invTrans * vec4(r.dir, 0));

        rayorigin = vec3(invTrans * vec4(r.orig, 1));
      
        vec3 newxyz = xyz;
        float a = dot(raydirection, raydirection);

        float b = dot(vec3(raydirection.x, raydirection.y, raydirection.z), (rayorigin - newxyz)) * 2;

        float c = dot((rayorigin - newxyz), (rayorigin - newxyz)) - (rad * rad);
        float determine = (b * b) - (4 * a * c);

        if (determine < 0) {
            return  pair<float, vec3>(0, vec3(-1, -1, -1));
        }

        float plust = (-b + sqrt(determine)) / (2 * a);
        float minust = (-b - sqrt(determine)) / (2 * a);

        //find t then



        //2 real positive
        if (plust > 0 && minust > 0) {
            if (plust < minust) {

                r.inter = vec3(trans * vec4(r.pos(rayorigin, raydirection, plust), 1));
                plust = glm::distance(r.orig, r.inter);
                return  pair<float, vec3>(plust, r.inter);
            }
            else {
                r.inter = vec3(trans * vec4(r.pos(rayorigin, raydirection, minust), 1));
                minust = glm::distance(r.orig, r.inter);
                return  pair<float, vec3>(minust, r.inter);
            }
        }

        //if both equal to eachother
        if (plust == minust) {

            r.inter = vec3(trans * vec4(r.pos(rayorigin, raydirection, plust), 1));
            plust = glm::distance(r.orig, r.inter);

            return  pair<float, vec3>(plust, r.inter);
        }

        //One positive one negative
        if (plust > 0 && minust < 0) {
            r.inter = vec3(trans * vec4(r.pos(rayorigin, raydirection, plust), 1));
            plust = glm::distance(r.orig, r.inter);
            return  pair<float, vec3>(plust, r.inter);
        }
        if (minust > 0 && plust < 0) {
            r.inter = vec3(trans * vec4(r.pos(rayorigin, raydirection, minust), 1));
            minust = glm::distance(r.orig, r.inter);
            return  pair<float, vec3>(minust, r.inter);
        }

        return  pair<float, vec3>(0, vec3(-1,-1,-1));
    }

    
    vec3 ambi, diffu, specul, emiss;
    float shini;
    mat4 trans;
    mat4 invTrans;
    mat3 transposeInv;
private:
    vec3 xyz;
    float rad;
  
   
    vec3 rayorigin;
    
};


class Triangle : public Scene {
public:



    Triangle(vec3& verts, vec3& verts2, vec3& verts3, vec3& vertsp, vec3& verts2p, vec3& verts3p, vec3& normal, vec3& normalInvTrans, mat4& trans, const mat4& inverseTrans, vec3 amb, vec3 dif,
        vec3 emissn, vec3 specula, float shinines)
        : A(verts), B(verts2), C(verts3), Ap(vertsp), Bp(verts2p), Cp(verts3p), normal(normal), inverseNormal(normalInvTrans), trans(trans), invTrans(inverseTrans)
    {
        for (int i = 0; i < 3; i++) {
            ambi[i] = amb[i];
            diffu[i] = dif[i];
            emiss[i] = emissn[i];
            specul[i] = specula[i];
            
        }
        shini = shinines;
    }

    float SolveBary(vec3 normal2, vec3 Triedge1, vec3 Triedge2, vec3 P, vec3 intersec) {
        vec3 normXEdge1 = cross(normal2, Triedge1);

        vec3 newNormal = -(normXEdge1 / dot(normXEdge1, Triedge2));

        return dot(newNormal, intersec) + dot(newNormal, P);
    }

    pair<float, vec3> intersection(ray r) { 
        
        
        vec3 raydirection = vec3(invTrans * vec4(r.dir, 0));
        if (dot(raydirection, normal) == 0) {
            return pair<float, vec3>(0, vec3(-1, -1, -1));
        }


        vec3 rayorigin = vec3(invTrans * vec4(r.orig, 1));
        
        float t = dot(normal, (A - rayorigin)) / dot(raydirection, normal);
      

        vec3 P = rayorigin + (t * raydirection);

        float beta = SolveBary(normal, C - B, A - C, C, P);

        float gamma = SolveBary(normal, A - C, B - A, A, P);

        float alpha = 1 - beta - gamma;

        if (alpha >= 0 && beta >= 0 && gamma >= 0) {
        //if (beta > 0.001 && gamma > 0.001 && beta + gamma < 1) {
            //r.inter = r.orig + (t * r.dir);
            //use regular transform
            r.inter = vec3(trans * vec4(P, 1));
            //r.inter = rayorigin + (t * raydirection);
            t = glm::distance(r.orig, r.inter);
            return pair<float, vec3>(t, r.inter);
            
        }
        else {
            return pair<float, vec3>(0, vec3(-1,-1,-1));
        }
       

    }

    vec3 ambi, diffu, specul, emiss;
    float shini;
    vec3 A;
    vec3 B;
    vec3 C;

    vec3 Ap;
    vec3 Bp;
    vec3 Cp;

    vec3 normal;
    vec3 inverseNormal;

    mat4 invTrans;
    mat4 trans;
private:
   
    
    
   
};

class Quad : public Scene {
public:
    Quad() {}

    float intersection(ray r) { return 0; }

private:

};

#endif
#pragma once
