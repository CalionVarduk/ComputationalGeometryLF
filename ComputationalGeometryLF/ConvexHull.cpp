#include "ConvexHull.h"

using namespace ComputationalGeometryLF;

GeometricPolygon^ ConvexHull::grahamScan(GeometricRectangle^ bounds, bool includeNonPoints)
{
	List<GeometricPoint^>^ points = BasicAlgorithms::getPointsWithinBounds(bounds, includeNonPoints);
	//points = (List<GeometricPoint^>^)Enumerable::Distinct(points);

	if(points->Count >= 3) {
		GeometricPoint^ p0 = points[BasicAlgorithms::getBottomMostIndex(points)];
		points->Sort(gcnew GeometricPoint::ComparerAngleCoords(p0));
		CVStack<GeometricPoint^>^ S = _grahamBuildStack(points);
		return _buildHullFromStack(S);
	}
	return gcnew GeometricPolygon();
}

GeometricPolygon^ ConvexHull::jarvisMarch(GeometricRectangle^ bounds, bool includeNonPoints)
{
	List<GeometricPoint^>^ points = BasicAlgorithms::getPointsWithinBounds(bounds, includeNonPoints);
	//points = (List<GeometricPoint^>^)Enumerable::Distinct(points);

	if(points->Count >= 3) {
		GeometricPoint^ bottom = points[BasicAlgorithms::getBottomMostIndex(points)];
		GeometricPoint^ top = points[BasicAlgorithms::getTopMostIndex(points)];
		CVStack<GeometricPoint^>^ S = gcnew CVStack<GeometricPoint^>();

		_jarvisGoUp(points, bottom, top, S);
		_jarvisGoDown(points, bottom, top, S);

		S->pop();
		return _buildHullFromStack(S);
	}
	return gcnew GeometricPolygon();
}

GeometricPolygon^ ConvexHull::quickHull(GeometricRectangle^ bounds, bool includeNonPoints)
{
	List<GeometricPoint^>^ points = BasicAlgorithms::getPointsWithinBounds(bounds, includeNonPoints);
	if(points->Count >= 3) {
		GeometricPoint^ A = points[BasicAlgorithms::getBottomMostIndex(points)];
		GeometricPoint^ B = points[BasicAlgorithms::getTopMostIndex(points)];
		
		Int32 middle = _quickArrange(points, 0, points->Count - 1, A, B);
		List<GeometricPoint^>^ right = _quickRecur(points, 0, middle, A, B);
		List<GeometricPoint^>^ left = _quickRecur(points, middle + 1, points->Count - 1, B, A);

		points->Clear();
		points->Add(A);
		points->AddRange(left);
		points->Add(B);
		points->AddRange(right);

		GeometricPoint^ p0 = points[BasicAlgorithms::getBottomMostIndex(points)];
		points->Sort(gcnew GeometricPoint::ComparerAngleCoords(p0));
		CVStack<GeometricPoint^>^ S = _grahamBuildStack(points);
		return _buildHullFromStack(S);
	}
	return gcnew GeometricPolygon();
}

CVStack<GeometricPoint^>^ ConvexHull::_grahamBuildStack(List<GeometricPoint^>^ points)
{
	CVStack<GeometricPoint^>^ S = gcnew CVStack<GeometricPoint^>();
	for(Int32 i = 0; i < 3; ++i) S->push(points[i]);

	for(Int32 i = 3; i < points->Count; ++i) {
		while(S->Count > 1 && BasicAlgorithms::orientationTest(S->at(1), S->top(), points[i]) <= 0)
			S->pop();
		S->push(points[i]);
	}
	return S;
}

GeometricPolygon^ ConvexHull::_buildHullFromStack(CVStack<GeometricPoint^>^ S)
{
	GeometricPolygon^ poly = gcnew GeometricPolygon(S->pop());
	while(!S->IsEmpty) poly->addPoint(S->pop());
	return poly;
}

