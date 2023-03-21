#ifndef _SHAPE_H_
#define _SHAPE_H_ 1

#include <glm/glm.hpp>
#include "variables.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "ray.h"
#include <utility>
#include <string>
using namespace std;
using namespace glm;


struct hitInfo {
    float t;
    vec3 n;
    vec3 inter;
    string type;
    const Shape* shape;
};

class Shape {
public:
    Shape() {}
    virtual bool intersection(ray r, hitInfo& hit, float tmin, float tmax) const = 0;
    virtual bool shadow(ray r) const = 0;
    vector<Shape*> objectz;
    vector<string> types;
    vector<const glm::vec3> vertexs;
    const glm::vec3 xyz;
    vector<float> lightposn;
    vector<float> lightcol;
    int numlights = 0;
    vec3 ambi, diffu, specul, emiss;
    float shini;
private:
    int test = 0;
};
#pragma once
#endif