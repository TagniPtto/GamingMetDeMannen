#pragma once
#include "Entity.h"
#include "utils.h"
#include "Character.h"

#include "SoundEffect.h"

#define TO_RADIAN(x) x * (3.1415f/180.f)
class Level;
class GameObject;
class Player:public Entity
{
public:
	Player();
	void Draw() const override;
	void Update(float elapsedSec) override;
	void LookAt(const Vector2f& target);

	void Attack();
	void Possess();
	void Possess(Character* newPossesion);
	Point2f GetPosition() const;
	void SetLevel(Level* level);
	const float viewingAngle{ TO_RADIAN(15.f) };

private:
	void HandleInput(float elapsedSec);

	Vector2f m_lookingDirection{0,0};

	SoundEffect* m_hoppinSound{ new SoundEffect{"HoppEffect.wav"}};

	Level* m_pcurrentLevel{nullptr};
	Character* m_pPossessedObject{nullptr};
};