void ConvexHull::_jarvisGoUp(List<GeometricPoint^>^ points, GeometricPoint^ bottom, GeometricPoint^ top, CVStack<GeometricPoint^>^ S)
{
	GeometricPoint::ComparerAngle^ angleFinder = gcnew GeometricPoint::ComparerAngle(bottom);
	GeometricPoint::ComparerDistance^ distFinder = gcnew GeometricPoint::ComparerDistance(bottom);
	GeometricPoint^ current = bottom;
	S->push(current);

	while(!GeometricPoint::ReferenceEquals(current, top)) {
		Int32 i = 0;
		while(points[i]->Y < current->Y || GeometricPoint::ReferenceEquals(points[i], current))
			++i;

		GeometricPoint^ nextHullPoint = points[i];
		while(i < points->Count) {
			if(points[i]->Y >= current->Y && !GeometricPoint::ReferenceEquals(points[i], current)) {
				Int32 angleCmp = angleFinder->Compare(nextHullPoint, points[i]);
				if(angleCmp >= 0)
					if(angleCmp > 0 || distFinder->Compare(nextHullPoint, points[i]) < 0)
						nextHullPoint = points[i];
			}
			++i;
		}

		current = nextHullPoint;
		S->push(current);
		angleFinder->Origin = current;
		distFinder->Origin = current;
	}
}

void ConvexHull::_jarvisGoDown(List<GeometricPoint^>^ points, GeometricPoint^ bottom, GeometricPoint^ top, CVStack<GeometricPoint^>^ S)
{
	GeometricPoint::ComparerAngle^ angleFinder = gcnew GeometricPoint::ComparerAngle(top);
	GeometricPoint::ComparerDistance^ distFinder = gcnew GeometricPoint::ComparerDistance(top);
	GeometricPoint^ current = top;
	S->push(current);

	while(!GeometricPoint::ReferenceEquals(current, bottom)) {
		Int32 i = 0;
		while(points[i]->Y > current->Y || GeometricPoint::ReferenceEquals(points[i], current))
			++i;

		GeometricPoint^ nextHullPoint = points[i];
		while(i < points->Count) {
			if(points[i]->Y <= current->Y && !GeometricPoint::ReferenceEquals(points[i], current)) {
				Int32 angleCmp = angleFinder->Compare(nextHullPoint, points[i]);
				if(angleCmp >= 0)
					if(angleCmp > 0 || distFinder->Compare(nextHullPoint, points[i]) < 0)
						nextHullPoint = points[i];
			}
			++i;
		}

		current = nextHullPoint;
		S->push(current);
		angleFinder->Origin = current;
		distFinder->Origin = current;
	}
}

Int32 ConvexHull::_quickArrange(List<GeometricPoint^>^ points, Int32 left, Int32 right, GeometricPoint^ sStart, GeometricPoint^ sEnd)
{
	Int32 middle = left - 1;

	for(Int32 i = left; i <= right; ++i) {
		if(BasicAlgorithms::orientationTest(points[i], sStart, sEnd) < 0) {
			GeometricPoint^ t = points[++middle];
			points[middle] = points[i];
			points[i] = t;
		}
	}
	return middle;
}

Tuple<GeometricPoint^, double>^ ConvexHull::_quickFindFarthest(List<GeometricPoint^>^ points, Int32 left, Int32 right, GeometricPoint^ sStart, GeometricPoint^ sEnd)
{
	GeometricPoint^ farthest = points[left];
	double orientF = BasicAlgorithms::orientationTest(farthest, sStart, sEnd);

	for(Int32 i = 1; i <= right; ++i) {
		double orientP = BasicAlgorithms::orientationTest(points[i], sStart, sEnd);
		if(orientP < orientF) {
			farthest = points[i];
			orientF = orientP;
		}
	}
	return Tuple::Create(farthest, orientF);
}

List<GeometricPoint^>^ ConvexHull::_quickRecur(List<GeometricPoint^>^ points, Int32 left, Int32 right, GeometricPoint^ sStart, GeometricPoint^ sEnd)
{
	List<GeometricPoint^>^ list = gcnew List<GeometricPoint^>();

	if(left <= right) {
		Tuple<GeometricPoint^, double>^ farRight = _quickFindFarthest(points, left, right, sStart, sEnd);

		if(farRight->Item2 < 0) {
			Int32 middle1 = _quickArrange(points, left, right, sStart, farRight->Item1);
			Int32 middle2 = _quickArrange(points, middle1 + 1, right, farRight->Item1, sEnd);

			if(middle1 >= left) list->AddRange(_quickRecur(points, left, middle1, sStart, farRight->Item1));
			else list->Add(sStart);
			list->Add(farRight->Item1);
			if(middle2 > middle1) list->AddRange(_quickRecur(points, middle1 + 1, middle2, farRight->Item1, sEnd));
			else list->Add(sEnd);
			return list;
		}
	}
	//list->Add(sStart);
	//list->Add(sEnd);
	return list;
}