#version 420 core

out vec4 FragColor;

in Vertex_DATA{
    vec2 tC;
    vec3 normal;
    vec3 position;
} vs_in;

uniform vec3 cameraPos;
uniform samplerCube skybox;

layout (binding = 1)uniform sampler2D noiseTex;
layout (binding = 2)uniform sampler2D waterTex;

uniform float time;

const float speed = 0.5;
const float scale = 0.8; 

float average(vec4 color) {
    return (color.r + color.g + color.b) / 3;
}

void main()
{    
    float refractionRatio = 0.751; 
    vec3 I = normalize(vs_in.position - cameraPos); 
    vec3 R = refract(I, normalize(vs_in.normal), refractionRatio); 
    vec4 refraction = vec4(texture(skybox, R).rgb, 1.0);

    vec4 waterShader = texture(waterTex,  vs_in.tC + (time * speed));
    vec4 finalWater = mix( waterShader,vec4(0.01,0.06,0.26,1), 0.5 );

    vec2 scaledTC = vs_in.tC * scale;

    vec4 ripple1 = texture(noiseTex, scaledTC + time*0.2*(speed*2));
    vec4 ripple2 = texture(noiseTex, scaledTC.xy + time*(speed*2) * vec2(-0.2, -0.2));

    ripple1.rgb = vec3(average(ripple1));
    ripple2.rgb = vec3(average(ripple2));

    FragColor = mix(refraction * ((ripple1 + ripple2) * 0.4) ,finalWater, 0.6) ;
}


