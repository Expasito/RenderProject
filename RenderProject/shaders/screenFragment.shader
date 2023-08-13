#version 450 core

in vec2 texturecoords_;
out vec4 FragColor;

uniform sampler2D framebuffer;

void main() {
	FragColor = texture(framebuffer, texturecoords_);
	//FragColor = vec4(texturecoords_, 0, 1);
}