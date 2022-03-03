#pragma once
#include <vector>
struct line {
	int x1, y1;
	int x2, y2;
};

struct Color {
    GLfloat r;
    GLfloat g;
    GLfloat b;
    GLfloat a;
};
Color getColor(GLint x, GLint y);
std::vector<line>lines;
std::vector<line>clip;
std::vector<line>clippedPolygon;
std::pair<int, int> basePoint;
void draw_line(int x1, int y1, int x2, int y2);
void mouse(int, int, int, int);
void mouseMove(int x, int y);
void draw_line(int x1, int y1, int x2, int y2);
void drawClipped();
void drawPixel(int, int, Color color);
int MinX();
int MaxX();
int MinY();
int MaxY();
void floodFill(int x, int y, float oldColor [3], float newColor[3]);
void processMenu(int option);
std::pair<int, int> getIntersection(line l1, line l2);
