#version 450 core

out vec4 FragColor;
in vec3 position;
in vec3 colors_;
in vec4 transformed_;
in vec3 camPos_;
in vec3 camFront_;

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

float dot(vec3 a, vec3 b) {
	return (a.x*b.x + a.y*b.y + a.z*b.z);
}

void main() {
	// this is for an ambient light
	//Light l = { camPos_,30 };


	//vec3 new_pos = transformed_.xyz;

	//float mag = magnitude(l.position - new_pos);
	//float dist = 1.0 / (mag*mag) * l.intensity;
	//FragColor = vec4(colors_.x * dist, colors_.y * dist, colors_.z * dist, 1);

	// this is for a directional light


	// the ray for pixel to camera
	vec3 ray = transformed_.xyz - camPos_;

	// camera direction is just camFront_;

	// the tails are in the same spot due to subtraction direction

	float angle = acos(dot(ray, camFront_) / (magnitude(ray) * magnitude(camFront_)))*180/3.14159265;

	if (abs(angle) < 20) {
		float mag = magnitude(ray);
		float dist = 1.0 / (mag * mag) * 75.0;
		FragColor = vec4(colors_.x * dist, colors_.y * dist, colors_.z * dist, 1);
	}
	else {
		float mag = magnitude(ray);
		if (mag < 5) {
			mag = 5;
		}
		float dist = 1.0 / (mag * mag*mag) * 75.0/4;
		FragColor = vec4(colors_.x * dist, colors_.y * dist, colors_.z * dist, 1);
	}

	FragColor = vec4(colors_,1);
	//FragColor = vec4(position, 1);

}