#include "pch.h"
#include "TerrainGrid.h"
#include "utils.h"
#include "Vector2f.h"

#include "cmath"
#include "iostream"

float TerrainGrid::surfaceThreshold = 0.5f;

TerrainGrid::TerrainGrid(unsigned int resolution,Point2f position , float size):
	m_resolution(resolution),m_position(position),m_size(size)
{
	for (unsigned int j{}; j < m_resolution; ++j) {
		for (unsigned int i{}; i < m_resolution; ++i) {
			m_points.push_back(new GridPoint{ Point2f{float(i * size),float(j * size)},std::sinf(i + j * m_resolution) });

		}
	}

}

TerrainGrid::~TerrainGrid()
{

	for (GridPoint*& point : m_points) {
		delete point;
		point = nullptr;
	}
}

TerrainGrid::TerrainGrid(const TerrainGrid& other)
{

}


TerrainGrid::TerrainGrid(TerrainGrid&& other):
	m_points(std::move(other.m_points)), m_resolution(other.m_resolution),m_position(other.m_position),m_size(other.m_size)
{
}


void TerrainGrid::Draw() const
{
	const static float dotRadius{5.0f};
	const static Color4f dotColor1{ 1.0f,0.5f,0.3f,1.0f };
	const static Color4f dotColor2{ 0.5f,0.1f,0.8f,1.0f };
	const static Color4f triangleColor{ 0.2f,0.3f,0.3f,1.0f };
	for (int i{}; i < m_resolution - 1; ++i)
	{
		for (int j{}; j < m_resolution - 1; ++j)
		{
			/*if (m_points[(i + 0) + (j + 0) * m_resolution]->m_value > TerrainGrid::surfaceThreshold) {
				utils::SetColor(dotColor1);
			}
			else {
				utils::SetColor(dotColor2);
			}
			utils::FillEllipse(m_points[(i + 0) + (j + 0) * m_resolution]->m_pos, dotRadius, dotRadius);*/
			GridPoint* (square[4]);
			square[0] = m_points[(i + 0) + (j + 0) * m_resolution];
			square[1] = m_points[(i + 1) + (j + 0) * m_resolution];
			square[2] = m_points[(i + 0) + (j + 1) * m_resolution];
			square[3] = m_points[(i + 1) + (j + 1) * m_resolution];

			Point2f squarePoints[9]{
				square[0]->m_pos,
				MC::InterpolateGridPoints(*square[0],*square[1],TerrainGrid::surfaceThreshold),
				square[1]->m_pos,
				MC::InterpolateGridPoints(*square[0],*square[2],TerrainGrid::surfaceThreshold),
				MC::InterpolateGridPoints(*square[0],*square[3],TerrainGrid::surfaceThreshold), // Middle Point
				MC::InterpolateGridPoints(*square[1],*square[3],TerrainGrid::surfaceThreshold),
				square[2]->m_pos,
				MC::InterpolateGridPoints(*square[2],*square[3],TerrainGrid::surfaceThreshold),
				square[3]->m_pos
			};

			int index{ MC::GetSquareIndex(square, TerrainGrid::surfaceThreshold) };

			int indices[9];
			memcpy(indices, &MC::SquareList[index * 9], 9 * sizeof(int));
			//const int* indices{&MC::SquareList[index*9]};

			utils::SetColor(triangleColor);
			for (int i{}; i < 9; i+=3) {
				if (indices[i + 0] == -1 or indices[i + 1] == -1 or indices[i + 2] == -1) {
					break;
				}
				utils::FillTriangle(squarePoints[indices[i+0]], squarePoints[indices[i + 1]], squarePoints[indices[i + 2]]);
			}
		}
	}
}

void TerrainGrid::Update(float elapsedSec)
{

}

void TerrainGrid::Brush(const Point2f& position, float radius, float valueChange)
{
	Vector2f v{m_position , position};
	v *= (1.f / m_size);
	std::cout << "indexPosition : " << std::round(v.x) << ","<< std::round(v.y) << std::endl;
	int index{ int(std::round(v.x) + std::round(v.y) * m_resolution) };
	int brushRange{ int(std::ceil(radius / m_size)) };
	for (int i{ -brushRange }; i < brushRange; ++i) {

		for (int j{ -brushRange }; j < brushRange; ++j) {
			int n_index{index + i +j*m_resolution};
			if (n_index > 0 and n_index < m_resolution * m_resolution) {
				m_points[n_index]->m_value += valueChange;
				m_points[n_index]->m_value = std::max(0.f, std::min(1.0f, m_points[n_index]->m_value));
			}
		}
	}

}


int MC::GetSquareIndex(GridPoint* (&square)[4] , float threshold)
{
	int index{};
	if (square[0]->m_value > threshold) index |= 1;
	if (square[1]->m_value > threshold) index |= 2;
	if (square[2]->m_value > threshold) index |= 4;
	if (square[3]->m_value > threshold) index |= 8;
	return index;
}

Point2f MC::InterpolateGridPoints(const GridPoint& p1, const GridPoint& p2, float threshold)
{
	return (Vector2f{ p1.m_pos } + Vector2f{ p1.m_pos,p2.m_pos }*0.5f).ToPoint2f();
	if ((p1.m_value > threshold and p2.m_value < threshold)or
		(p1.m_value < threshold and p2.m_value > threshold)) {
		return (Vector2f{ p1.m_pos } + Vector2f{ p1.m_pos,p2.m_pos } *std::abs((threshold) / (p2.m_value - p1.m_value))).ToPoint2f();
	}
	else {
		
	}
}
