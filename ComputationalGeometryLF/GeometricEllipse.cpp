#include "GeometricEllipse.h"

using namespace ComputationalGeometryLF;

GeometricEllipse::GeometricEllipse() : GeometricObject()
{}

GeometricEllipse::GeometricEllipse(CVPoint<double> origin, CVSize<double> size) : GeometricObject()
{
	Origin = origin;
	Size = size;
}

GeometricEllipse::GeometricEllipse(double x, double y, double width, double height) : GeometricObject()
{
	X = x;
	Y = y;
	Width = width;
	Height = height;
}

bool GeometricEllipse::isEllipse()
{ return true; }

void GeometricEllipse::draw(Graphics^ graphics, Pen^ pen, CVPoint<double>% translation)
{
	pen->Color = _mainColor;
	graphics->DrawEllipse(pen, (float)(_boundingBox.Left + translation.X), (float)(translation.Y - _boundingBox.Top - _boundingBox.Height),
								(float)(_boundingBox.Width), (float)(_boundingBox.Height));
}

void GeometricEllipse::fill(Graphics^ graphics, SolidBrush^ brush, CVPoint<double>% translation)
{
	brush->Color = Color::FromArgb(_mainColor.A >> 1, _mainColor);
	graphics->FillEllipse(brush, (float)(_boundingBox.Left + translation.X), (float)(translation.Y - _boundingBox.Top - _boundingBox.Height),
									(float)(_boundingBox.Width), (float)(_boundingBox.Height));
}

String^ GeometricEllipse::ToString()
{
	return "Ellipse: [Origin: (" + X.ToString() + ", " + Y.ToString() + "), Size: (" + Width.ToString() + ", " + Height.ToString() + ")]";
}