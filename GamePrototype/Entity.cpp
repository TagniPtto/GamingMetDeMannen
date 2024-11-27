#include "pch.h"
#include "Entity.h"
#include <iostream>

#include "Texture.h"

Level* Entity::g_pLevel = nullptr;

Entity::Entity(Vector2f position, Vector2f velocity,float angle, Texture* texture) :
	m_Position(position), m_Velocity(velocity) , m_pTexture(texture), m_angle(angle)
{
	
}

void Entity::Draw() const
{
	glPushMatrix(); {

		glTranslatef(m_Position.x, m_Position.y, 0.f);
		glRotatef(m_angle, 0.f, 0.f, 1.f);



		if (m_pTexture != nullptr) {
			float width{ m_pTexture->GetWidth() * m_textureDispaySize };
			float height{ m_pTexture->GetHeight() * m_textureDispaySize };
			Rectf dst{ -width / 2.f , -height / 2.f ,width,height };
			m_pTexture->Draw(dst);
		}
		else {
			utils::DrawEllipse(Ellipsef{ {},m_hitboxRadius,m_hitboxRadius });
		}

	}glPopMatrix();


	
}

void Entity::Update(float elapsedSec)
{
	if (!m_isActive)return;
	m_Velocity += m_Force * elapsedSec;
	m_Position += m_Velocity * elapsedSec;
	m_angle += m_angularVelocity * elapsedSec;
}

void Entity::Print() const
{
	std::cout << "Entity : " << std::endl;
	std::cout << "Position"<<" [ "<<m_Position << " ] " << std::endl;
	std::cout << "Velocity"<<" [ "<<m_Velocity << " ] " << std::endl;
	std::cout << "hitboxRadius"<<" [ "<<m_hitboxRadius << " ] " << std::endl;
}

Vector2f Entity::GetPosition() const
{
	return m_Position;
}

void Entity::SetPosition(Vector2f position)
{
	m_Position = position;
}


Vector2f Entity::GetVelocity()
{
	return m_Velocity;
}

void Entity::SetVelocity(const Vector2f& velocity)
{
	m_Velocity = velocity;
}

Vector2f Entity::GetForce()
{
	return m_Force;
}

void Entity::SetForce(const Vector2f& force)
{
	m_Force = force;
}

float Entity::GetAngularVelocity() const
{
	return m_angularVelocity;
}

void Entity::SetAngularVelocity(float angVel)
{
	m_angularVelocity = angVel;
}

float Entity::GetAngle() const
{
	return m_angle;
}

void Entity::SetAngle(float angle)
{
	m_angle = angle;
}


void Entity::Attack(Entity& target, float damage)
{
	target.m_Health -= damage;
}

void Entity::KnockBack(Entity& target, float force)
{
	target.m_Velocity += Vector2f{m_Position.ToPoint2f(),target.m_Position.ToPoint2f() }.Normalized() * force;
}

int Entity::GetHealth()
{
	return m_Health;
}

void Entity::SetHealth(float health)
{
	m_Health = health;
}

float Entity::GetHitboxRadius()
{
	return m_hitboxRadius;
}


