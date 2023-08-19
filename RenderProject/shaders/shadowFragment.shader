#version 450 core

in vec2 texturecoords_;
out vec4 FragColor;

uniform sampler2D framebuffer;
uniform int isDepth;

uniform vec3 camPos;

void main() {

	float far = 1000.0;
	float near = .01;
	int div = 10;

	if (isDepth == 1) {
		float depth = texture(framebuffer, texturecoords_).x;

		depth = (depth) * (1 / far - 1 / near) + 1 / near;

		FragColor = vec4(vec3(1 - 1 / depth / div), 1);

	}
	else {
		FragColor = vec4(texture(framebuffer, texturecoords_));
	}

	FragColor = vec4(vec3(gl_FragCoord.xyz) + camPos, 1);

	float start = 400;
	float end = 800.5;

	FragColor = vec4(vec3((gl_FragCoord.xy)/800.5 -.5,0), 1);

	//FragColor = vec4(1.0, 0.0, 0.0, 1.0);

	//FragColor = vec4(vec3(0.0,1.0,0.0), 1.0);




	//FragColor = vec4(texturecoords_, 0, 1);
}