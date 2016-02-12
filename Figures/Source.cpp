#include <vector>
#include <iostream>

#include <GL/glut.h>

#include "Figures.h"


void display(void);
void initialize(void);

int main(int argc, char** argv)
{
	glutInit(& argc, argv);

	initialize();
	
	glutDisplayFunc(display);
	glutMainLoop();

	return 0;
}

void initialize(void)								// OpenGL initialization.
{
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Draw");
	glClearColor(0.9, 0.9, 0.9, 0.0);
	glViewport (0, 0, 600, 600);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 25, 0, 25);
	glMatrixMode(GL_MODELVIEW);	
	glLoadIdentity();
}

void display(void) 
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLineWidth(2);

	std::vector<Point> points, points2;
	Point P[6] = {
		5.0, 5.0,
		5.0, 10.0,
		11.0, 10.0,
		10.0, 5.0,
		15.0, 5.0,
		10.0, 1.0};

	for(int i = 0; i < 6; i++)
		points.push_back(P[i]);

	Point P2[4] = {
		12.0, 13.0,
		16.0, 17.0,
		20.0, 13.0,
		16.0, 9.0};

	for(int i = 0; i < 4; i++)
		points2.push_back(P2[i]);


	Polygon polygon;								//Пример использования классов
	Quadrangle quadrangle;
	Square square;
	Circle circle;

	try									
	{
		polygon.setVertices(points);
		polygon.setColor(0.3, 0.1, 1.0);

		points.pop_back();
		points.pop_back();

		quadrangle.setVertices(points);

		square.setVertices(points2);
		square.setColor(0.8, 0.0, 0.0);

		circle.setCoordinates(7.0, 7.0, 4.0);
		circle.setColor(0.0, 0.8, 0.0);
		
		std::vector<Figure*> figures;

		figures.push_back(&polygon);
		figures.push_back(&quadrangle);
		figures.push_back(&square);
		figures.push_back(&circle);

		for(int i = 0; i < figures.size(); i++)
			figures[i]->draw();
	}
	catch(const char* message)
	{
		std::cout << message << std::endl;
	}

	glFlush();
}
