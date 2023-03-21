#pragma once
#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_ 1
#include <glm/glm.hpp>
//#include "variables.h"
#include "Shape.h"
//#include <GL/glew.h>
//#include <GL/glut.h>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "ray.h"
#include <utility>
#include <string>
using namespace std;
using namespace glm;
#include "Shape.h"

class Triangle : public Shape
{
public:
	Triangle(vec3 verts,vec3 verts2,vec3 verts3, mat4 trans,vec3 amb,vec3 dif,
        vec3 emissn,vec3 specula,float shinines);
	bool intersection(ray r, hitInfo& hit, float tmin, float tmax) const;
    vec3 ambi, diffu, specul, emiss;
    float shini;
    glm::vec3 A;
    glm::vec3 B;
    glm::vec3 C;
    mat4 trans;
};
#endif