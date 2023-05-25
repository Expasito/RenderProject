#version 330 core

out vec4 FragColor;
in vec3 position;


void main() {
	FragColor = vec4(1, 1, 1, 1);
	//if (mod(floor(position.x*10.0001),2) == 0.0) {
	//	FragColor = vec4(150/255.0,75/255.0,0, 1);
	//}
	//else {
	//	FragColor = vec4(255/255.0, 255/255.0, 0, 1);
	//}
	//if (position.x <.01 && position.x >-.01 && position.y <.01 && position.y >-.01) {
	//	FragColor = vec4(1, 0, 0, 1);
	//}

}