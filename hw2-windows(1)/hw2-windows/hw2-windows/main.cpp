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
#include <tuple>
#include <utility>



using namespace glm;
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


void divide(vec3 & vector3, double t) {
    vector3 = vec3(vector3.x / t, vector3.y / t, vector3.z / t);
}

float clamp(float i, int x) {
    if (i > x) {
        return float(x);
    }
    return i;
}

void write_color(int index, BYTE pixels[], vec3 pixel_color) {
    //Why did we have to reverse this isnt it in the correct order when we do 0, 1, 2
    pixels[index] = (unsigned char)pixel_color[2];
    pixels[index+1] = (unsigned char)pixel_color[1];
    pixels[index+2] = (unsigned char)pixel_color[0];
}

float visibility(ray r, Scene newScene, int index) {
    bool visible = true;
    float blockers = 1.0;
    hitInfo hitfo;
    float tmin = .0001;
    float tmax = 1000000.0f;
    pair<float, vec3> newpair;
    int i;

    for (i = 0; i < newScene.objectz.size(); i++) {
        visible = (newScene.objectz[i])->intersection(r, hitfo, tmin, tmax);
    } 
    return ;
}


float max(float x, float y) {
    if (x > y) { return x; }
    return y;
}

vec3 ComputeLight(vec3 direction, vec3 lightcolor, vec3 normal, vec3 halfvec, vec3 mydiffuse, vec3 myspecular, float myshininess) {

    float nDotL = dot(normal, direction);
    vec3 lambert = mydiffuse * lightcolor * max(nDotL, 0.0);

    float nDotH = dot(normal, halfvec);
    vec3 phong = myspecular * lightcolor * pow(max(nDotH, 0.0), myshininess);

    vec3 retval = lambert + phong;
    return retval;
}



vec3 pixcolor(ray r, hitInfo& hitfo, Scene& newScene, int depth) {
    hitInfo hit;
    float tmax = 10000.0f; float tmin = .001;
    vec3 color = vec3(0, 0, 0);
    vec3 intersection = hitfo.inter;
    vec3 diff = vec3(0,0,0); vec3 specular = vec3(0, 0, 0); vec3 ambient = vec3(.2, .2, .2);
    vec3 emiss = vec3(0, 0, 0); 
    float shiny = 0.0;
    ambient = (hitfo.shape)->ambi;
    emiss = (hitfo.shape)->emiss;
    diff = (hitfo.shape)->diffu;
    specular = (hitfo.shape)->specul;
    shiny = (hitfo.shape)->shini;
    vec3 eyedirn = normalize(eyeinit - hitfo.inter); //direction to the eye
    if (depth == 0) {
        return color; //compute light
    }

    for (int i = 0; i < newScene.numlights; i++) {
        vec3 lightdir;
        vec3 lightcol;
        vec3 half1;
        if (newScene.lightposn[(i * 4)+3] == 0) { //directional
            vec3 lightposition = vec3(newScene.lightposn[(i * 4)], newScene.lightposn[(i * 4) + 1], newScene.lightposn[(i * 4) + 2]);
            lightdir = normalize(lightposition); //as specified by the directional light
            half1 = normalize(lightdir + eyedirn);
            lightcol = vec3(newScene.lightcol[(i * 3)], newScene.lightcol[(i * 3) + 1], newScene.lightcol[(i * 3) + 2]);     
        }
        else {
            vec3 lightpos = vec3(newScene.lightposn[(i * 4)], newScene.lightposn[(i * 4) + 1], newScene.lightposn[(i * 4) + 2]);
            //use old intersectino if this fuck
            lightdir = normalize(lightpos - intersection); //find the light direction 
            intersection += (.01f * lightdir);
          
            lightcol = vec3(newScene.lightcol[(i * 3)], newScene.lightcol[(i * 3) + 1], newScene.lightcol[(i * 3) + 2]);
            half1 = normalize(lightdir + eyedirn); //finding the half vector 
        }
      
        ray r(hitfo.inter, lightdir); //cast a ray from the point of intersection, in the light direction

        if ((hitfo.shape)->intersection(r,hit,tmin,tmax) == 1) {
            color += ComputeLight(lightdir, lightcol, hitfo.n, half1, diff, specular, shiny);
        }
    }
     color += ambient + emiss;
    //looping through the lights
    return color;
}



int main(int argc, char* argv[]) {
    bool isHit = false;
    Scene newScene = Scene();
    hitInfo hit;
    FreeImage_Initialise();
    readfile(argv[1], newScene);
    float image_width = w;
    float image_height = h;
    vec3 origin = eyeinit;
    //fov 
    fovy = fovy * (3.14 / 180.0);
    float newFovy = tan(fovy / 2.0);

    float aspect = image_width / image_height;
    float fovx = (newFovy * aspect);
    float tmax = 10000.0f;
    float tmin = .001;
    vec3 w = glm::normalize(eyeinit - center);
    vec3 u = glm::normalize(cross(upinit, w));
    vec3 v = cross(w, u);

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";
    int pix = image_width * image_height;
    BYTE* pixels = new BYTE[3 * pix];
    int index = 0;
    vec3 pixel_color = vec3(0, 0, 0);
    for (float i = 0; i < image_height; ++i) {
       
        for (float j = 0; j < image_width; ++j) {

            //dir = aU + bV - W
            float alpha = fovx * ((j - (float(image_width) / 2.0)) / (float(image_width) / 2.0));
            float beta = newFovy * (((float(image_height) / 2.0) - i) / (float(image_height) / 2.0));
            vec3 direction = (alpha * u) + (beta * v) - w;
            direction = glm::normalize(direction);
            ray r(origin, direction);
            pixel_color = vec3(0, 0, 0);
            for (int i = 0; i < newScene.objectz.size(); i++) {
                isHit = (newScene.objectz[i])->intersection(r,hit,tmin,tmax); //eye ray check
                if(isHit){
                    tmax = hit.t;
                }
            }
       
            if (isHit) {
               pixel_color = pixcolor(r,hit,); //recursively raytrace the pixel color
            }
           
            pixel_color = vec3(int(clamp(pixel_color[0], 1) * 255), int(clamp(pixel_color[1], 1) * 255), int(clamp(pixel_color[2], 1) * 255));
            //depending on the intersection compute the color 
            int d = pixel_color[0];
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
