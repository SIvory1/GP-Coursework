#pragma once
#include <string>
#include <vector>
#include <GL\glew.h>
#include "ShaderManager.h"
class CubemapManager
{
public:
	CubemapManager();
	~CubemapManager();

	void CubeMapLoader();
	void CubemapVAO();
	void DrawCubemap();
	void InitalizeCubeMap();
	void CubeVertexArrayObject();
	void DrawCube();

private:

	unsigned int cubemapTextureID;
	unsigned int cubeVAO, cubeVBO;
	unsigned int skyboxVAO, skyboxVBO;
};