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

//layout(std430, binding = 3) uniform name2{
//	sampler2D textures;
//}



uniform sampler2D t1;
uniform sampler2D t2;
uniform sampler2D depth1;
uniform sampler2D depth2;

in VS_OUT{
	vec3 FragPos;
vec3 Normal;
vec2 TexCoords;
vec4 FragPosLightSpace1;
vec4 FragPosLightSpace2;
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

float ShadowCalculation(vec4 fragPosLightSpace, sampler2D text) {
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;
	float closestDepth = texture(text, projCoords.xy).r;
	float currentDepth = projCoords.z;
	float shadow = currentDepth > closestDepth ? 1.0 : 0.0;

	//// if depth is outside of the range, then assume not in shadow
	//if (projCoords.z > 1.0 || projCoords.z < 0.0) {
	//	shadow = 0.0;
	//}
	//// remove shadows outsize of the texture range
	//if (projCoords.x > 1.0 || projCoords.x < 0.0) {
	//	shadow = 0.0;
	//}
	//// remove also for y axis
	//if (projCoords.y > 1.0 || projCoords.y < 0.0) {
	//	shadow = 0.0;
	//}
	return shadow;
}

//vec3 ShadowCalculation(vec4 fragPosLightSpace) {
//	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
//	projCoords = projCoords * 0.5 + 0.5;
//	return projCoords;
//	float closestDepth = texture(depth, projCoords.xy).r;
//	float currentDepth = projCoords.z;
//	float shadow = currentDepth > closestDepth ? 1.0 : 0.0;
//	return shadow;
//}

void main() {


	//vec3 specular = vec3(texture(t2, texturecoords_).x);
	vec3 specular = vec3(1, 1, 1);

	DirectionalLight l = {{ -1,-1,1}, { 1,1,1 }, { .5,.5,.5 }, { 1,1,1 }};

	vec3 result = directional(l,specular);
	//vec3 result = vec3(0, 0, 0);
	//vec3 result = vec3(.25);
	//result = vec3(0);

	//Light l2 = { {0,0,0},{0,0,0},{0,0,0},{0,0,0} };
	//result += base(l2);
	for (int i = 0; i < size-size; i++) {
		result += base(data[i],specular);
		//result += data[i].position;
	}
	//result = vec3(0, 0, 0);


	//result += data[1].position;
	
	float shadow1 = ShadowCalculation(fs_in.FragPosLightSpace1, depth1);
	float shadow2 = ShadowCalculation(fs_in.FragPosLightSpace2, depth2);

	vec3 result_;
	if (shadow1 + shadow2 >.5) {
		result_ = vec3(0, 0, 0);
	}
	else {
		//result_ = result;
		result_ = vec3(1, 1, 1);
	}
	


	
	//FragColor = vec4(vec3(shadow), 1);
	// if shadow is 1, then we are in shadow
	//result_ = (1 - shadow1)* result;
	
	FragColor = vec4(result_, 1);


	FragColor = vec4(vec3(shadow1), 1);

	//FragColor = vec4(result, 1);

	//FragColor = vec4(1, 1, 1, 1);


	//FragColor = vec4(ShadowCalculation(fs_in.FragPosLightSpace).xy/2,0,1);

	//FragColor = vec4(vec3(shadow), 1);



	//FragColor = vec4(1, 1, 1, 1);

	//FragColor = vec4(shadow, shadow, shadow, 1);

	//FragColor = texture(depth, texturecoords_);

	//FragColor = vec4(1);

	//FragColor = texture(t1, texturecoords_);


	//float depth = gl_FragCoord.z * (1 / 1000.0 - 1 / .01) + 1/.01;
	//FragColor = vec4(result,1);
	//FragColor = vec4(vec3(depth), 1);

	//FragColor = texture(t1, texturecoords_);

	//FragColor = vec4(data[0].position, 1);

	//FragColor = vec4(data[0], data[1], data[2], 1);


}