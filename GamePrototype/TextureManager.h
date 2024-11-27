#pragma once
#include <vector>
#include <map>

class Texture;
class TextureManager
{
public:
	TextureManager();
	~TextureManager();

	TextureManager(TextureManager const& other) = delete;
	TextureManager& operator=(TextureManager const& other) = delete;

	TextureManager(TextureManager&& other) noexcept;
	TextureManager& operator=(TextureManager&& other)noexcept;

	void AddTexture(const std::string& texturePath, const std::string& name);
	void DrawTexture(const std::string& name, const Rectf& dstRect, const Rectf& srcRect) const;
	void DrawTexture(int i, const Rectf& dstRect, const Rectf& srcRect)	const;
	Texture* GetTexture(const std::string& name) const;
	Texture* GetTexture(int i)	const;
private:
	std::map<std::string, int> m_dictionary;
	std::vector<Texture*> m_pTextures; 
};

