#version 450 core
layout(location = 0) in vec3 position;
layout(location = 1) in float index;
layout(location = 2) in float height;
layout(location = 3) in vec2 text;
out vec3 position_;
out float height_;
out vec2 text_;

void main() {
	position_ = vec3(position);
	height_ = height;
	text_ = text;
	gl_Position = vec4(position.x+index, position.y+height, position.z, 1);
}