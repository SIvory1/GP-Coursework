#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include <SDL/SDL_mixer.h>
#include "DisplayGame.h" 
#include "ShaderManager.h"
#include "MeshManager.h"
#include "TextureManager.h"
#include "transform.h"
#include "AudioManager.h"
#include "GameManager.h"


class GameObject
{
public:
	GameObject();
	~GameObject();

	void CombineObject();
	void CombineObject1();


private:

	DisplayGame gameDisplay;
	MeshManager mesh;
	MeshManager mesh1;
	MainCamera mainCamera;
	ShaderManager shader;
	AudioManager gameAudio;
//	GameManager gameManager;

}; 
