/*****************************************************************************/
/* This is the program skeleton for homework 2 in CSE167 by Ravi Ramamoorthi */
/* Extends HW 1 to deal with shading, more transforms and multiple objects   */
/*****************************************************************************/

// This is the basic include file for the global variables in the program.  
// Since all files need access to it, we define EXTERN as either blank or 
// extern, depending on if included in the main program or not.  


#define EXTERN extern 

EXTERN int amount; // The amount of rotation for each arrow press
EXTERN vec3 eye; // The (regularly updated) vector coordinates of the eye 
EXTERN vec3 up;  // The (regularly updated) vector coordinates of the up  
vec3 eyeinit(0.0,0.0,5.0) ; // Initial eye position, also for resets
vec3 upinit(0.0,1.0,0.0) ; // Initial up position, also for resets
vec3 center(0.0,0.0,0.0) ; // Center look at point 
int amountinit = 5;
int w = 500, h = 500 ; // width and height 
float fovy = 90.0 ; // For field of view
vec3 eyeinit ; 
vec3 upinit ; 
vec3 center ; 
int amountinit;
int w, h ; 
float fovy ; 
EXTERN vec3 ambient ; 
EXTERN vec3 diffuse ; 
EXTERN vec3 specular ; 
EXTERN vec3 emission ; 
EXTERN float shininess ; 
EXTERN int numobjects ; 
EXTERN vector<float> lightcol ; 
EXTERN vector<int> lightpos ;
EXTERN vector<float> enablelighting ;

