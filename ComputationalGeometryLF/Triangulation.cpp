#include "Triangulation.h"

using namespace ComputationalGeometryLF;

GeometricPolygon^ Triangulation::GetCounterClockwiseOrdered(GeometricPolygon^ p)
{
	double area = _signedArea(p);
	if(area < 0) {
		GeometricPolygon^ ccwp = gcnew GeometricPolygon();
		for(Int32 i = p->PointCount - 1; i >= 0; --i)
			ccwp->addPoint(p->getPointAt(i));
		return ccwp;
	}
	return p;
}

bool Triangulation::isPolygonMonotoneY(GeometricPolygon^ p)
{
	List<GeometricPoint^>^ points = BasicAlgorithms::getPolygonPoints(GetCounterClockwiseOrdered(p));
	Int32 upper = BasicAlgorithms::getTopMostIndex(points);
	Int32 lower = BasicAlgorithms::getBottomMostIndex(points);
	double Y = points[upper]->Y;

	if(upper > lower) {
		for(Int32 i = upper + 1; i < points->Count; ++i) {
			if(points[i]->Y <= Y) Y = points[i]->Y;
			else return false;
		}
		for(Int32 i = 0; i <= lower; ++i) {
			if(points[i]->Y <= Y) Y = points[i]->Y;
			else return false;
		}
		for(Int32 i = lower + 1; i <= upper; ++i) {
			if(points[i]->Y >= Y) Y = points[i]->Y;
			else return false;
		}
	}
	else {
		for(Int32 i = upper + 1; i <= lower; ++i) {
			if(points[i]->Y <= Y) Y = points[i]->Y;
			else return false;
		}
		for(Int32 i = lower + 1; i < points->Count; ++i) {
			if(points[i]->Y >= Y) Y = points[i]->Y;
			else return false;
		}
		for(Int32 i = 0; i <= upper; ++i) {
			if(points[i]->Y >= Y) Y = points[i]->Y;
			else return false;
		}
	}
	return true;
}

TriangulatedPolygon^ Triangulation::TriangulateNaive(GeometricPolygon^ p)
{
	if(!IntersectionPoints::isPolygonSimple(p)) return nullptr;

	TriangulatedPolygon^ poly = gcnew TriangulatedPolygon(p);
	List<GeometricPoint^>^ points = BasicAlgorithms::getPolygonPoints(GetCounterClockwiseOrdered(p));
	for(Int32 i = 0; i < points->Count; ++i) points[i]->MainColor = Color::White;

	poly->TriangleGraphRoot->Points = points;
	_recTriangulate(poly->TriangleGraphRoot, poly->Segments);
	return poly;
}

TriangulatedPolygon^ Triangulation::TriangulateMonotoneY(GeometricPolygon^ p)
{
	if(!isPolygonMonotoneY(p)) return nullptr;

	List<MonoPoint^>^ points = _getSortedMonoPoints(BasicAlgorithms::getPolygonPoints(GetCounterClockwiseOrdered(p)));
	List<GeometricSegment^>^ data = gcnew List<GeometricSegment^>();
	CVStack<MonoPoint^>^ S = gcnew CVStack<MonoPoint^>();

	Int32 countM1 = points->Count - 1;
	S->push(points[0]);
	S->push(points[1]);

	for(Int32 i = 2; i < countM1; ++i) {
		MonoPoint^ p = points[i];
		MonoPoint^ last = S->top();

		if(last->OnLeftChain != p->OnLeftChain) {
			while(S->Count > 1)
				data->Add(gcnew GeometricSegment(p->Point->Location, S->pop()->Point->Location));
			S->pop();
		}
		else {
			S->pop();
			if(p->OnLeftChain) {
				while(!S->IsEmpty && BasicAlgorithms::orientationTest(last->Point, S->top()->Point, p->Point) < 0) {
					last = S->pop();
					data->Add(gcnew GeometricSegment(last->Point->Location, p->Point->Location));
				}
			}
			else {
				while(!S->IsEmpty && BasicAlgorithms::orientationTest(last->Point, S->top()->Point, p->Point) > 0) {
					last = S->pop();
					data->Add(gcnew GeometricSegment(last->Point->Location, p->Point->Location));
				}
			}
			
		}
		S->push(last);
		S->push(p);
	}

	S->pop();
	while(S->Count > 1)
		data->Add(gcnew GeometricSegment(points[countM1]->Point->Location, S->pop()->Point->Location));

	TriangulatedPolygon^ poly = gcnew TriangulatedPolygon(p);
	poly->Segments = data;
	return poly;
}

