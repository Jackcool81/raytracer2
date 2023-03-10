/******************************************************************************/
/* This is the program skeleton for homework 2 in CSE167 by Ravi Ramamoorthi  */
/* Extends HW 1 to deal with shading, more transforms and multiple objects    */
/******************************************************************************/

// You shouldn't have to edit this file at all!

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include <GL/glew.h>
#include <GL/glut.h>
#include "shaders.h"
#include "Transform.h"
#include <FreeImage.h>
#include "UCSD/grader.h"
#include "Geometry.h"
#include "ray.h"
#include "Scene.h"



using namespace std ; 

// Main variables in the program.  
#define MAINPROGRAM 
#include "variables.h" 
#include "readfile.h" // prototypes for readfile.cpp  

void display(void) ;  // prototype for display function.  

Grader grader;
bool allowGrader = false;


void saveScreenshot(BYTE pixels[], string fname) {
 
   //an array of all the pixels in our image, reason why its mulitplied by 
  //3 is for the color value of each pixel
  

  FIBITMAP *img = FreeImage_ConvertFromRawBits(pixels, w, h, w * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, true);

  std::cout << "Saving screenshot: " << fname << "\n";

  FreeImage_Save(FIF_PNG, img, fname.c_str(), 0);
  delete pixels;
}


void init() {
  
}

void mult(vec3& vector3, double t) {
    vector3 = vec3(vector3.x * t, vector3.y * t, vector3.z * t);
}
/*
vec3 ray_color(const ray& r) {
    vec3 unit_direction = vec3(r.direction());
    auto t = 0.5 * (unit_direction.y + 1.0);
    return mult(vec3(1.0, 1.0, 1.0), (1.0 - t)) + mult(vec3(0.5, 0.7, 1.0),t) ;
}
*/

/*
float Intersection(float min_t, Scene min_primitive) {

    return 0;
}
*/
int* FindIntersection(ray r, vector<Scene*> a, Scene newScene) {
    float min_t = 1000000; // number of bounces from read file
    Scene min_primitive;
    /*
    float t = sa.intersection(r);
    if (t == 0) {
        return false;
    }
    else {
        return true;
    }
    
    */
    int pixel_color[3] = { 0,0,0 };
    float t = 0;
    for (int i = 0; i < newScene.objectz.size(); i++) {
        //float t = 0;
        
        if (newScene.types[i] == "Sphere") {
            t = static_cast<Sphere*>(newScene.objectz[i])->intersection(r);
        }
        else {
            t = static_cast<Triangle*>(newScene.objectz[i])->intersection(r);

        }

        if (t > 0 && newScene.types[i] == "Sphere") {
            pixel_color[0] = 0;
            pixel_color[1] = 255;
            pixel_color[2] = 255;
           
        }
        else if (t > 0 && newScene.types[i] == "Triangle") {
            pixel_color[0] = 255;
            pixel_color[1] = 0;
            pixel_color[2] = 255;
            
        }
       
        if (t > 0 && t < min_t) {
            min_primitive = *a[i];
            min_t = t;
        }


    }

 
    return pixel_color;
  
    
   
   
    //loop through all primitives in the scene (passed in parameter)
        //t = the intersection for each primitive (shape) (smallest positive root)
          //t > 0 && t < min_t:
             //min_primitive = current primitive (the primitive we want to return the one with the smallest root) 
             //min_t = t
    //return r.at(min_t);
    //return min_t;
}


void divide(vec3 & vector3, double t) {
    vector3 = vec3(vector3.x / t, vector3.y / t, vector3.z / t);
}

void write_color(int index, BYTE pixels[], int pixel_color[]) {
    //Why did we have to reverse this isnt it in the correct order when we do 0, 1, 2
    pixels[index] = (unsigned char) pixel_color[2];
    pixels[index+1] = (unsigned char) pixel_color[1];
    pixels[index+2] = (unsigned char) pixel_color[0];
}

int main(int argc, char* argv[]) {
    
    Scene newScene = Scene();
    FreeImage_Initialise();
    readfile(argv[1], newScene);
    


    float image_width = w;
    float image_height = h;

    vec3 origin = eyeinit;
    
    float degreeToRad = (3.14 / 180.0);
    float radToDegrees = (180 / 3.14);


    //fov 
    fovy = fovy * (3.14 / 180.0);
    float newFovy = tan(fovy / 2.0);
     //with the aspect ratio
    float aspect = image_width / image_height;
    float fovx = (newFovy * aspect);

    // Render
    vec3 w = glm::normalize(eyeinit - center);
    vec3 u = glm::normalize(cross(upinit, w));
    vec3 v = cross(w, u);

 
   
   // fovx = tan(fovy / (2.0 * degreeToRad));
    //fovx *= image_width / (float)image_height;
   // fovx = 2 * atan(fovx);
   // fovx = fovx * radToDegrees;
   


   
    //do we have to normalize as we did in lookat

    //modelview = lookAt(eyeinit, center, upinit);

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";
    int pix = image_width * image_height;
    BYTE* pixels = new BYTE[3 * pix];
    int index = 0;
    for (float i = 0; i < image_height; ++i) {
       
        for (float j = 0; j < image_width; ++j) {
           
            //dir = aU + bV - W
            float alpha = fovx * ((j - (float(image_width) / 2.0)) / (float(image_width) / 2.0));
            float beta = newFovy * (((float(image_height) / 2.0) - i) / (float(image_height) / 2.0));
            vec3 direction = (alpha * u) + (beta * v) - w;
            //direction = direction / sqrt((direction.x * direction.x) + (direction.y * direction.y) + (direction.z * direction.z));
            direction = glm::normalize(direction);
            ray r(origin, direction);
          
            //printf("%f %f %f", origin, direction);

            int* pixel_color = FindIntersection(r, newScene.objectz, newScene);
            //check intersection with the ray and the scene
       

            
            //depending on the intersection compute the color 
            write_color(index, pixels, pixel_color);
            index+=3;
        }
    }

    std::cerr << "\nDone.\n";
    string img = "test.png";
    saveScreenshot(pixels, img);
    FreeImage_DeInitialise();
  
  return 0;
}
