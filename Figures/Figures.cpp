#include <vector>
#include <iostream>
#include <cmath>

#include <GL/glut.h>

#include "Figures.h"


//////////////// Figure ////////////////

Figure::Figure(void)
{
	_color.red = 0.0;
	_color.green = 0.0;
	_color.blue = 0.0;
}

void Figure::setColor(float red, float green, float blue)
{
	_color.red = red;
	_color.green = green;
	_color.blue = blue;
}

Figure::~Figure(void)
{
}

//////////////// Polygon ////////////////

Polygon::Polygon(void) : Figure()
{
	_nVertices = 0;
}

void Polygon::setVertices(std::vector<Point> &vertices)
{	
	if(vertices.size() > 2)
	{
		_nVertices = vertices.size();
		for (int i = 0; i < _nVertices; i++)
			_vertices.push_back(vertices[i]);
	}
	else
	{
		throw "Polygon: There should be more than two vertices in a figure!";
	}
}

void Polygon::draw(void)
{
	if (_nVertices != 0)
	{
		glColor3f(_color.red, _color.green, _color.blue);

		glBegin(GL_LINE_STRIP);
			for (int i = 0; i < _nVertices; i++)
				glVertex2d(_vertices[i].x, _vertices[i].y);

			glVertex2d(_vertices[0].x, _vertices[0].y);
		glEnd();
	}
	else
		throw "The figure is not set!";
}

Polygon::~Polygon(void)
{
}

//////////////// Triangle ////////////////

Triangle::Triangle(void) : Polygon()
{
}

void Triangle::setVertices(std::vector<Point> &vertices)
{
	if(vertices.size() == 3)
	{
		_nVertices = vertices.size();
		for (int i = 0; i < _nVertices; i++)
			_vertices.push_back(vertices[i]);
	}
	else
	{
		throw "Triangle: There should be three vertices in the Triangle!";
	}
}

Triangle::~Triangle(void)
{
}

//////////////// Quadrangle ////////////////

Quadrangle::Quadrangle(void) : Polygon()
{
}

void Quadrangle::setVertices(std::vector<Point> &vertices)
{
	if(vertices.size() == 4)
	{
		_nVertices = vertices.size();
		for (int i = 0; i < _nVertices; i++)
			_vertices.push_back(vertices[i]);
	}
	else
	{
		throw "Quadrangle: There should be four vertices in a Quadrangle!";
	}
}

Quadrangle::~Quadrangle(void)
{
}

//////////////// Rectangle ////////////////

Rectangle::Rectangle(void) : Quadrangle()
{
}

void Rectangle::setVertices(std::vector<Point> &vertices)
{
	if(vertices.size() == 4)
	{
		if(checkDiagonalsEquality(vertices) == 1)
		{
			_nVertices = vertices.size();
			for (int i = 0; i < _nVertices; i++)
				_vertices.push_back(vertices[i]);
		}
		else
		{
			throw "Ractangle: Diagonals are not equal! It is not a Ractangle!";
		}
	}
	else
	{
		throw "Ractangle: There should be four vertices in a Ractangle!";
	}
}

bool Rectangle::checkDiagonalsEquality(std::vector<Point> &vertices)
{
	double diagonal1 = sqrt(pow(vertices[2].x - vertices[0].x, 2) + pow(vertices[2].y - vertices[0].y, 2));
	double diagonal2 = sqrt(pow(vertices[3].x - vertices[1].x, 2) + pow(vertices[3].y - vertices[1].y, 2));
	
	if(diagonal1 == diagonal2)
		return true;
	else
		return false;
}

Rectangle::~Rectangle(void)
{
}

//////////////// Square ////////////////

Square::Square(void) : Rectangle()
{
}

void Square::setVertices(std::vector<Point> &vertices)
{
	if(vertices.size() == 4)
	{
		if(checkDiagonalsEquality(vertices) == 1)
		{
			if(chekEdgesEquality(vertices) == 1)
			{
				_nVertices = vertices.size();
				for (int i = 0; i < _nVertices; i++)
					_vertices.push_back(vertices[i]);
			}
			else
			{
				throw "Square: Edges are not equal! It is not a Square";
			}
		}
		else
		{
			throw "Square: Diagonals are not equal! It is not a Square!";
		}
	}
	else
	{
		throw "Square: There should be four vertices in a Square!";
	}
}

bool Square::chekEdgesEquality(std::vector<Point> &vertices)
{
	double edge1 = sqrt(pow(vertices[0].x - vertices[1].x, 2) + pow(vertices[0].y - vertices[1].y, 2));
	double edge2 = sqrt(pow(vertices[1].x - vertices[2].x, 2) + pow(vertices[1].y - vertices[2].y, 2));
	
	if(edge1 == edge2)
		return true;
	else
		return false;
}

Square::~Square(void)
{
}

//////////////// Circle ////////////////

Circle::Circle(void) : Figure()
{
	_radius = 0;
	_center.x = 0.0;
	_center.y = 0.0;
}

void Circle::setCoordinates(Point center, double radius)
{
	_center = center;
	_radius = radius;
}

void Circle::setCoordinates(double centerX, double centerY, double radius)
{
	_center.x = centerX;
	_center.y = centerY;
	_radius = radius;
}

void Circle::draw(void)
{
	if(_radius > 0)
	{
		glColor3f(_color.red, _color.green, _color.blue);

		glBegin(GL_LINE_LOOP); 

			for(int i = 0; i < 100; i++) 
			{ 
				double angle = 2.0 * 3.1415926 * double(i) / 100.0; 

				double dx = _radius * cos(angle);
				double dy = _radius * sin(angle);

				glVertex2f(_center.x + dx, _center.y + dy);
			}

		glEnd();
	}
	else
	{
		throw "Circle: Radius should be more than zero!";
	}
}

Circle::~Circle(void)
{
}
