#pragma once

#include <vector>

class Character;
class Texture;

class Level
{
public:
	explicit Level();
	explicit Level(const std::string& svgPath, const std::string& texturePath);
	~Level();

	void Update(float elapsedSec);
	void Draw()const;

	friend std::ofstream& operator<<(std::ofstream& out,const Level& l);
	Rectf GetBorder();
	
	std::vector<Character*> m_objects;

private:
	//void CollisionHandling(int index, std::vector<GameObject*>& objects);

	float m_scale;
	Texture* m_pTexture;
	std::vector<std::vector<Point2f>> m_mapVertices;
	Rectf m_border;
};