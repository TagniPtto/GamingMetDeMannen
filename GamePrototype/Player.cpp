#include "pch.h"
#include <iostream>
#include <list>

#include "utils.h"

#include "Player.h"

#include "Level.h"

#include "Projectile.h"
#include "Pizza.h"

Player::Player(Vector2f position, Vector2f velocity, Oven* oven) :
	Entity::Entity(position, velocity),
	m_pInputBuffer(5),
	m_pSfxManager(),
	g_pOven(oven)
{

	m_pSfxManager.AddSfx("sfx/OvenDoor.mp3","NormalShot1");
	m_pSfxManager.AddSfx("sfx/BowRelease.mp3","NormalShot2");
	m_pSfxManager.AddSfx("sfx/Cannon-shot.mp3","CannonShot");
	m_pSfxManager.AddSfx("sfx/FlameTrower.mp3","flame");
	m_pSfxManager.AddSfx("sfx/Orb2.mp3","orb");
	m_pSfxManager.SetVolume("NormalShot1",MIX_MAX_VOLUME/4);
	m_pSfxManager.SetVolume("NormalShot2",MIX_MAX_VOLUME/4);
	m_pSfxManager.SetVolume("CannonShot",MIX_MAX_VOLUME/4);
	m_pSfxManager.SetVolume("flame",MIX_MAX_VOLUME/8);
	m_pSfxManager.SetVolume("orb", MIX_MAX_VOLUME/4);

	
}
Player::~Player()
{
}
void Player::Update(float elapsedSec)
{

	m_pInputBuffer.Update(elapsedSec);
	g_pOven->Update(elapsedSec);
	Entity::Update(elapsedSec);

	if (g_pOven->m_shootingFunctionIndex == 1) {
		if (g_pLevel->leftMouse) {
			Shoot();
		}
	}
	
	if (m_Position.x < 0 or m_Position.x > g_pLevel->m_viewport.width) {
		m_Velocity = Vector2f{ -m_Velocity.x,m_Velocity.y };
	}
	if (m_Position.y < 0 or m_Position.y > g_pLevel->m_viewport.height) {
		m_Velocity = Vector2f{ m_Velocity.x,-m_Velocity.y };
	}
	m_Position = Vector2f{
		utils::Clamp(0, g_pLevel->m_viewport.width, m_Position.x),
		utils::Clamp(0, g_pLevel->m_viewport.height, m_Position.y)
	};
	HandleKeyBoardInput(elapsedSec);
	m_Velocity.x = utils::Clamp(-MaxMovementSpeed, MaxMovementSpeed, m_Velocity.x);
	m_Velocity.y = utils::Clamp(-MaxMovementSpeed, MaxMovementSpeed, m_Velocity.y);

}

void Player::Draw() const
{
	utils::SetColor(Color4f{1.0f,0.2f,0.1f,1.0f});
	utils::DrawLine(m_Position.ToPoint2f(), (m_Position + m_DirectionVector).ToPoint2f() , 1.0f);



	const float scale{1.2f};
	glPushMatrix(); {

		glTranslatef(m_Position.x, m_Position.y, 0.f);
		glRotatef(m_angle ,0.f,0.f,1.f);
		glScalef(scale, scale, 1.0f);
		g_pOven->Draw();

	}glPopMatrix();

}

void Player::HandleKeyBoardInput(float elapsedSec)
{

	//std::cout << "angle: " << m_angle << std::endl;

	if (int((m_pInputBuffer)[0]) & int(Input::ARROW_DOWN)) {
		m_Velocity.y -= MaxMovementSpeed * acceleration * elapsedSec;
	}
	if (int((m_pInputBuffer)[0]) & int(Input::ARROW_UP)) {
		m_Velocity.y += MaxMovementSpeed * acceleration * elapsedSec;
	}
	if (int((m_pInputBuffer)[0]) & int(Input::ARROW_LEFT)) {
		m_Velocity.x -= MaxMovementSpeed * acceleration * elapsedSec;
	}
	if (int((m_pInputBuffer)[0]) & int(Input::ARROW_RIGHT)) {
		m_Velocity.x += MaxMovementSpeed * acceleration * elapsedSec;
	}

	if (int((m_pInputBuffer)[0]) == int(Input::EMPTY)) {
		
	}

	const Uint8* keyboard_state_array = SDL_GetKeyboardState(NULL);
	if (keyboard_state_array[SDL_SCANCODE_1]) {
		g_pOven->m_shootingFunctionIndex = 0;
		g_pOven->SetProjectile(0);
	}
	if (keyboard_state_array[SDL_SCANCODE_2]) {
		g_pOven->m_shootingFunctionIndex = 0;
		g_pOven->SetProjectile(1);

	}
	if (keyboard_state_array[SDL_SCANCODE_3]) {
		g_pOven->m_shootingFunctionIndex = 0;
		g_pOven->SetProjectile(2);
	}
	if (keyboard_state_array[SDL_SCANCODE_4]) {
		g_pOven->m_shootingFunctionIndex = 1;
		g_pOven->SetProjectile(3);
	}
	if (keyboard_state_array[SDL_SCANCODE_5]) {
		g_pOven->m_shootingFunctionIndex = 0;
		g_pOven->SetProjectile(4);
	}
	if (keyboard_state_array[SDL_SCANCODE_6]) {
		g_pOven->m_shootingFunctionIndex = 0;
		g_pOven->SetProjectile(5);
	}
	m_Velocity *= 0.99f;
}

void Player::HandleMouseInput()
{
	directionVector = Vector2f{ m_Position.ToPoint2f() , Point2f{Level::mouseX,Level::mouseY} };
	//std::cout << "directionVector: " << directionVector  << std::endl;
	m_angle = -directionVector.AngleWith(Vector2f{ 1.0f,0.0f }) * 180.f / 3.14f - 90;

}

void Player::Shoot()
{
	const float knockBackPercentage{ 0.1f };
	//m_pSfxManager.Play("CannonShot");

	g_pOven->Shoot(g_pLevel->GetProjectiles(),this);
	//
}

const SfxManager& Player::GetSfxManager()
{
	return m_pSfxManager;
}




