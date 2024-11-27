#include "pch.h"
#include "Projectile.h"

#include "Texture.h"
#include "Enemy.h"

Texture* Projectile::g_pBasicProjectileTexture;

const float Projectile::maxLifeTime = 0.9f;
const float Projectile::minLifeTime = 0.2f;

Projectile::Projectile(Vector2f position, Vector2f velocity, float angle, Entity* parent, Texture* txt):
	Entity::Entity(position, velocity , angle, txt),m_pParent(parent),
	m_lifeTime((std::rand() % int((maxLifeTime - minLifeTime) * 10)) * 0.1f + minLifeTime),
	m_timer(0)
{
	m_hitboxRadius += 4;
}

void Projectile::Update(float elapsedSec, std::list<Enemy*>& enemies, std::list<Projectile*>& projectiles)
{

	Entity::Update(elapsedSec);
	for (Enemy*& e: enemies) {
		if (utils::IsOverlapping(Circlef{m_Position.ToPoint2f(),m_hitboxRadius}, Circlef{ e->GetPosition().ToPoint2f(),e->GetHitboxRadius()})) {
			m_Health = 0;
			Attack(*e,5);
		}
	}
	//TODO: check if overlapping with enemys
}

void Projectile::Draw() const
{
	Entity::Draw();
}

void Projectile::SetParent(Entity* parent)
{
	m_pParent = parent;
}

float Projectile::GetProjectileSpeed() {
	return 500.f;
}
void Projectile::SetLifeTime(float time)
{
	m_lifeTime = time;
}
float Projectile::logDeclineFunction(float coeff, float x, float A )
{
	return A* exp(coeff * log(-(x - 1)));
}
Projectile* Projectile::Clone() const
{
	return new Projectile(*this);
}
