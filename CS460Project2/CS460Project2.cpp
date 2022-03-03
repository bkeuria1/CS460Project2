
#define NOMINMAX
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <iostream>
#include <freeglut.h>
#include <vector>
#include <algorithm>
#include "CS460Project2.h";
//x points
 int LEFT = 150; 
 int RIGHT = 250;
//y points
 int TOP = 150;
 int BOTTOM = 350;
void mouse(int button, int state, int x, int y) {
	glDisable(GL_LINE_STIPPLE);
	int x1, y1;
	int x2, y2;
	if (button == GLUT_LEFT_BUTTON) {
		
			//lines.pop_back();
			//td::cout << " Size after " << lines.size() << std::endl;
			//glClear(GL_COLOR_BUFFER_BIT);
			for (auto it = std::begin(lines); it != std::end(lines); ++it) {
				line current = *it;
				draw_line(current.x1, current.y1, current.x2, current.y2);
			}
		
		if (state == GLUT_DOWN) {
			//	std::cout << "GLUT-UP" << std::endl;
			basePoint = std::make_pair(x, y);
			glutPostRedisplay();
		}
		else if (state == GLUT_UP) {
			//std::cout << "GLUT-UP" << std::endl;
			std::cout << " Size after " << lines.size() << std::endl;
			draw_line(basePoint.first, basePoint.second, x, y);
			lines.push_back({ basePoint.first, basePoint.second, x, y });
			basePoint = std::make_pair(x, y);
		}
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		//std::cout << "Right Button Clicked" << std::endl;
		//done_drawing = true;
		draw_line(basePoint.first, basePoint.second, x, y);
		lines.push_back({ basePoint.first, basePoint.second, x, y });
	}
}

void mouseMove(int x, int y) {
	//glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_LINE_STIPPLE);
	//draw all the lines
	
	for (auto it = std::begin(lines); it != std::end(lines); ++it) {
		line current = *it;
		draw_line(current.x1, current.y1, current.x2, current.y2);
	}
	draw_line(basePoint.first, basePoint.second, x, y);
	glutPostRedisplay();
}

void draw_line(int x1, int y1, int x2, int y2) {
	glBegin(GL_LINES);
	//glDisable(GL_LINE_STIPPLE);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glEnd();
	//glFlush();
	
}


void drawPixel(GLint x, GLint y,Color color) {
	glColor3f(color.r,color.g,color.b);
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
	glFlush();
	//glutPostRedisplay();
}
Color getColor(GLint x, GLint y) {
	Color color;
	glReadPixels(x, y, 1, 1, GL_RGBA, GL_FLOAT, &color);
	return color;
}

