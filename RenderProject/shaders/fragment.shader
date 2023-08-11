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


uniform sampler2D t1;
uniform sampler2D t2;


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


uniform vec3 light;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

void main() {

	Material material = { 
		{1,0,0},
		{.5,.5,.5},
		{1,1,1},
		512
	};

	Light light = {
		{0,0,0},
		{.1,.1,.1},
		{0,0,0},
		{1,1,1}
	};

	vec3 FragPos = transformed_.xyz;

	// calculate ambient light
	vec3 ambient = light.ambient * material.ambient;

	// calculate diffuse
	vec3 norm = normalize(normals_);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0);
	vec3 diffuse = light.diffuse * (diff*material.diffuse);

	vec3 viewDir = normalize(camPos_ - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(float(max(dot(viewDir, reflectDir), 0)), material.shininess);
	vec3 specular = light.specular * (spec * material.specular);


	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result,1);


}