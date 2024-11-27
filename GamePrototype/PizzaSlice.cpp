#include "pch.h"
#include "PizzaSlice.h"
#include "Enemy.h"


Texture* PizzaSlice::g_pPizzaSliceTexture;

PizzaSlice::PizzaSlice(Vector2f position, Vector2f velocity, float angle, Entity* parent) :
	Projectile::Projectile(position, velocity.Normalized()* GetProjectileSpeed(), angle, parent, g_pPizzaSliceTexture)
{

}

void PizzaSlice::Update(float elapsedSec, std::list<Enemy*>& enemies, std::list<Projectile*>& projectiles)
{
	Entity::Update(elapsedSec);
	for (Enemy*& e : enemies) {
		if (utils::IsOverlapping(Circlef{ m_Position.ToPoint2f(),m_hitboxRadius }, Circlef{ e->GetPosition().ToPoint2f(),e->GetHitboxRadius() })) {
			m_Health = 0;
			Attack(*e, 10);
		}
	}
}

void PizzaSlice::Draw() const
{
	Entity::Draw();
}

Projectile* PizzaSlice::Clone()const
{
	return new PizzaSlice(*this);
}

float PizzaSlice::GetProjectileSpeed()
{
	return 600;
}
