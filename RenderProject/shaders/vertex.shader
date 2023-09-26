#version 450 core
layout (location = 0) in vec3 aPos;
layout(location = 1) in vec3 translations;
layout(location = 2) in vec3 rotations;
layout(location = 3) in vec3 scalations;
layout(location = 4) in vec3 colors;
layout(location = 5) in vec3 colours;
layout(location = 6) in vec3 normals;
layout(location = 7) in vec2 texturecoords;


out vec3 colors_;
out vec3 colours_;
out vec3 position;
out vec4 transformed_;
out vec3 normals_;
out vec2 texturecoords_;


mat4 translate(float x, float y, float z) {
    return mat4(
        vec4(1.0, 0.0, 0.0, 0.0),
        vec4(0.0, 1.0, 0.0, 0.0),
        vec4(0.0, 0.0, 1.0, 0.0),
        vec4(x, y, z, 1.0)
    );
}

mat4 scale(float x, float y, float z) {
    return mat4(
        vec4(x, 0.0, 0.0, 0.0),
        vec4(0.0, y, 0.0, 0.0),
        vec4(0.0, 0.0, z, 0.0),
        vec4(0.0, 0.0, 0.0, 1.0)
    );
}

mat4 RotateX(float phi) {
    return mat4(
        vec4(1., 0., 0., 0),
        vec4(0., cos(phi), -sin(phi), 0.),
        vec4(0., sin(phi), cos(phi), 0.),
        vec4(0., 0., 0., 1.));
}

mat4 RotateY(float theta) {
    return mat4(
        vec4(cos(theta), 0., -sin(theta), 0),
        vec4(0., 1., 0., 0.),
        vec4(sin(theta), 0., cos(theta), 0.),
        vec4(0., 0., 0., 1.));
}

mat4 RotateZ(float psi) {
    return mat4(
        vec4(cos(psi), -sin(psi), 0., 0),
        vec4(sin(psi), cos(psi), 0., 0.),
        vec4(0., 0., 1., 0.),
        vec4(0., 0., 0., 1.));
}

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 camPos;
uniform vec3 camFront;


uniform mat4 lightSpaceMatrix1;
uniform mat4 lightSpaceMatrix2;




out vec3 camPos_;
out vec3 camFront_;

out VS_OUT{
    vec3 FragPos;
vec3 Normal;
vec2 TexCoords;
vec4 FragPosLightSpace1;
vec4 FragPosLightSpace2;
} vs_out;


void main(){
	position = vec3(aPos);
    colors_ = vec3(colors);
    colours_ = vec3(colours);
    normals_ = normals;
    texturecoords_ = texturecoords;
    camPos_ = camPos;
    camFront_ = camFront;

    mat4 model = translate(-translations.x, translations.y, translations.z) * RotateX(rotations.x) * RotateY(rotations.y) * RotateZ(rotations.z) * scale(scalations.x, scalations.y, scalations.z);
    
    //mat4 model = mat4(1);
    transformed_ = model * vec4(aPos, 1);
    //transformed_ = vec4(aPos, 1);

    vs_out.FragPos = vec3(transformed_);
    vs_out.Normal = transpose(inverse(mat3(model))) * normals;
    vs_out.TexCoords = texturecoords;
    vs_out.FragPosLightSpace1 = lightSpaceMatrix1 * vec4(vs_out.FragPos, 1.0);
    vs_out.FragPosLightSpace2 = lightSpaceMatrix2 * vec4(vs_out.FragPos, 1.0);

    gl_Position = projection * view * vec4(vs_out.FragPos,1.0);


}