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
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

layout(std430, binding = 3) buffer name
{
	int size;
	Light data[];
};




uniform sampler2D t1;
uniform sampler2D t2;
uniform sampler2D depth;

in VS_OUT{
	vec3 FragPos;
vec3 Normal;
vec2 TexCoords;
vec4 FragPosLightSpace;
} fs_in;


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



struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};



struct DirectionalLight {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct SpotLight {
	vec3 position;
	vec3 direction;
	float angle;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

//uniform Light light;

uniform Material material;


vec3 directional(DirectionalLight light, vec3 specular_) {
	vec3 FragPos = transformed_.xyz;

	// calculate ambient light
	vec3 ambient = light.ambient * material.ambient;

	// calculate diffuse
	vec3 norm = normalize(normals_);
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(norm, lightDir), 0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	// specular
	vec3 viewDir = normalize(camPos_ - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(float(max(dot(viewDir, reflectDir), 0)), material.shininess);
	vec3 specular = light.specular * (spec * specular_);


	vec3 result = ambient + diffuse + specular;
	return result;
}

vec3 base(Light light,vec3 specular_) {
	vec3 FragPos = transformed_.xyz;

	// calculate ambient light
	vec3 ambient = light.ambient * material.ambient;

	// calculate diffuse
	vec3 norm = normalize(normals_);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	// specular
	vec3 viewDir = normalize(camPos_ - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(float(max(dot(viewDir, reflectDir), 0)), material.shininess);
	vec3 specular = light.specular * (spec * specular_);
	vec3 result = ambient + diffuse + specular;
	return result;
}

float ShadowCalculation(vec4 fragPosLightSpace) {
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;
	float closestDepth = texture(t1, projCoords.xy).r;
	float currentDepth = projCoords.z;
	float shadow = currentDepth > closestDepth ? 1.0 : 0.0;
	return shadow;
}

void main() {


	vec3 specular = vec3(texture(t2, texturecoords_).x);

	DirectionalLight l = {{ 0,-1,0 }, { 1,1,1 }, { .5,.5,.5 }, { 1,1,1 }
	};

	vec3 result = directional(l,specular);
	//result = vec3(0);

	//Light l2 = { {0,0,0},{0,0,0},{0,0,0},{0,0,0} };
	//result += base(l2);
	for (int i = 0; i < size-size; i++) {
		result += base(data[i],specular);
	}
	//result = vec3(0, 0, 0);


	//result += data[1].position;
	
	float shadow = ShadowCalculation(fs_in.FragPosLightSpace);

	//FragColor = vec4(vec3(shadow), 1);

	if (shadow < .5) {
		FragColor = vec4(1);
	}
	else {
		FragColor = vec4(0);
	}

	FragColor = vec4(1);

	//FragColor = texture(t1, texturecoords_);


	//float depth = gl_FragCoord.z * (1 / 1000.0 - 1 / .01) + 1/.01;
	//FragColor = vec4(result,1);
	//FragColor = vec4(vec3(depth), 1);

	//FragColor = texture(t1, texturecoords_);

	//FragColor = vec4(data[0].position, 1);

	//FragColor = vec4(data[0], data[1], data[2], 1);


}