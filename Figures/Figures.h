#pragma once

struct Point
{
	GLdouble x;
	GLdouble y;
};

struct Color
{
	float red;
	float green;
	float blue;
};

class Figure
{
public:
	Figure(void);
	virtual void draw(void) = 0;
	void setColor(float, float, float);
	~Figure(void);
protected:
	Color _color;
};

class Polygon : public Figure
{
public:
	Polygon(void);
	virtual void setVertices(std::vector<Point> &);
	virtual void draw(void);
	virtual ~Polygon(void);
protected:
	std::vector<Point> _vertices;
	int _nVertices;
};

class Triangle : public Polygon
{
public:
	Triangle(void);
	virtual void setVertices(std::vector<Point> &);
	virtual ~Triangle(void);
};

class Quadrangle : public Polygon
{
public:
	Quadrangle(void);
	virtual void setVertices(std::vector<Point> &);
	virtual ~Quadrangle(void);
};

class Rectangle : public Quadrangle
{
public:
	Rectangle(void);
	virtual void setVertices(std::vector<Point> &);
	virtual ~Rectangle(void);
protected:
	bool checkDiagonalsEquality(std::vector<Point> &);
};

class Square : public Rectangle
{
public:
	Square(void);
	virtual void setVertices(std::vector<Point> &);
	virtual ~Square(void);
protected:
	bool chekEdgesEquality(std::vector<Point> &);
};

class Circle : public Figure
{
public:
	Circle(void);
	void setCoordinates(Point, double);
	void setCoordinates(double, double, double);
	virtual void draw(void);
	virtual ~Circle(void);
private:
	Point _center;
	double _radius;
};
