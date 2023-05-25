# RenderProject

RenderProject is a simple renderer to be used for a physics engine coming soon. The main goal is to make a very simple interface for using OpenGL code.

RenderProject supports loading .obj files as long as they are exported with Trianglulated Meshes selected

The basic code structure is:

#include <Render/Render.h>

int main(){
  Render::init();
  Render::loadModel("model file path","model name");
  Render::addInstance("model name",{translations},{rotations},{scales});
  
  while(Render::keepWindow){
    Render::rendAll();
  }
  
  Render::exit()
}


The only changes the user needs to make is specify the 3 components for each instance of a model. There currently is no error checking for misnamed models but that is in the works.

