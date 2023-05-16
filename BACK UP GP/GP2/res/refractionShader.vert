#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec3 aNormal;
layout (location = 1) in vec2 TextC;



   out vec3 RefractionNormal;
  out vec3 RefractionPosition;



 out   vec3 PerlinNormal;
  out  vec4 PerlinPos; 


out vec2 tC;

uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
    tC = TextC;
   PerlinPos = transform * vec4(aPos, 1.0);
    gl_Position = transform * vec4(aPos, 1.0);

   RefractionNormal = mat3(transpose(inverse(model))) * aNormal;
   RefractionPosition = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * vec4(RefractionPosition, 1.0);
}  