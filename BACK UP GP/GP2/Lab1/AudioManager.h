#pragma once
#include <SDL/SDL_mixer.h>
#include <SDL/SDL.h>
#include <iostream>
#include <vector>

class AudioManager
{
	public:
		AudioManager();
		~AudioManager();
		
		void AddSound(const char* path);
		void AddAudio(const char* path);
		void PlaySound(const int which) const;
		void PlayAudio();

	private:
		Mix_Music* music;
		std::vector<Mix_Chunk*> soundHolder;
};

