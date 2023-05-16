#pragma once
#include <string>
#include <GL\glew.h>
#include "transform.h"
#include "MainCamera.h"

class ShaderManager
{
public:
	ShaderManager();
	~ShaderManager();

	void Bind(); //Set gpu to use our shaders
	void UpdateShader(const Transform& transform, const MainCamera& cam);
	void UpdateCubemap(const MainCamera& cam);
	void InitalizeShader(const std::string& vertFile, const std::string& fragFile);
	void initGeo();

	GLuint getID() { return this->program; }

	std::string ShaderManager::ShaderLoader(const std::string& fileName);
	void ShaderManager::CheckForError(GLuint shader, GLuint flag, bool programOn, const std::string& errorMessage);
	GLuint ShaderManager::CreateShader(const std::string& text, unsigned int type);

	// ------------------------------------------------------------------------
	void setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(program, name.c_str()), (int)value);

		if ((glGetUniformLocation(program, name.c_str()) == -1))
		{
		//std::cerr << "Unable to load shader: " << name.c_str() << std::endl;
		//	__debugbreak();
		}
	}
	// ------------------------------------------------------------------------
	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(program, name.c_str()), value);

		if ((glGetUniformLocation(program, name.c_str()) == -1))
		{
		//	std::cerr << "Unable to load shader: " << name.c_str() << std::endl;
		//	__debugbreak();
		}
	}
	// ------------------------------------------------------------------------
	void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(program, name.c_str()), value);

		//if ((glGetUniformLocation(shaderID, name.c_str()) == -1))
		//{
		//	std::cerr << "Unable to load shader: " << name.c_str() << std::endl;
		//	__debugbreak();
		//}
	}
	// ------------------------------------------------------------------------
	void setVec2(const std::string& name, const glm::vec2& value) const
	{
		glUniform2fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);

		if ((glGetUniformLocation(program, name.c_str()) == -1))
		{
		//	std::cerr << "Unable to load shader: " << name.c_str() << std::endl;
		//	__debugbreak();
		}
	}
	void setVec2(const std::string& name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(program, name.c_str()), x, y);

		if ((glGetUniformLocation(program, name.c_str()) == -1))
		{//
		//	std::cerr << "Unable to load shader: " << name.c_str() << std::endl;
		//	__debugbreak();
		}
	}
	// ------------------------------------------------------------------------
	void setVec3(const std::string& name, const glm::vec3& value) const
	{
		glUniform3fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);

		if ((glGetUniformLocation(program, name.c_str()) == -1))
		{
		//	std::cerr << "Unable to load shader: " << name.c_str() << std::endl;
			//__debugbreak();
		}
	}
	void setVec3(const std::string& name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(program, name.c_str()), x, y, z);

		if ((glGetUniformLocation(program, name.c_str()) == -1))
		{
		//	std::cerr << "Unable to load shader: " << name.c_str() << std::endl;
		//	__debugbreak();
		}
	}
	// ------------------------------------------------------------------------
	void setVec4(const std::string& name, const glm::vec4& value) const
	{
		glUniform4fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);

		if ((glGetUniformLocation(program, name.c_str()) == -1))
		{
		//	std::cerr << "Unable to load shader: " << name.c_str() << std::endl;
			//__debugbreak();
		}
	}
	void setVec4(const std::string& name, float x, float y, float z, float w)
	{
		glUniform4f(glGetUniformLocation(program, name.c_str()), x, y, z, w);

		if ((glGetUniformLocation(program, name.c_str()) == -1))
		{
		//	std::cerr << "Unable to load shader: " << name.c_str() << std::endl;
		//	__debugbreak();
		}
	}
	// ------------------------------------------------------------------------
	void setMat2(const std::string& name, const glm::mat2& mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &mat[0][0]);

		if ((glGetUniformLocation(program, name.c_str()) == -1))
		{
		//	std::cerr << "Unable to load shader: " << name.c_str() << std::endl;
		//	__debugbreak();
		}
	}
	// ------------------------------------------------------------------------
	void setMat3(const std::string& name, const glm::mat3& mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &mat[0][0]);

		if ((glGetUniformLocation(program, name.c_str()) == -1))
		{
		//	std::cerr << "Unable to load shader: " << name.c_str() << std::endl;
		//	__debugbreak();
		}
	}
	// ------------------------------------------------------------------------
	void setMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &mat[0][0]);

		if ((glGetUniformLocation(program, name.c_str()) == -1))
		{
	     //	std::cerr << "Unable to load shader: " << name.c_str() << std::endl;
			//__debugbreak();
		}
	}


private:
	static const unsigned int NUM_OF_SHADERS = 2; // number of shaders

	enum
	{
		TRANSFORM_U,
		VIEWMATRIX_U,
		PROJECTION_U,
		MODEL_U,
		CAMERAPOS_U,
		NUM_OF_UNIFORMS
	};
	GLuint program; // Track the shader program
	GLuint shaders[NUM_OF_SHADERS]; //array of shaders
	GLuint shaders1[3];
	GLuint uniforms[NUM_OF_UNIFORMS]; //no of uniform variables
};
