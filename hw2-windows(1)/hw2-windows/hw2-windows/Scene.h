#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <glm/glm.hpp>
#include "variables.h"

#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
using namespace glm;
using namespace std;
class Scene {
public:
    Scene() {}
    /*
    Scene(const vec3& origin, const vec3& direction)
        : orig(origin), dir(direction)
    {}
    */
    //vec3 origin() const { return orig; }
    //vec3 direction() const { return dir; }
    /*
    vec3 at(double t) const {
        return orig + vec3(t * dir[0], t * dir[1], t * dir[2]);
    }
    */
    const int maxobjects = 10;
    int numobjects;
    vector<Scene> objects;


private:
    int test = 0;
};



class Sphere : public Scene {
public:
    Sphere() {}

    vec3 intersection() const { return vec3(0, 0, 0); }
    int radius;
    vec3 center; //xyz
private:

};

class Triangle : public Scene {
public:
    Triangle() {}

    vec3 intersection() const { return vec3(0, 0, 0); }

private:

};

class Quad : public Scene {
public:
    Quad() {}

    vec3 intersection() const { return vec3(0, 0, 0); }

private:

};

#endif
#pragma once
