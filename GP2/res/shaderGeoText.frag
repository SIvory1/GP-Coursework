//Version number
#version 400
//Layout Qualifer
layout( location = 0 ) out vec4 fragcolor;
//Unfrom variable
uniform float randColourX;
uniform float randColourY;
uniform float randColourZ;
 
 uniform sampler2D diffuse;

 in vec2 TexCoords;

void main()
{
//Setting each vector component to uniform varaible then setting final colour
	fragcolor = texture2D(diffuse, TexCoords);
}