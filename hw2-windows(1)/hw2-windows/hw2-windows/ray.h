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

    vec3 origin() const { return orig; }
    vec3 direction() const { return dir; }

    vec3 at(double t) const {
        return orig + vec3(t * dir[0], t * dir[1], t * dir[2]);
    }

public:
    vec3 orig;
    vec3 dir;
};

#endif