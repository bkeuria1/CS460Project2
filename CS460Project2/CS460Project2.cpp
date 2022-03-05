
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
 bool scaling = false;
void mouse(int button, int state, int x, int y) {
	glDisable(GL_LINE_STIPPLE);
	int x1, y1;
	int x2, y2;
	if (button == GLUT_LEFT_BUTTON ) {
		
			//lines.pop_back();
			std::cout << x<< ","<<y << std::endl;
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

		if (state == GLUT_UP && scaling == true) {
			viewPort.clear();
			viewPort.push_back({300, 700 - y, x, 700 - y });
			viewPort.push_back({ 300, 700 - 380, 300, 700 - y });
			viewPort.push_back({ 300, 700 - 380, x, 700 - 380 });
			viewPort.push_back({ x, 700 - 380, x, 700 - y });

			mapToViewPort();
			glutPostRedisplay();
		}


	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		//std::cout << "Right Button Clicked" << std::endl;
		//done_drawing = true;
		draw_line(basePoint.first, 700-basePoint.second, x, 700-y);
		lines.push_back({ basePoint.first, 700-basePoint.second, x, 700-y });
	}
}

void mouseMove(int x, int y) {
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_LINE_STIPPLE);
	//draw all the lines
	
	for (auto it = std::begin(lines); it != std::end(lines); ++it) {
		line current = *it;
		draw_line(current.x1, current.y1, current.x2, current.y2);
	}
	for (auto it = std::begin(clip); it != std::end(clip); ++it) {
		line current = *it;
		draw_line(current.x1, current.y1, current.x2, current.y2);
	}
	
	//Upper left to upper right
	draw_line(300, 700-y, x, 700-y);
	draw_line(300,700-380 , 300, 700 - y);
	draw_line(300, 700 - 380, x, 700 - 380);
	draw_line(x, 700 - 380, x, 700 - y);
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
			
			int interX = getIntersection(clipVertex, vertex).first;
			int interY = getIntersection(clipVertex, vertex).second;
			
				if (!isInside(x1, y1) && isInside(x2, y2)) {
					//std::cout << "Inter" << std::endl;
					if (interX != -1 && interY != -1) {
						std::cout << "out to in" << std::endl;
						std::cout << "inter x,y" << interX << "," << interY<<std::endl;
						vertex.x1 = interX;
						vertex.y1 = interY;
					}
				}else if(isInside(x1, y1) && !isInside(x2, y2)){
					if (interX != -1 && interY != -1) {
						std::cout << "In to out" << std::endl;
						std::cout << "New POINTS: " << x1 << "," << y1 << std::endl;
						std::cout << "New POINTS: " << x2 << "," << y2 << std::endl;
						std::cout << "inter x,y" << interX << "," << interY;
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
	lines.erase(std::remove_if(lines.begin(), lines.end(), [](line const& l) { return !isInside(l.x1, l.y1) && !isInside(l.x2, l.y2); }),
		lines.end());
	glClear(GL_COLOR_BUFFER_BIT);
	std::cout << "AFTER changing" << std::endl;
	for (auto it = std::begin(lines); it != std::end(lines); ++it) {
		line current = *it;
		//std::cout << "X1: " << current.x1 << " , " << "Y1: " << current.y1 << std::endl;
		//std::cout << "X2: " << current.x2 << " , " << "Y2: " << current.y2 << std::endl;
		draw_line(current.x1, current.y1, current.x2, current.y2);
	}

	glutPostRedisplay();
}

int MinX(std::vector<line> vlines) {
	int minX = INT_MAX;
	for (auto l : vlines) {
	
		minX = std::min({ l.x1, l.x2,minX });
	}
	return minX;


}
int MaxX(std::vector<line> vlines) {
	int maxX = INT_MIN;
	for (auto l : vlines) {
		maxX = std::max({ l.x1, l.x2,maxX });
	}
	return maxX;
}

int MinY(std::vector<line> vlines) {
	int minY = INT_MAX;
	for (auto l : vlines) {
		minY = std::min({ l.y1, l.y2,minY });

	}
	return minY;
}

int MaxY(std::vector<line> vlines) {
	int maxY = INT_MIN;
	for (auto l : vlines) {
		maxY = std::max({ l.y1, l.y2,maxY });

	}
	return maxY;
}


void boundFill(int x, int y, Color boundC, Color newC) {
	//define boundaries
	//std::cout  << x << "," << y << std::endl;

	int left = MinX(lines);
	std::cout << "LEFT BOUNDARY"<<left << std::endl;
	int right = MaxX(lines);
	std::cout << "Right BOUNDARY" << right << std::endl;
	int top = MinY(lines);
	std::cout << "Top BOUNDARY" << top << std::endl;
	int bottom = MaxY(lines);
	std::cout << "Bottom " << bottom << std::endl;
	Color check;
	glReadPixels(x, y, 1, 1, GL_RGBA, GL_FLOAT, &check);
	Color color = getColor(x, y);

	if (x<=LEFT|| x>=RIGHT || y<=TOP || y>=BOTTOM ) {
		//std::cout << "Out of bounds" << x<<","<<y << std::endl;
		return;
	}

	if (x<=left || x>=right || y<=top || y>=bottom) {
		//std::cout << "out of bounds" << x<<","<<y << std::endl;
		return;
	}
	
	 if (color.r == boundC.r && color.g == boundC.g && color.b == boundC.b) {
		std::cout << "Pixel " << x << "," << y << "\n";
		return;
	}

	else if (color.r == newC.r && color.g == newC.g && color.b == newC.b) {
		std::cout << "Pixel has new color already " << x << "," << y << "\n";
		return;
	}

	else {
		//std::cout << "New color drawnL "<<x<<","<<y << "\n";
		drawPixel(x, y, newC);
		//glFlush();

		boundFill(x, y + 1, boundC, newC);
		boundFill(x, y - 1, boundC, newC);
		boundFill(x + 1, y, boundC, newC);
		boundFill(x - 1, y, boundC, newC);
	
		
	}
	std::cout << "DONE" << std::endl;
	glColor3f(1.0,1.0, 1.0);
	/*Color current = getColor(x, y);
	std::cout << (current.r) << std::endl;
	std::cout << (current.b) << std::endl;
	std::cout << (current.b) << std::endl;*/

	
	//glFlush();
}
	
void drawViewPort() {
	draw_line(300, 300, 340, 300);
	viewPort.push_back({ 300, 300, 340, 300 });
	draw_line(300, 300, 300, 380);
	viewPort.push_back({ 300, 300, 300, 380 });
	draw_line(340, 300, 340, 380);
	viewPort.push_back({ 340, 300, 340, 380 });
	draw_line(300, 380, 340, 380);
	viewPort.push_back({ 300, 380,340, 380 });
	glutPostRedisplay();
}

	//get the cordinates of each line

void mapToViewPort(){
	int x1VP, y1VP, x2VP,  y2VP;

	float scaleX = (float)(MaxX(viewPort) - MinX(viewPort)) / (MaxX(clip) - MinX(clip));
	float scaleY = (float) (MaxY(viewPort) - MinY(viewPort)) / (MaxY(clip ) - MinY(clip));

	for (auto l : lines) {
		x1VP = MinX(viewPort) + (l.x1 - MinX(clip)) * scaleX;
		y1VP = MinY(viewPort) + (l.y1 - MinY(clip)) * scaleY;
		x2VP = MinX(viewPort) + (l.x2 - MinX(clip)) * scaleX;
		y2VP = MinY(viewPort) + (l.y2 - MinY(clip)) * scaleY;
		std::cout << x1VP << "," << y1VP << std::endl;
		draw_line(x1VP, y1VP, x2VP, y2VP);
	}

	glutPostRedisplay();



}



// change the color based on the menu options
void processMenu(int option)
{
	Color old = {1.0f,1.0f,1.0f};
	

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
		
		boundFill((MinX(lines) + MaxX(lines)) / 2, (MinY(lines)+MaxY(lines)) / 2, old, newColor);
		break;
	case 3:
		Color color = getColor(150, 150);
		scaling = true;
		glutMotionFunc(mouseMove);
		std::cout << color.r << color.g << color.b << std::endl;
		drawViewPort();
		mapToViewPort();
		break;
	case 4:
		scaling = false;

	}
}



int main(int argc, char** argv) {

	// create a window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE | GLUT_ALPHA);
	glutInitWindowPosition(300, 300);
	glutInitWindowSize(700, 700);
	glutCreateWindow("Computer Graphics");
	glClear(GL_COLOR_BUFFER_BIT);
	//glClearColor(0.0, 1.0, 1.0, 0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 700.0, 0.0, 700.0);

	glPointSize(1.0f);
	// register a display callback
	glutDisplayFunc(drawClipped);
	// create a menu and add entries
	int menu = glutCreateMenu(processMenu);
	glutAddMenuEntry("Polygon Clipping", 1);
	glutAddMenuEntry("Region Filling", 2);
	glutAddMenuEntry("Window to View Port Mapping", 3);
	glutAddMenuEntry("Reset Mouse", 4);
	glutAttachMenu(GLUT_MIDDLE_BUTTON);

	glutMouseFunc(mouse);


	// event processing loop
	
	glutMainLoop();

	return 0;
}
