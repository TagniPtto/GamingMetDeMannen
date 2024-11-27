#include "pch.h"
#include "TextureManager.h"
#include <iostream>

#include "Texture.h"

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
	for (Texture*& pTexture: m_pTextures) {
		delete pTexture;
		pTexture = nullptr;
	}
}

TextureManager::TextureManager(TextureManager&& other)noexcept :
	m_pTextures{std::move(other.m_pTextures)},
	m_dictionary(std::move(other.m_dictionary))
{

}

TextureManager& TextureManager::operator=(TextureManager&& other)noexcept
{
	for (Texture*& pTexture : m_pTextures) {
		delete pTexture;
		pTexture = nullptr;
	}
	m_pTextures = std::move(other.m_pTextures);
	return *this;
}

void TextureManager::AddTexture(const std::string& texturePath, const std::string& name)
{
	if (m_dictionary.find(name) == m_dictionary.end()) {
		m_dictionary.insert({ name , m_pTextures.size()});
		m_pTextures.push_back(new Texture{ texturePath });
	}
	else {
		return;
	}
}

void TextureManager::DrawTexture(const std::string& name, const Rectf& dstRect, const Rectf& srcRect) const
{
	if (m_dictionary.find(name) == m_dictionary.end()) {
		return;
	}
	DrawTexture(m_dictionary.at(name), dstRect, srcRect);
}

void TextureManager::DrawTexture(int i, const Rectf& dstRect, const Rectf& srcRect) const
{
	if (i >= m_pTextures.size()) {

	}
	m_pTextures[i]->Draw(dstRect,srcRect);
}

Texture* TextureManager::GetTexture(const std::string& name) const
{
	if (m_pTextures.size() < 1)return nullptr;
	if (m_dictionary.find(name) == m_dictionary.end()) {
		return nullptr;
	}
	return GetTexture(m_dictionary.at(name));
}

Texture* TextureManager::GetTexture(int i) const
{
	return m_pTextures[i];
}

