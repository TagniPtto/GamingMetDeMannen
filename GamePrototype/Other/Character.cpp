#include "pch.h"
#include "Character.h"

Character::Character(Vector2f position, Vector2f velocity):
	Entity::Entity(position,velocity),m_target(nullptr)
{

}

void Character::ScanSurrounding(const std::vector<Character*>& surrounding)
{
	const float radius{ 120.f };
	for (Character* pe: surrounding) {
		if (pe != this) {
			if (Vector2f(m_position.ToPoint2f(), pe->GetPosition()).Length() < radius) {
				if (pe->IsSus()) {
					if (!pe->isDead) {
						m_target = pe;
						return;
					}
				}
			}
		}

	}

}

void Character::Update(float elapsedSec)
{
	if (m_target == nullptr) {
		m_color = Color4f{ .5f, .5f, .5f, 1.f };
	}
	else {
		m_color = Color4f{ .5f, .8f, .1f, 1.f };
		if (m_target->isDead) {
			m_target = nullptr;
		}
	}
	if (isDead) {
		return;
	}

	if (m_Health < 0) {
		isDead = true;
	}

	if (isPossesed) {
		Entity::Update(elapsedSec);
		return;
	}
	if (m_target == nullptr) {
		Entity::Update(elapsedSec);
		ScanSurrounding(m_level->m_objects);
	}
	else {
		if (m_target->isDead) {
			m_target = nullptr;
			return;
		}
		const float attackRange{ 20 };
		if (Vector2f{ m_position.ToPoint2f(),m_target->m_position.ToPoint2f() }.Length() < attackRange) {
			Attack(elapsedSec);
			m_color = Color4f{ 1.f, .0f, .1f, 1.f };
		}
		m_position += (m_target->GetPosition() - this->GetPosition()).Normalized()*m_movSPeed*elapsedSec;
	}

	BorderCollision();

}

void Character::Draw() const
{
	if (isDead) {
		return;
	}

	utils::SetColor(m_color);
	Ellipsef e{m_position.ToPoint2f(),m_hitBoxRadius,m_hitBoxRadius };
	utils::DrawEllipse(e);

}

void Character::SetLevel(Level* level)
{
	m_level = level;
}

void Character::Attack(float elapsedSec)
{

	const float attackDamage{ 5.f};
	m_target->m_Health -= attackDamage* elapsedSec;


}

void Character::BorderCollision()
{
	Rectf border{ m_level->GetBorder() };
	const float epsilon{0.01f};
	if ((m_position.x - border.left) > border.width) {
		m_position.x = border.left +  border.width - epsilon;
		m_velocity.x *= -1;
	}
	if ((m_position.x - border.left) < 0) {
		m_position.x = border.left + epsilon;
		m_velocity.x *= -1;
	}
	if ((m_position.y - border.bottom) > border.height) {
		m_velocity.y *= -1;
		m_position.y = border.bottom + border.height - epsilon;
	}
	if ((m_position.y - border.bottom) < 0) {
		m_velocity.y *= -1;
		m_position.y = border.bottom + epsilon;
	}
}
