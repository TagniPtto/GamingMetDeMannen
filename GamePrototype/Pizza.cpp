#include "pch.h"

#include "Projectile.h"
#include "Pizza.h"
#include "PizzaSlice.h"

#include "Enemy.h"
#include "Texture.h"

Texture* Pizza::g_pPizzaTexture;

Pizza::Pizza(Vector2f position, Vector2f velocity, float angle, Entity* parent):
	Projectile::Projectile(position, velocity, angle,parent, g_pPizzaTexture)
{
	
	m_hitboxRadius += 4;
}

void Pizza::Update(float elapsedSec, std::list<Enemy*>& enemies, std::list<Projectile*>& projectiles)
{
	bool Split{ false };
	for (Enemy*& e : enemies) {
		if (utils::IsOverlapping(Circlef{ m_Position.ToPoint2f(),m_hitboxRadius }, Circlef{ e->GetPosition().ToPoint2f(),e->GetHitboxRadius() })) {
			if (!Split) {
				const float angle{ 3.14f / 4.0f };
				const float projectileSpeed{ 100 };
				const float offsetSpawn{ 10.f };
				for (int i{}; i < 8; i++) {
					const Vector2f direction{ cosf(angle * i),sinf(angle * i) };
					PizzaSlice* p{ new PizzaSlice(m_Position + direction * offsetSpawn,direction * projectileSpeed,i * (angle*180.f/3.14f),this) };
					p->SetAngularVelocity(angle*(180.f / 3.14f) * 4.0f);
					projectiles.push_back(p);
				}
				Split = true;
			}
			m_Health = 0;
			Attack(*e, 5);
		}

	}
	Entity::Update(elapsedSec);
}

void Pizza::Draw() const
{
	Entity::Draw();
}

Projectile* Pizza::Clone()const
{
	return new Pizza(*this);
}

float Pizza::GetProjectileSpeed()
{
	return 400.f;
}
