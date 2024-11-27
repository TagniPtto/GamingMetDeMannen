#pragma once
#include "utils.h"
class Camera final
{
public:
	Camera(float width ,float heigth,Point2f position = Point2f{0.f,0.f }, float rotation = 0.f,float zoom = 1.f);
	~Camera();

	void Translate(Vector2f translation);
	void Rotate(float degrees);
	void Zoom(float scaler);
	float GetZoom() const;
	float GetRotation() const;
	Vector2f GetPosition() const;

	void Aim(const Rectf& levelRect , Point2f trackCenter);
	void Reset();


	void Update(float elapsedSec);
	void ApplyTransformation() const;
private:
	void HandleInput();
	Vector2f m_Position;
	float m_Zoom;
	float m_Rotation;
	float m_Width;
	float m_Height;


};

