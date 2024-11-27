#include "pch.h"
#include "GameObject.h"

#include "Matrix2x3.h"
#include <iostream>


GameObject::GameObject():
	GameObject(Vector2f{}, Vector2f{float(std::rand()%40-20),float(std::rand()%40-20)})
{

}

GameObject::GameObject(std::vector<Point2f>& pol):
	Entity(Vector2f{}, Vector2f{}), m_pTexture(nullptr)
{
	m_HitBox = pol;
}

GameObject::GameObject(Vector2f position, Vector2f velocity):
	Entity(position,velocity), m_pTexture(nullptr)
{
	const float size{ 10.f };
	m_HitBox.push_back(Point2f{ -size,size });
	m_HitBox.push_back(Point2f{ size,size });
	m_HitBox.push_back(Point2f{ size,-size });
	m_HitBox.push_back(Point2f{ -size,-size });
}

void GameObject::Update(float elapsedSec)
{
	Entity::Update(elapsedSec);
	const float maxSpeed{100};
	const float minSpeed{0};
	m_velocity = m_velocity.Normalized() * std::min(maxSpeed,m_velocity.Length());
}

void GameObject::Draw() const
{

	utils::SetColor(m_color);
	glPushMatrix(); {
	glTranslatef(m_position.x, m_position.y,1.f);
	utils::DrawPolygon(m_HitBox);
	//glTranslatef(-m_position.x, -m_position.y, 1.f);
	}
	glPopMatrix();
	//glTranslatef(-m_position.x, -m_position.y,1.f);
}

void GameObject::SetPosition(Point2f position)
{
	m_position = Vector2f{ position };
}

bool Collistion(GameObject& g1, GameObject& g2)
{

		std::vector<Point2f> intersection;

		Matrix2x3 t1;
		t1.SetAsTranslate(g1.m_position.x, g1.m_position.x);
		std::vector<Point2f> p1{ std::move(t1.Transform(g1.m_HitBox)) };

		Matrix2x3 t2;
		t2.SetAsTranslate(g2.m_position.x, g2.m_position.x);
		std::vector<Point2f> p2{ std::move(t2.Transform(g2.m_HitBox)) };


		if (utils::IntersectPolygons(p1, p2, intersection)) {
			for (const Point2f& p : intersection) {

				Vector2f v1{ g1.m_position.ToPoint2f(),p };
				Vector2f v2{ g2.m_position.ToPoint2f(),p };
				g2.m_velocity += v2.DotProduct(g1.m_velocity) * v2.Normalized();
				g1.m_velocity += v1.DotProduct(g2.m_velocity) * v1.Normalized();
			}
		}
		if (intersection.size()>0) {
			return true;
		}
		else {
			false;
		}
}

bool Collistion(GameObject& g1, const std::vector<Point2f>& vertices)
{
	std::vector<Point2f> intersection;

	Matrix2x3 t1;
	t1.SetAsTranslate(g1.m_position.x, g1.m_position.x);
	std::vector<Point2f> p1{ std::move(t1.Transform(g1.m_HitBox)) };

	if (utils::IntersectPolygons(p1, vertices, intersection)) {

		for (const Point2f& p : intersection) {

			Vector2f v1{ g1.m_position.ToPoint2f(),p };
			g1.m_position += v1;
			g1.m_velocity *= -1;
		}

	}
	if (intersection.size() > 0) {
		return true;
	}
	else {
		false;
	}
}
