#pragma once
#include "utils.h"
#include "Animation.h"
#include "TextureManager.h"
#include <map>

class Texture;

class AnimationManager
{
public:

	explicit AnimationManager(const std::string& texturePath);
	explicit AnimationManager(Texture* texture);
	~AnimationManager();

	AnimationManager(const AnimationManager& other);
	AnimationManager& operator=(const AnimationManager& other);

	AnimationManager(AnimationManager&& other) noexcept;
	AnimationManager& operator=(AnimationManager&& other) noexcept;

	
	void Draw(const Rectf& dstRect , int offset = 0) const;
	void Update(float elapsedSec);

	void AddAnimation(const std::string& name, Point2f loc, float width, float height, int frameCount, float frameDuration, Animation::AnimationMode mode = Animation::AnimationMode::normal);
	bool SetAnimation(int animationIndex);
	bool SetAnimation(const std::string& animationName);

	float GetSpriteSheetHeight()const;
	float GetSpriteSheetWidth()const;
	const Animation& GetCurrentAnimation() const;
	int GetCurrentAnimationIndex()const;
	int GetAnimationIndex(const std::string& animationName)const;

private:

	int m_currentAnimation;

	Texture* m_pAnimationSheet;
	std::map<std::string, int> m_animationDict;
	std::vector<Animation*> m_pAnimations;
};

