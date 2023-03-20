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
#include <tuple>
#include <utility>
using namespace std;
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

    Sphere() {}
    Sphere(const vec3& center, const float& radius, const mat4& transformation, mat4& inverseTrans, vec3 invOrigin,
        vec3 amb, vec3 dif, vec3 emissn, vec3 specula, float shinines)
        : xyz(center), rad(radius), trans(transformation), invTrans(inverseTrans), rayorigin(invOrigin)
    {
        for (int i = 0; i < 3; i++) {
            ambi[i] = amb[i];
            diffu[i] = dif[i];
            emiss[i] = emissn[i];
            specul[i] = specula[i];
            
        }
        shini = shinines;
    }
    //do we need to multiple our orignal center by the modelview then the transform matrix? 

    int shadow(ray r) {
        vec3 rayorigin = vec3(inverse(trans) * vec4(r.orig, 1));
        vec3 raydirection = glm::normalize(vec3(invTrans * vec4(r.dir, 0)));
        //compute the hit
        vec3 newxyz = xyz;
        float a = dot(raydirection, raydirection);
        float b = dot(vec3(raydirection.x, raydirection.y, raydirection.z), (rayorigin - newxyz)) * 2;
        float c = dot((rayorigin - newxyz), (rayorigin - newxyz)) - (rad * rad);
        float determine = (b * b) - (4 * a * c);

        if (determine < 0) {
            return 1; //its visible
        }

        float plust = (-b + sqrt(determine)) / (2 * a);
        float minust = (-b - sqrt(determine)) / (2 * a);

        if (plust > 0 && minust > 0) {
            if (plust < minust) {
                return 0;
            }
            else {
                return 0;
            }
        }

        //if both equal to eachother
        if (plust == minust) {
            return 0;
        }

        //One positive one negative
        if (plust > 0 && minust < 0) {
            return 0;
        }
        if (minust > 0 && plust < 0) {
            return 0;
        }
        return 1;
    }

    vec3 center() const { return xyz; }
    vec3 ambient() const { return ambi; }
    vec3 diffuse() const { return diffu; }
    vec3 specular() const { return specul; }
    vec3 emission() const { return emiss; }
    float radius() const { return rad; }

    tuple<float,vec3,mat4,mat4> intersection(ray r) {

        //vec3 rayorigin = vec3(inverse(trans) * vec4(r.orig, 1));
        
        vec3 raydirection = glm::normalize(vec3(invTrans * vec4(r.dir, 0)));
        vec3 origOrigin = r.orig;
        vec4 temp = invTrans * vec4(r.orig, 1);
        r.orig = vec3(temp.x, temp.y, temp.z);
        //compute the hit
        vec3 newxyz = xyz;
        float a = dot(raydirection, raydirection);
        float b = dot(vec3(raydirection.x, raydirection.y, raydirection.z), (r.orig - newxyz)) * 2;
        float c = dot((r.orig - newxyz), (r.orig - newxyz)) - (rad * rad);
        float determine = (b * b) - (4 * a * c);
        if (determine < 0) {
            return  tuple<float, vec3, mat4, mat4>(0, vec3(-1, -1, -1),trans, invTrans);
        }
        float plust = (-b + sqrt(determine)) / (2 * a);
        float minust = (-b - sqrt(determine)) / (2 * a);
        //find t then
        //2 real positive
        if (plust > 0 && minust > 0) {
            if (plust < minust) {
                r.inter = vec3(trans * vec4(r.pos(r.orig, raydirection, plust), 1));
                plust = glm::distance(origOrigin, r.inter);
                return  tuple<float, vec3, mat4, mat4>(plust,r.inter,trans, invTrans);
            }
            else {
                r.inter = vec3(trans * vec4(r.pos(r.orig, raydirection, minust), 1));
                minust = glm::distance(origOrigin, r.inter);
                return  tuple<float, vec3, mat4, mat4>(minust, r.inter,trans,invTrans);
            }
        }

        //if both equal to eachother
        if (plust == minust) {

            r.inter = vec3(trans * vec4(r.pos(r.orig, raydirection, plust), 1));
            plust = glm::distance(origOrigin, r.inter);
            return  tuple<float, vec3, mat4, mat4>(plust, r.inter, trans, invTrans);
        }

        //One positive one negative
        if (plust > 0 && minust < 0) {
            r.inter = vec3(trans * vec4(r.pos(r.orig, raydirection, plust), 1));
            plust = glm::distance(origOrigin, r.inter);
            return  tuple<float, vec3, mat4, mat4>(plust, r.inter, trans, invTrans);
        }
        if (minust > 0 && plust < 0) {
            r.inter = vec3(trans * vec4(r.pos(r.orig, raydirection, minust), 1));
            minust = glm::distance(origOrigin, r.inter);
            return  tuple<float, vec3, mat4, mat4>(minust, r.inter,trans, invTrans);
        }
        return  tuple<float, vec3, mat4, mat4>(0, vec3(-1,-1,-1),trans, invTrans);
    }

 public:   
    vec3 ambi, diffu, specul, emiss;
    float shini;
    mat4 trans;
