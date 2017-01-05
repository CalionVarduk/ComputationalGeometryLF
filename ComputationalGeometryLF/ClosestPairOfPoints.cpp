#include "ClosestPairOfPoints.h"

using namespace ComputationalGeometryLF;

PointPair^ ClosestPairOfPoints::findByTree(GeometricRectangle^ bounds, bool includeNonPoints)
{
	_initStructures(bounds, includeNonPoints);

	while(iCurrent < points->Count) {
		current = points[iCurrent++];
		neighbours = tree->getRange(current, 4, 4);

		_updateDelta();
		_updateActivePoints();

		tree->insert(current);
	}

	PointPair^ outcome = Tuple::Create(A, B, Math::Sqrt(delta));
	_cleanUp();
	return outcome;
}

PointPair^ ClosestPairOfPoints::findByMerge(GeometricRectangle^ bounds, bool includeNonPoints)
{
	_initStructures(bounds, includeNonPoints);
	pAux = gcnew array<GeometricPoint^>(points->Count);

	_mergePairs();
	Int32 halfLength = 2;
	Int32 fullLength, lastLength, trimmedLength;

	while(halfLength < points->Count) {
		fullLength = halfLength << 1;
		lastLength = points->Count & (fullLength - 1);
		trimmedLength = points->Count - lastLength;

		for(Int32 i = 0; i < trimmedLength; i += fullLength)
			_recurPair(i, i + halfLength, i + fullLength);

		if(lastLength > halfLength)
			_recurPair(trimmedLength, trimmedLength + halfLength, points->Count);

		halfLength = fullLength;
	}
	return Tuple::Create(A, B, Math::Sqrt(delta));
}

void ClosestPairOfPoints::_mergePairs()
{
	for(Int32 i = 1, j; i < points->Count; i += 2) {
		j = i - 1;
		double newDelta = points[j]->getDistanceSquaredTo(points[i]);
		if(newDelta < delta) {
			delta = newDelta;
			A = points[j];
			B = points[i];
		}
		if(points[j] > points[i]) {
			GeometricPoint^ temp = points[j];
			points[j] = points[i];
			points[i] = temp;
		}
	}
}

void ClosestPairOfPoints::_updateDelta(Int32 firstNeighbour, Int32 lastNeighbour)
{
	for(Int32 i = firstNeighbour; i <= lastNeighbour; ++i) {
		double newDelta = current->getDistanceSquaredTo(points[i]);
		if(newDelta < delta) {
			delta = newDelta;
			A = current;
			B = points[i];
		}
	}
}

void ClosestPairOfPoints::_recurPair(Int32 left, Int32 middle, Int32 right)
{
	Int32 l = left;
	Int32 r = middle;
	Int32 k = left;

	while(l < middle && r < right) {
		if(points[l] < points[r]) {
			current = pAux[k++] = points[l++];
			Int32 firstN = r - 3;
			Int32 lastN = r + 4;
			_updateDelta((firstN < middle) ? middle : firstN, (lastN >= right) ? right - 1 : lastN);
		}
		else {
			current = pAux[k++] = points[r++];
			Int32 firstN = l - 3;
			Int32 lastN = l + 4;
			_updateDelta((firstN < left) ? left : firstN, (lastN >= middle) ? middle - 1 : lastN);
		}
	}

	while(l < middle) pAux[k++] = points[l++];
	while(r < right) pAux[k++] = points[r++];
	for(Int32 i = left; i < right; ++i) points[i] = pAux[i];
}

void ClosestPairOfPoints::_initStructures(GeometricRectangle^ bounds, bool includeNonPoints)
{
	points = BasicAlgorithms::getPointsWithinBounds(bounds, includeNonPoints);
	//points = (List<GeometricPoint^>^)Enumerable::Distinct(points);
	points->Sort(gcnew GeometricPoint::ComparerX());

	tree = gcnew BSTree<GeometricPoint^>();
	delta = (bounds->Width * bounds->Width) + (bounds->Height * bounds->Height) + 1;
}

void ClosestPairOfPoints::_updateDelta()
{
	for(Int32 i = 0; i < neighbours->Count; ++i) {
		double newDelta = current->getDistanceSquaredTo(neighbours[i]);
		if(newDelta < delta) {
			delta = newDelta;
			A = current;
			B = neighbours[i];
		}
	}
}

void ClosestPairOfPoints::_updateActivePoints()
{
	while((current->X - points[iFirst]->X) > delta)
		tree->remove(points[iFirst++]);
}

void ClosestPairOfPoints::_cleanUp()
{
	A = nullptr;
	B = nullptr;
	tree = nullptr;
	points = nullptr;
	pAux = nullptr;
	neighbours = nullptr;
	current = nullptr;
	delta = 0;
	iFirst = 0;
	iCurrent = 0;
}