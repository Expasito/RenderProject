#version 450 core

in vec2 texturecoords_;
out vec4 FragColor;

uniform sampler2D framebuffer;
uniform int isDepth;

void main() {

	float far = 30.0;
	float near = .01;
	int div = 100;
	
	if (isDepth==1) {
		float depth = texture(framebuffer, texturecoords_).x;

		depth = (depth) * (1 / far - 1 / near) + 1 / near;

		FragColor = vec4(vec3(1-1/depth/div),1);

	}
	else {
		FragColor = vec4(texture(framebuffer, texturecoords_));
	}


	//FragColor = vec4(1, 0, 0, 1);
	


	//FragColor = vec4(texturecoords_, 0, 1);
}