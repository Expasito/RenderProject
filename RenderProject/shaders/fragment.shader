#version 330 core

out vec4 FragColor;
in vec3 position;
in vec3 colors_;
in vec4 transformed_;

struct Light {
	vec3 position;
	float intensity;
};

// gives the magnitude of a vector
float magnitude(vec3 vec) {
	return (sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z));
}

vec3 normalize(vec3 vec) {
	float mag = magnitude(vec);
	return vec3(vec.x/mag,vec.y/mag,vec.z/mag);
}


void main() {
	Light l = { {0,0,0 },30 };
	vec3 new_pos = transformed_.xyz;

	float mag = magnitude(l.position - new_pos);
	float dist = 1.0 / (mag*mag) * l.intensity;
	FragColor = vec4(colors_.x * dist, colors_.y * dist, colors_.z * dist, 1);


}