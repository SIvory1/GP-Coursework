#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;
in vec2 tC;

uniform vec3 cameraPos;
uniform samplerCube skybox;
uniform sampler2D diffuse;
uniform vec3 waterColour;


vec4 reflectionColour;
vec4 refractionColour;

vec4 reflection()
{
    vec3 I = normalize(Position - cameraPos);
    vec3 R = reflect(I, normalize(Normal));
    return vec4(texture(skybox, R).rgb, 1.0);
}

vec4 refraction()
{
    float ratio = 1.00 / 1.52;
    vec3 I = normalize(Position - cameraPos);
    vec3 R = refract(I, normalize(Normal), ratio);
    return vec4(texture(skybox, R).rgb, 1.0);
}

void main()
{     

reflectionColour = reflection();
refractionColour = refraction();

vec3 colour = mix(reflectionColour, refractionColour, 0.5).xyz;

vec3 lightColor = vec3(0.8,0.8,0.8);

//colour = mix(waterColour,lightColor, 0.6);

FragColor = vec4(colour, 1.0);
}



