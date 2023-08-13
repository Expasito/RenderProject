#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texturecoords;

out vec2 texturecoords_;

void main() {

	texturecoords_ = texturecoords;

	gl_Position = vec4(position.x,position.y,position.z,1);
}