List<GeometricPoint^>^ Triangulation::GetPolygonVertexTypes(GeometricPolygon^ p)
{
	List<GeometricPoint^>^ points = BasicAlgorithms::getPolygonPoints(GetCounterClockwiseOrdered(p));
	List<GeometricPoint^>^ starts = gcnew List<GeometricPoint^>();
	List<GeometricPoint^>^ splits = gcnew List<GeometricPoint^>();
	List<GeometricPoint^>^ ends = gcnew List<GeometricPoint^>();
	List<GeometricPoint^>^ merges = gcnew List<GeometricPoint^>();

	for(Int32 i = 0; i < points->Count; ++i) {
		GeometricPoint^ p = points[i];
		GeometricPoint^ prev = points[(i == 0) ? points->Count - 1 : i - 1];
		GeometricPoint^ next = points[(i == points->Count - 1) ? 0 : i + 1];

		if(_monoBelow(prev, p) && _monoBelow(next, p)) {
			double orient = BasicAlgorithms::orientationTest(p, prev, next);
			if(orient < 0) {
				p->MainColor = StartPointColor;
				starts->Add(p);
			}
			else if(orient > 0) {
				p->MainColor = SplitPointColor;
				splits->Add(p);
			}
		}
		else if(_monoAbove(prev, p) && _monoAbove(next, p)) {
			double orient = BasicAlgorithms::orientationTest(p, prev, next);
			if(orient < 0) {
				p->MainColor = EndPointColor;
				ends->Add(p);
			}
			else if(orient > 0) {
				p->MainColor = MergePointColor;
				merges->Add(p);
			}
		}
	}
	
	points->Clear();
	points->AddRange(starts);
	points->AddRange(splits);
	points->AddRange(ends);
	points->AddRange(merges);
	return points;
}

List<GeometricPoint^>^ Triangulation::GetTrichromaticPoints(GeometricPolygon^ p)
{
	TriangulatedPolygon^ tp = TriangulateNaive(p);
	if(tp == nullptr) return nullptr;

	List<GeometricPoint^>^ points = gcnew List<GeometricPoint^>(p->PointCount + 1);
	
	tp->TriangleGraphRoot->Points[0]->MainColor = Color::Green;
	tp->TriangleGraphRoot->Points[1]->MainColor = Color::Red;
	tp->TriangleGraphRoot->Points[2]->MainColor = Color::Blue;
	points->AddRange(tp->TriangleGraphRoot->Points);

	for(Int32 i = 0; i < tp->TriangleGraphRoot->Neighbours->Count; ++i)
		_traverseTriangleGraph(tp->TriangleGraphRoot->Neighbours[i], tp->TriangleGraphRoot, points);
	return points;
}

double Triangulation::_signedArea(GeometricPolygon^ p)
{
	double area = 0;

	if(p->PointCount > 2) {
		CVPoint<double> p1 = p->getPointAt(p->PointCount - 1);
		CVPoint<double> p2 = p->getPointAt(0);
		area = (p1.X * p2.Y) - (p2.X * p1.Y);

		for(Int32 i = 1; i < p->PointCount; ++i) {
			p1 = p->getPointAt(i - 1);
			p2 = p->getPointAt(i);
			area += (p1.X * p2.Y) - (p2.X * p1.Y);
		}
		area *= 0.5;
	}
	return area;
}

