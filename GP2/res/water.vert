#version 420 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 textCoor;
layout (location = 2) in vec3 normals;

out Vertex_DATA{
    vec2 tC;
    vec3 normal;
    vec3 position;
} vs_Out;

uniform mat4 model;
uniform mat4 transform;

void main()
{
    vs_Out.tC = textCoor;
    vs_Out.normal = mat3(transpose(inverse(model))) * normals;
    vs_Out.position = vec3(model * vec4(pos, 1.0));

    gl_Position = transform * vec4(pos, 1.0);
}  


