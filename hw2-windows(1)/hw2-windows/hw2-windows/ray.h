#pragma once
#ifndef RAY_H
#define RAY_H

#include <glm/glm.hpp>


using namespace glm;

class ray {
public:
    ray() {}
    ray(const vec3& origin, const vec3& direction)
        : orig(origin), dir(direction)
    {}


    vec3 pos(vec3 start, vec3 going, double t) const {
        return start + vec3(t * going[0], t * going[1], t * going[2]);
    }

public:
    vec3 orig;
    vec3 dir;
    vec3 inter = vec3(0,0,0);
};

#endif