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

int visibility(ray r, Scene newScene) {
    int blocked = 1;
    float blockers = 1.0;
    pair<float, vec3> newpair;
    int i;

    /*
    this is for our visibility
     for (i = 0; i < newScene.objectz.size(); i++) {
      if (i == index) {
          continue;
      }
      if (newScene.types[i] == "Sphere") {

          newpair = static_cast<Sphere*>(newScene.objectz[i])->intersection(r);
      }
      if (newpair.first > 0.0101) {
          return 0;
      }
      if (newScene.types[i] == "Triangle") {
          newpair = static_cast<Triangle*>(newScene.objectz[i])->intersection(r);
      }
      if (newpair.first > 0.0101) {
         //return 0;
          newpair.first = 0;

      }
  }

    */

    return 1;
    //return blocked;
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



vec3 pixcolor(vec3 stuff, Scene* prim, int depth, Scene newScene, ray reflection) {
    vec3 color = vec3(0, 0, 0);
    vec3 inter = stuff;
    int name = 0;
    vec3 diff = vec3(0, 0, 0);
    vec3 specular = vec3(0, 0, 0);
    vec3 ambient = vec3(0, 0, 0);
    vec3 emiss = vec3(0, 0, 0);
    float shiny = 0.0;
    vec3 normal;
    vec3 newIntersection;
    //Adding ambi and emiss
    
    name = 1;
    

    
    specular = vec3(0.9, 0.9, 0.9);
    shiny = 20;

    vec3 sphereCenter = static_cast<Sphere*>(prim)->center(); //getting the world coord center of the sphere    


    float offset = 0.01;

    normal = normalize(vec3(static_cast<Sphere*>(prim)->invTrans * vec4(inter, 1)) - sphereCenter);

    mat3 matrix = mat3(transpose(static_cast<Sphere*>(prim)->invTrans));
    normal = normalize(matrix * normal);



   
    vec3 eyedirn = normalize(eyeinit - inter); //direction to the eye

  


    vec3 lightdir;
    vec3 lightcol;
    vec3 half1;
   
    float one = 1.0f;
    float atten = one;
    vec3 lightpos = vec3(0,2,0);
    lightdir = normalize(lightpos - inter); //find the light direction 
    float dist = glm::distance(lightpos, inter);
   
  
    inter += (.01f * lightdir);
    lightcol = vec3(1,1,1);
    half1 = normalize(lightdir + eyedirn); //finding the half vector 

    

    ray r(inter, lightdir); //cast a ray from the point of intersection, in the light direction




    if (visibility(r, newScene) == 1) {

        color += atten * ComputeLight(lightdir, lightcol, normal, half1, diff, specular, shiny);
    }




    
    color += ambient + emiss;


    /* This was for reflections
    vec3 directional = -eyedirn + 2.0f * dot(eyedirn, normal) * normal;
    ray reflec(newIntersection + (normal * 0.01f), glm::normalize(directional));
    tuple<string, Scene*, vec3, int> e = intersection(reflec, newScene);
    if (get<0>(e) != "") {
        color += (specular * pixcolor(e, depth - 1, newScene, reflec));
    }
    */


    return color;
}

vec3 FindIntersection(ray r, vector<Scene*> a, Scene newScene, FIBITMAP* map) {
    float min_t = 1000000; // number of bounces from read file
    Scene min_primitive;
    int index;
    int pixel_color[3] = { 0,0,0 };
    float t = 0;
    
    string thetype = "";

    for (int i = 0; i < newScene.objectz.size(); i++) {
        //float t = 0;
        
        if (newScene.types[i] == "Sphere") {
            t = static_cast<Sphere*>(newScene.objectz[i])->intersection(r);
        }
        else {
            t = static_cast<Triangle*>(newScene.objectz[i])->intersection(r);
        }

       
       
        if (t > 0 && t < min_t) {
            min_primitive = *a[i];
            index = i;
            min_t = t;
            thetype = newScene.types[i];
        }


    }

    if (thetype == "Sphere") {
        
        Sphere* obj = static_cast<Sphere*>(newScene.objectz[index]);
        unsigned h = FreeImage_GetHeight(map);
        unsigned w = FreeImage_GetWidth(map);
        float u = obj->uv.x;
        float v = obj->uv.y;
        float textureMapX = (u * w) + .5;
        float textureMapY = (v * h) + .5;
        textureMapX = static_cast<int>(textureMapX);
        textureMapY = static_cast<int>(textureMapY);
        RGBQUAD color;
        FreeImage_GetPixelColor(map, textureMapX, h - textureMapY, &color);
        pixel_color[0] = color.rgbRed;
        pixel_color[1] = color.rgbGreen;
        pixel_color[2] = color.rgbBlue;
        //hit = distance;
    }
    else if (thetype == "Triangle") {
        pixel_color[0] = 255;
        pixel_color[1] = 0;
        pixel_color[2] = 255;
        //hit = distance;

    }
    return vec3(pixel_color[0]/255.0, pixel_color[1]/255.0, pixel_color[2]/255.0) +pixcolor(r.pos(r.orig, r.dir, min_t), static_cast<Sphere*>(newScene.objectz[0]), 5, newScene, r);
  
    
   
   
    //loop through all primitives in the scene (passed in parameter)
        //t = the intersection for each primitive (shape) (smallest positive root)
          //t > 0 && t < min_t:
             //min_primitive = current primitive (the primitive we want to return the one with the smallest root) 
             //min_t = t
    //return r.at(min_t);
    //return min_t;
}


float clamp(float i, int x) {
    if (i > x) {
        return float(x);
    }
    return i;
}

void divide(vec3 & vector3, double t) {
    vector3 = vec3(vector3.x / t, vector3.y / t, vector3.z / t);
}

void write_color(int index, BYTE pixels[], vec3 pixel_color) {
    //Why did we have to reverse this isnt it in the correct order when we do 0, 1, 2
    pixels[index] = (unsigned char)pixel_color[2];
    pixels[index + 1] = (unsigned char)pixel_color[1];
    pixels[index + 2] = (unsigned char)pixel_color[0];
}


int main(int argc, char* argv[]) {
    
    Scene newScene = Scene();
    FreeImage_Initialise();
    readfile(argv[1], newScene);
    


    float image_width = w;
    float image_height = h;

    vec3 origin = eyeinit;
    
  


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

 

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";
    int pix = image_width * image_height;
    BYTE* pixels = new BYTE[3 * pix];
    int index = 0;


    FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename("texturenew.jpg");
    FIBITMAP* bmp = FreeImage_Load(fif, "texturenew.jpg");

    unsigned width = FreeImage_GetWidth(bmp);
    unsigned height = FreeImage_GetHeight(bmp);
    RGBQUAD color; 
    unsigned int x, y;
    x = 600;
    y = 600;
    FreeImage_GetPixelColor(bmp, x, y, &color);
    printf("%u", color.rgbBlue);



    for (float i = 0; i < image_height; ++i) {
       
        for (float j = 0; j < image_width; ++j) {
           
            //dir = aU + bV - W
            float alpha = fovx * ((j - (float(image_width) / 2.0)) / (float(image_width) / 2.0));
            float beta = newFovy * (((float(image_height) / 2.0) - i) / (float(image_height) / 2.0));
            vec3 direction = (alpha * u) + (beta * v) - w;
            direction = glm::normalize(direction);
            ray r(origin, direction);
          

            vec3 pixel_color = FindIntersection(r, newScene.objectz, newScene, bmp);
            //check intersection with the ray and the scene
       
            pixel_color = vec3(int(clamp(pixel_color[0], 1) * 255), int(clamp(pixel_color[1], 1) * 255), int(clamp(pixel_color[2], 1) * 255));
            
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
