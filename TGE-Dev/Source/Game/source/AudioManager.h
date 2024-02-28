#pragma once
#include <tge/audio/audio.h>
#include <string>
#include <nlohmann/json.hpp>
#include <map>
class AudioManager
{
public:
	AudioManager();
	~AudioManager();

	void Init();
	Tga::Audio GetAudio(std::string aName);

private:
	std::map<std::string, Tga::Audio> myAudio;
};

