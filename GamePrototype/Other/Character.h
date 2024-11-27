#pragma once

#include "Entity.h"
#include "Level.h"
class Character:public Entity
{
public:
	Character();
	Character(Vector2f position , Vector2f velocity);
	void ScanSurrounding(const std::vector<Character*>& surrounding);
	void Update(float elapsedSec) override;
	void Draw()const override;
	void SetLevel(Level* level);
	void Attack(float elapsedSec);
	void BorderCollision();

	Level* m_level;
	Entity* m_target;
	float m_movSPeed{ 80.f };
	const float m_hitBoxRadius{ 20.f };
	Color4f m_color{ Color4f{ .5f, .5f, .5f, 1.f } };
};

