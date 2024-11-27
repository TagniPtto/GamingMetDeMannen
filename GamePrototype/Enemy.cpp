#include "pch.h"
#include "Enemy.h"
#include "Player.h"

#include<iostream>

Player* Enemy::g_Target;
Texture* Enemy::g_basicEnemySpriteSheet;

Enemy::Enemy(Vector2f position, bool jumps, bool anticipates, float speed, float size, float health) :
	m_animationManager(g_basicEnemySpriteSheet), Entity::Entity(position),
	m_sfxManager(),
	m_speed(speed),
	m_size(size),
	m_anticipates(anticipates),
	m_jumps(jumps),
	m_jumpRange(float(std::rand() % 4))
{
	m_hitboxRadius += 4 + m_size;
	m_Health  = health;
	const float spriteWidth{ g_basicEnemySpriteSheet->GetWidth() / 4.0f };
	const float spriteHeight{ g_basicEnemySpriteSheet->GetHeight() / 5.0f };

	//m_sfxManager.AddSfx("sfx", );
	m_sfxManager.AddSfx("sfx/Teleport1.mp3","tp1");
	m_sfxManager.AddSfx("sfx/Teleport2.mp3","tp2");
	m_sfxManager.AddSfx("sfx/AllienDeath.mp3","a_d");
	m_sfxManager.SetVolume("tp1",MIX_MAX_VOLUME/4);
	m_sfxManager.SetVolume("tp2",MIX_MAX_VOLUME/4);
	m_sfxManager.SetVolume("a_d",MIX_MAX_VOLUME);
	m_animationManager.AddAnimation("Idle",		Point2f{0,0},					spriteWidth,spriteHeight,4,0.2f , Animation::AnimationMode::looped);
	m_animationManager.AddAnimation("Walk",		Point2f{ 0,1 * spriteHeight },		spriteWidth,spriteHeight,4, 0.1f, Animation::AnimationMode::looped);
	m_animationManager.AddAnimation("Attack",	Point2f{ 0,2 * spriteHeight },	spriteWidth,spriteHeight,4, 0.1f);
	m_animationManager.AddAnimation("Hurt",		Point2f{ 0,3 * spriteHeight },	spriteWidth,spriteHeight,4, 0.1f);
	m_animationManager.AddAnimation("Death",	Point2f{ 0,4* spriteHeight },	spriteWidth,spriteHeight,4, 0.1f);

	m_animationManager.SetAnimation("Walk");
}


void Enemy::Update(float elapsedSec)
{
	if (g_Target == nullptr) return;

	if (m_Health < 0.1f) {

		return;
	}

	if (utils::IsOverlapping(Circlef{ m_Position.ToPoint2f(),m_hitboxRadius }, Circlef{ g_Target->GetPosition().ToPoint2f(),g_Target->GetHitboxRadius() })) {
		m_Health -= 5;
		Attack(*g_Target, 1);
		if (m_Health <= 0) {
			m_animationManager.SetAnimation("Death");
		}
	}

	//if (m_animationManager.GetCurrentAnimationIndex() == m_animationManager.GetAnimationIndex("Death"))return;

	Vector2f directionVector{ m_Position.ToPoint2f(),g_Target->GetPosition().ToPoint2f() };

	if (m_anticipates) {
		directionVector = Vector2f{ m_Position.ToPoint2f(),g_Target->GetPosition().ToPoint2f() + g_Target->GetVelocity() * directionVector.Norm() * 0.005f };
	}

	if (m_jumps) {
		m_jumpTimer += elapsedSec;
		if (m_jumpTimer > m_jumpCooldown) {
			if (m_animationManager.GetCurrentAnimationIndex() != m_animationManager.GetAnimationIndex("Hurt")) {
				m_animationManager.SetAnimation("Hurt");
				m_sfxManager.Play("tp1");
			}
			else {
				if (m_animationManager.GetCurrentAnimation().m_EofAnimation) {
				
					float jump = utils::Clamp(100, 150.f, m_Velocity.Norm()* (0.6f + m_jumpRange));

					m_Position += m_Velocity.Normalized() * jump;
					m_jumpTimer = 0;
					m_animationManager.SetAnimation("Walk");
				}

			}
		}
	}

	
	m_Velocity = directionVector.Normalized() * m_speed;
	m_angle = -directionVector.AngleWith(Vector2f{ 1.0f,0.0f }) * 180.f / 3.14f + 180.f;
	Entity::Update(elapsedSec);
	m_Force = {};
	m_animationManager.Update(elapsedSec);
}

void Enemy::Draw() const
{
	float width{ 50 }, height{ 50 };
	Rectf dst{ -width / 2.f,-height / 2.f ,width,height };
	const float scale{0.7f + m_size};

	glPushMatrix(); {
		
		glTranslatef(m_Position.x, m_Position.y, 0.f);
		glRotatef(m_angle, 0.f, 0.f, 1.f);
		glScalef(scale, scale,1.0f);
		
		m_animationManager.Draw(dst);

	}glPopMatrix();
	Entity::Draw();
}

int Enemy::GetScore()
{
	const float jumpValue{2.0f};
	const float speedValue{1.5f};
	const float anticipationValue{1.2f};
	const float sizeValue{1.0};
	return int(int(m_jumps) * m_jumpRange* jumpValue + m_speed * speedValue + anticipationValue*int(m_anticipates) + sizeValue * int(m_size));
}
