#include "Sphere.h"

Sphere::Sphere(glm::vec3 center, float radius, const glm::mat4& transformation, glm::mat4& inverseTrans, glm::mat4& invtranspose, glm::vec3 invOrigin,
    glm::vec3 amb, glm::vec3 dif, glm::vec3 emissn, glm::vec3 specula, float shinines)
    : center(center), rad(radius), trans(transformation), invTrans(inverseTrans),invTranspose(invtranspose) {}

bool Sphere::intersection(ray r, hitInfo& hit, float tmin, float tmax)const {
    vec3 raydirection = vec3(invTrans * vec4(r.dir, 0));
    vec3 rayorigin = vec3(invTrans * vec4(r.orig, 1));
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
            hit.n = vec3(invTranspose * vec4(hit.n, 1));
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