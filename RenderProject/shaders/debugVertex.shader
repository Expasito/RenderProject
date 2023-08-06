#version 450 core
layout(location = 0) in vec3 position;
layout(location = 1) in float index;
layout(location = 2) in float height;
out vec3 position_;
out float height_;

void main() {
	position_ = vec3(position);
	height_ = height;
	gl_Position = vec4(position.x+index, position.y+height, position.z, 1);
}