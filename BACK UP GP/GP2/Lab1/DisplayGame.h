#pragma once
#include <SDL/SDL.h>
#include <GL\glew.h>
#include <iostream>
#include <string>
using namespace std;


class DisplayGame
{
public:
	DisplayGame();
	~DisplayGame();
	void InitalizeDisplay();
	void ChangeBuffer();
	void ClearDisplay(float r, float g, float b, float a);

	float getX();
	float getY();

private:

	void ReturnError(std::string errorMessage);

	SDL_Window* window; //holds pointer to out window
	int screenX;
	int screenY;
	SDL_GLContext glContext;
};

