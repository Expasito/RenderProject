#version 450 core

out vec4 FragColor;
in vec3 position;
in vec3 colors_;
in vec4 transformed_;
in vec3 camPos_;
in vec3 camFront_;
in vec3 colours_;
in vec3 normals_;
in vec2 texturecoords_;


struct Light {
	vec3 position;
	float intensity;
};

// gives the magnitude of a vector
double magnitude(dvec3 vec) {
	return (sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z));
}

dvec3 normalize(dvec3 vec) {
	double mag = magnitude(vec);
	return vec3(vec.x/mag,vec.y/mag,vec.z/mag);
}

double dot(dvec3 a, dvec3 b) {
	return (a.x*b.x + a.y*b.y + a.z*b.z);
}


uniform vec3 light;

void main() {
	// this is for an ambient light
	//Light l = { camPos_,30 };
	//Light l = { {0,50,0},2000 };
	//vec3 new_pos = transformed_.xyz;

	//float mag = magnitude(l.position - new_pos);
	//float dist = 1.0 / (mag*mag) * l.intensity;

	//vec3 colors2_ = (colors_ + colours_) / 2;
	//FragColor = vec4(colors2_.x * dist, colors2_.y * dist, colors2_.z * dist, 1);
	// 
	
	dvec3 fragPos = transformed_.xyz;

	// light data
	dvec3 lightPos = light;
	dvec3 lightColor = { .5,.5,.5 };
	float strength = 0.1;

	// calculate ambient light
	dvec3 ambient = strength * lightColor;

	// calculate diffuse
	dvec3 norm = normalize(normals_);
	dvec3 lightDir = normalize(lightPos - fragPos);
	double diff = max(dot(norm, lightDir), 0);
	dvec3 diffuse = diff * lightColor;

	// specular too
	float specStrength = .5;
	dvec3 viewDir = normalize(camPos_ - fragPos);
	dvec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(float(max(dot(viewDir, reflectDir), 0)), 128);
	dvec3 specular = specStrength * spec * lightColor;

	// sum it up

	//FragColor = vec4((diffuse+ambient + specular)*colors_, 1);
	// 
	//FragColor = vec4(normals_, 1);
	//FragColor = vec4(abs(normals_), 1);
	FragColor = vec4((diffuse + ambient + specular) * abs(normals_), 1);
	
	//FragColor = texture(text, texturecoords_);

	//FragColor = vec4(colours_, 1);

	// this is for a directional light


	// the ray for pixel to camera
	//vec3 ray = transformed_.xyz - camPos_;

	// camera direction is just camFront_;

	// the tails are in the same spot due to subtraction direction

	//float angle = acos(dot(ray, camFront_) / (magnitude(ray) * magnitude(camFront_)))*180/3.14159265;

	//if (abs(angle) < 20) {
		//float mag = magnitude(ray);
		//float dist = 1.0 / (mag * mag) * 75.0;
		//FragColor = vec4(colors_.x * dist, colors_.y * dist, colors_.z * dist, 1);
	//}
	//else {
		//float mag = magnitude(ray);
		//if (mag < 5) {
			//mag = 5;
		//}
		//float dist = 1.0 / (mag * mag*mag) * 75.0/4;
		//FragColor = vec4(colors_.x * dist, colors_.y * dist, colors_.z * dist, 1);
	//}

	//FragColor = vec4(colors_,1);
	//FragColor = vec4(position, 1);

}