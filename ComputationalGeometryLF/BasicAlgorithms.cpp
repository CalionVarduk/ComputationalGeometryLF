#include "BasicAlgorithms.h"

using namespace ComputationalGeometryLF;

inline double BasicAlgorithms::orientationTest(double pX, double pY, double sStartX, double sStartY, double sEndX, double sEndY)
{
	return ((sStartX - pX) * (sEndY - pY)) - ((sEndX - pX) * (sStartY - pY));
}

inline double BasicAlgorithms::orientationTest(double pX, double pY, GeometricSegment^ s)
{ return orientationTest(pX, pY, s->StartX, s->StartY, s->EndX, s->EndY); }

inline double BasicAlgorithms::orientationTest(GeometricPoint^ p, GeometricPoint^ sStart, GeometricPoint^ sEnd)
{ return orientationTest(p->X, p->Y, sStart->X, sStart->Y, sEnd->X, sEnd->Y); }

inline double BasicAlgorithms::orientationTest(GeometricPoint^ p, GeometricSegment^ s)
{ return orientationTest(p->X, p->Y, s->StartX, s->StartY, s->EndX, s->EndY); }

Tuple<double, double>^ BasicAlgorithms::orientationTest(GeometricSegment^ s1, GeometricSegment^ s2)
{
	return Tuple::Create(orientationTest(s1->StartX, s1->StartY, s2), orientationTest(s1->EndX, s1->EndY, s2));
}

bool BasicAlgorithms::containmentTest(GeometricObject^ o1, GeometricObject^ o2)
{
	return ((o1->BoundingLeft >= o2->BoundingLeft) && (o1->BoundingRight <= o2->BoundingRight) &&
			(o1->BoundingBottom >= o2->BoundingBottom) && (o1->BoundingTop <= o2->BoundingTop));
}

bool BasicAlgorithms::pointContainmentTest(double pX, double pY, GeometricObject^ o)
{
	return ((pX >= o->BoundingLeft) && (pX <= o->BoundingRight) &&
			(pY >= o->BoundingBottom) && (pY <= o->BoundingTop));
}

inline bool BasicAlgorithms::pointContainmentTest(GeometricPoint^ p, GeometricObject^ o)
{ return pointContainmentTest(p->X, p->Y, o); }

bool BasicAlgorithms::belongingnessTest(double pX, double pY, GeometricSegment^ s)
{
	return (pointContainmentTest(pX, pY, s)) ? (orientationTest(pX, pY, s) == 0) : false;
}

inline bool BasicAlgorithms::belongingnessTest(GeometricPoint^ p, GeometricSegment^ s)
{ return belongingnessTest(p->X, p->Y, s); }

bool BasicAlgorithms::belongingnessTest(GeometricSegment^ s1, GeometricSegment^ s2)
{
	if(containmentTest(s1, s2)) {
		Tuple<double, double>^ orientation = orientationTest(s1, s2);
		return (orientation->Item1 == 0 && orientation->Item2 == 0);
	}
	return false;
}

bool BasicAlgorithms::boundingBoxIntersectionTest(GeometricObject^ o1, GeometricObject^ o2)
{
	return ((o1->BoundingRight >= o2->BoundingLeft) && (o1->BoundingLeft <= o2->BoundingRight) &&
			(o1->BoundingTop >= o2->BoundingBottom) && (o1->BoundingBottom <= o2->BoundingTop));
}

bool BasicAlgorithms::intersectionTest(GeometricSegment^ s1, GeometricSegment^ s2)
{
	if(boundingBoxIntersectionTest(s1, s2)) {
		return ((orientationTest(s1->StartX, s1->StartY, s2) * orientationTest(s1->EndX, s1->EndY, s2) <= 0) &&
				(orientationTest(s2->StartX, s2->StartY, s1) * orientationTest(s2->EndX, s2->EndY, s1) <= 0));
	}
	return false;
}

GeometricPoint^ BasicAlgorithms::getIntersectionPoint(GeometricSegment^ s1, GeometricSegment^ s2)
{
	if(intersectionTest(s1, s2)) {
		double a = (s1->EndY - s1->StartY) / (s1->EndX - s1->StartX);
		double b = s1->StartY - (s1->StartX * a);
		double c = (s2->EndY - s2->StartY) / (s2->EndX - s2->StartX);
		double d = s2->StartY - (s2->StartX * c);

		GeometricPoint^ intersection = gcnew GeometricPoint();
		intersection->X = (b - d) / (c - a);
		intersection->Y = b + (a * intersection->X);
		return intersection;
	}
	return nullptr;
}

