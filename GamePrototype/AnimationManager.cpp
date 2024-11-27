#include "pch.h"
#include "AnimationManager.h"
#include "Texture.h"
#include <iostream>


AnimationManager::AnimationManager(const std::string& texturePath) :
	m_pAnimationSheet(new Texture(texturePath)), m_currentAnimation(0)
{

}

AnimationManager::AnimationManager(Texture* texture):
	m_pAnimationSheet(texture), m_currentAnimation(0)
{
}


AnimationManager::~AnimationManager()
{
	m_pAnimationSheet = nullptr;
	for (Animation*& pAnimation : m_pAnimations) {
		delete pAnimation;
		pAnimation = nullptr;
	}
}

AnimationManager::AnimationManager(const AnimationManager& other):
	m_animationDict(other.m_animationDict), m_currentAnimation(other.m_currentAnimation) ,m_pAnimationSheet(other.m_pAnimationSheet)
{
	m_pAnimations.clear();
	for (Animation* anim: other.m_pAnimations) {
		m_pAnimations.push_back(new Animation(*anim));
	}

}

AnimationManager& AnimationManager::operator=(const AnimationManager& other)
{
	m_animationDict = other.m_animationDict;
	m_currentAnimation = other.m_currentAnimation;

	for (Animation*& anim : m_pAnimations) {
		delete anim;
		anim = nullptr;
	}
	m_pAnimations.clear();
	for (Animation* anim : other.m_pAnimations) {
		m_pAnimations.push_back(new Animation(*anim));
	}
	return *this;
}

AnimationManager::AnimationManager(AnimationManager&& other) noexcept :
	m_pAnimations(std::move(other.m_pAnimations)), m_animationDict(std::move(other.m_animationDict)),
	m_pAnimationSheet(std::move(other.m_pAnimationSheet)), m_currentAnimation(std::move(other.m_currentAnimation))
{
	other.m_pAnimationSheet = nullptr;
}

AnimationManager& AnimationManager::operator=(AnimationManager&& other)noexcept
{
	m_pAnimations = std::move(other.m_pAnimations);
	m_animationDict = std::move(other.m_animationDict);
	m_pAnimationSheet = other.m_pAnimationSheet;
	other.m_pAnimationSheet = nullptr;


	return *this;
}

void AnimationManager::AddAnimation(const std::string& name, Point2f loc, float width, float height, int frameCount, float frameDuration, Animation::AnimationMode mode)
{
	if (m_pAnimationSheet == nullptr) {
		std::cout << " no SpriteSheet" << std::endl;
	}
	m_pAnimations.push_back(new Animation(loc, width, height, frameDuration, frameCount, mode, m_pAnimationSheet));
	m_animationDict.insert({name,m_animationDict.size()});
}

void AnimationManager::Draw(const Rectf& dstRect, int offset) const
{
	if (m_currentAnimation >= m_pAnimations.size()) {
		return;
	}
	m_pAnimations[m_currentAnimation]->Draw(dstRect,offset);
}

void AnimationManager::Update(float elapsedSec)
{
	if (m_currentAnimation >= m_pAnimations.size()) {
		return; 
	}
	m_pAnimations[m_currentAnimation]->Update(elapsedSec);
}

float AnimationManager::GetSpriteSheetHeight() const
{
	return m_pAnimationSheet->GetHeight();
}

float AnimationManager::GetSpriteSheetWidth() const
{
	return m_pAnimationSheet->GetWidth();
}

bool AnimationManager::SetAnimation(int animationIndex) {
	if (animationIndex > m_pAnimations.size()){
		return false;
	}
	m_currentAnimation = animationIndex;
	m_pAnimations[m_currentAnimation]->Reset();
	return false;
}
bool AnimationManager::SetAnimation(const std::string& animationName) {

	return SetAnimation(GetAnimationIndex(animationName));
}
const Animation& AnimationManager::GetCurrentAnimation() const {
	return *m_pAnimations[m_currentAnimation];
}
int AnimationManager::GetCurrentAnimationIndex() const
{
	return m_currentAnimation;
}
int AnimationManager::GetAnimationIndex(const std::string& animationName) const
{
	if (m_animationDict.find(animationName) == m_animationDict.end()) {
		return -1;
	}
	return m_animationDict.at(animationName);
}

