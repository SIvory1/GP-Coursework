#include "ShaderManager.h"
#include <iostream>
#include <fstream>

ShaderManager::ShaderManager()
{
	// init variables
	program = NULL;	
}

ShaderManager::~ShaderManager()
{
	for (unsigned int i = 0; i < NUM_OF_SHADERS; i++)
	{
		glDetachShader(program, shaders[i]); //detach shader from program
	}
	glDeleteProgram(program); // delete the program
}

void ShaderManager::InitalizeShader(const std::string& vertFile, const std::string& fragFile)
{
	program = glCreateProgram(); // create shader program 
	shaders[0] = CreateShader(ShaderLoader(vertFile), GL_VERTEX_SHADER); // create vertex shader
	shaders[1] = CreateShader(ShaderLoader(fragFile), GL_FRAGMENT_SHADER); // create fragment shader

	for (unsigned int i = 0; i < NUM_OF_SHADERS; i++)
	{
		glAttachShader(program, shaders[i]); //attach shader to program,
		glDeleteShader(shaders[i]); //once shaders are attached they are deleted
	}

	glLinkProgram(program); //create executables that will run on the GPU shaders
	CheckForError(program, GL_LINK_STATUS, true, "Error: Shader program linking failed"); // checks for error

	glValidateProgram(program); //checks if program is valid
	CheckForError(program, GL_VALIDATE_STATUS, true, "Error: Shader program not valid");

	// gets location of uniforms
	uniforms[TRANSFORM_U] = glGetUniformLocation(program, "transform"); 
	uniforms[VIEWMATRIX_U] = glGetUniformLocation(program, "view"); 
	uniforms[PROJECTION_U] = glGetUniformLocation(program, "projection"); 
	uniforms[MODEL_U] = glGetUniformLocation(program, "model"); 
	uniforms[CAMERAPOS_U] = glGetUniformLocation(program, "cameraPos"); 
}
void ShaderManager::initGeo()
{
	program = glCreateProgram(); // create shader program (openGL saves as ref number)
	shaders1[0] = CreateShader(ShaderLoader("..\\res\\shaderGeoText.vert"), GL_VERTEX_SHADER); // create vertex shader
	shaders1[1] = CreateShader(ShaderLoader("..\\res\\shaderGeoText.geom"), GL_GEOMETRY_SHADER); // create fragment shader
	shaders1[2] = CreateShader(ShaderLoader("..\\res\\shaderGeoText.frag"), GL_FRAGMENT_SHADER); // create fragment shader


	for (unsigned int i = 0; i < 3; i++)
	{
		glAttachShader(program, shaders1[i]); //add all our shaders to the shader program "shaders" 
	}

	glBindAttribLocation(program, 0, "VertexPosition"); // associate attribute variable with our shader program attribute (in this case attribute vec3 position;)
	glBindAttribLocation(program, 1, "VertexTexCoord");
	glBindAttribLocation(program, 2, "VertexNormal");

	glLinkProgram(program); //create executables that will run on the GPU shaders
	//CheckShaderError(shaderID, GL_LINK_STATUS, true, "Error: Shader program linking failed"); // cheack for error

	glValidateProgram(program); //check the entire program is valid
	//CheckShaderError(shaderID, GL_VALIDATE_STATUS, true, "Error: Shader program not valid");

	uniforms[TRANSFORM_U] = glGetUniformLocation(program, "transform"); // associate with the location of uniform variable within a program
}

void ShaderManager::Bind()
{
	glUseProgram(program); //installs the program object specified by program as part of rendering state
}

void ShaderManager::UpdateShader(const Transform& transform, const MainCamera& cam)
{
	glm::mat4 mvp = cam.GetViewProjection() * transform.GetModel();
	glUniformMatrix4fv(uniforms[TRANSFORM_U], 1, GLU_FALSE, &mvp[0][0]);
}

// updates cubemap values
void ShaderManager::UpdateCubemap(const MainCamera& cam)
{
	glm::mat4 view = (glm::mat3(cam.GetView())); // need to remove translation so it doesnt move with player
	glm::mat4 projection = cam.GetProjection(); // get projection matrix

	// send unfiorm location and value to shader 
	glUniformMatrix4fv(uniforms[VIEWMATRIX_U], 1, GLU_FALSE, &view[0][0]);
	glUniformMatrix4fv(uniforms[PROJECTION_U], 1, GLU_FALSE, &projection[0][0]);
}

GLuint ShaderManager::CreateShader(const std::string& text, unsigned int type)
{
	GLuint shader = glCreateShader(type); //create shader based on specified type

	if (shader == 0) //if == 0 shader no created
		std::cerr << "Error type creation failed " << type << std::endl;

	const GLchar* stringSource[1]; //convert strings into list of c-strings
	stringSource[0] = text.c_str();
	GLint lengths[1];
	lengths[0] = text.length();

	glShaderSource(shader, 1, stringSource, lengths); //send source code to opengl
	glCompileShader(shader); //get open gl to compile shader code

	CheckForError(shader, GL_COMPILE_STATUS, false, "Error compiling shader!"); //check for compile error

	return shader;
}

// loads shader from file
std::string ShaderManager::ShaderLoader(const std::string& filename)
{
	std::ifstream file;
	file.open((filename).c_str());

	std::string output;
	std::string line;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else
	{
		std::cerr << "Unable to load shader: " << filename << std::endl;
	}

	return output;
}

// used to reutrn errors 
void ShaderManager::CheckForError(GLuint shader, GLuint flag, bool programOn, const std::string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (programOn)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE)
	{
		if (programOn)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);

		std::cerr << errorMessage << ": '" << error << "'" << std::endl;
	}
}