//Version Number
#version 400

//The layout qualifers
layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 TexCoords;

//Uniform variable
uniform mat4 transform;

//Passing out the normal and position data
out VS_OUT {
   vec2 texCoords;
} gs_out;

void main()
{
	//Assigning the normal and position data
	gs_out.texCoords = TexCoords;

	// Sets the position of the current vertex
	gl_Position = transform * vec4(VertexPosition, 1.0);
}