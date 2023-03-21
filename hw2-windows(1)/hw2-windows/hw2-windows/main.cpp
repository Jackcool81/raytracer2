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
/*
int* FindIntersection(ray r, vector<Scene*> a, Scene newScene) {
    float min_t = 1000000; // number of bounces from read file
    Scene min_primitive;

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
            min_t = t;
            thetype = newScene.types[i];



        }


    }

    if (thetype == "Sphere") {
        pixel_color[0] = 0;
        pixel_color[1] = 255;
        pixel_color[2] = 255;
        //hit = distance;
    }
    else if (thetype == "Triangle") {
        pixel_color[0] = 255;
        pixel_color[1] = 0;
        pixel_color[2] = 255;
        //hit = distance;

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

*/


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

int visibility(ray r, Scene newScene, hitInfo& hi) {
    int blocked = 1;
    float blockers = 1.0;
    pair<float, vec3> newpair;
    int i;
    hitInfo hit;
    /*
      for (i = 0; i < newScene.objectz.size(); i++) {
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
            return 0;
        }
    }
    
    */
    bool hitter = false;
    bool finalhit = false;
    /*
        for (i = 0; i < newScene.objectz.size(); i++) {
      
      
        if (newScene.types[i] == "Sphere") {
            
            hitter = static_cast<Sphere*>(newScene.objectz[i])->intersection(r, hit);

        }

        if (newpair.first > 0.0101) {
            return 0;
        }
        if (newScene.types[i] == "Triangle") { //this function returns a number when it shouldn't
            hitter = static_cast<Triangle*>(newScene.objectz[i])->intersection(r, hit);

        }

        if (newpair.first > 0.0101) {
          // return 0;
            newpair.first = 0;
           
        }
    }
    
    */

    
    
   
    
    
    
   
   
   
    return 1;
    //return blocked;
}

