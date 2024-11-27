#include "pch.h"
#include "Camera.h"
#include <iostream>

Camera::Camera(float width, float heigth,Point2f position , float rotation , float zoom):
	m_Width(width),m_Height(heigth), m_Position(position), m_Rotation(rotation), m_Zoom(zoom)
{

}

Camera::~Camera()
{
}

void Camera::Translate(Vector2f translation)
{
	m_Position += translation;
}

void Camera::Rotate(float degrees)
{
	m_Rotation += degrees;
}

void Camera::Zoom(float scaler)
{
	m_Zoom += scaler;
}

void Camera::Update(float elapsedSec)
{
	HandleInput();
}

void Camera::HandleInput()
{
	const Uint8* keyboard_state_array = SDL_GetKeyboardState(NULL);
	const float rotationSpeed{1.f};
	if (keyboard_state_array[SDL_SCANCODE_R]) {
		
		Rotate(rotationSpeed);
	}
	if (keyboard_state_array[SDL_SCANCODE_T]) {

		Rotate(-rotationSpeed);
	}
	//custom moveHandles
	const float moveSpeed(10.f);
	if (keyboard_state_array[SDL_SCANCODE_LEFT]) {
		m_Position.x -= moveSpeed;
	}
	if (keyboard_state_array[SDL_SCANCODE_RIGHT]) {
		m_Position.x += moveSpeed;

	}
	if (keyboard_state_array[SDL_SCANCODE_UP]) {
		m_Position.y += moveSpeed;

	}
	if (keyboard_state_array[SDL_SCANCODE_DOWN]) {
		m_Position.y -= moveSpeed;

	}

}

float Camera::GetZoom() const
{
	return m_Zoom;
}

float Camera::GetRotation() const
{
	return m_Rotation;
}

Vector2f Camera::GetPosition() const
{
	return m_Position;
}

void Camera::Aim(const Rectf& levelRect, Point2f trackCenter)
{
	glPushMatrix();	
	Point2f screenCenter{};
	Point2f RelativeTrackCenter(trackCenter.x - levelRect.left, trackCenter.y - levelRect.bottom);
	if (m_Width > levelRect.width) {
		m_Position.x = std::min(std::max(RelativeTrackCenter.x, m_Width / 2.f), levelRect.width - m_Width / 2.f);
	}
	else {
		m_Position.x = std::min(std::max(RelativeTrackCenter.x, m_Width/2.f), levelRect.width - m_Width / 2.f);
	}
	if (m_Height > levelRect.height) {
		m_Position.y = trackCenter.y;
	}
	else {
		m_Position.y = std::min(std::max(RelativeTrackCenter.y, m_Height / 2.f), levelRect.height - m_Height / 2.f);
	}

	float scale{std::min(levelRect.width,levelRect.height)};
	glTranslatef(screenCenter.x * (1.f - scale), screenCenter.y * (1.f - scale), 0);

}

void Camera::Reset()
{
	glPopMatrix();
}

void Camera::ApplyTransformation() const
{

	Vector2f center{ m_Width / 2.f,m_Height / 2.f };
	//scaling

	float scale{ m_Zoom };
	glTranslatef(center.x * (1.f - scale), center.y * (1.f - scale), 0);
	glScalef(scale, scale, 1.f);

	//rotation
	glRotatef(m_Rotation, 0.f, 0.f, 1.f);

	const float degToRad{ 3.14f / 180.f };
	Vector2f b1{ std::cosf(m_Rotation * degToRad),std::sinf(m_Rotation * degToRad) };
	Vector2f b2{ std::cosf((m_Rotation + 90) * degToRad), std::sinf((m_Rotation + 90) * degToRad) };

	
	Vector2f center_B{ b1.DotProduct(center),b2.DotProduct(center) };
	Vector2f shift_B{ center_B - center };
	glTranslatef(shift_B.x, shift_B.y, 0.f);

	//translating;
	glTranslatef(-m_Position.x + center.x, -m_Position.y+ center.y,0.f);

}

