/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.

Michael Ngo 90003217 

Scene generation 20% (Completed)
	Objects loaded correctly from the files and displayed in proper position 10
	Correct Rendering using CS 300 shaders 10
Tests 70% (Completed)
	Sphere Vs Sphere 5
	AABB Vs Sphere 5
	Sphere Vs AABB 5
	AABB Vs AABB 5
	Point Vs Sphere 5
	Point Vs AABB 5
	Point Vs Triangle 5
	Point Vs Plane 5
	Ray Vs Plane 5
	Ray Vs AABB 5
	Ray Vs Sphere 5
	Ray Vs Triangle 5
	Plane Vs AABB 5
	Plane Vs Triangle 5
Interactivity 5% (Completed)
	Camera Implementation 5
Miscellaneous issues 5% (Completed)
	Missing information in README 2
	Application does not compile 1
	Application cannot be executed 1
	Scene setup incorrect 1

Hours Spent: 10

Special instructions:
File to look for the tests are in "Collision.h"

To build:
Place models folder into the main folder so file structure looks something like:
CS350_michael.ngo_2
  CS350_Engine
    imgui
    includes
    lib
    models (you have to add!)
      4Sphere.obj
      bunny.obj
      bunny_high_poly.obj
      cube.obj
      cube2.obj
      cup.obj
      lucy_princeton.obj
      quad.obj
      sphere.obj
      sphere_modified.obj
    AssetManager.cpp
    AssetManager.h 
    (and all other source files)
  readme.txt
      
The engine build in debug and release x64
Camera movements are ccontrolled by WASD and shift to go faster with an imgui to change speed value too
Imgui also has the depth copy button with a drop down menu to select the geomatry buffers
imgui for lights and colors values to change

End Header --------------------------------------------------------*/