List<GeometricPoint^>^ BasicAlgorithms::getPointsWithinBounds(GeometricRectangle^ bounds, bool includeNonPoints)
{
	List<GeometricPoint^>^ points = gcnew List<GeometricPoint^>(0);

	if(includeNonPoints) {
		for(Int32 i = 0; i < Toolbox::getObjectCount(); ++i) {
			GeometricObject^ obj = Toolbox::getObject(i);
			if(obj->isPoint()) _getPointsCheckPoint(bounds, obj->interpretAs<GeometricPoint>(), points);
			else if(obj->isSegment()) _getPointsCheckSegment(bounds, obj, points);
			else if(obj->isRectangle()) _getPointsCheckRectangle(bounds, obj, points);
			else if(obj->isPolygon()) _getPointsCheckPolygon(bounds, obj, points);
		}
	}
	else for(Int32 i = 0; i < Toolbox::getObjectCount(); ++i) {
		GeometricObject^ obj = Toolbox::getObject(i);
		if(obj->isPoint()) _getPointsCheckPoint(bounds, obj->interpretAs<GeometricPoint>(), points);
	}

	return points;
}

Int32 BasicAlgorithms::getBottomMostIndex(List<GeometricPoint^>^ points)
{
	Int32 index = 0;
	for(Int32 i = 1; i < points->Count; ++i) {
		if(points[i]->Y <= points[index]->Y) {
			if(points[i]->Y < points[index]->Y) index = i;
			else if(points[i]->X < points[index]->X) index = i;
		}
	}
	return index;
}

Int32 BasicAlgorithms::getTopMostIndex(List<GeometricPoint^>^ points)
{
	Int32 index = 0;
	for(Int32 i = 1; i < points->Count; ++i) {
		if(points[i]->Y >= points[index]->Y) {
			if(points[i]->Y > points[index]->Y) index = i;
			else if(points[i]->X > points[index]->X) index = i;
		}
	}
	return index;
}

Int32 BasicAlgorithms::getRightMostIndex(List<GeometricPoint^>^ points)
{
	Int32 index = 0;
	for(Int32 i = 1; i < points->Count; ++i) {
		if(points[i]->X >= points[index]->X) {
			if(points[i]->X > points[index]->X) index = i;
			else if(points[i]->Y > points[index]->Y) index = i;
		}
	}
	return index;
}

Int32 BasicAlgorithms::getLeftMostIndex(List<GeometricPoint^>^ points)
{
	Int32 index = 0;
	for(Int32 i = 1; i < points->Count; ++i) {
		if(points[i]->X <= points[index]->X) {
			if(points[i]->X < points[index]->X) index = i;
			else if(points[i]->Y < points[index]->Y) index = i;
		}
	}
	return index;
}

List<GeometricSegment^>^ BasicAlgorithms::getSegmentsWithinBounds(GeometricRectangle^ bounds, bool containedOnly)
{
	List<GeometricSegment^>^ segments = gcnew List<GeometricSegment^>(0);

	if(containedOnly) {
		for(Int32 i = 0; i < Toolbox::getObjectCount(); ++i) {
			GeometricObject^ obj = Toolbox::getObject(i);
			if(obj->isSegment() && containmentTest(obj, bounds))
				segments->Add(obj->interpretAs<GeometricSegment>());
		}
	}
	else for(Int32 i = 0; i < Toolbox::getObjectCount(); ++i) {
		GeometricObject^ obj = Toolbox::getObject(i);
		if(obj->isSegment()) {
			GeometricSegment^ segment = obj->interpretAs<GeometricSegment>();
			if(pointContainmentTest(segment->StartX, segment->StartY, bounds) || pointContainmentTest(segment->EndX, segment->EndY, bounds))
				segments->Add(segment);
		}
	}
	return segments;
}

List<GeometricSegment^>^ BasicAlgorithms::getPolygonSegments(GeometricPolygon^ p)
{
	List<GeometricSegment^>^ segments = gcnew List<GeometricSegment^>();

	for(Int32 i = 1; i < p->PointCount; ++i)
		segments->Add(gcnew GeometricSegment(p->getPointAt(i - 1), p->getPointAt(i)));
	segments->Add(gcnew GeometricSegment(p->getPointAt(p->PointCount - 1), p->getPointAt(0)));

	return segments;
}