bool Triangulation::_monoBelow(GeometricPoint^ p1, GeometricPoint^ p2)
{
	return (p1->Y < p2->Y) || (p1->Y == p2->Y && p1->X > p2->X);
}

bool Triangulation::_monoAbove(GeometricPoint^ p1, GeometricPoint^ p2)
{
	return (p1->Y > p2->Y) || (p1->Y == p2->Y && p1->X < p2->X);
}

List<MonoPoint^>^ Triangulation::_getSortedMonoPoints(List<GeometricPoint^>^ points)
{
	Int32 upper = BasicAlgorithms::getTopMostIndex(points);
	Int32 lower = BasicAlgorithms::getBottomMostIndex(points);
	Int32 currentLeft = (upper == points->Count - 1) ? 0 : upper + 1;
	Int32 currentRight = (upper == 0) ? points->Count - 1 : upper - 1;

	List<MonoPoint^>^ mPoints = gcnew List<MonoPoint^>();
	mPoints->Add(gcnew MonoPoint(points[upper], true));

	if(upper > lower) {
		while(currentLeft != lower && currentRight != lower) {
			if(_monoAbove(points[currentLeft], points[currentRight])) {
				mPoints->Add(gcnew MonoPoint(points[currentLeft], true));
				currentLeft = ((currentLeft == points->Count - 1) ? 0 : currentLeft + 1);
			}
			else {
				mPoints->Add(gcnew MonoPoint(points[currentRight], false));
				--currentRight;
			}
		}
		while(currentLeft != lower) {
			mPoints->Add(gcnew MonoPoint(points[currentLeft], true));
			currentLeft = ((currentLeft == points->Count - 1) ? 0 : currentLeft + 1);
		}
		while(currentRight != lower) {
			mPoints->Add(gcnew MonoPoint(points[currentRight], false));
			--currentRight;
		}
	}
	else {
		while(currentLeft != lower && currentRight != lower) {
			if(_monoAbove(points[currentLeft], points[currentRight])) {
				mPoints->Add(gcnew MonoPoint(points[currentLeft], true));
				++currentLeft;
			}
			else {
				mPoints->Add(gcnew MonoPoint(points[currentRight], false));
				currentRight = ((currentRight == 0) ? points->Count - 1 : currentRight - 1);
			}
		}
		while(currentLeft != lower) {
			mPoints->Add(gcnew MonoPoint(points[currentLeft], true));
			++currentLeft;
		}
		while(currentRight != lower) {
			mPoints->Add(gcnew MonoPoint(points[currentRight], false));
			currentRight = ((currentRight == 0) ? points->Count - 1 : currentRight - 1);
		}
	}

	mPoints->Add(gcnew MonoPoint(points[lower], false));
	return mPoints;
}

void Triangulation::_setTrichromaticColor(GeometricPoint^ toSet, GeometricPoint^ set1, GeometricPoint^ set2, List<GeometricPoint^>^ OUT_points)
{
	if(set1->MainColor == Color::Green)
		toSet->MainColor = (set2->MainColor == Color::Red) ? Color::Blue : Color::Red;
	else if(set1->MainColor == Color::Red)
		toSet->MainColor = (set2->MainColor == Color::Green) ? Color::Blue : Color::Green;
	else
		toSet->MainColor = (set2->MainColor == Color::Red) ? Color::Green : Color::Red;

	OUT_points->Add(toSet);
}

void Triangulation::_traverseTriangleGraph(TriangulationRegion^ node, TriangulationRegion^ parent, List<GeometricPoint^>^ OUT_points)
{
	List<GeometricPoint^>^ nPts = node->Points;
	if(nPts[0]->MainColor == Color::White) _setTrichromaticColor(nPts[0], nPts[1], nPts[2], OUT_points);
	else if(nPts[1]->MainColor == Color::White) _setTrichromaticColor(nPts[1], nPts[0], nPts[2], OUT_points);
	else _setTrichromaticColor(nPts[2], nPts[0], nPts[1], OUT_points);

	for(Int32 i = 0; i < node->Neighbours->Count; ++i)
		if(node->Neighbours[i] != parent) _traverseTriangleGraph(node->Neighbours[i], node, OUT_points);
}

