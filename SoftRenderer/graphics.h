#pragma once
#include <vector>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

void PutPixel(std::vector<uint32_t>& buffer, int x, int y, uint32_t color);
void DrawLine(std::vector<uint32_t>& buffer, int x1, int y1, int x2, int y2, uint32_t color);
void DrawAndFilledTriangle(std::vector<uint32_t>& buffer, int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);