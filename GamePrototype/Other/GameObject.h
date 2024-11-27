#pragma once
#include "Entity.h"
#include "Texture.h"
#include <vector>




class GameObject : public Entity
{
public:
	friend bool Collistion(GameObject& g1, GameObject& g2);
	friend bool Collistion(GameObject& g1, const std::vector<Point2f>& vertices);
	explicit GameObject();
	explicit GameObject(std::vector<Point2f>& pol);
	explicit GameObject(Vector2f position, Vector2f velocity = Vector2f{0.f,0.f});
	virtual void Update(float elapsedSec) override;
	virtual void Draw() const override;
	virtual void SetPosition(Point2f position);

	Color4f m_color{ .5f,.5f,1.f,1.f };

private:

	Texture* m_pTexture;
	std::vector<Point2f> m_HitBox;
};

