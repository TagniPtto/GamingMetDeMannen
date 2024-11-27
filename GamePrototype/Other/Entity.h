#pragma once

#include "utils.h"


class Entity
{
public:
	explicit Entity();
	explicit Entity(Vector2f position, Vector2f velocity);
	virtual void Update(float elapsedSec);
	virtual void Draw()const;
	Point2f GetPosition() const;
	bool IsSus()const;

	Vector2f m_velocity;
	Vector2f m_position;
	float m_Health{20.f};

	bool isDead{ false };
	bool isPossesed{ false };
	bool beingSus;
protected:
	float counter{0};
	float m_torque;
	float hitBoxRadius;
	
};

