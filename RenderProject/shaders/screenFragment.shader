#version 450 core

in vec2 texturecoords_;
out vec4 FragColor;

uniform sampler2D framebuffer;
uniform int isDepth;

void main() {

	float far = 1000.0;
	float near = .01;
	
	if (isDepth==1) {
		float depth = texture(framebuffer, texturecoords_).x;

		depth = (depth) * (1 / 1000.0 - 1 / .01) + 1 / .01;

		FragColor = vec4(vec3(1-1/depth/100),1);

	}
	else {
		FragColor = vec4(texture(framebuffer, texturecoords_));
	}

	


	//FragColor = vec4(texturecoords_, 0, 1);
}