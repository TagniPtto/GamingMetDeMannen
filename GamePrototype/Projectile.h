#pragma once
#include "Entity.h"
#include <list>


class Enemy;
class Projectile : public Entity
{
public:
	static const float maxLifeTime;
	static const float minLifeTime;
	static Texture* g_pBasicProjectileTexture;

	Entity* m_pParent{nullptr};
	float m_lifeTime;
	float m_timer;

public:
	Projectile(Vector2f position,Vector2f velocity,float angle, Entity* parent = nullptr, Texture* txt = nullptr);
	Projectile(const Projectile& other) = default;

	virtual void Update(float elapsedSec, std::list<Enemy*>& enemies, std::list<Projectile*>& projectiles);
	virtual void Draw()const override;
	void SetParent(Entity* parent);
	void SetLifeTime(float time);

	static float logDeclineFunction(float coeff, float x, float A = 1.0f);

	virtual float GetProjectileSpeed();
	virtual Projectile* Clone() const;
};


