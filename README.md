# raytracer
Ray tracer from skelton HW2

Jack Wagner, Austin Spencer Marking

jmw001
jmw001@ucsd.edu
amarking
amarking@ucsd.edu

https://raviucsdgroup.s3.amazonaws.com/hw3/28f2d306779e5ab565b7e8c6053da31f/20230322055353/index.html

texture map link

For this assignment we really struggled on our shadowing, we ended up being able to only render shadows within the simple scenes like scene4 and only on 
spheres so we removed our shadow method for the later scenes which would cause much issue. We jumped around as we got stuck on this issue 
and ended up doing some texture mapping, reflections, and having fun with our raytracer.
Some of the cool scenes we got are in our texture map link which is just a google drive link. The assignment was very hard, and we aren't satisfied with the 
results but we love graphics and raytracing so we plan to continue working on the raytracer over spring break to get some nicer looking scenes. 
We also didn't upload a scene7 as we ran out of time, we believe we could render the dragon it would just take many hours as we didn't implement a 
acceleration structure. But we think the geometry would be correct. 

Our shadow method is currently commented out in the main.cpp file in the function called visibility
Our recursive raycasting is in our pixcolo function in main.cpp also commented out

Both these worked somehwat but not good enough to turn in ^