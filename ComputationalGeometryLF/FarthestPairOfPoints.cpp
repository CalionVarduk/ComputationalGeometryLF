#include "FarthestPairOfPoints.h"

using namespace ComputationalGeometryLF;

PointPair^ FarthestPairOfPoints::find(GeometricRectangle^ bounds, bool includeNonPoints)
{
	GeometricPolygon^ hull = ConvexHull::grahamScan(bounds, includeNonPoints);

	Int32 apIndex = _antipodalIndex(hull, 0, 1);
	Tuple<CVPoint<double>, CVPoint<double>, double>^ farCheck;
	Tuple<CVPoint<double>, CVPoint<double>, double>^ farthest;

	farthest = _farthestOf3(hull->getPointAt(hull->PointCount - 1), hull->getPointAt(0), hull->getPointAt(apIndex));

	for(Int32 i = 1; i < hull->PointCount; ++i) {
		apIndex = _antipodalIndex(hull, i, apIndex);
		farCheck = _farthestOf3(hull->getPointAt(i - 1), hull->getPointAt(i), hull->getPointAt(apIndex));

		if(farCheck->Item3 > farthest->Item3)
			farthest = farCheck;
	}

	return Tuple::Create(gcnew GeometricPoint(farthest->Item1), gcnew GeometricPoint(farthest->Item2), farthest->Item3);
}

Tuple<CVPoint<double>, CVPoint<double>, double>^ FarthestPairOfPoints::_farthestOf3(CVPoint<double> p1, CVPoint<double> p2, CVPoint<double> p3)
{
	CVPoint<double> delta = p1 - p2;
	double p1p2 = (delta.X * delta.X) + (delta.Y * delta.Y);

	delta = p1 - p3;
	double p1p3 = (delta.X * delta.X) + (delta.Y * delta.Y);

	delta = p2 - p3;
	double p2p3 = (delta.X * delta.X) + (delta.Y * delta.Y);

	if(p1p2 > p1p3)
		return (p1p2 > p2p3) ? Tuple::Create(p1, p2, p1p2) : Tuple::Create(p2, p3, p2p3);
	return (p1p3 > p2p3) ? Tuple::Create(p1, p3, p1p3) : Tuple::Create(p2, p3, p2p3);
}

Int32 FarthestPairOfPoints::_antipodalIndex(GeometricPolygon^ hull, Int32 iPoint, Int32 iStart)
{
	Int32 current = iStart, next = iStart + 1;
	CVPoint<double> delta = hull->getPointAt(iPoint) - hull->getPointAt(iStart);
	double maxDist = (delta.X * delta.X) + (delta.Y * delta.Y);

	while(next != iStart) {
		if(next == hull->PointCount) next = 0;

		delta = hull->getPointAt(iPoint) - hull->getPointAt(next);
		double dist = (delta.X * delta.X) + (delta.Y * delta.Y);

		if(dist > maxDist) {
			maxDist = dist;
			current = next++;
		}
		else break;
	}
	return current;
}