//draws the clipping Boundary
void drawClipped() {


	//glClear(GL_COLOR_BUFFER_BIT);
	glLineStipple(2, 0xAAAA);
	glEnable(GL_LINE_STIPPLE);
	//glColor3f(0.0, 0.0, 1.0);
	//glLineWidth(30);
	//glBegin(GL_LINE_LOOP);
	clip.push_back({ LEFT, TOP, RIGHT, TOP });
	clip.push_back({ LEFT, BOTTOM, RIGHT, BOTTOM });
	clip.push_back({ LEFT,TOP, LEFT, BOTTOM });
	clip.push_back({ RIGHT, TOP, RIGHT, BOTTOM });
	for (auto it = std::begin(clip); it != std::end(clip); ++it) {
		line current = *it;
		draw_line(current.x1, current.y1, current.x2, current.y2);
	}
	glDisable(GL_LINE_STIPPLE);
	/*Color newColor = { 1.0f,0.0f,0.0f };
	std::cout << getColor(0, 0).r << " " << getColor(0, 0).g << getColor(0, 0).b << std::endl;
	drawPixel(0, 0, newColor);
	std::cout << getColor(0, 0).r << " " << getColor(0, 0).g << getColor(0, 0).b << std::endl;*/
	//glEnd();
	glFlush();
	

}
std::pair<int, int> getIntersection(line l1, line l2) {
	int x = -1;
	int y = -1;
	int a1 =(l1.y2 - l1.y1);
	int b1 = (l1.x1 - l1.x2);
	int c1 = a1 * (l1.x1) + b1 * (l1.y1);

	int a2 = (l2.y2 - l2.y1);
	int b2 = (l2.x1 - l2.x2);
	int c2 = (a2 * (l2.x1) + b2* (l2.y1));
	double determinant = std::abs( a1* b2 - a2 * b1);
	//std::cout << "The determinant "<< determinant << std::endl;

	 x = std::abs((b2 * c1 - b1 * c2) / determinant);
	 y = std::abs((a1 * c2 - a2 * c1) / determinant);

	// std::cout << "X INTER: " << x << std::endl;
	// std::cout << "Y INTER: " << y << std::endl;
	
	 int l1xMAX = (l1.x2 > l1.x1)  ? l1.x2 : l1.x1;
	 int l1xMIN =  (l1.x2 > l1.x1) ? l1.x1 : l1.x2;

	 int l1yMAX =  (l1.y2 > l1.y1) ? l1.y2 : l1.y1;
	 int l1yMIN =  (l1.y2 > l1.y1) ? l1.y1 : l1.y2;

	 int l2xMAX =  (l2.x2 > l2.x1) ? l2.x2 : l2.x1;
	 int l2xMIN =  (l2.x2 > l2.x1) ? l2.x1 : l2.x2;

	 int l2yMAX =  (l2.y2 > l2.y1) ? l2.y2 : l2.y1;
	 int l2yMIN =  (l2.y2 > l2.y1) ? l2.y1 : l2.y2;

	// std::cout << "XL1 Range: " <<l1xMIN<< " - " << l1xMAX << std::endl;
	 //std::cout << "Y INTER: " << y << std::endl;



	 if (x >= l1xMIN && x <= l1xMAX &&
		 y >= l1yMIN && y <= l1yMAX &&
		 x >= l2xMIN && x <= l2xMAX &&
		 y >= l2yMIN && y <= l2yMAX) {

		 return std::make_pair(x, y);

	 }
	 //no intersection case
	 return std::make_pair(-1, -1);


	
}
boolean isInside( int x,int y) {
	if (x>=LEFT && x<=RIGHT && y>=TOP && y<=BOTTOM ) {
		return true;
	}
	return false;
}
void clipPolygon() {

	
	for (auto &clipVertex : clip) {
		for (auto &vertex : lines) {
			int x1 = vertex.x1;
			int y1 = vertex.y1;
			int x2 = vertex.x2;
			int y2 = vertex.y2;
			//std::cout << "X1: " << x1 << " , " << "Y1: " << y1 << std::endl;
			//std::cout << "X2: " << x2 << " , " << "Y2: " << y2 << std::endl;
			int interX = getIntersection(clipVertex, vertex).first;
			int interY = getIntersection(clipVertex, vertex).second;
			
				if (!isInside(x1, y1) && isInside(x2, y2)) {
					//std::cout << "Inter" << std::endl;
					if (interX != -1 && interY != -1) {
						/*std::cout << "out to in" << std::endl;
						std::cout << "inter x,y" << interX << "," << interY<<std::endl;*/
						vertex.x1 = interX;
						vertex.y1 = interY;
					}
				}else if(isInside(x1, y1) && !isInside(x2, y2)){
					if (interX != -1 && interY != -1) {
						/*std::cout << "In to out" << std::endl;
						std::cout << "New POINTS: " << x1 << "," << y1 << std::endl;
						std::cout << "New POINTS: " << x2 << "," << y2 << std::endl;
						std::cout << "inter x,y" << interX << "," << interY;*/
						vertex.x2 = interX;
						vertex.y2 = interY;
					}
				}
				else if (!isInside(x1, y1) && !isInside(x2, y2)) {
					if (interX != -1 && interY != -1) {
						std::cout << "Both not in, but intersect" << std:: endl;
						std::cout << x1 << "," << y1 << std::endl;
						std::cout << x2 << "," << y2 << std::endl;
						std::cout << "inter x,y" << interX << "," << interY << std::endl;
						if (x2 > x1 && y2 < y1) {
							vertex.x1 = interX;
							vertex.y1 = interY;
						}
						else {
							vertex.x2 = interX;
							vertex.y2 = interY;
						}
					
					}

				}
			
		}
	}

	for (auto v : lines) {

		std::cout << "NEW LINES" << std::endl;
		std::cout << "New POINTS: " << v.x1 << "," << v.y1 << std::endl;
		std::cout << "New POINTS: " << v.x2 << "," << v.y2 << std::endl;
	}
	lines.erase(std::remove_if(lines.begin(), lines.end(), [](line const& l) { return !isInside(l.x1, l.y1) && !isInside(l.x2, l.y2); }),
		lines.end());
	//glClear(GL_COLOR_BUFFER_BIT);
	std::cout << "AFTER changing" << std::endl;
	for (auto it = std::begin(lines); it != std::end(lines); ++it) {
		line current = *it;
		//std::cout << "X1: " << current.x1 << " , " << "Y1: " << current.y1 << std::endl;
		//std::cout << "X2: " << current.x2 << " , " << "Y2: " << current.y2 << std::endl;
		draw_line(current.x1, current.y1, current.x2, current.y2);
	}

	glutPostRedisplay();
}

int MinX() {
	int minX = INT_MAX;
	for (auto l : lines) {
	
		minX = std::min({ l.x1, l.x2,minX });
	}
	return minX;


}
int MaxX() {
	int maxX = INT_MIN;
	for (auto l : lines) {
		maxX = std::max({ l.x1, l.x2,maxX });
	}
	return maxX;
}

