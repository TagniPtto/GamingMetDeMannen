#include "pch.h"
#include "Player.h"
#include "GameObject.h"
#include "Character.h"
#include <iostream>

const static float length{ 150.f };
Player::Player():
	m_pPossessedObject(nullptr), m_lookingDirection(Vector2f{})
{
	if (m_hoppinSound == nullptr) {
		std::cout << "failed to load hopping noise" << std::endl;
	}
}

void Player::Draw() const
{
	utils::SetColor(Color4f{1.f,0.2f,0.2f,1.f});
	const float width{10.f};
	const float height{10.f};
	
	utils::DrawRect(Rectf{m_position.x-width/2.f,m_position.y - height / 2.f ,width ,height });

	const float directionAngle{- m_lookingDirection.AngleWith(Vector2f{1.0f,0.0f}) };


	utils::DrawLine(m_position.ToPoint2f(), (m_position + m_lookingDirection).ToPoint2f());
	utils::FillArc(m_position.ToPoint2f(), length,length, directionAngle - viewingAngle / 2.f, directionAngle + viewingAngle / 2.f);
	
	//utils::SetColor(Color4f{0.f,0.8f,0.2f,1.f});
	//for (Character* c : m_pcurrentLevel->m_objects) {
	//	if (c != m_pPossessedObject) {
	//		Vector2f v{ m_position.ToPoint2f(),c->GetPosition() };

	//		utils::DrawLine(m_position.ToPoint2f(), c->GetPosition());
	//		float characterAngle{ -v.AngleWith(Vector2f{ 1.0f,0.0f }) };
	//		const float directionAngle{ -m_lookingDirection.AngleWith(Vector2f{1.0f,0.0f}) };
	//		utils::FillArc(m_position.ToPoint2f(), 100, 100, characterAngle- viewingAngle/2.f,characterAngle + viewingAngle / 2.f);
	//		if ((std::abs(characterAngle - directionAngle) < viewingAngle) && (v.Length() < length)) {

	//		}
	//	}

	//}
}

void Player::Update(float elapsedSec)
{
	const float frictionCoefficient{1.0f};
	HandleInput(elapsedSec);
	if (m_pPossessedObject == nullptr) {
		m_velocity *= (1 - elapsedSec) * frictionCoefficient;
	
		Entity::Update(elapsedSec);
	}
	else {
		m_pPossessedObject->m_velocity *= (1 - elapsedSec) * frictionCoefficient;
		m_position = Vector2f{ m_pPossessedObject->GetPosition() };
	}

}

void Player::LookAt(const Vector2f& target)
{
	m_lookingDirection = target - m_position;
}

void Player::Attack()
{
	if (m_pPossessedObject == nullptr) {
		Possess();
	}
	else {
		m_pPossessedObject->beingSus = true;
	}
}

void Player::Possess()
{
	if (m_pcurrentLevel == nullptr)return;
	for (Character* c : m_pcurrentLevel->m_objects) {
		if (c != m_pPossessedObject) {
			Vector2f v{ m_position.ToPoint2f(),c->GetPosition() };
			float characterAngle{ -v.AngleWith(Vector2f{ 1.0f,0.0f }) };
			const float directionAngle{ -m_lookingDirection.AngleWith(Vector2f{1.0f,0.0f}) };
			if ((std::abs(characterAngle - directionAngle) < viewingAngle) && (v.Length() < length)) {
				if (m_pPossessedObject != nullptr) {
				m_pPossessedObject->isPossesed = false;
				
				}
				if (!c->isDead) {
					m_hoppinSound->Play(false);
					Possess(c);
					c->beingSus = true;
					c->isPossesed = true;
					c->isDead = false;
				}

			}
		}

	}

}

void Player::Possess(Character* newPossesion)
{
	m_pPossessedObject = newPossesion;
}

Point2f Player::GetPosition() const
{
	return m_position.ToPoint2f();
}

void Player::SetLevel(Level* level)
{
	m_pcurrentLevel = level;
}

void Player::HandleInput(float elapsedSec)
{

	const Uint32 mouseState{ SDL_GetMouseState(NULL, NULL) };
	 const Uint8* keyboardState =  SDL_GetKeyboardState(NULL);
	 const float movSpeed{200.f};
	 if (m_pPossessedObject == nullptr) {
		 if (keyboardState[SDL_SCANCODE_UP]) {
			 m_velocity.y += movSpeed * elapsedSec;
		 }
		 if (keyboardState[SDL_SCANCODE_DOWN]) {
			 m_velocity.y -= movSpeed * elapsedSec;
		 }
		 if (keyboardState[SDL_SCANCODE_LEFT]) {

			 m_velocity.x -= movSpeed * elapsedSec;
		 }
		 if (keyboardState[SDL_SCANCODE_RIGHT]) {
			 m_velocity.x += movSpeed * elapsedSec;
		 }
	 }
	 else {
		 if (keyboardState[SDL_SCANCODE_UP]) {
			 m_pPossessedObject->m_velocity.y += movSpeed * elapsedSec;
		 }
		 if (keyboardState[SDL_SCANCODE_DOWN]) {
			 m_pPossessedObject->m_velocity.y -= movSpeed * elapsedSec;
		 }
		 if (keyboardState[SDL_SCANCODE_LEFT]) {

			 m_pPossessedObject->m_velocity.x -= movSpeed * elapsedSec;
		 }
		 if (keyboardState[SDL_SCANCODE_RIGHT]) {
			 m_pPossessedObject->m_velocity.x += movSpeed * elapsedSec;
		 }
	 }
	 //if (SDL_BUTTON(mouseState)==1) {

	 //}
	 //if (SDL_BUTTON(mouseState) == 3) {

	 //}
	 //int mouseX;
	 //int MouseY;
	 //SDL_GetMouseState(&mouseX, &MouseY);

	 //std::cout << Vector2f{ float(mouseX), float(MouseY) } << std::endl;
	 //m_lookingDirection = Vector2f{ float(mouseX),float(MouseY )} - m_position;
}
