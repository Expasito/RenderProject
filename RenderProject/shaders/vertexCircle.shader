#version 330 core
layout(location = 0) in vec3 aPos; 
out vec3 position;
out float radius;

void main() {
	position = vec3(aPos);
	radius = 1;
	gl_Position = vec4(aPos.x/3, aPos.y, aPos.z, 1.0);
}