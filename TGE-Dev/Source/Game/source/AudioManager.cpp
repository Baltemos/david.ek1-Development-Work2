#include "stdafx.h"
#include "AudioManager.h"
#include <fstream>

AudioManager::AudioManager()
{
}

AudioManager::~AudioManager()
{
}

void AudioManager::Init()
{
	std::string path = "Components/Editors/AudioManager.json";

	std::string resolvedPath = Tga::Settings::ResolveGameAssetPath(path);
	std::ifstream file(resolvedPath);
	nlohmann::json data;
	file >> data;


	for (auto& elem : data["Audio"])
	{
		Tga::Audio audio;
		std::string filePath = elem["FilePath"];
		bool shouldLoop = elem["ShouldLoop"];
		int amountOfChannels = elem["AmountOfChannels"];

		audio.Init(filePath.c_str(), shouldLoop, amountOfChannels);
		
		myAudio[filePath] = audio;
	}
}

Tga::Audio AudioManager::GetAudio(std::string aFilePath)
{
	std::map<std::string, Tga::Audio>::iterator it;
	it = myAudio.find(aFilePath);

	if (it == myAudio.end())
	{
		Tga::Audio audio;
		audio.Init(aFilePath.c_str(), false);
		myAudio[aFilePath] = audio;
	}

	return Tga::Audio(myAudio[aFilePath]);
}
