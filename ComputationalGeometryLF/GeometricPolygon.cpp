#include "GeometricPolygon.h"

using namespace ComputationalGeometryLF;

GeometricPolygon::GeometricPolygon()
	: GeometricObject(), _points(gcnew List<CVPoint<double>>(2))
{}

GeometricPolygon::GeometricPolygon(GeometricPoint^ origin)
	: GeometricObject(), _points(gcnew List<CVPoint<double>>(2))
{
	_points->Add(origin->Location);
	_fixBoundingBox();
}

GeometricPolygon::GeometricPolygon(CVPoint<double> origin)
	: GeometricObject(), _points(gcnew List<CVPoint<double>>(2))
{
	_points->Add(origin);
	_fixBoundingBox();
}

GeometricPolygon::GeometricPolygon(double originX, double originY)
	: GeometricObject(), _points(gcnew List<CVPoint<double>>(2))
{
	_points->Add(CVPoint<double>(originX, originY));
	_fixBoundingBox();
}

void GeometricPolygon::reset(GeometricPoint^ origin)
{ reset(origin->Location); }

void GeometricPolygon::reset(CVPoint<double> origin)
{
	_points->Clear();
	_points->Add(origin);
	_fixBoundingBox();
}

void GeometricPolygon::reset(double originX, double originY)
{ reset(CVPoint<double>(originX, originY)); }

void GeometricPolygon::offset(GeometricPoint^ dpt)
{ offset(dpt->X, dpt->Y); }

void GeometricPolygon::offset(CVPoint<double> dpt)
{ offset(dpt.X, dpt.Y); }

void GeometricPolygon::offset(double dx, double dy)
{
	_boundingBox.Left += dx;
	_boundingBox.Top += dx;
	for(Int32 i = 0; i < _points->Count; ++i)
		_points[i].offset(dx, dy);
}

void GeometricPolygon::addPoint(GeometricPoint^ pt)
{ addPoint(pt->Location); }

void GeometricPolygon::addPoint(CVPoint<double> pt)
{
	_points->Add(pt);
	_fixBoundingBox();
}

void GeometricPolygon::addPoint(double x, double y)
{ addPoint(CVPoint<double>(x, y)); }

CVPoint<double> GeometricPolygon::getPointAt(Int32 i)
{ return _points[i]; }

void GeometricPolygon::modPointAt(Int32 i, double newX, double newY)
{
	_points[i] = CVPoint<double>(newX, newY);
	_fixBoundingBox();
}

bool GeometricPolygon::isPolygon()
{ return true; }

void GeometricPolygon::draw(Graphics^ graphics, Pen^ pen, CVPoint<double>% translation)
{
	array<PointF>^ translPoints = gcnew array<PointF>(PointCount);
	for(Int32 i = 0; i < PointCount; ++i) {
		translPoints[i].X = (float)(_points[i].X + translation.X);
		translPoints[i].Y = (float)(translation.Y - _points[i].Y);
	}

	pen->Color = _mainColor;
	graphics->DrawPolygon(pen, translPoints);
}

void GeometricPolygon::fill(Graphics^ graphics, SolidBrush^ brush, CVPoint<double>% translation)
{
	brush->Color = Color::FromArgb(_mainColor.A >> 2, _mainColor);
	graphics->FillRectangle(brush, (float)(_boundingBox.Left + translation.X), (float)(translation.Y - _boundingBox.Top - _boundingBox.Height),
									(float)(_boundingBox.Width + 1), (float)(_boundingBox.Height + 1));
}

String^ GeometricPolygon::ToString()
{
	StringBuilder^ sb = gcnew StringBuilder("Polygon: [");
	for(Int32 i = 0; i < PointCount; ++i)
		sb->Append("(" + _points[i].X.ToString() + ", " + _points[i].Y.ToString() + "), ");
	sb->Remove(sb->Length - 2, 2);
	return sb->Append("]")->ToString();
}

void GeometricPolygon::_fixBoundingBox()
{
	if(PointCount > 0) {
		double minX = _points[0].X;
		double maxX = _points[0].X;
		double minY = _points[0].Y;
		double maxY = _points[0].Y;

		for(Int32 i = 1; i < PointCount; ++i) {
			CVPoint<double> pt = _points[i];
			if(pt.X < minX) minX = pt.X;
			if(pt.X > maxX) maxX = pt.X;
			if(pt.Y < minY) minY = pt.Y;
			if(pt.Y > maxY) maxY = pt.Y;
		}
		_boundingBox = CVRectangle<double>::FromLTRB(minX, minY, maxX, maxY);
	}
	else _boundingBox = CVRectangle<double>();
}