int MinY() {
	int minY = INT_MAX;
	for (auto l : lines) {
		minY = std::min({ l.y1, l.y2,minY });

	}
	return minY;
}

int MaxY() {
	int maxY = INT_MIN;
	for (auto l : lines) {
		maxY = std::max({ l.y1, l.y2,maxY });

	}
	return maxY;
}


void floodFill(int x, int y, Color oldC, Color newC) {
	//define boundaries
	std::cout  << x << "," << y << std::endl;

	int left = MinX();
	std::cout << "LEFT BOUNDARY"<<left << std::endl;
	int right = MaxX();
	std::cout << "Right BOUNDARY" << right << std::endl;
	int top = MinY();
	std::cout << "Top BOUNDARY" << top << std::endl;
	int bottom = MaxY();
	std::cout << "Bottom " << bottom << std::endl;

	/*if (x<left || x>right || y<top || y>bottom) {
		std::cout << "Out of bounds" << x<<","<<y << std::endl;
		return;
	}*/
	//float buffer[3];
	//glReadPixels(0, 0, 1, 1, GL_RGB, GL_FLOAT, buffer);
	////access buffer     (assuming x and y are a position on the screen)
	//std::cout << buffer[0] << std::endl; //R
	//std::cout << buffer[1] << std::endl; //G
	//std::cout << buffer[2] << std::endl; //B
	//glClear(GL_COLOR_BUFFER_BIT);
	//glBegin(GL_POINTS);
	//std::cout << (newC.r) << std::endl;
	//std::cout << (newC.b) << std::endl;
	//std::cout << (newC.b) << std::endl;
	
	

	Color color = getColor(x, y);
	std::cout << "Colors" << std::endl;
	std::cout << (color.r) << std::endl;
	std::cout << (color.b) << std::endl;
	std::cout << (color.b) << std::endl;

	std::cout << (newC.r) << std::endl;
	std::cout << (newC.b) << std::endl;
	std::cout << (newC.b) << std::endl;

	if (color.r == newC.r && color.g == newC.g && color.b == newC.b) {
		std::cout << "Pixel has new color already " << x << "," << y << "\n";
		return;
	}

	else {
		std::cout << "New color drawnL "<<x<<","<<y << "\n";
		drawPixel(x, y, newC);
		//glFlush();

	/*	floodFill(x, y + 1, oldC, newC);
		floodFill(x, y - 1, oldC, newC);
		floodFill(x + 1, y, oldC, newC);
		floodFill(x - 1, y, oldC, newC);
	*/
		
	}
	Color current = getColor(x, y);
	std::cout << (current.r) << std::endl;
	std::cout << (current.b) << std::endl;
	std::cout << (current.b) << std::endl;

	
	//glFlush();
}
	




// change the color based on the menu options
void processMenu(int option)
{
	Color old = {0.0f,0.0f,0.0f};
	

	Color newColor = {1.0f,0.0f,0.0f};
	
	switch (option) {	
	case 1:
		clipPolygon();
		/*std::cout << clip.at(0).x1 << " " << clip.at(0).y1 << " " << clip.at(0).x2 << " " << clip.at(0).y2 << std::endl;
		std::cout << lines.at(0).x1 << " " << lines.at(0).y1 << " " << lines.at(0).x2 << " " << lines.at(0).y2 << std::endl;;*/
		std::cout<<"X :" << getIntersection(clip.at(0), lines.at(0)).first << std::endl;;
		std::cout << "Y: " << getIntersection(clip.at(0), lines.at(0)).second << std::endl;
		break;
	case 2:
		std::cout << getColor(150, 150).r << " " << getColor(150, 150).g << getColor(150, 150).b << std::endl;
		drawPixel(150, 150, newColor);
		std::cout<<getColor(150, 150).r<<" "<< getColor(150, 150).g<<getColor(150,150).b<<std::endl;
		//floodFill(0,0, old, newColor);
		break;
	case 3:
		break;
	}
}



int main(int argc, char** argv) {

	// create a window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE | GLUT_ALPHA);
	glutInitWindowPosition(300, 300);
	glutInitWindowSize(1000, 500);
	glutCreateWindow("Computer Graphics");
	//glClear(GL_COLOR_BUFFER_BIT);
	//glClearColor(0.0, 1.0, 1.0, 0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 1000.0, 500.0, 0.0);

	glPointSize(1.0f);
	// register a display callback
	glutDisplayFunc(drawClipped);
	// create a menu and add entries
    int menu = glutCreateMenu(processMenu);
	glutAddMenuEntry("Polygon Clipping", 1);
	glutAddMenuEntry("Region Filling", 2);
	glutAddMenuEntry("Window to View Port Mapping", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMove);

	// event processing loop
	
	glutMainLoop();

	return 0;
}
