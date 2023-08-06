#version 450 core

out vec4 FragColor;
in vec3 position;
in float height_;
void main() {
	//circle a = { {Cam.x,Cam.y,Cam.z},1 };

	//FragColor = vec4(position.y,1,1,0);
	//FragColor = vec4(position+1, 1);
	FragColor = vec4(1, 1, 1, .5);
	//float diff = radius - ((position.x) * (position.x) + (position.y) * (position.y));
	//if (diff>=0) {
	//	FragColor = vec4(1, 0, 0, 1);
	//}
	//else {
	//	discard;
	//}
	//FragColor = vec4(1, 1, 1, 1);

}