bool Triangulation::_regionNeighbourCheck(TriangulationRegion^ r1, TriangulationRegion^ r2)
{
	Int32 sharedVertexes = 0;
	for(Int32 i = 0; i < r1->Points->Count; ++i) {
		for(Int32 j = 0; j < r2->Points->Count; ++j) {
			if(r1->Points[i] == r2->Points[j]) {
				if(++sharedVertexes > 1) return true;
				break;
			}
		}
	}
	return false;
}

void Triangulation::_fixGraphRegions(TriangulationRegion^ newRegion, TriangulationRegion^ currentRegion)
{
	Int32 i = 0, j = currentRegion->Neighbours->Count - 1;
	while(i <= j) {
		if(_regionNeighbourCheck(newRegion, currentRegion->Neighbours[i])) {
			newRegion->Neighbours->Add(currentRegion->Neighbours[i]);
			currentRegion->Neighbours[i]->Neighbours->Add(newRegion);
			currentRegion->Neighbours[i]->Neighbours->Remove(currentRegion);
			currentRegion->Neighbours->RemoveAt(i);
			--j;
		}
		else ++i;
	}
	currentRegion->Neighbours->Add(newRegion);
	newRegion->Neighbours->Add(currentRegion);
}

void Triangulation::_recTriangulate(TriangulationRegion^ current, List<GeometricSegment^>^ segments)
{
	List<GeometricPoint^>^ points = current->Points;
	if(points->Count < 4) return;
	Int32 leftMost = BasicAlgorithms::getLeftMostIndex(points);
	Int32 next = (leftMost == points->Count - 1) ? 0 : leftMost + 1;
	Int32 prev = (leftMost == 0) ? points->Count - 1 : leftMost - 1;

	Int32 furthest = -1;
	double orientF = -1;

	for(Int32 i = 0; i < points->Count; ++i) {
		if(i != next && i != prev && i != leftMost) {
			double orientP = BasicAlgorithms::orientationTest(points[i], points[next], points[prev]);
			if(orientP > 0 && orientP > orientF) {
				double orientP1 = BasicAlgorithms::orientationTest(points[i], points[leftMost], points[next]);
				double orientP2 = BasicAlgorithms::orientationTest(points[i], points[prev], points[leftMost]);
				if(orientP1 > 0 && orientP2 > 0) {
					furthest = i;
					orientF = orientP;
				}
			}
		}
	}
	
	if(furthest == -1) {
		segments->Add(gcnew GeometricSegment(points[next]->Location, points[prev]->Location));

		TriangulationRegion^ triangle = gcnew TriangulationRegion();
		triangle->Points->Add(points[leftMost]);
		triangle->Points->Add(points[next]);
		triangle->Points->Add(points[prev]);
		points->RemoveAt(leftMost);

		_fixGraphRegions(triangle, current);
		_recTriangulate(current, segments);
	}
	else {
		segments->Add(gcnew GeometricSegment(points[furthest]->Location, points[leftMost]->Location));
		TriangulationRegion^ current2 = gcnew TriangulationRegion();

		if(furthest < leftMost) {
			current2->Points = points->GetRange(furthest, leftMost - furthest + 1);
			points->RemoveRange(furthest + 1, leftMost - furthest - 1);
		}
		else {
			current2->Points = points->GetRange(leftMost, furthest - leftMost + 1);
			points->RemoveRange(leftMost + 1, furthest - leftMost - 1);
		}

		_fixGraphRegions(current2, current);
		_recTriangulate(current, segments);
		_recTriangulate(current2, segments);
	}
}