List<GeometricPoint^>^ BasicAlgorithms::getPolygonPoints(GeometricPolygon^ p)
{
	List<GeometricPoint^>^ points = gcnew List<GeometricPoint^>();
	for(Int32 i = 0; i < p->PointCount; ++i)
		points->Add(gcnew GeometricPoint(p->getPointAt(i)));
	return points;
}

String^ BasicAlgorithms::interpretPointOrientationTest(double outcome)
{
	return (outcome < 0) ? "The POINT is on the RIGHT side of the SEGMENT." :
			(outcome > 0) ? "The POINT is on the LEFT side of the SEGMENT." :
							"The POINT and the SEGMENT are COLLINEAR.";
}

String^ BasicAlgorithms::interpretSegmentOrientationTest(Tuple<double, double>^ outcome)
{
	if(outcome->Item1 < 0) {
		return (outcome->Item2 < 0) ? "The 1ST SEGMENT is on the RIGHT side of the 2ND SEGMENT." :
				(outcome->Item2 > 0) ? "The 1ST SEGMENT's START and END lie on OPPOSITE sides of the 2ND SEGMENT." :
										"The 1ST SEGMENT's START is on the RIGHT side of the 2ND SEGMENT and END is COLLINEAR with the 2ND SEGMENT.";
	}
	else if(outcome->Item1 > 0) {
		return (outcome->Item2 > 0) ? "The 1ST SEGMENT is on the LEFT side of the 2ND SEGMENT." :
				(outcome->Item2 < 0) ? "The 1ST SEGMENT's START and END lie on OPPOSITE sides of the 2ND SEGMENT." :
										"The 1ST SEGMENT's START is on the LEFT side of the 2ND SEGMENT and END is COLLINEAR with the 2ND SEGMENT.";
	}

	return (outcome->Item2 < 0) ? "The 1ST SEGMENT's START is COLLINEAR with the 2ND SEGMENT and END is on the RIGHT side of the 2ND SEGMENT." :
			(outcome->Item2 > 0) ? "The 1ST SEGMENT's START is COLLINEAR with the 2ND SEGMENT and END is on the LEFT side of the 2ND SEGMENT." :
									"The SEGMENTS are COLLINEAR.";
}

void BasicAlgorithms::_getPointsCheckPoint(GeometricRectangle^ bounds, GeometricPoint^ point, List<GeometricPoint^>^ points)
{
	if(BasicAlgorithms::pointContainmentTest(point, bounds)) points->Add(point);
}

void BasicAlgorithms::_getPointsCheckSegment(GeometricRectangle^ bounds, GeometricObject^ obj, List<GeometricPoint^>^ points)
{
	GeometricSegment^ s = obj->interpretAs<GeometricSegment>();
	_getPointsCheckPoint(bounds, gcnew GeometricPoint(s->Start), points);
	_getPointsCheckPoint(bounds, gcnew GeometricPoint(s->End), points);
}

void BasicAlgorithms::_getPointsCheckRectangle(GeometricRectangle^ bounds, GeometricObject^ obj, List<GeometricPoint^>^ points)
{
	if(!GeometricObject::ReferenceEquals(obj, bounds)) {
		GeometricRectangle^ r = obj->interpretAs<GeometricRectangle>();
		_getPointsCheckPoint(bounds, gcnew GeometricPoint(r->Left, r->Bottom), points);
		_getPointsCheckPoint(bounds, gcnew GeometricPoint(r->Right, r->Bottom), points);
		_getPointsCheckPoint(bounds, gcnew GeometricPoint(r->Right, r->Top), points);
		_getPointsCheckPoint(bounds, gcnew GeometricPoint(r->Left, r->Top), points);
	}
}

void BasicAlgorithms::_getPointsCheckPolygon(GeometricRectangle^ bounds, GeometricObject^ obj, List<GeometricPoint^>^ points)
{
	GeometricPolygon^ p = obj->interpretAs<GeometricPolygon>();
	for(Int32 i = 0; i < p->PointCount; ++i)
		_getPointsCheckPoint(bounds, gcnew GeometricPoint(p->getPointAt(i)), points);
}