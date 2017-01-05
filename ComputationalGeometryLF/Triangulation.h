#pragma once

#include "IntersectionPoints.h"

namespace ComputationalGeometryLF
{
	using namespace System;
	using namespace System::Drawing;
	using namespace CVStructures;

	public ref class TriangulationRegion
	{
		public:
			List<GeometricPoint^>^ Points;
			List<TriangulationRegion^>^ Neighbours;

			TriangulationRegion()
				: Points(gcnew List<GeometricPoint^>(4)), Neighbours(gcnew List<TriangulationRegion^>())
			{ }
	};

	public ref class TriangulatedPolygon
	{
		public:
			GeometricPolygon^ Polygon;
			TriangulationRegion^ TriangleGraphRoot;
			List<GeometricSegment^>^ Segments;

			TriangulatedPolygon(GeometricPolygon^ p)
				: Polygon(p), TriangleGraphRoot(gcnew TriangulationRegion()), Segments(gcnew List<GeometricSegment^>())
			{ }

			~TriangulatedPolygon()
			{
				_disposeNode(TriangleGraphRoot, nullptr);
			}

		private:
			void _disposeNode(TriangulationRegion^ node, TriangulationRegion^ parent)
			{
				if(node != nullptr) {
					for(Int32 i = 0; i < node->Neighbours->Count; ++i) {
						if(node->Neighbours[i] != parent)
							_disposeNode(node->Neighbours[i], node);
						node->Neighbours[i] = nullptr;
					}
				}
			}
	};

	public ref class MonoPoint
	{
		public:
			GeometricPoint^ Point;
			bool OnLeftChain;

			MonoPoint(GeometricPoint^ point, bool onLeftChain)
				: Point(point), OnLeftChain(onLeftChain)
			{ }
	};

	public ref class Triangulation abstract
	{
		public:
			static Color StartPointColor = Color::Green;
			static Color EndPointColor = Color::Red;
			static Color SplitPointColor = Color::DarkViolet;
			static Color MergePointColor = Color::DeepSkyBlue;

			static GeometricPolygon^ GetCounterClockwiseOrdered(GeometricPolygon^ p);
			static bool isPolygonMonotoneY(GeometricPolygon^ p);
			
			static TriangulatedPolygon^ TriangulateNaive(GeometricPolygon^ p);
			static TriangulatedPolygon^ TriangulateMonotoneY(GeometricPolygon^ p);
			static List<GeometricPoint^>^ GetPolygonVertexTypes(GeometricPolygon^ p);

			static List<GeometricPoint^>^ GetTrichromaticPoints(GeometricPolygon^ p);

		private:
			static double _signedArea(GeometricPolygon^ p);
			static bool _monoBelow(GeometricPoint^ p1, GeometricPoint^ p2);
			static bool _monoAbove(GeometricPoint^ p1, GeometricPoint^ p2);
			static List<MonoPoint^>^ _getSortedMonoPoints(List<GeometricPoint^>^ points);

			static void _setTrichromaticColor(GeometricPoint^ toSet, GeometricPoint^ set1, GeometricPoint^ set2, List<GeometricPoint^>^ OUT_points);
			static void _traverseTriangleGraph(TriangulationRegion^ node, TriangulationRegion^ parent, List<GeometricPoint^>^ OUT_points);

			static bool _regionNeighbourCheck(TriangulationRegion^ r1, TriangulationRegion^ r2);
			static void _fixGraphRegions(TriangulationRegion^ newRegion, TriangulationRegion^ currentRegion);

			static void _recTriangulate(TriangulationRegion^ current, List<GeometricSegment^>^ segments);
	};
}