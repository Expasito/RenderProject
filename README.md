# RenderProject

RenderProject is my attempt at making a fully functioning Renderer using OpenGL. The main goal is to make a simple interface for visuals that can be used for several.
Projects in the future. Requirements are that the interface to draw to the screen is super easy and understandable, the performance is phenomenal and hardly impacts the systems that use it (unless a lot is being drawn), supports 3D and 2D objects, lighting/shadows, and user input. 
<br>
The most important goal for this project is to get practice using OpenGL and making a renderer as well as getting the renderer setup for my game engine in the future.
Assets for the render must be in the .obj format, but more formats may be supported in the future. Currently, .obj files must be exported with Triangulated Meshes.



RenderProject is a simple renderer to be used for a physics engine coming soon. The main goal is to make a very simple interface for using OpenGL code.

RenderProject supports loading .obj files as long as they are exported with Trianglulated Meshes selected.

Here is a small demo of some terrain the renderer generated. 
This frame had 6.7 million cubes drawn to the screen along with some lighting
![image](https://github.com/Expasito/RenderProject/assets/93100379/e2e6a613-e6e1-4522-b196-68b774565586)



Currently, lighting and shadows are taking the most performance out of the system except for when there are millions of entities being drawn.
Plans to fix the bad performance such as culling and computing model matricies on the CPU when they get updated. This would require a large modification to the program but should improve the GPU performance.

There is a custom data structure that was created for this project located in Depend/utils/FillerArray.h. Context for this problem is that I wanted a fast way to add, edit and remove objects from a scene. Using a regular arraylist/vector was too slow due to insertions and deletions, and a hashtable would not work due to not having all elements sequential. This meant that I would have an O(n) operation somewhere in the most critical component of my render. 

To fix this, I created the 'FillerArray' data structure which is a hashmap that has all elements sequential. The main idea behind it is an array holds the elements and a hashmap contains the indexes of each element. So really, this is just two data structures being used together. The runtime for each main operation is described below:
  <br><br> (1) When adding an element, it gets put at the end of the array and added to the hashtable where ever it happens to be. Adding to a hashtable and adding to the end of the array are both O(1). 
    <br><br> (2) When editing, the hashmap gives the index in the array for where the element is, and then returns it. This also is O(1) since getting an element from a hashtable is O(1) and getting an element from an array is O(1). 
    <br><br> (3) Removing an element requires deleting the element from the hashtable, taking the last element from the array and placing it where the element was removed, and finally updating the last elements index based on the hashtable. 
<br><br>All 3 of these operations are O(1) so I was able to get O(1) for add, edit, insert and delete along with a continuous buffer. Overall, this means that adding any number of elements is basically a free operation unless the array or hashtable need to be resized. This addition to my project significantly improved the number of elements I could add while the renderer was drawing as well as improve the performance of drawing lots of elements.


Here is an example of shadows from a mountain causing the blocks behind it to be less bright. Also on the left is the start of a debug screen which measures the fps for each frame.
![image](https://github.com/Expasito/RenderProject/assets/93100379/6f712100-6da1-42e9-8d1f-2e4fb5ad2154)
