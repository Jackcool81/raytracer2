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

struct hitInfo {
    float t;
    vec3 n;
    vec3 inter;
    string type;
    Shape* shape;
};
class Scene {
public:
    Scene() {}
    vector<Shape*> objectz;
    vector<vec3> vertexs;
    vector<float> lightposn;
    vector<float> lightcol;
    string type;
    int numlights = 0;
};
class Shape {
public:
    Shape() {}
    virtual bool intersection(ray r, hitInfo& hit, float tmin, float tmax) const = 0;
    virtual bool shadow(ray r) const = 0;
    vector<Shape*> objectz;
    vector<string> types;
    vector<vec3> vertexs;
    vec3 xyz;
    vector<float> lightposn;
    vector<float> lightcol;
    int numlights = 0;
    vec3 ambi, diffu, specul, emiss;
    float shini;
    private:
    int test = 0;
};

class Sphere : public Shape {
public:

    Sphere() {}
    Sphere(const vec3& center, const float& radius, const mat4& transformation, mat4& inverseTrans, const mat4& invtranspose, vec3 invOrigin,
        vec3 amb, vec3 dif, vec3 emissn, vec3 specula, float shinines)
        : xyz(center), rad(radius), trans(transformation), invTrans(inverseTrans), rayorigin(invOrigin), invTranspose(invtranspose)
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

    bool intersection(ray r, hitInfo& hit, float tmin, float tmax) {
        vec3 raydirection = vec3(invTrans * vec4(r.dir, 0));
        rayorigin = vec3(invTrans * vec4(r.orig, 1));
        vec3 newxyz = xyz;
        float a = dot(raydirection, raydirection);
        float b = dot(vec3(raydirection.x, raydirection.y, raydirection.z), (rayorigin - newxyz)) * 2;
        float c = dot((rayorigin - newxyz), (rayorigin - newxyz)) - (rad * rad);
        float determine = (b * b) - (4 * a * c);
        if (determine < 0) {
            return false;
        }
        float plust = (-b + sqrt(determine)) / (2 * a);
        float minust = (-b - sqrt(determine)) / (2 * a);
        if (plust > 0 && minust > 0) {
            if (plust < minust) {
                r.inter = vec3(trans * vec4(r.pos(rayorigin, raydirection, plust), 1));
                hit.inter = r.inter;
                plust = glm::distance(r.orig, r.inter);
                hit.t = plust;
                hit.n = normalize(r.inter - xyz);
                hit.n = vec3(invTranspose * vec4(hit.n,1));
                hit.inter = r.inter;
                hit.type = "Sphere";
                hit.shape = this;
                return true;
            }
            else {
                r.inter = vec3(trans * vec4(r.pos(rayorigin, raydirection, minust), 1));
                hit.inter = r.inter;
                minust = glm::distance(r.orig, r.inter);
                hit.n = vec3(invTranspose * vec4(hit.n, 1));
                hit.t = minust;
                hit.n = normalize(r.inter - xyz);
                hit.n = vec3(invTranspose * vec4(hit.n, 1));
                hit.inter = r.inter;
                hit.type = "Sphere";
                hit.shape = this;
                return  true;
            }
        }
        if (plust == minust) {
            r.inter = vec3(trans * vec4(r.pos(rayorigin, raydirection, plust), 1));
            hit.inter = r.inter;
            plust = glm::distance(r.orig, r.inter);
            hit.t = plust;
            hit.n = normalize(r.inter - xyz);
            hit.type = "Sphere";
            hit.shape = this;
            return  true;
        }
        if (plust > 0 && minust < 0) {
            r.inter = vec3(trans * vec4(r.pos(rayorigin, raydirection, plust), 1));
            hit.inter = r.inter;
            plust = glm::distance(r.orig, r.inter);
            hit.t = plust;
            hit.n = normalize(r.inter - xyz);
            hit.type = "Sphere";
            hit.shape = this;
            return  true;
        }
        if (minust > 0 && plust < 0) {
            r.inter = vec3(trans * vec4(r.pos(rayorigin, raydirection, minust), 1));
            hit.inter = r.inter;
            minust = glm::distance(r.orig, r.inter);
            hit.t = minust;
            hit.n = normalize(r.inter - xyz);
            hit.type = "Sphere";
            hit.shape = this;
            return  true;
        }
        return false;
    }

    vec3 ambi, diffu, specul, emiss;
    float shini;
    mat4 trans;
    mat4 invTrans;
    mat4 invTranspose;

private:

    vec3 xyz;
    float rad;
    vec3 rayorigin;
    
};


class Triangle : public Shape {
public:

    Triangle(vec3 verts, vec3 verts2, vec3 verts3, mat4 trans, vec3 amb, vec3 dif,
        vec3 emissn, vec3 specula, float shinines)
        : A(verts), B(verts2), C(verts3), trans(trans)
    {
        for (int i = 0; i < 3; i++) {
            ambi[i] = amb[i];
            diffu[i] = dif[i];
            emiss[i] = emissn[i];
            specul[i] = specula[i];     
        }
        shini = shinines;
    }

    bool intersection(ray r, hitInfo& hit, float tmin, float tmax) {
        float ax = A[0];
        float ay = A[1]; 
        float az = A[2];
        float bx = B[0];
        float by = B[1];
        float bz = B[2];
        float cx = C[0];
        float cy = C[1];
        float cz = C[2];
        float dx = r.dir[0];
        float dy = r.dir[1];
        float dz = r.dir[2];
        float ox = r.orig[0];
        float oy = r.orig[1];
        float oz = r.orig[2];
        float a = ax - bx;
        float d = ax - cx;
        float g = dx;
        float b = ay - by;
        float e = ay - cy;
        float h = dy;
        float c = az - bz;
        float f = az - cz;
        float i = dz;
        float j = ax - ox;
        float k = ay - oy;
        float l = az - oz;
        float ei = e * i;
        float hf = h * f;
        float gf = g * f;
        float di = d * i;
        float dh = d * h;
        float eg = e * g;
        float ak = a * k;
        float jb = j * b;
        float jc = j * c;
        float al = a * l;
        float bl = b * l;
        float kc = k * c;
        float m = a * (ei - hf) + b * (gf - di) + c * (dh - eg);

        float beta = j * (ei - hf) + k * (gf - di) + l * (dh - eg);
        beta = beta / m;
        if (beta <= 0 || beta >= 1) {
            return false;
        }
        float gamma = i * (ak - jb) + h * (jc - al) + g * (bl - kc);
        gamma = gamma / m;

        float t = f * (ak - jb) + e * (jc - al) + d * (bl - kc);
        t = t / m;
        t = -1.0f * t;

        vec3 n = normalize(cross(B - A,C - A));
        if (gamma <= 0 || beta + gamma >= 1) {
            return false;
        }
        if(t < tmin && t > tmax){
            r.inter = r.orig + (t * r.dir);
            t = glm::distance(r.orig, r.inter);
            hit.t = t;
            hit.n = normalize(cross(B - A, C - A));
            hit.inter = r.inter;
            hit.type = "Triangle";
            hit.shape = this;
            return true;
        }
 
        else {
            return false;
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

#endif
#pragma once
