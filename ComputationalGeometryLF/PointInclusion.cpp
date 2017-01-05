#include "PointInclusion.h"

using namespace ComputationalGeometryLF;

// http://geomalgorithms.com/a03-_inclusion.html
bool PointInclusion::IsInsidePolygon(double pX, double pY, GeometricPolygon^ poly)
{
	GeometricPolygon^ P = Triangulation::GetCounterClockwiseOrdered(poly);
	Int32 wn = 0;

	Int32 count = P->PointCount - 1;
	for(Int32 i = 0; i < count; ++i)
		wn += _testEdge(pX, pY, P, i, i + 1);
	wn += _testEdge(pX, pY, P, count, 0);

	return (wn != 0);
}

inline bool PointInclusion::IsInsidePolygon(GeometricPoint^ p, GeometricPolygon^ poly)
{ return IsInsidePolygon(p->X, p->Y, poly); }

bool PointInclusion::IsInsideConvexPolygon(double pX, double pY, GeometricPolygon^ poly)
{
	List<GeometricPoint^>^ points = BasicAlgorithms::getPolygonPoints(Triangulation::GetCounterClockwiseOrdered(poly));
	Int32 left = BasicAlgorithms::getLeftMostIndex(points);
	Int32 right = BasicAlgorithms::getRightMostIndex(points);

	if(pX < points[left]->X  || pX > points[right]->X) return false;

	List<GeometricPoint^>^ bottomChain = gcnew List<GeometricPoint^>();
	List<GeometricPoint^>^ topChain = gcnew List<GeometricPoint^>();

	if(left < right) {
		bottomChain->AddRange(points->GetRange(left, right - left + 1));
		topChain->AddRange(points->GetRange(right, points->Count - right));
		topChain->AddRange(points->GetRange(0, left + 1));
	}
	else {
		topChain->AddRange(points->GetRange(right, left - right + 1));
		bottomChain->AddRange(points->GetRange(left, points->Count - left));
		bottomChain->AddRange(points->GetRange(0, right + 1));
	}

	Int32 b1 = _searchIncr(bottomChain, pX);
	if(b1 == 0) return false;
	Int32 b0 = b1 - 1;

	Int32 t0 = _searchDecr(topChain, pX);
	if(t0 == topChain->Count - 1) return false;
	Int32 t1 = t0 + 1;

	bool isLeftToBottom = BasicAlgorithms::orientationTest(gcnew GeometricPoint(pX, pY), bottomChain[b0], bottomChain[b1]) > 0;
	bool isLeftToTop = BasicAlgorithms::orientationTest(gcnew GeometricPoint(pX, pY), topChain[t0], topChain[t1]) > 0;

	return (isLeftToBottom && isLeftToTop);
}

inline bool PointInclusion::IsInsideConvexPolygon(GeometricPoint^ p, GeometricPolygon^ poly)
{ return IsInsideConvexPolygon(p->X, p->Y, poly); }

inline Int32 PointInclusion::_testEdge(double pX, double pY, GeometricPolygon^ P, int iStart, int iEnd)
{
	CVPoint<double> vs = P->getPointAt(iStart);
	CVPoint<double> ve = P->getPointAt(iEnd);

	if(vs.Y <= pY) {
		if(ve.Y > pY && BasicAlgorithms::orientationTest(pX, pY, vs.X, vs.Y, ve.X, ve.Y) > 0)
			return 1;
	}
	else if(ve.Y <= pY && BasicAlgorithms::orientationTest(pX, pY, vs.X, vs.Y, ve.X, ve.Y) < 0)
		return -1;

	return 0;
}

inline Int32 PointInclusion::_searchIncr(List<GeometricPoint^>^ list, double x)
{
	Int32 l = 0, r = list->Count - 1;

	while(l < r) {
		Int32 m = (l + r) >> 1;
		if(list[m]->X >= x) r = m - 1;
		else l = m + 1;
	}
	return l;
}

inline Int32 PointInclusion::_searchDecr(List<GeometricPoint^>^ list, double x)
{
	Int32 l = 0, r = list->Count - 1;

	while(l < r) {
		Int32 m = (l + r) >> 1;
		if(list[m]->X <= x) r = m - 1;
		else l = m + 1;
	}
	return l;
}