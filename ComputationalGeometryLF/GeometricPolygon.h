#pragma once

#include "GeometricPoint.h"

namespace ComputationalGeometryLF
{
	using namespace System::Text;
	using namespace System::Collections::Generic;

	public ref class GeometricPolygon : public GeometricObject
	{
		public:
		#pragma region Properties
			property Int32 PointCount {
				public: Int32 get() { return _points->Count; }
			}
		#pragma endregion

			GeometricPolygon();
			GeometricPolygon(GeometricPoint^ origin);
			GeometricPolygon(CVPoint<double> origin);
			GeometricPolygon(double originX, double originY);

			void reset(GeometricPoint^ origin);
			void reset(CVPoint<double> origin);
			void reset(double originX, double originY);

			void offset(GeometricPoint^ dpt);
			void offset(CVPoint<double> dpt);
			void offset(double dx, double dy);

			void addPoint(GeometricPoint^ origin);
			void addPoint(CVPoint<double> pt);
			void addPoint(double x, double y);

			CVPoint<double> getPointAt(Int32 i);
			void modPointAt(Int32 i, double newX, double newY);

			virtual bool isPolygon() override;

			virtual void draw(Graphics^ graphics, Pen^ pen, CVPoint<double>% translation) override;
			virtual void fill(Graphics^ graphics, SolidBrush^ brush, CVPoint<double>% translation) override;
			virtual String^ ToString() override;

		private:
			List<CVPoint<double>>^ _points;

			void _fixBoundingBox();
	};
}