#pragma once
#include <vector>
#include "math.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

struct Color {
	uint32_t black = 0xFF000000;
	uint32_t white = 0XFFFFFFFF;
	uint32_t red = 0xFFFF0000;
	uint32_t orange = 0xFFFFA500;
	uint32_t yellow = 0xFFFFFF00;
	uint32_t green = 0xFF008000;
	uint32_t blue = 0xFF0000FF;
	uint32_t navy = 0xFF000080;
	uint32_t purple = 0xFF800080;
	uint32_t pink = 0xFFFFC0CB;
	uint32_t brown = 0xFFA52A2A;
	uint32_t lightgreen = 0xFF90EE90;
	uint32_t skyblue = 0xFF87CEEB;
};

struct Triangle {
	Vec3 p[3];
};

struct Mesh {
	std::vector<Triangle> tris;
};

struct GameObject {
	Mesh mesh;
	Vec3 scale;
	Vec3 rotation;
	Vec3 position;
	Color color;
};

void FillScreen(std::vector<uint32_t>& buffer, uint32_t color);
void PutPixel(std::vector<uint32_t>& buffer, int x, int y, uint32_t color);
void DrawLine(std::vector<uint32_t>& buffer, int x1, int y1, int x2, int y2, uint32_t color);
void DrawAndFilledTriangle(std::vector<uint32_t>& buffer, int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
void DrawGameObject(std::vector<uint32_t>& buffer, const GameObject& obj);