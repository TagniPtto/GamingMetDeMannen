
#include "pch.h"
#include "Animation.h"
#include "Texture.h"
#include <iostream>


Animation::Animation(Point2f position, float width, float height, float frame_duration, int frame_count, AnimationMode mode, Texture* animationSheet) :
	m_position(position), m_width(width), m_height(height), m_FRAME_DURATION(frame_duration),
	m_currentFrameIndex(0), m_currentFrame_displayTime(0.f), m_frameCount(frame_count), m_mode(mode), m_pAnimationSheet(animationSheet)
{
}

void Animation::Draw(const Rectf& dstRect , int offset) const {
	const float verticalSpacing{};
	const float horizontalSpacing{};


	float posX{ m_position.x + ((m_currentFrameIndex + offset) % m_frameCount) * (m_width + horizontalSpacing) };
	float posY{ m_position.y + int(posX / m_pAnimationSheet->GetWidth()) * (m_height + verticalSpacing) };
	Rectf srcRect{
		 float(int(posX) % int(m_pAnimationSheet->GetWidth()))
		,float(int(posY) % int(m_pAnimationSheet->GetHeight()))
		,m_width,m_height };
	m_pAnimationSheet->Draw(dstRect, srcRect);
}


void Animation::Reset() {
	m_currentFrameIndex = 0;
	m_EofAnimation = false;
	m_animationIncrement = 1;
	m_currentFrame_displayTime = 0;
}
void Animation::Update(float elapsedSec)
{
	m_currentFrame_displayTime += elapsedSec;
	if (m_currentFrame_displayTime >= m_FRAME_DURATION) {
		m_currentFrame_displayTime = 0;

		m_currentFrameIndex += m_animationIncrement;
		switch (m_mode) {
		case AnimationMode::looped:
			m_currentFrameIndex %= m_frameCount;
			break;
		case AnimationMode::loopedBackAndForward:
			if ((m_currentFrameIndex) >= (m_frameCount - 1) || (m_currentFrameIndex) <= 0) {
				m_animationIncrement *= -1;
			}
			break;
		case AnimationMode::normal:
			if (m_currentFrameIndex >= m_frameCount - 1.f) {
				m_EofAnimation = true;
			}
			m_currentFrameIndex = int(std::min(m_frameCount - 1.f, float(m_currentFrameIndex)));
			break;
		}

		//std::cout << "frameIndex : " << m_currentFrameIndex << std::endl;
	}
}
Animation::AnimationMode Animation::GetAnimationMode() const
{
	return m_mode;
}

std::ostream& operator<<(std::ostream& out, const Animation& anim)
{
	out << anim.m_currentFrameIndex << std::endl;
	return out;
}
