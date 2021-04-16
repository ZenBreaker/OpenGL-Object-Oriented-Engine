/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.

Michael Ngo 90003217 

Assignment 1 & 2 10% (Completed)
	Assignment 1 & 2 requirements are all completed and working 10
Scene creation 10% (Completed)
	Scene created using specified objects 10
Octree 10% (Partial)
	Top down creation of the Octree 10
	Creation of adaptive octree 10
BSP Tree 15% (Partial)
	Top down creation of the BSP tree 10
	Choosing appropriate split planes 5
Serialization of tree 10% (Incomplete)
	BSP Tree serialized and loaded on demand 10
Interactivity 10% (Partial)
	Toggle Octree display 5
	Show all levels of the octree in different colors 5
	Toggle BSP tree display 0
	Show all nodes of the BSP tree in a different color 0
Miscellaneous issues 10% (Completed)
	Missing information in README 4
	Application does not compile 2
	Application cannot be executed 2
	Scene setup incorrect 2

Hours Spent: 25

Special instructions:
File to look for bsp and octtree in their respective files, "BSP.h" and "OctTree.h"

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
