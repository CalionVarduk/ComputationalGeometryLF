#include "GeometricObject.h"

using namespace ComputationalGeometryLF;

GeometricObject::GeometricObject()
	: _isDrawn(true), _boundingBox(0, 0, 0, 0), _mainColor(Color::Blue), _style(DrawStyle::Draw)
{}

bool GeometricObject::isPoint()
{ return false; }

bool GeometricObject::isSegment()
{ return false; }

bool GeometricObject::isRectangle()
{ return false; }

bool GeometricObject::isEllipse()
{ return false; }

bool GeometricObject::isPolygon()
{ return false; }

inline bool GeometricObject::isInView(CVRectangle<double>% view)
{
	return (_boundingBox.Right >= view.Left && _boundingBox.Left < view.Right) &&
			(_boundingBox.Bottom >= view.Top && _boundingBox.Top < view.Bottom);
}