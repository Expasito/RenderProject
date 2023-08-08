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



out vec3 camPos_;
out vec3 camFront_;


void main(){
	position = vec3(aPos);
    colors_ = vec3(colors);
    colours_ = vec3(colours);
    normals_ = normals;
    texturecoords_ = texturecoords;
    camPos_ = camPos;
    camFront_ = camFront;


    // now we can use this in the other shader
    vec4 transformed_coordinates = translate(-translations.x, translations.y, translations.z) * RotateX(rotations.x) * RotateY(rotations.y) * RotateZ(rotations.z) * scale(scalations.x, scalations.y, scalations.z) * vec4(aPos, 1);
    
    transformed_ = transformed_coordinates;

    gl_Position = projection * view * transformed_coordinates;


}