private:
    vec3 xyz;
    float rad;
  
    mat4 invTrans;
    vec3 rayorigin;
    
};


class Triangle : public Scene {
public:

    Triangle(const vec3& verts, const vec3& verts2, const vec3& verts3, const mat4& transform, const mat4& inverseTrans, vec3& origin, vec3 amb, vec3 dif,
        vec3 emissn, vec3 specula, float shinines)
        : A(verts), B(verts2), C(verts3), transf(transform), invTrans(inverseTrans), rayorigin(origin)

    {
        for (int i = 0; i < 3; i++) {
            ambi[i] = amb[i];
            diffu[i] = dif[i];
            emiss[i] = emissn[i];
            specul[i] = specula[i];
        }
        shini = shinines;
    }

    //tuple<float, vec3, mat4, mat4> intersection(ray r);

    float SolveBary(vec3 normal, vec3 Triedge1, vec3 Triedge2, vec3 P, vec3 intersec) {
        vec3 normXEdge1 = cross(normal, Triedge1);

        vec3 newNormal = -(normXEdge1 / dot(normXEdge1, Triedge2));

        return dot(newNormal, intersec) + dot(newNormal, P);
    }

    int shadow(ray r) {
        vec3 normal = glm::normalize(cross((C - A), (B - A)));
        if (dot(r.dir, normal) == 0) {
            return 1;
        }
        vec3 raydirection = glm::normalize(vec3(transf * vec4(r.dir, 0)));
        vec3 orig = vec3(inverse(transf) * vec4(r.orig, 1));
        float t = dot(normal, (A - orig)) / dot(raydirection, normal);
        vec3 P = orig + (t * raydirection);
        float beta = SolveBary(normal, C - B, A - C, C, P);
        float gamma = SolveBary(normal, A - C, B - A, A, P);
        float alpha = 1 - beta - gamma;
        if (alpha >= 0 && beta >= 0 && gamma >= 0) {
            return 0;
        }
        else {
            return 1;
        }
    }
        
   
    tuple<float, vec3, mat4, mat4> intersection(ray r) {
  
        vec3 normal = glm::normalize(cross((B - A), (C - A)));
        //vec3 raydirection = glm::normalize(vec3(invTrans * vec4(r.dir, 0)));
        vec3 raydirection = normalize(vec3(invTrans * vec4(r.dir, 1)));
        vec4 temp = invTrans * vec4(r.orig, 1);
        float offset = .01;
        r.orig = vec3(temp.x, temp.y, temp.z);
        if (dot(raydirection, normal) == 0) {
            return tuple<float, vec3, mat4, mat4>(0, vec3(-1, -1, -1), transf, invTrans);
        } 
            float t = dot(normal, (A - r.orig)) / dot(raydirection, normal);
            vec3 P = r.orig + (t * raydirection);
            vec4 tempz = transf * vec4(P, 1);
            P = vec3(tempz.x, tempz.y, tempz.z);
            float beta = SolveBary(normal, C - B, A - C, C, P);
            float gamma = SolveBary(normal, A - C, B - A, A, P);
            float alpha = 1 - beta - gamma;

            if (alpha >= 0 && beta >= 0 && gamma >= 0) {
                r.inter = P;
                t = glm::distance(r.orig, r.inter);
                return tuple<float, vec3, mat4, mat4>(t, r.inter, transf, invTrans);
            }
            else {
                return tuple<float, vec3, mat4, mat4>(0, vec3(-1, -1, -1), transf, invTrans);
            }
     }
    

public:
    vec3 ambi, diffu, specul, emiss;
    float shini;
    vec3 A;
    vec3 B;
    vec3 C;
private:
    mat4 invTrans, transf;
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
