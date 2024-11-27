#include "pch.h"
#include "FireParticle.h"
#include "Enemy.h"

#include <iostream>



FireParticle::FireParticle(Vector2f position, Vector2f velocity, float angle, Entity* parent):
	Projectile::Projectile( position,  velocity,  angle,  parent),
	m_form(std::rand()% 3)
{
	
}

void FireParticle::Update(float elapsedSec, std::list<Enemy*>& enemies, std::list<Projectile*>& projectiles)
{
	m_timer += elapsedSec;
	if (m_timer > m_lifeTime) {
		m_Health = 0;
	}
	const float maxDisplaySize{15};
	const float offset{ 1.f };
	const float x{((3*m_timer/m_lifeTime) - 1) };
	m_textureDispaySize = maxDisplaySize * std::abs(float(expf(-2*(x * x))));
	m_hitboxRadius = m_textureDispaySize;


	for (Enemy*& e : enemies) {
		if (utils::IsOverlapping(Circlef{ m_Position.ToPoint2f(),m_hitboxRadius }, Circlef{ e->GetPosition().ToPoint2f(),e->GetHitboxRadius() })) {
			m_timer += elapsedSec;
			Attack(*e, 2.f * logDeclineFunction(5.f/6.f,m_timer/m_lifeTime) * elapsedSec);
		}
	}
	Entity::Update(elapsedSec);
}

void FireParticle::Draw() const
{
	glPushMatrix(); {

		glTranslatef(m_Position.x, m_Position.y, 0.f);
		glRotatef(m_angle, 0.f, 0.f, 1.f);
		const float percentage{ m_timer / m_lifeTime };
		const float rPercentage{ m_timer / m_lifeTime };
		const float lightness{ logDeclineFunction(1.f/5.f,percentage)};
		const float alpha{ logDeclineFunction(1.f / 4.f,percentage) };

		const float r{ 255.f };
		const float g{ logDeclineFunction(1.f / 2.f,percentage) * 240.f };
		const float b{ rPercentage * 66.f };

		Color4f c{	(lightness * r) / 255.f,
					(lightness * g) / 255.f,
					(lightness * b) / 255.f, alpha };
		
		utils::SetColor(c);
		Point2f p1{ cosf(0) * m_textureDispaySize, sinf(0)* m_textureDispaySize };
		Point2f p5{ cosf(120.f * 3.14f/180.f) * m_textureDispaySize, sinf(120.f * 3.14f / 180.f)* m_textureDispaySize };
		Point2f p6{ cosf(240.f * 3.14f / 180.f) * m_textureDispaySize, sinf(240.f * 3.14f / 180.f)* m_textureDispaySize };
		

		switch (m_form) {
			case 0:
				utils::FillTriangle(p1, p5, p6);
				break;
			case 1:
				utils::FillEllipse(Ellipsef{ {},m_textureDispaySize,m_textureDispaySize });
				break;
			default:
				utils::FillRect(Rectf{ -m_textureDispaySize / 2.f ,-m_textureDispaySize / 2.f ,m_textureDispaySize ,m_textureDispaySize });
				break;
		}

	}glPopMatrix();
	
}

Projectile* FireParticle::Clone() const
{
	FireParticle* p{ new FireParticle(*this) };
	p->m_form = std::rand() % m_formcount;
	return p;
}
