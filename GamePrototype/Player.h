#pragma once

#include "Entity.h"
#include "Texture.h"


#include "InputBuffer.h"
#include "AnimationManager.h"
#include "SfxManager.h"
#include "Oven.h"

#include <list>

class Projectile;
class Level;


class Player : public Entity
{
	enum class ProjectileType {
		none,
		Bread,
		Cake,
		Pizza,
	};
public:


	Player(Vector2f position = {}, Vector2f velocity = {} , Oven* oven = nullptr);
	~Player();
	void Update(float elapsedSec) override;
	
	void Draw() const override;
	void HandleKeyBoardInput(float elapsedSec);
	void HandleMouseInput();

	void Shoot();
	const SfxManager& GetSfxManager();

	bool isInvincible{false};
private:

	const float MaxMovementSpeed{450 };
	const float acceleration{ 0.9f };
	float m_elapsedSec{};

	Vector2f directionVector;
	SfxManager m_pSfxManager;
	InputBuffer m_pInputBuffer;

	bool m_isShielded{false};

public:
	Oven* g_pOven;

};

