#version 330 core
out vec4 FragColor;


 in   vec3 PerlinNormal;
 in   vec4 PerlinPos; 


in   vec3 RefractionNormal;
 in  vec3 RefractionPosition;


in vec2 tC;

uniform vec3 cameraPos;
uniform samplerCube skybox; 


vec4 refraction()
{
   float ratio = 1.00 / 1.33;
    vec3 I = normalize(RefractionPosition - cameraPos);
    vec3 R = refract(I, normalize(RefractionNormal), ratio);

 
    vec4 lightColor = vec4(0.0,0.0,0.1,1.0);


    vec4 john = vec4(texture(skybox, R).rgb, 1.0);

    vec4 colour = mix(john,lightColor, 0.5);

    // this is different to this way FragColor = vec4(colour, 1.0); as colour is already a vec 4 we do not need to cast it
    return colour;
}

uniform float time;

uniform float maxDist; //fog max distance
uniform float minDist; //fog min distance
uniform float fogDensity;
uniform vec3 fogColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{             
 vec4 refraction = refraction();
 	vec2 pos = - 1.0 + 2.0 * tC;

	vec4 noise = texture2D( texture1, tC ); //noise texture
	vec2 T1 = tC + vec2( 1.5, - 1.5 ) * time; // "time" 'animates' the texture
	vec2 T2 = tC + vec2( - 0.5, 2.0 )  * time; // as above

	T1.x += noise.x * 10; //offset change these 4 values to see the change in frequency, see notes at end of shader.
	T1.y += noise.y * 10;
	T2.x -= noise.y * 0.2; //this just offsets the texture coordinates
	T2.y += noise.z * 0.2; // but allows us to offset y&z in opposite directions

	float p = texture2D( texture1, T1).a; //get the alpha from the noise texture

	vec4 color = texture2D( texture2, T2); //coloured texture offset can here or above
	vec4 temp = color * ( vec4(p) * 2.0 ) + color; //add/remove the last colour

	if( temp.r > 1.0 ) { temp.bg += clamp( temp.r - 2.0, 0.0, 100.0 ); } // again play about with these
	if( temp.g > 1.0 ) { temp.rb += temp.g - 1.0; }
	if( temp.b > 1.0 ) { temp.rg += temp.b - 1.0; } // = vec2(0.0,0.0)

	FragColor = temp;
	float dist = abs( PerlinPos.z ); //absolute value
 //  float  fogFactor = (maxDist - dist)/(maxDist - minDist);
//	fogFactor = clamp( fogFactor, 0.0, 1.0 ); // constrain range
  vec3 blue = vec3(0.0,0.0,0.1);

 FragColor = mix( refraction, vec4( blue, refraction.w ), 0.5 ); 
}


