#pragma once
#ifndef _SPHERE_H_
#define _SPHERE_H 1
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
class Sphere : public Shape
{
public:
    Sphere(glm::vec3 center, float radius, const glm::mat4& transformation, glm::mat4& inverseTrans, glm::mat4& invtranspose, glm::vec3 invOrigin,
        glm::vec3 amb, glm::vec3 dif, glm::vec3 emissn, glm::vec3 specula, float shinines);
    bool intersection(ray r, hitInfo& hit, float tmin, float tmax) const;
    glm::vec3 ambi, diffu, specul, emiss, center;
    float shini, rad;
    glm::mat4 trans;
    glm::mat4 invTrans;
    glm::mat4 invTranspose;
    glm::vec3 rayorign;
};
#endif