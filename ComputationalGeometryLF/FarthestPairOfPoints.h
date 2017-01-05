#pragma once

#include "ConvexHull.h"

namespace ComputationalGeometryLF
{
	using namespace System;
	using namespace System::Linq;
	using namespace CVStructures;

	public ref class FarthestPairOfPoints abstract
	{
		public:
			static PointPair^ find(GeometricRectangle^ bounds, bool includeNonPoints);

		private:
			static Tuple<CVPoint<double>, CVPoint<double>, double>^ _farthestOf3(CVPoint<double> p1, CVPoint<double> p2, CVPoint<double> p3);
			static Int32 _antipodalIndex(GeometricPolygon^ hull, Int32 iPoint, Int32 iStart);
	};
}