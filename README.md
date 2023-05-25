# RenderProject

RenderProject is a simple renderer to be used for a physics engine coming soon. The main goal is to make a very simple interface for using OpenGL code.

RenderProject supports loading .obj files as long as they are exported with Trianglulated Meshes selected

The basic code structure is:

#include <Render/Render.h>

int main(){<br><br>
  Render::init();<br>
  Render::loadModel("model file path","model name");<br>
  Render::addInstance("model name",{translations},{rotations},{scales});<br>
  
  while(Render::keepWindow){<br>
    Render::rendAll();<br>
  }<br>
  
  Render::exit()<br>
}<br>


The only changes the user needs to make is specify the 3 components for each instance of a model. There currently is no error checking for misnamed models but that is in the works.

