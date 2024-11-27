#include "pch.h"
#include "SfxManager.h"

SfxManager::SfxManager()
{
}

SfxManager::~SfxManager()
{
	for (SoundEffect*& sfct: m_sfx) {
		delete sfct;
		sfct = nullptr;
	}
}

SfxManager::SfxManager(SfxManager&& other) noexcept:
	m_sfx{ std::move(other.m_sfx) },
	m_dictionary(std::move(other.m_dictionary))
{
}

SfxManager& SfxManager::operator=(SfxManager&& other) noexcept
{
	m_sfx = std::move(other.m_sfx);
	m_dictionary = std::move(other.m_dictionary);
	return *this;
}

void SfxManager::AddSfx(const std::string& filePath, const std::string& name)
{
	if (m_dictionary.find(name) == m_dictionary.end()) {
		m_dictionary.insert({ name , m_sfx.size() });
		m_sfx.push_back(new SoundEffect{filePath});
	}
	else {
		return;
	}

}

void SfxManager::Play(const std::string& name) const
{
	if (m_dictionary.find(name) == m_dictionary.end()) {
		return;
	}
	Play(m_dictionary.at(name));
}

void SfxManager::Play(int i) const
{
	m_sfx[i]->Play(false);
}

void SfxManager::SetVolume(const std::string& name,int value) const
{
	if (m_dictionary.find(name) == m_dictionary.end()) {
		return;
	}
	SetVolume(m_dictionary.at(name),value);
}

void SfxManager::SetVolume(int i ,int value) const
{
	m_sfx[i]->SetVolume(value);
}
