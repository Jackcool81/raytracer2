// Transform.cpp: implementation of the Transform class.

// Note: when you construct a matrix using mat4() or mat3(), it will be COLUMN-MAJOR
// Keep this in mind in readfile.cpp and display.cpp
// See FAQ for more details or if you're having problems.

#include "Transform.h"

// Helper rotation function.  Please implement this.  
mat3 Transform::rotate(const float degrees, const vec3& axis) 
{
    vec3 a = glm::normalize(axis);
    float cosTheta = cos(glm::radians(degrees));
    float sinTheta = sin(glm::radians(degrees));
    mat3 matrix1 = mat3(1, 0, 0, 0, 1, 0, 0, 0, 1);
    float x = a[0];
    float y = a[1];
    float z = a[2];
    mat3 matrix2 = mat3(x*x, x*y, x*z, x*y, y*y, y*z, x*z, y*z, z*z);
    mat3 matrix3 = mat3(0, z, -y, -z, 0, x, y, -x, 0);
    mat3 ret = cosTheta * matrix1 + (1 - cosTheta) * matrix2 + sinTheta * matrix3;
    // You will change this return call
    return ret;
}

void Transform::left(float degrees, vec3& eye, vec3& up) 
{
    // YOUR CODE FOR HW2 HERE
    // Likely the same as in HW 1.  
}

void Transform::up(float degrees, vec3& eye, vec3& up) 
{
    // YOUR CODE FOR HW2 HERE 
    // Likely the same as in HW 1. 
    vec3 eyeNorm = glm::normalize(eye);
    vec3 upNorm = glm::normalize(up);
    vec3 x = glm::cross(eyeNorm, upNorm);
    eye = rotate(degrees, x)*eye;
    up = rotate(degrees, x)*up;
}

mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up) 
{
    mat4 ret;
    // YOUR CODE FOR HW2 HERE
    // Likely the same as in HW 1.
    vec3 c = glm::normalize(eye);
    vec3 b = glm::normalize(up);
    vec3 a = glm::cross(b, c);

    return mat4(a[0], b[0], c[0], 0, a[1], b[1], c[1], 0, a[2], b[2], c[2], 0, glm::dot(-a, eye), glm::dot(-b, eye), glm::dot(-c, eye), 1);
    
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
    mat4 ret;
    // YOUR CODE FOR HW2 HERE
    // New, to implement the perspective transform as well.  
    return ret;
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz) 
{
    mat4 ret;
    // YOUR CODE FOR HW2 HERE
    // Implement scaling 
    ret = mat4(sx, 0, 0, 0, 0, sy, 0, 0, 0, 0, sz, 0, 0, 0, 0, 1);
    return ret;
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz) 
{
    mat4 ret;
    // YOUR CODE FOR HW2 HERE
    // Implement translation 
    ret = mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, tx, ty, tz, 1);
    return ret;
}

// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec) 
{
    vec3 x = glm::cross(up,zvec); 
    vec3 y = glm::cross(zvec,x); 
    vec3 ret = glm::normalize(y); 
    return ret; 
}


Transform::Transform()
{

}

Transform::~Transform()
{

}
