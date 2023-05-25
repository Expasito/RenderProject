#version 330 core

out vec4 FragColor;
in vec3 position;
in float radius;

void main() {
	//circle a = { {Cam.x,Cam.y,Cam.z},1 };

	FragColor = vec4(0,0,0,0);
	float diff = radius - ((position.x) * (position.x) + (position.y) * (position.y));
	if (diff>=0) {
		FragColor = vec4(1, 0, 0, 1);
	}
	else {
		discard;
	}
	//FragColor = vec4(1, 1, 1, 1);

}