#include "SoundFactory.h"
#include <iostream>

SoundFactory& SoundFactory::Inst() {
		static SoundFactory instance;
		return instance;
}

SoundFactory::SoundFactory()
	: chunks(), musics()
{
}

SoundFactory::~SoundFactory() {
	// free chunks
	for (auto p : chunks) {
		Mix_FreeChunk(p.second);
	}

	// free musics
	for (auto p : musics) {
		Mix_FreeMusic(p.second);
	}
}

Mix_Chunk* SoundFactory::GetChunk(const std::string& name) {
	if (chunks.count(name) > 0) {
		return chunks[name];
	}
	else {
		Mix_Chunk* result = Mix_LoadWAV(name.c_str());
		if (result) {
			chunks[name] = result;
		}
		return result;
	}
}

Mix_Music* SoundFactory::GetMusic(const std::string& name) {
	if (musics.count(name) > 0) {
		return musics[name];
	}
	else {
		Mix_Music* result = Mix_LoadMUS(name.c_str());
		if (result) {
			musics[name] = result;
		}
		return result;
	}
}

void SoundFactory::PlaySound(const std::string& name, int numLoops) {
	Mix_Chunk* c = GetChunk(name);
	if (c) {
		Mix_PlayChannel(-1, c, numLoops);
	}
}

void SoundFactory::PlayMusic(const std::string& name, int numLoops) {
	Mix_HaltMusic();
	Mix_Music* m = GetMusic(name);
	if (m) {
		Mix_PlayMusic(m, numLoops);
	}
}
