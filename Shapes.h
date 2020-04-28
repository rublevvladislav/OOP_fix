#pragma once
#include "Base.h"

class Shape : public Printable
{
public:
	static int GetCount() 
	{
		return shapesCount;
	}
	Shape()
	{
		shapesCount++;
	}
	Shape(Shape const & shape)
	{
		shapesCount++;
	}
	virtual ~Shape() 
	{
		shapesCount--;
	}
	static int shapesCount;
};

class Point : public Shape 
{
public:
	Point(double const x, double const y) : Shape(), m_x(x), m_y(y) {}
	void Print(std::ostream& out) const override
	{
		out << "Point(" << m_x << "," << m_y << ")" << std::endl;
	}
	double GetX() const { return m_x; }
	double GetY() const { return m_y; }
	static double DistanceBetweenPoints(Point const & p1, Point const & p2) 
	{
		double distX = abs(p1.GetX() - p2.GetX());
		double distY = abs(p1.GetY() - p2.GetY());
		return sqrt(distX * distX + distY * distY);
	}
private:
	double m_x, m_y;
};

class Circle : public Shape, public Named
{
public:
	Circle(std::string const & name, Point const & center, double const & radius)
		: Shape(), Named(name), m_center(center), m_radius(radius) 
	{
		if (radius <= 0)
		throw std::runtime_error("radius should be more 0");
	}
	double Area() const 
	{
		const double PI = std::atan(1.0) * 4;
		return PI * m_radius * m_radius;
	}
	void Print(std::ostream& out) const override
	{
		out << m_name << " Circle(center:(" << m_center.GetX() << "," << m_center.GetY() << "), radius(" 
			<< m_radius << ")) Area: " << this->Area() << std::endl;
	}
private:
	Point m_center;
	double m_radius;
};

class Rect : public Shape, public Named
{
public:
	Rect(std::string const & name, Point const & leftpoint, Point const & rightpoint)
		: Shape(), Named(name), m_leftpoint(leftpoint), m_rightpoint(rightpoint) 
	{
		m_width = abs(m_leftpoint.GetX() - m_rightpoint.GetX());
		m_height = abs(m_leftpoint.GetY() - m_rightpoint.GetY());
		if (m_width == 0 || m_height == 0) throw std::runtime_error("width and height should be more 0");
	};
	double Area() const 
	{
		return m_width * m_height;
	}
	void Print(std::ostream& out) const override
	{
		out << m_name << " Rect(width:" << m_width <<", height:"
			<< m_height << ") Area: " << this->Area() << std::endl;
	}
private:
	Point m_leftpoint, m_rightpoint;
	double m_width, m_height;
};

class Square : public Shape, public Named
{
public:
	Square(std::string const & name, Point const & point, double const side)
		: Shape(), Named(name), m_point(point), m_side(side) {}
	double Area() const 
	{
		return m_side * m_side;
	}
	void Print(std::ostream& out) const override
	{
		out << m_name << " Square(side:" << m_side << ", point(" << m_point.GetX() << "," << m_point.GetY() 
			<< ")" ") Area: " << this->Area() << std::endl;
	}
private:
	Point m_point;
	double m_side;
};

class Polyline : public Shape, public Named
{
public:
	Polyline(std::string const & name) : Shape(), Named(name) {}
	void AddPoint(Point const & point) 
	{
		m_points.push_back(point);
	}
	double Length() const 
	{
		if (m_points.size() < 2) return 0.0;
		double lenght = 0;
		for (unsigned int i = 0; i < m_points.size() - 1; i++) 
		{
			Point a = m_points[i];
			Point b = m_points[i+1];
			lenght += Point::DistanceBetweenPoints(a,b);
		}
		return lenght;
	}
	void Print(std::ostream& out) const override
	{
		out << m_name << " Polyline(" << std::endl;
		for (unsigned int i = 0; i < m_points.size(); i++) 
		{
			Point ptr = m_points[i];
			ptr.Print(out);
		}
		out << ") Lenght: " << this->Length() << std::endl;
	}
protected:
	Vector<Point> m_points;
};

class Polygon : public Polyline
{
public:
	Polygon(std::string const & name) : Polyline(name) {}
	double Length() const
	{
		return Polyline::Length() + Point::DistanceBetweenPoints(m_points[0], m_points[m_points.size() - 1]);
	}
	void Print(std::ostream& out) const override
	{
		out << m_name << " Polygon(" << std::endl;
		for (unsigned int i = 0; i < m_points.size(); i++) 
		{
			Point ptr = m_points[i];
			ptr.Print(out);
		}
		out << ") Lenght: " << this->Length() << std::endl;
	}
};