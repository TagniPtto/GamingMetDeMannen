#pragma once


#include <list>
#include "AnimationManager.h"
#include "Projectile.h"
class Player;

typedef void (*ShootingFunction)(Projectile* prjc, std::list<Projectile*>& projectileList , Player* parent);

class Oven
{

public:

	float m_reloadTime{0.6f};
	float m_reloadTimer{0.f};

	float m_projectileEjectionForce{500};

	const float m_maxAmmo;
	float m_currentAmmo;
	std::string m_shootSfx;

	static const std::vector<Projectile*>* g_projectileLibrary;
	static const std::vector<ShootingFunction>* g_shootingLibrary;


	unsigned int m_projectileIndex;
	unsigned int m_shootingFunctionIndex;
	AnimationManager m_pAnimationManager_Oven;
	AnimationManager m_pAnimationManager_MuzzleFlash;



public:
	Oven(int maxAmmo, int startingAmmo,unsigned int projectileIndex,unsigned int shootingFncIndex, Texture* ta_oven, Texture* ta_muzzleFlash);
	void Update(float elapsedSec);
	void Shoot(std::list<Projectile*>& projectiles, Player* parent);
	void Draw() const;
	void SetProjectile(unsigned int index);
};

