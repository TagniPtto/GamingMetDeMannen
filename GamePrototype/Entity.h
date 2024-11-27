#pragma once
#include "utils.h"

class Texture;
class Level;
class Entity
{

public:

	explicit Entity(Vector2f position = Vector2f{ 0.f,0.f }, Vector2f velocity = Vector2f{ 0.f,0.f }, float angle = 0,Texture* texture = nullptr);
	virtual ~Entity() = default;

	explicit Entity(const Entity& other) = default;
	Entity& operator=(const Entity& other) = default;

	explicit Entity(Entity&& other) = default;
	Entity& operator=(Entity&& other) = default;

	virtual void Draw() const;
	virtual void Update(float elapsedSec);
	virtual void Print() const;

	Vector2f GetPosition() const;
	void SetPosition(Vector2f position);
	Vector2f GetVelocity();
	void SetVelocity(const Vector2f& velocity);
	Vector2f GetForce();
	void SetForce(const Vector2f& force);
	float GetAngularVelocity() const;
	void SetAngularVelocity(float angVel);
	float GetAngle() const;
	void SetAngle(float angle);

	int GetHealth();
	void SetHealth(float health);
	float GetHitboxRadius();

	virtual void Attack(Entity& target, float damage);
	void KnockBack(Entity& target, float force);
	bool m_isActive{ true };

public:
	static Level* g_pLevel;
	float m_textureDispaySize{ 0.1f };


protected:
	float m_hitboxRadius{ 6 };
	float m_Health{ 10.f };
	bool m_IsAlive{ true };
	Vector2f m_DirectionVector{ 1,0 };

	
	Texture* m_pTexture{nullptr};

	//physics
	float m_angularVelocity{ 0 };
	float m_angle{0};
	Vector2f m_Force{};
	Vector2f m_Velocity{};
	Vector2f m_Position{};
};

