#pragma once
#ifndef RAY_H
#define RAY_H

#include <glm/glm.hpp>

#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

class ray {
public:
    ray() {}
    ray(const vec3& origin, const vec3& direction)
        : orig(origin), dir(direction)
    {}

public:
    vec3 orig;
    vec3 dir;
};

#endif