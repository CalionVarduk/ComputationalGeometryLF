#include "GeometricPoint.h"

using namespace ComputationalGeometryLF;

GeometricPoint::GeometricPoint() : GeometricObject()
{
	_boundingBox.Width = 3;
	_boundingBox.Height = 3;
	set(0, 0);
}

GeometricPoint::GeometricPoint(GeometricPoint^ pt) : GeometricObject()
{
	_boundingBox.Width = 3;
	_boundingBox.Height = 3;
	set(pt->X, pt->Y);
}

GeometricPoint::GeometricPoint(CVPoint<double> pt) : GeometricObject()
{
	_boundingBox.Width = 3;
	_boundingBox.Height = 3;
	set(pt.X, pt.Y);
}

GeometricPoint::GeometricPoint(double x, double y) : GeometricObject()
{
	_boundingBox.Width = 3;
	_boundingBox.Height = 3;
	set(x, y);
}

void GeometricPoint::set(GeometricPoint^ pt)
{ set(pt->X, pt->Y); }

void GeometricPoint::set(CVPoint<double> pt)
{ set(pt.X, pt.Y); }

void GeometricPoint::set(double x, double y)
{ X = x; Y = y; }

void GeometricPoint::offset(GeometricPoint^ dpt)
{ offset(dpt->X, dpt->Y); }

void GeometricPoint::offset(CVPoint<double> dpt)
{ offset(dpt.X, dpt.Y); }

void GeometricPoint::offset(double dx, double dy)
{ _boundingBox.Left += dx; _boundingBox.Top += dy; }

double GeometricPoint::getDeltaXTo(GeometricPoint^ point)
{ return point->X - X; }

double GeometricPoint::getDeltaYTo(GeometricPoint^ point)
{ return point->Y - Y; }

CVPoint<double> GeometricPoint::getDeltaTo(GeometricPoint^ point)
{ return CVPoint<double>(getDeltaXTo(point), getDeltaYTo(point)); }

double GeometricPoint::getDistanceSquaredTo(GeometricPoint^ point)
{
	CVPoint<double> delta = getDeltaTo(point);
	return (delta.X * delta.X) + (delta.Y * delta.Y);
}

double GeometricPoint::getDistanceTo(GeometricPoint^ point)
{ return Math::Sqrt(getDistanceSquaredTo(point)); }

bool GeometricPoint::isPoint()
{ return true; }

void GeometricPoint::draw(Graphics^ graphics, Pen^ pen, CVPoint<double>% translation)
{
	pen->Color = _mainColor;
	graphics->DrawRectangle(pen, (float)(_boundingBox.Left + translation.X), (float)(translation.Y - _boundingBox.Top - 2), 2.0f, 2.0f);
}

void GeometricPoint::fill(Graphics^ graphics, SolidBrush^ brush, CVPoint<double>% translation)
{
	brush->Color = _mainColor;
	graphics->FillRectangle(brush, (float)(X + translation.X), (float)(translation.Y - Y), 1.0f, 1.0f);
}

String^ GeometricPoint::ToString()
{
	return "Point: [" + X.ToString() + ", " + Y.ToString() + "]";
}

Int32 GeometricPoint::CompareTo(Object^ other)
{
	GeometricPoint^ p = (GeometricPoint^)other;
	return (Y > p->Y) ? 1 : (Y < p->Y) ? -1 : 0;
}

bool GeometricPoint::operator> (GeometricPoint^ p1, GeometricPoint^ p2)
{ return (p1->Y > p2->Y); }

bool GeometricPoint::operator< (GeometricPoint^ p1, GeometricPoint^ p2)
{ return (p1->Y < p2->Y); }