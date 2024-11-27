#include "pch.h"
#include "Entity.h"

Entity::Entity():
	Entity(Vector2f(0.f,0.f), Vector2f(0.f, 0.f))
{

}

Entity::Entity(Vector2f position, Vector2f velocity):
	m_position(position),m_velocity(velocity),beingSus(false)
{

}

void Entity::Update(float elapsedSec)
{
	m_position += m_velocity*elapsedSec; 
	if (beingSus) {
		counter += elapsedSec;
		if (counter > 2.f) {
			beingSus = false;
			counter = 0.f;
		}
	}
}

void Entity::Draw() const
{
	const float radius{10.f};
	utils::FillEllipse(m_position.ToPoint2f(), radius, radius);
}

Point2f Entity::GetPosition() const
{
	return m_position.ToPoint2f();
}

bool Entity::IsSus() const
{
	return beingSus;
}