hitInfo intersection(ray r, Scene newScene) {
 //   tuple <string, Scene*, vec3, int> geek ("", NULL, vec3(0,0,0), -1);
   
    float min_t = 1000000.0f; // number of bounces from read file
    Scene min_primitive;

   // int pixel_color[3] = { 0,0,0 };
  //  pair<float, vec3> newpair(-1, vec3(-1,-1,-1));
    //pair<float, vec3> bestpair;
    hitInfo hitter;
    //float t = 0;
    bool finalhit = false;
    string thetype = "";
    hitInfo hit;

    for (int i = 0; i < newScene.objectz.size(); i++) {
        //float t = 0;

        if (newScene.types[i] == "Sphere") {
            hitter = static_cast<Sphere*>(newScene.objectz[i])->intersection(r);
        }
        else {
            hitter = static_cast<Triangle*>(newScene.objectz[i])->intersection(r);
        }

        if (hitter.ishit) {
            if (hitter.t > 0.001 && hitter.t < min_t) {
                min_t = hitter.t;
                hit.inter = hitter.inter;
                hit.n = hitter.n;
                hit.prim = hitter.prim;
                hit.t = hitter.t;
                hit.type = hitter.type;
                hit.ishit = hitter.ishit;





                finalhit = hitter.ishit;
            }
        }
      
/*
 if (newpair.first > 0 && newpair.first < min_t) { //Distance check
            min_primitive = *newScene.objectz[i];
            min_t = newpair.first;
            thetype = newScene.types[i];
            get<0>(geek) = thetype;
            get<1>(geek) = newScene.objectz[i];
            get<2>(geek) = newpair.second;
            get<3>(geek) = i;


        }
*/
     


    }

    
  //  return pixel_color;

  //  hit = minHit;
    
    return hit;
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

vec3 normalChecker(tuple<string, Scene*, vec3> stuff, bool light, vec3 lightdir) {
    vec3 returnval = vec3(0, 0, 0);
    if (light == true) { //point
        if (get<0>(stuff) == "Sphere") {


        }
        if (get<0>(stuff) == "Triangle") {


        }
    }
    if (light == false) { //directional
        if (get<0>(stuff) == "Sphere") {


        }
        if (get<0>(stuff) == "Triangle") {


        }
    }
    return returnval;
}


vec3 pixcolor(hitInfo hit, int depth, Scene newScene, ray reflected) {
    vec3 color = vec3(0, 0, 0);
    //vec3 inter = get<2>(stuff);
    vec3 inter = vec3(0,0,0);
    int name = 0;
    vec3 diff = vec3(0,0,0);
    vec3 specular = vec3(0, 0, 0);
    vec3 ambient = vec3(.2, .2, .2);
    vec3 emiss = vec3(0, 0, 0);
    float shiny = 0.0;
    vec3 normal;
    vec3 newIntersection;
    if (hit.type == "Sphere") {
        ambient = static_cast<Sphere*>(hit.prim)->ambi;
        emiss = static_cast<Sphere*>(hit.prim)->emiss;
        diff = static_cast<Sphere*>(hit.prim)->diffu;
        specular = static_cast<Sphere*>(hit.prim)->specul;
        shiny = static_cast<Sphere*>(hit.prim)->shini;
        normal = hit.n;
        inter = hit.inter;
    }
    if (hit.type == "Triangle") {
        ambient = static_cast<Triangle*>(hit.prim)->ambi;
        emiss = static_cast<Triangle*>(hit.prim)->emiss;
        diff = static_cast<Triangle*>(hit.prim)->diffu;
        specular = static_cast<Triangle*>(hit.prim)->specul;
        shiny = static_cast<Triangle*>(hit.prim)->shini;
        normal = hit.n;
        inter = hit.inter;
    }

    
    /*
    //GETS ALL THE MATERIAL INFORMATION AND THE NORMAL
    if (get<0>(stuff) == "Sphere") {
        name = 1;
        ambient = static_cast<Sphere*>(get<1>(stuff))->ambi;
        emiss = static_cast<Sphere*>(get<1>(stuff))->emiss;
       // color += static_cast<Sphere*>(get<1>(stuff))->ambi + static_cast<Sphere*>(get<1>(stuff))->emiss;
        diff = static_cast<Sphere*>(get<1>(stuff))->diffu;
        specular = static_cast<Sphere*>(get<1>(stuff))->specul;
        shiny = static_cast<Sphere*>(get<1>(stuff))->shini;
        vec3 sphereCenter = static_cast<Sphere*>(get<1>(stuff))->center(); //getting the world coord center of the sphere    
       
        
             float offset = 0.01;
             
        //normal = normalize(vec3(static_cast<Sphere*>(get<1>(stuff))->invTrans * vec4(inter, 1)) - sphereCenter);
        newIntersection = inter;
        //intersection += (normal * offset);
        mat3 matrix = mat3(transpose(static_cast<Sphere*>(get<1>(stuff))->invTrans));
        normal = normalize(matrix * normal);
        
        //normal = normalize(intersection - sphereCenter);
       
        
        //normal = normalize(intersection - sphereCenter); //finding the normal
       
    }
      if (get<0>(stuff) == "Triangle") {
        ambient = static_cast<Triangle*>(get<1>(stuff))->ambi;
        emiss = static_cast<Triangle*>(get<1>(stuff))->emiss;
        diff = static_cast<Triangle*>(get<1>(stuff))->diffu;
        specular = static_cast<Triangle*>(get<1>(stuff))->specul;
        shiny = static_cast<Triangle*>(get<1>(stuff))->shini;
        vec3 A = static_cast<Triangle*>(get<1>(stuff))->A; //getting the world coord center of the sphere
        vec3 B = static_cast<Triangle*>(get<1>(stuff))->B; //getting the world coord center of the sphere
        vec3 C = static_cast<Triangle*>(get<1>(stuff))->C; //getting the world coord center of the sphere
        float offset = 0.01;
        mat3 matrix = mat3(inverse(static_cast<Triangle*>(get<1>(stuff))->trans));
        A = matrix * A;
        B = matrix * B;
        C = matrix * C;
        normal = normalize(cross(normalize(B - A), normalize(C - A)));
        //normal = glm::normalize(cross(normalize(C - A), normalize(B - A)));
       // normal = vec3(transpose(static_cast<Triangle*>(get<1>(stuff))->trans) * vec4(normal, 1));
        newIntersection = inter;

       // intersection += (normal * offset);


    }
    */
    
  
   
    vec3 eyedirn = normalize(eyeinit - inter); //direction to the eye

    if (depth == 0) {
        return color; //compute light
    }
    
    //This is for the shadow rays
    
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
            lightdir = normalize(lightpos - inter); //find the light direction 
            inter += (.01f * lightdir);
          
            lightcol = vec3(newScene.lightcol[(i * 3)], newScene.lightcol[(i * 3) + 1], newScene.lightcol[(i * 3) + 2]);
            half1 = normalize(lightdir + eyedirn); //finding the half vector 
        }
      
       // vec3 direction = glm::normalize(position - input);
       
       // Ray r = Ray(intersection + (float)0.3 * lightdir, direction);
        //ray r(intersection, lightdir); //cast a ray from the point of intersection, in the light direction
        ray r(inter, lightdir); //cast a ray from the point of intersection, in the light direction

        //possiblities the problem is we are getting to much shadow
        //visibibly is always returning 0
        //as is our intersection methods
        // SHADOWS ARE NOT CORRECT
        //could be the ray we pass into intersection, could be the intersection method handling the ray
        //debug both


        if (visibility(r, newScene, hit) == 1) {
            
           
            
            color += ComputeLight(lightdir, lightcol, normal, half1, diff, specular, shiny);
        }
         }
    
  
        
       
      

   
    //vec3 vpar = glm::dot(normal , glm::normalize(reflected.dir)) * normal;
    //vec3 reflecdir = glm::normalize(reflected.dir - (float)1.5 * vpar);
  //  reflected.dir = glm::normalize(reflected.dir);
   // vec3 reflect = (dot(reflected.dir, normal) * normal) * 2.0f;
    //point = intersection + offset? + normal
    //eye direction - from intersection
    //normal
    //ray reflec(newIntersection + (0.01f * normal), reflecdir);
    
    
    
    
    //This is for recursive rays
    /*
     vec3 directional = -eyedirn + 2.0f * dot(eyedirn, normal) * normal;
    ray reflec(newIntersection + (normal * 0.01f), glm::normalize(directional));
     bool hitInter = intersection(reflec, newScene, hit);
    if (hitInter) {
        color += (specular * pixcolor(hit, depth - 1, newScene, reflec));

    }
    */

   
    color += ambient + emiss;
   
    //looping through the lights
    
    
    
    

    return color;
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
            // int* pixel_color = FindIntersection(r, newScene.objectz, newScene);
            if (i == 50 &&j == 10) {
               pixel_color = vec3(0, 0, 0);
            }
            hitInfo hit;
             //check intersection with the ray and the scene
            hit = intersection(r, newScene); //eye ray check
            if (hit.ishit) {
                
               pixel_color = pixcolor(hit, 5,  newScene, r); //recursively raytrace the pixel color
            
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
