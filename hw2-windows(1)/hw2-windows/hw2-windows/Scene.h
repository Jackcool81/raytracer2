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
    pair<float, vec3> intersection(ray r) {


        //vec3 rayorigin = vec3(inverse(trans) * vec4(r.orig, 1));
        vec3 raydirection = glm::normalize(vec3(invTrans * vec4(r.dir, 0)));
        rayorigin = vec3(invTrans * vec4(r.orig, 1));

        //compute the hit
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
private:
    vec3 xyz;
    float rad;
  
   
    vec3 rayorigin;
    
};


class Triangle : public Scene {
public:



    Triangle(vec3& verts, vec3& verts2, vec3& verts3, const mat4& inverseTrans, vec3& origin, vec3 amb, vec3 dif,
        vec3 emissn, vec3 specula, float shinines)
        : A(verts), B(verts2), C(verts3), trans(inverseTrans), rayorigin(origin)
    {
        for (int i = 0; i < 3; i++) {
            ambi[i] = amb[i];
            diffu[i] = dif[i];
            emiss[i] = emissn[i];
            specul[i] = specula[i];
            
        }
        shini = shinines;
    }

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


        vec3 raydirection = glm::normalize(vec3(trans * vec4(r.dir, 0)));
        vec3 orig = vec3(inverse(trans) * vec4(r.orig, 1));

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
        

    //, float &minDist
    pair<float, vec3> findIntersection(const ray &ray) const
    {
        vec3 normal = normalize(cross((B - A), (C - A)));
        vec3 newA = vec3(trans * vec4(A,1)) ;
        vec3 newB = vec3(trans * vec4(B, 1));
        vec3 newC = vec3(trans * vec4(C, 1));
    
    const glm::vec3 BminusA = B - A;
    const glm::vec3 CminusA = C - A;
    const float t = (dot(A, normal) - glm::dot(ray.orig, normal)) / glm::dot(ray.dir, normal);
    //if (t > 0 && t < minDist) {
        const glm::vec3 P = ray.orig + t * ray.dir;
        const glm::vec3 PminusA = P - A;
        // solve equations: P-A=a(B-A)+b(C-A)
        // TODO: refactor this shit
        float PminusA_1;
        float PminusA_2;
        float BminusA_1;
        float BminusA_2;
        float CminusA_1;
        float CminusA_2;
        bool ready = false;
        if (!ready && BminusA.x != 0.0) {
            PminusA_1 = PminusA.x;
            BminusA_1 = BminusA.x;
            CminusA_1 = CminusA.x;
            if (CminusA.y != 0.0) {
                PminusA_2 = PminusA.y;
                BminusA_2 = BminusA.y;
                CminusA_2 = CminusA.y;
                ready = true;
            } else if (CminusA.z != 0.0) {
                PminusA_2 = PminusA.z;
                BminusA_2 = BminusA.z;
                CminusA_2 = CminusA.z;
                ready = true;
            }
        }
        if (!ready && BminusA.y != 0) {
            PminusA_1 = PminusA.y;
            BminusA_1 = BminusA.y;
            CminusA_1 = CminusA.y;
            if (CminusA.x != 0.0) {
                PminusA_2 = PminusA.x;
                BminusA_2 = BminusA.x;
                CminusA_2 = CminusA.x;
                ready = true;
            } else if (CminusA.z != 0.0) {
                PminusA_2 = PminusA.z;
                BminusA_2 = BminusA.z;
                CminusA_2 = CminusA.z;
                ready = true;
            }
        }
        if (!ready && BminusA.z != 0) {
            PminusA_1 = PminusA.z;
            BminusA_1 = BminusA.z;
            CminusA_1 = CminusA.z;
            if (CminusA.x != 0.0) {
                PminusA_2 = PminusA.x;
                BminusA_2 = BminusA.x;
                CminusA_2 = CminusA.x;
                ready = true;
            } else if (CminusA.y != 0.0) {
                PminusA_2 = PminusA.y;
                BminusA_2 = BminusA.y;
                CminusA_2 = CminusA.y;
                ready = true;
            }
        }
        if (!ready) {
            return pair<float, vec3>(0, vec3(-1, -1, -1));
        }
        const float b1 = (PminusA_2 * BminusA_1 - PminusA_1 * BminusA_2);
        const float b2 = (CminusA_2 * BminusA_1 - CminusA_1 * BminusA_2);
        const float b = b1 / b2;
        const float a = (PminusA_1 - b * CminusA_1) / BminusA_1;
        
        if ((0 <= a) && (a <= 1)
                && (0 <= b) && (b <= 1)
                && (a + b <= 1)) {
            //minDist = t;

         
            /*
            
            
                result.t = t;
            result.point = P;
            result.normal = normal;
            result.primitive = this;
            result.empty = false;
            
            */
        
            return pair<float, vec3>(glm::distance(ray.orig, P) , P);
        }
    
    return pair<float, vec3>(0, vec3(-1, -1, -1));
}





   
    pair<float, vec3> intersection(ray r) { 
        //vec3 normal = glm::normalize(cross(normalize(C - A), normalize(B - A)));

        vec3 normal = normalize(cross(normalize(B - A), normalize(C - A)));
      //  vec3 raydirection = glm::normalize(vec3(trans * vec4(r.dir, 0)));
        vec3 raydirection = vec3(trans * vec4(r.dir, 0));
        if (dot(raydirection, normal) == 0) {
            return pair<float, vec3>(0, vec3(-1, -1, -1));
        }


        vec3 orig = vec3(trans * vec4(r.orig, 1));
        rayorigin = orig;
        float t = dot(normal, (A - rayorigin)) / dot(raydirection, normal);
      

        vec3 P = rayorigin + (t * raydirection);

        float beta = SolveBary(normal, C - B, A - C, C, P);

        float gamma = SolveBary(normal, A - C, B - A, A, P);

        float alpha = 1 - beta - gamma;

        if (alpha >= 0 && beta >= 0 && gamma >= 0) {
            r.inter = r.orig + (t * r.dir);
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
    mat4 trans;
private:
   
    
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
