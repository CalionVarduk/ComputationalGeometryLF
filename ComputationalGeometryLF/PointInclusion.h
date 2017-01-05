#pragma once

#include "Triangulation.h"

namespace ComputationalGeometryLF
{
	using namespace System;
	using namespace System::Drawing;
	using namespace CVStructures;

	public ref class PointInclusion abstract
	{
		public:
			static bool IsInsidePolygon(double pX, double pY, GeometricPolygon^ poly);
			static bool IsInsidePolygon(GeometricPoint^ p, GeometricPolygon^ poly);

			static bool IsInsideConvexPolygon(double pX, double pY, GeometricPolygon^ poly);
			static bool IsInsideConvexPolygon(GeometricPoint^ p, GeometricPolygon^ poly);

		private:
			static Int32 _testEdge(double pX, double pY, GeometricPolygon^ P, int iStart, int iEnd);

			static Int32 _searchIncr(List<GeometricPoint^>^ list, double x);
			static Int32 _searchDecr(List<GeometricPoint^>^ list, double x);
	};
}