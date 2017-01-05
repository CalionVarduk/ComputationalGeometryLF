#include "GeometricSegment.h"

using namespace ComputationalGeometryLF;

GeometricSegment::GeometricSegment()
	: GeometricObject(), _start(0, 0), _end(0, 0)
{}

GeometricSegment::GeometricSegment(CVPoint<double> start, CVPoint<double> end)
	: GeometricObject(), _start(start), _end(end)
{
	_fixHorizontal();
	_fixVertical();
}

GeometricSegment::GeometricSegment(double startX, double startY, double endX, double endY)
	: GeometricObject(), _start(startX, startY), _end(endX, endY)
{
	_fixHorizontal();
	_fixVertical();
}

void GeometricSegment::set(CVPoint<double> start, CVPoint<double> end)
{ Start = start; End = end; }

void GeometricSegment::set(double startX, double startY, double endX, double endY)
{ set(CVPoint<double>(startX, startY), CVPoint<double>(endX, endY)); }

void GeometricSegment::offset(GeometricPoint^ dpt)
{ offset(dpt->Location); }

void GeometricSegment::offset(CVPoint<double> dpt)
{ Start += dpt; End += dpt; }

void GeometricSegment::offset(double dx, double dy)
{ offset(CVPoint<double>(dx, dy)); }

bool GeometricSegment::isSegment()
{ return true; }

void GeometricSegment::draw(Graphics^ graphics, Pen^ pen, CVPoint<double>% translation)
{
	pen->Color = _mainColor;
	graphics->DrawLine(pen, (float)(_start.X + translation.X), (float)(translation.Y - _start.Y),
							(float)(_end.X + translation.X), (float)(translation.Y - _end.Y));
}

void GeometricSegment::fill(Graphics^ graphics, SolidBrush^ brush, CVPoint<double>% translation)
{
	brush->Color = Color::FromArgb(_mainColor.A >> 2, _mainColor);
	graphics->FillRectangle(brush, (float)(_boundingBox.Left + translation.X), (float)(translation.Y - _boundingBox.Top - _boundingBox.Height),
									(float)(_boundingBox.Width + 1), (float)(_boundingBox.Height + 1));
}

String^ GeometricSegment::ToString()
{
	return "Segment: [Start: (" + _start.X.ToString() + ", " + _start.Y.ToString() + "), End: (" + _end.X.ToString() + ", " + _end.Y.ToString() + ")]";
}

void GeometricSegment::_fixHorizontal()
{
	if(_start.X < _end.X) {
		_boundingBox.Left = _start.X;
		_boundingBox.Width = _end.X - _start.X;
	}
	else {
		_boundingBox.Left = _end.X;
		_boundingBox.Width = _start.X - _end.X;
	}
}

void GeometricSegment::_fixVertical()
{
	if(_start.Y < _end.Y) {
		_boundingBox.Top = _start.Y;
		_boundingBox.Height = _end.Y - _start.Y;
	}
	else {
		_boundingBox.Top = _end.Y;
		_boundingBox.Height = _start.Y - _end.Y;
	}
}