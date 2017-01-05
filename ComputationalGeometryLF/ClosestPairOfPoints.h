#pragma once

#include "BasicAlgorithms.h"

namespace ComputationalGeometryLF
{
	using namespace System;
	using namespace System::Linq;
	using namespace CVStructures;

	public ref class ClosestPairOfPoints abstract
	{
		public:
			static PointPair^ findByTree(GeometricRectangle^ bounds, bool includeNonPoints);
			static PointPair^ findByMerge(GeometricRectangle^ bounds, bool includeNonPoints);

		private:
			static GeometricPoint^ A = nullptr;
			static GeometricPoint^ B = nullptr;
			static BSTree<GeometricPoint^>^ tree = nullptr;
			static List<GeometricPoint^>^ points = nullptr;
			static array<GeometricPoint^>^ pAux = nullptr;
			static List<GeometricPoint^>^ neighbours = nullptr;
			static GeometricPoint^ current = nullptr;
			static double delta = 0;
			static Int32 iFirst = 0;
			static Int32 iCurrent = 0;

			static void _mergePairs();
			static void _updateDelta(Int32 firstNeighbour, Int32 lastNeighbour);
			static void _recurPair(Int32 left, Int32 middle, Int32 right);

			static void _initStructures(GeometricRectangle^ bounds, bool includeNonPoints);
			static void _updateDelta();
			static void _updateActivePoints();

			static void _cleanUp();
	};
}