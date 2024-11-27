#pragma once

#include "SoundEffect.h"
#include <vector>
#include <map>

class SfxManager
{
public:
	SfxManager();
	~SfxManager();

	SfxManager(const SfxManager& other) = delete;
	SfxManager& operator=(const SfxManager& other) = delete;

	SfxManager(SfxManager&& other) noexcept;
	SfxManager& operator=(SfxManager&& other) noexcept;

	void AddSfx(const std::string& filePath, const std::string& name);
	void Play(const std::string& name) const;
	void Play(int i) const;
	void SetVolume(const std::string& name, int value) const;
	void SetVolume(int i, int value) const;


	std::map<std::string, int> m_dictionary; 
	std::vector<SoundEffect*> m_sfx;

	

};

