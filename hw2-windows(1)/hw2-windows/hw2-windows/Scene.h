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
    

private:
    int test = 0;
};



class Sphere : public Scene {
public:
    Sphere() {}
    Sphere(const vec3& center, const int& radius)
        : xyz(center), rad(radius)
    {}
    //do we need to multiple our orignal center by the modelview then the transform matrix? 

    vec3 center() const { return xyz; }
    int radius() const { return rad; }
    mat4 trans; //hold the transfoomr values, center point its at
    float intersection(ray r) {
        vec3 newxyz = xyz*mat3(modelview);
        float a = dot(r.dir, r.dir);
        
        float b = dot(vec3(r.dir.x * 2, r.dir.y * 2, r.dir.z * 2), (r.orig - newxyz));

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
    int rad;
};

class Triangle : public Scene {
public:
    Triangle() {}

    float intersection(ray r) { return 0; }

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
