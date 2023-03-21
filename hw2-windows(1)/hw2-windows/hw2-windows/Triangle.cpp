#include "Triangle.h"

Triangle::Triangle(vec3 verts, vec3 verts2, vec3 verts3, mat4 trans, vec3 amb, vec3 dif,
    vec3 emissn, vec3 specula, float shinines)
    : A(verts), B(verts2), C(verts3), trans(trans)
{
    for (int i = 0; i < 3; i++) {
        ambi[i] = amb[i];
        diffu[i] = dif[i];
        emiss[i] = emissn[i];
        specul[i] = specula[i];
    }
    shini = shinines;
}

bool Triangle::intersection(ray r, hitInfo& hit, float tmin, float tmax) const
{
        float ax = A[0];
        float ay = A[1];
        float az = A[2];
        float bx = B[0];
        float by = B[1];
        float bz = B[2];
        float cx = C[0];
        float cy = C[1];
        float cz = C[2];
        float dx = r.dir[0];
        float dy = r.dir[1];
        float dz = r.dir[2];
        float ox = r.orig[0];
        float oy = r.orig[1];
        float oz = r.orig[2];
        float a = ax - bx;
        float d = ax - cx;
        float g = dx;
        float b = ay - by;
        float e = ay - cy;
        float h = dy;
        float c = az - bz;
        float f = az - cz;
        float i = dz;
        float j = ax - ox;
        float k = ay - oy;
        float l = az - oz;
        float ei = e * i;
        float hf = h * f;
        float gf = g * f;
        float di = d * i;
        float dh = d * h;
        float eg = e * g;
        float ak = a * k;
        float jb = j * b;
        float jc = j * c;
        float al = a * l;
        float bl = b * l;
        float kc = k * c;
        float m = a * (ei - hf) + b * (gf - di) + c * (dh - eg);

        float beta = j * (ei - hf) + k * (gf - di) + l * (dh - eg);
        beta = beta / m;
        if (beta <= 0 || beta >= 1) {
            return false;
        }
        float gamma = i * (ak - jb) + h * (jc - al) + g * (bl - kc);
        gamma = gamma / m;

        float t = f * (ak - jb) + e * (jc - al) + d * (bl - kc);
        t = t / m;
        t = -1.0f * t;

        vec3 n = normalize(cross(B - A, C - A));
        if (gamma <= 0 || beta + gamma >= 1) {
            return false;
        }
        if (t < tmin && t > tmax) {
            r.inter = r.orig + (t * r.dir);
            t = glm::distance(r.orig, r.inter);
            hit.t = t;
            hit.n = normalize(cross(B - A, C - A));
            hit.inter = r.inter;
            hit.type = "Triangle";
            hit.shape = this;
            return true;
        }

        else {
            return false;
        }
}
