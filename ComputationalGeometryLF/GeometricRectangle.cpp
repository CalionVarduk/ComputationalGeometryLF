#include "GeometricRectangle.h"

using namespace ComputationalGeometryLF;

GeometricRectangle::GeometricRectangle() : GeometricObject()
{}

GeometricRectangle::GeometricRectangle(CVPoint<double> origin, CVSize<double> size) : GeometricObject()
{
	Origin = origin;
	Size = size;
}

GeometricRectangle::GeometricRectangle(double left, double bottom, double width, double height) : GeometricObject()
{
	Left = left;
	Bottom = bottom;
	Width = width;
	Height = height;
}

GeometricRectangle::GeometricRectangle(CVPoint<double> bottomLeft, CVPoint<double> topRight) : GeometricObject()
{
	Origin = bottomLeft;
	Width = topRight.X - bottomLeft.X;
	Height = topRight.Y - bottomLeft.Y;
}

void GeometricRectangle::set(CVPoint<double> origin, CVSize<double> size)
{ set(origin.X, origin.Y, size.Width, size.Height); }

void GeometricRectangle::set(double left, double bottom, double width, double height)
{
	Left = left;
	Bottom = bottom;
	Width = width;
	Height = height;
}

void GeometricRectangle::set(CVPoint<double> bottomLeft, CVPoint<double> topRight)
{ set(bottomLeft.X, bottomLeft.Y, topRight.X - bottomLeft.X, topRight.Y - bottomLeft.Y); }

void GeometricRectangle::offset(GeometricPoint^ dpt)
{ offset(dpt->X, dpt->Y); }

void GeometricRectangle::offset(CVPoint<double> dpt)
{ offset(dpt.X, dpt.Y); }

void GeometricRectangle::offset(double dx, double dy)
{
	Left += dx;
	Bottom += dy;
}

bool GeometricRectangle::isRectangle()
{ return true; }

void GeometricRectangle::draw(Graphics^ graphics, Pen^ pen, CVPoint<double>% translation)
{
	pen->Color = _mainColor;
	graphics->DrawRectangle(pen, (float)(_boundingBox.Left + translation.X), (float)(translation.Y - _boundingBox.Top - _boundingBox.Height),
									(float)(_boundingBox.Width), (float)(_boundingBox.Height));
}

void GeometricRectangle::fill(Graphics^ graphics, SolidBrush^ brush, CVPoint<double>% translation)
{
	brush->Color = Color::FromArgb(_mainColor.A >> 1, _mainColor);
	graphics->FillRectangle(brush, (float)(_boundingBox.Left + translation.X), (float)(translation.Y - _boundingBox.Top - _boundingBox.Height),
									(float)(_boundingBox.Width + 1), (float)(_boundingBox.Height + 1));
}

String^ GeometricRectangle::ToString()
{
	return "Rectangle: [Origin: (" + Left.ToString() + ", " + Bottom.ToString() + "), Size: (" + Width.ToString() + ", " + Height.ToString() + ")]";
}