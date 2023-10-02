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




float ShadowCalculation(vec4 fragPosLightSpace, sampler2D text) {
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;

	// first make sure the projection coordinates are within the 0 to 1 range
	// and not to far out to me measured
	if (projCoords.x > 1 ||
		projCoords.x < 0 ||
		projCoords.y < 0 ||
		projCoords.y > 1 ||
		projCoords.z > 1
		) {
		return 0;
	}

	float closestDepth = texture(text, projCoords.xy).r;
	float currentDepth = projCoords.z;
	// add since we are culling front faces

	//float d = dot(fs_in.Normal, normalize(vec3(0, 0, 0) - vec3(1, 100, 1))) * .5 + .5;
	//float d = dot(fs_in.Normal, normalize(vec3(0, 0, 0) - vec3(1, 100, 1)));

	//float bias = -d*.0001;
	float bias = max(0.005 * (1.0 - dot(fs_in.Normal, normalize(vec3(0, 0, 0) - vec3(1, 100, 1)))), 0.0005);
	//bias = .00005;
	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
	//float shadow = currentDepth > closestDepth ? 1.0 : 0.0;



	return shadow;
}

vec3 directional(DirectionalLight light, vec3 specular_) {
	vec3 FragPos = transformed_.xyz;

	// calculate ambient light
	vec3 ambient = light.ambient * material.ambient;
	//vec3 ambient = light.ambient * colors_;

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

	float shadow1 = ShadowCalculation(fs_in.FragPosLightSpace1, depth1);

	vec3 result = (ambient + (diffuse + specular) * (1 - shadow1)) * colors_;

	//vec3 result = ambient + diffuse + specular;
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
	float shadow1 = ShadowCalculation(fs_in.FragPosLightSpace1, depth1);

	vec3 result = ambient + (diffuse + specular) * (1 - shadow1);
	//vec3 result = ambient + diffuse + specular;
	return result;
}



void main() {


	//vec3 specular = vec3(texture(t2, texturecoords_).x);
	vec3 specular = vec3(1, 1, 1);

	//DirectionalLight l = { { -1,-1,1}, { 1,1,1 }, { .5,.5,.5 }, { 1,1,1 } };
	DirectionalLight l = {{ 1,-1,1}, { 1,1,1 }, { .5,.5,.5 }, { .05,.05,.05}};


	vec3 result = directional(l,specular);

	//vec3 result = vec3(1);


	for (int i = 0; i < size-size; i++) {
		result += base(data[i],specular);
	}

	
	//float shadow1 = ShadowCalculation(fs_in.FragPosLightSpace1, depth1);
	//float shadow2 = ShadowCalculation(fs_in.FragPosLightSpace2, depth2);


	//result = (1 - shadow1) * result;

	




	
	FragColor = vec4(result, 1);



}