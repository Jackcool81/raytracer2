#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <glm/glm.hpp>
//#include "variables.h"
//#include <GL/glew.h>
//#include <GL/glut.h>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "ray.h"
#include <utility>
#include "Shape.h"
using namespace std;
using namespace glm;
using namespace std;

struct hitInfo {
    float t;
    vec3 n;
    vec3 inter;
    string type;
    const Shape* shape;
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

#endif
#pragma once
