#ifndef CHUNK_FACTORY__H
#define CHUNK_FACTORY__H

#include <SDL_mixer.h>
#include <map>
#include <string>

class SoundFactory {
public:
	~SoundFactory();
	static SoundFactory& Inst();

	Mix_Chunk* GetChunk(const std::string& name);
	Mix_Music* GetMusic(const std::string& name);

	// Playback
	//
	static const int INFINITE_LOOPS = -1;

	// Play a sound.
	void PlaySound(const std::string& name, int numLoops = 0);

	// Play music.
	void PlayMusic(const std::string& name, int numLoops = INFINITE_LOOPS);

private:
	SoundFactory();
	std::map<std::string, Mix_Chunk*> chunks;
	std::map<std::string, Mix_Music*> musics;
};

#endif /* CHUNK_FACTORY__H */
