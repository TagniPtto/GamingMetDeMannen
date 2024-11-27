#include "pch.h"
#include "Oven.h"
#include "Player.h"
#include <iostream>

const std::vector<Projectile*>* Oven::g_projectileLibrary{ };
const std::vector<ShootingFunction>* Oven::g_shootingLibrary{ };

Oven::Oven(int maxAmmo, int startingAmmo, unsigned int projectileIndex, unsigned int shootingFncIndex, Texture* ta_oven, Texture* ta_muzzleFlash):
	m_maxAmmo(maxAmmo), m_currentAmmo(startingAmmo),
	m_pAnimationManager_MuzzleFlash(ta_muzzleFlash),
	m_pAnimationManager_Oven(ta_oven),
	m_projectileIndex(projectileIndex),
	m_shootingFunctionIndex(shootingFncIndex)
{
	float spriteWidth{ m_pAnimationManager_Oven.GetSpriteSheetWidth() / 4.f };
	float spriteHeight{ m_pAnimationManager_Oven.GetSpriteSheetHeight() / 2.0f };
	m_pAnimationManager_Oven.AddAnimation("Idle", Point2f{ 0,0 }, spriteWidth, spriteHeight, 4, 0.03f, Animation::AnimationMode::looped);
	m_pAnimationManager_Oven.AddAnimation("Shoot", Point2f{ 0,spriteHeight }, spriteWidth, spriteHeight, 4, 0.03f, Animation::AnimationMode::normal);


	spriteWidth = m_pAnimationManager_MuzzleFlash.GetSpriteSheetWidth() / 4.f;
	spriteHeight = m_pAnimationManager_MuzzleFlash.GetSpriteSheetHeight();
	m_pAnimationManager_MuzzleFlash.AddAnimation("main", Point2f{ 0,0 }, spriteWidth, spriteHeight, 4, 0.05f, Animation::AnimationMode::normal);
}


void Oven::Update(float elapsedSec)
{

	if (m_pAnimationManager_Oven.GetCurrentAnimation().m_EofAnimation) {
		m_pAnimationManager_Oven.SetAnimation("Idle");
	}
	m_pAnimationManager_Oven.Update(elapsedSec);
	m_pAnimationManager_MuzzleFlash.Update(elapsedSec);


	m_reloadTimer += elapsedSec;
	if (m_reloadTimer > m_reloadTime) {
		m_currentAmmo += 1.0f;
		m_reloadTimer = 0;
		if (m_currentAmmo > m_maxAmmo) {
			m_currentAmmo = m_maxAmmo;
		}
		std::cout << "Ammo : " << m_currentAmmo << std::endl;
	}

}

void Oven::Shoot(std::list<Projectile*>& projectiles, Player* parent)
{

	if (m_currentAmmo < 1) {
		return;
	}
	m_reloadTimer = 0;
	m_pAnimationManager_Oven.SetAnimation("Shoot");
	m_pAnimationManager_MuzzleFlash.SetAnimation("main");
	(*g_shootingLibrary)[m_shootingFunctionIndex]((*g_projectileLibrary)[m_projectileIndex% g_projectileLibrary->size()], projectiles, parent);

	switch (m_projectileIndex) {
		case 0:
			parent->GetSfxManager().Play("NormalShot1");
			--m_currentAmmo;
			break;
		case 1:
			parent->GetSfxManager().Play("NormalShot1");
			--m_currentAmmo;
			break;
		case 2:
			parent->GetSfxManager().Play("NormalShot1");
			m_currentAmmo -= 2.5f;
			break;
		case 3:
			parent->GetSfxManager().Play("flame");
			m_currentAmmo -= 0.05f;
			break;
		case 4:
			parent->GetSfxManager().Play("orb");
			m_currentAmmo -= 0.5f;
			break;
		case 5:
			parent->GetSfxManager().Play("orb");
			m_currentAmmo -= 0.5f;
			break;
	}

}

void Oven::Draw() const
{


	float width{ 50 }, height{ 50 };
	Rectf dst{ -width / 2.f,-height / 2.f ,width,height };


	m_pAnimationManager_Oven.Draw(dst);

	glPushMatrix(); {
		float muzzleFlash_size{ 30 };
		glTranslatef(0, muzzleFlash_size + 10, 0);

		glRotatef(90, 0.f, 0.f, 1.f);
		if (!m_pAnimationManager_MuzzleFlash.GetCurrentAnimation().m_EofAnimation) {
			m_pAnimationManager_MuzzleFlash.Draw(Rectf{ -muzzleFlash_size,-muzzleFlash_size,2 * muzzleFlash_size,2 * muzzleFlash_size });
		}
	}glPopMatrix();

	
}

void Oven::SetProjectile(unsigned int index)
{
	m_projectileIndex = index % g_projectileLibrary->size();
}
