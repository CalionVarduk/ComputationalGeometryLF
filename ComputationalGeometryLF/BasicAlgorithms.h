#pragma once

#include "Toolbox.h"
#include "CV++ Utilities\CVStructures.h"

namespace ComputationalGeometryLF
{
	using namespace System;
	using namespace CVStructures;

	typedef Tuple<GeometricPoint^, GeometricPoint^, double> PointPair;

	public ref class BasicAlgorithms abstract
	{
		public:
			static double orientationTest(double pX, double pY, double sStartX, double sStartY, double sEndX, double sEndY);
			static double orientationTest(double pX, double pY, GeometricSegment^ s);
			static double orientationTest(GeometricPoint^ p, GeometricPoint^ sStart, GeometricPoint^ sEnd);
			static double orientationTest(GeometricPoint^ p, GeometricSegment^ s);
			static Tuple<double, double>^ orientationTest(GeometricSegment^ s1, GeometricSegment^ s2);
			
			static bool containmentTest(GeometricObject^ o1, GeometricObject^ o2);
			static bool pointContainmentTest(double pX, double pY, GeometricObject^ o);
			static bool pointContainmentTest(GeometricPoint^ p, GeometricObject^ o);

			static bool belongingnessTest(double pX, double pY, GeometricSegment^ s);
			static bool belongingnessTest(GeometricPoint^ p, GeometricSegment^ s);
			static bool belongingnessTest(GeometricSegment^ s1, GeometricSegment^ s2);

			static bool boundingBoxIntersectionTest(GeometricObject^ o1, GeometricObject^ o2);
			static bool intersectionTest(GeometricSegment^ s1, GeometricSegment^ s2);

			static GeometricPoint^ getIntersectionPoint(GeometricSegment^ s1, GeometricSegment^ s2);

			static List<GeometricPoint^>^ getPointsWithinBounds(GeometricRectangle^ bounds, bool includeNonPoints);
			static Int32 getBottomMostIndex(List<GeometricPoint^>^ points);
			static Int32 getTopMostIndex(List<GeometricPoint^>^ points);
			static Int32 getRightMostIndex(List<GeometricPoint^>^ points);
			static Int32 getLeftMostIndex(List<GeometricPoint^>^ points);

			static List<GeometricSegment^>^ getSegmentsWithinBounds(GeometricRectangle^ bounds, bool containedOnly);

			static List<GeometricSegment^>^ getPolygonSegments(GeometricPolygon^ p);
			static List<GeometricPoint^>^ getPolygonPoints(GeometricPolygon^ p);

			static String^ interpretPointOrientationTest(double outcome);
			static String^ interpretSegmentOrientationTest(Tuple<double, double>^ outcome);

		private:
			static void _getPointsCheckPoint(GeometricRectangle^ bounds, GeometricPoint^ point, List<GeometricPoint^>^ points);
			static void _getPointsCheckSegment(GeometricRectangle^ bounds, GeometricObject^ obj, List<GeometricPoint^>^ points);
			static void _getPointsCheckRectangle(GeometricRectangle^ bounds, GeometricObject^ obj, List<GeometricPoint^>^ points);
			static void _getPointsCheckPolygon(GeometricRectangle^ bounds, GeometricObject^ obj, List<GeometricPoint^>^ points);
	};
}