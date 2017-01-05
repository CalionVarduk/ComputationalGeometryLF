#pragma once

#include "BasicAlgorithms.h"

namespace ComputationalGeometryLF
{
	using namespace System;
	using namespace System::Linq;
	using namespace CVStructures;

	public ref class ConvexHull abstract
	{
		public:
			static GeometricPolygon^ grahamScan(GeometricRectangle^ bounds, bool includeNonPoints);
			static GeometricPolygon^ jarvisMarch(GeometricRectangle^ bounds, bool includeNonPoints);
			static GeometricPolygon^ quickHull(GeometricRectangle^ bounds, bool includeNonPoints);

		private:
			static CVStack<GeometricPoint^>^ _grahamBuildStack(List<GeometricPoint^>^ points);
			static GeometricPolygon^ _buildHullFromStack(CVStack<GeometricPoint^>^ S);

			static void _jarvisGoUp(List<GeometricPoint^>^ points, GeometricPoint^ bottom, GeometricPoint^ top, CVStack<GeometricPoint^>^ S);
			static void _jarvisGoDown(List<GeometricPoint^>^ points, GeometricPoint^ bottom, GeometricPoint^ top, CVStack<GeometricPoint^>^ S);

			static Int32 _quickArrange(List<GeometricPoint^>^ points, Int32 left, Int32 right, GeometricPoint^ sStart, GeometricPoint^ sEnd);
			static Tuple<GeometricPoint^, double>^ _quickFindFarthest(List<GeometricPoint^>^ points, Int32 left, Int32 right, GeometricPoint^ sStart, GeometricPoint^ sEnd);
			static List<GeometricPoint^>^ _quickRecur(List<GeometricPoint^>^ points, Int32 left, Int32 right, GeometricPoint^ sStart, GeometricPoint^ sEnd);
	};
}