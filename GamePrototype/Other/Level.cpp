#include "pch.h"
#include "Level.h"

#include "SVGParser.h"
#include "Texture.h"
#include "utils.h"

#include <iostream>
#include "GameObject.h"
#include "Character.h"

Level::Level():
	Level(NULL , NULL)
{

}

Level::Level(const std::string& svgPath, const std::string& texturePath):
	m_scale(2.f)
{
	m_pTexture = new Texture{texturePath};
	if (!SVGParser::GetVerticesFromSvgFile(svgPath, m_mapVertices)) {
		std::cout << "couldn't load : " << svgPath << std::endl;
	}
	if (m_mapVertices.size()<1) {
		std::cout << "no vertices are present" << std::endl;
	}
	else {
		m_border = utils::GetBoundingRectangle(m_mapVertices[0]);
		m_mapVertices.erase(m_mapVertices.begin());
	}

	const int count{ 20 };
	for (int i{}; i < count; ++i) {
		m_objects.push_back(new Character(
			Vector2f{ m_border.left + float(std::rand() % int(m_border.width)),m_border.bottom + float(std::rand() % int(m_border.height)) },
			Vector2f{float(std::rand()%40-20),float(std::rand() % 40 - 20) }));
		m_objects[m_objects.size() - 1]->SetLevel(this);
	}
}

Level::~Level()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void Level::Update(float elapsedSec)
{
	for (Character* object1 : m_objects) {
		object1->Update(elapsedSec);
		if (object1->isDead) {

		}
	}
		//for (int i{ 0 }; i < m_mapVertices.size();++i) {
		//	if (Collistion(*object1, m_mapVertices[i])) {
		//		object1->m_color = Color4f{.1f,1.f,.5f,1.f};
		//		std::cout << "hit" << std::endl;
		//	}
		//	else {
		//		object1->m_color = Color4f{ 1.f,.5f,1.f,1.f };
		//	}
		//}
	//CollisionHandling(0, m_objects);
	
}

//void Level::CollisionHandling(int index, std::vector<GameObject*>& objects) {
//
//	if (index - 1 > objects.size())return;
///*	static int count{};
//	if (count > 1000) {
//		return;
//	}
//	else*/{
//		for (int i{ index+1 }; i < objects.size();++i) {
//			Collistion(*objects[index], *objects[i]);
//		}
//		++index;
//		
//		CollisionHandling(index,objects);
//	}
//}
void Level::Draw() const
{
	//glScalef(m_scale, m_scale,1.f);
	utils::SetColor(Color4f{0.7f,0.7f ,0.7f ,1.f});
	for (std::vector<Point2f> shape : m_mapVertices) {
		utils::FillPolygon(shape);
	}
	for (const Character* object : m_objects) {
		object->Draw();
	}
	//glScalef(1.f / m_scale, 1.f/m_scale,1.f);
}

Rectf Level::GetBorder()
{
	return m_border;
}


std::ofstream& operator<<(std::ofstream& out, const Level& l)
{
	std::cout << "Map" << std::endl;
	std::cout << "scale" << l.m_scale << std::endl;
	return out;
}
