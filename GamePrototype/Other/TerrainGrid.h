#pragma once

#include <vector>

struct GridPoint {
	Point2f m_pos;
	float m_value;
};


class TerrainGrid
{
public:
	static float surfaceThreshold;
private:

	float m_size;
	Point2f m_position;
	int m_resolution;


	TerrainGrid* (m_neighbours[8]);
	std::vector<GridPoint*> m_points;
	std::vector<Point2f> m_Triangles;

public:
	TerrainGrid(unsigned int resolution = 200, Point2f position = Point2f{0.f,0.f},float size = 20.f);
	~TerrainGrid();

	TerrainGrid(const TerrainGrid& other);
	TerrainGrid& operator=(const TerrainGrid& other) = default;

	TerrainGrid(TerrainGrid&& other);
	TerrainGrid& operator=(TerrainGrid&& other) = default;

	void Draw() const;
	void Update(float elapsedSec);
	void Brush(const Point2f& position, float radius,float valueChange);
};

namespace MC {
	int GetSquareIndex(GridPoint* (&square)[4], float threshold);
	Point2f InterpolateGridPoints(const GridPoint& p1, const GridPoint& p2, float threshold);

	const static int SquareList[] = {
		-1,-1,-1,-1,-1,-1,-1,-1,-1,
		 0, 1, 3,-1,-1,-1,-1,-1,-1,
		 1, 2, 5,-1,-1,-1,-1,-1,-1,
		 0, 2, 5, 0, 5, 3,-1,-1,-1,
		 3, 7, 6,-1,-1,-1,-1,-1,-1,
		 0, 1, 7, 0, 7, 6,-1,-1,-1,
		 1, 2, 5, 3, 7, 6,-1,-1,-1,
		 0, 2, 6, 2, 5, 6, 6, 5, 7,
		 5, 8, 7,-1,-1,-1,-1,-1,-1,

		 0, 1, 3, 5, 8, 7,-1,-1,-1,
		 1, 2, 8, 1, 8, 7,-1,-1,-1,
		 0, 2, 8, 0, 8, 3, 3, 8, 7,
		 3, 5, 8, 3, 8, 6,-1,-1,-1,
		 0, 8, 6, 0, 1, 8, 1, 5, 8,
		 2, 8, 6, 1, 2, 6, 3, 1, 6,
		 0, 2, 6, 2, 8, 6,-1,-1,-1,
	};
}

