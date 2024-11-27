#pragma once

#include "utils.h"


class Texture;

class Animation {

public:
	enum class AnimationMode {
		normal,
		looped,
		loopedBackAndForward,
	};
	Animation(Point2f position, float width, float height, float frame_duration, int frame_count, AnimationMode mode, Texture* animationSheet);
	~Animation()noexcept = default;

	friend std::ostream& operator<<(std::ostream& out, const Animation& anim);
	Animation(const Animation& other) noexcept = default;
	Animation& operator=(const Animation& other)noexcept = default;

	Animation(Animation&& other)noexcept = default;
	Animation& operator=(Animation&& other)noexcept = default;

	void Update(float elapsedSec);
	void Draw(const Rectf& dstRect , int offset = 0)const;
	void Reset();
	AnimationMode GetAnimationMode()const;

	bool m_EofAnimation{ false };
private:

	const AnimationMode m_mode;
	Point2f m_position;
	float m_width;
	float m_height;
	int m_frameCount;


	int m_animationIncrement{ 1 };
	int m_currentFrameIndex;
	float m_currentFrame_displayTime;
	const float m_FRAME_DURATION;

	Texture* m_pAnimationSheet;
};
std::ostream& operator<<(std::ostream& out, const Animation& anim);