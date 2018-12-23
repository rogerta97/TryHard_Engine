# TryHard_Engine

We are a pair of students on UPC Barcelona, developing our own 3D Engine, TryHard Engine. In this website all of our work will be explained and detailed. Development cycle took 5 months. 


# What we did? 

## Roger Tello

- Component-based structure with gameobjects. 
- Loading Geometry & Textures. 
- Hierarchy panel & Gameobject parenting. 
- Inspector panel to see object properties. 
- Created components: Mesh, Material...
- Octree for space partitioning.
- Basic Resources Saved to binary.
- Load & Save for gameobjects & components. 
- Resource Manager & browser. 
- Basic structure for UI system. 
- Multicamera render pipeline & Cam interpolation.
- Prefab System.
- Skybox. 

- UI Elements: Canvas, Label, Image, Text, Checkbox...
- Callback System for Buttons & Checkbox. 
- Event System. 
- Scene Interpolation.

## New innovations:

- Any camera can be assigned as game camera and it can ve previewed in it's own dock
- An octree instead of quadtree
- A Skybox that moves with each camera
- Resources built with a map
- Guizmos
- Drag and drop to assign parenting in the hierarchy
- An explorer to select items to load
- Any gameobject can have tags assigned
- Prefabs can be created after editing a model

## How to use:

### Basics:

- Any gameobject can be picked with the mouse, and it's transform can be edited with the guizmo or in the inspector.
- To change the guizmo mode, select a mode in the inspector of a transform component.
- After loading models, the current scene can be saved and loaded later.
- Pressing the play button will inter game mode. When exiting this mode, objects will return to the state they were when entering the mode.

### Tools:

- Console: Shows and hides the console panel, the console LOGS the start LOGS and when you import geometry it shows the details of it.
- RandomNum: Shows and hides a random generator tool panel.
- Configuration: Shows the configuration panel where we can find a Menu Items with functionalities that apply to the scene or the engine workflow, it also shows the system information.
- Performance: Shows the init and  Start-up performance of the modules and the run time graphs of the modules with preupdate, update and post-update, as well as panels.
- Inspector: Shows information about the selected game object and offers some options like changing the texture.
- Hierarchy: Displays the game objects in the scene.
- Explorer: Select and load models
- Tags: Manage the tags of the gameobjects
- Clocks: See the real time and the game time, and accelerate or slow down game time with a slider

### Help:

- DEMO GUI: shows the original demo panel from ImGui.
- Documentation:opens the github repository page.  
- Report a bug:brings you to github issues.
- License:brings you to the MIT license about the github repository

### Camera movement:

- WASD: movement (forward,left,backwards,right.
- F: camera focuses the last mesh.
- R & F: move up and down the camera.
- Alt + left mouse click: camera orbits around selected game object with mouse movement.
- Supr: deletes the selected gameobject. 
- Mouse wheel: zoom in and out.
- Right Click: look in de direction of mouse movement.	
- Import .FBX files: Drag and drop into the scene.

## Warning

This Release has a big size due to the size of the textures of the skybox and the t-rex.

# Contacts

Github repository link: https://github.com/rogerta97/TryHard_Engine
Pol Ferrando: https://github.com/polferrando98
Roger Tello: https://github.com/rogerta97

## License

MIT License

Copyright (c) [year] [fullname]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
