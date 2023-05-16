#version 420 core
out vec4 FragColor;

in Vertex_DATA{
    vec2 tC;
    vec3 Normal;
    vec3 Position;
} vs_in;

uniform vec3 cameraPos;
uniform samplerCube skybox;
layout (binding = 1) uniform sampler2D diffuse;

void main()
{    

    vec3 I = normalize(vs_in.Position - cameraPos); //vector to camera

    vec3 R = reflect(I, normalize(vs_in.Normal)); //calculate the reflection direction for the incident vector

    vec4 Colour = vec4(texture(skybox, R).rgb, 1.0);

    vec4 roughColor = texture2D(diffuse, vs_in.tC);

    FragColor = mix(Colour, roughColor, 0.5);
}
