#pragma once

#include "GeometricObject.h"

namespace ComputationalGeometryLF
{
	using namespace System::Collections;

	public ref class GeometricPoint : public GeometricObject, public IComparable
	{
		public:
		#pragma region Properties
			property double X {
				public: double get() { return _boundingBox.Left + 1; }
				public: void set(double value) { _boundingBox.Left = value - 1; }
			}

			property double Y {
				public: double get() { return _boundingBox.Top + 1; }
				public: void set(double value) { _boundingBox.Top = value - 1; }
			}

			property CVPoint<double> Location {
				public: CVPoint<double> get() { return CVPoint<double>(X, Y); }
				public: void set(CVPoint<double> value)
						{
							X = value.X;
							Y = value.Y;
						}
			}
		#pragma endregion

			GeometricPoint();
			GeometricPoint(GeometricPoint^ pt);
			GeometricPoint(CVPoint<double> pt);
			GeometricPoint(double x, double y);

			void set(GeometricPoint^ pt);
			void set(CVPoint<double> pt);
			void set(double x, double y);

			void offset(GeometricPoint^ dpt);
			void offset(CVPoint<double> dpt);
			void offset(double dx, double dy);

			double getDeltaXTo(GeometricPoint^ pt);
			double getDeltaYTo(GeometricPoint^ pt);
			CVPoint<double> getDeltaTo(GeometricPoint^ pt);

			double getDistanceSquaredTo(GeometricPoint^ pt);
			double getDistanceTo(GeometricPoint^ pt);

			virtual bool isPoint() override;

			virtual void draw(Graphics^ graphics, Pen^ pen, CVPoint<double>% translation) override;
			virtual void fill(Graphics^ graphics, SolidBrush^ brush, CVPoint<double>% translation) override;
			virtual String^ ToString() override;

#pragma region Comparison Thing-a-ma-jigs
			virtual Int32 CompareTo(Object^ other);

			static bool operator> (GeometricPoint^ p1, GeometricPoint^ p2);
			static bool operator< (GeometricPoint^ p1, GeometricPoint^ p2);

			ref class ComparerX : public System::Collections::Generic::IComparer<GeometricPoint^>
			{
				public:
					ComparerX();
					virtual int Compare(GeometricPoint^ p1, GeometricPoint^ p2);
			};

			ref class ComparerY : public System::Collections::Generic::IComparer<GeometricPoint^>
			{
				public:
					ComparerY();
					virtual int Compare(GeometricPoint^ p1, GeometricPoint^ p2);
			};

			ref class ComparerAngle : public System::Collections::Generic::IComparer<GeometricPoint^>
			{
				public:
					GeometricPoint^ Origin;
					ComparerAngle(GeometricPoint^ origin);
					virtual int Compare(GeometricPoint^ p1, GeometricPoint^ p2);

				private:
					double _oTest(double dp1x, double dp1y, double dp2x, double dp2y);
			};

			ref class ComparerDistance : public System::Collections::Generic::IComparer<GeometricPoint^>
			{
				public:
					GeometricPoint^ Origin;
					ComparerDistance(GeometricPoint^ origin);
					virtual int Compare(GeometricPoint^ p1, GeometricPoint^ p2);
			};

			ref class ComparerAngleCoords : public System::Collections::Generic::IComparer<GeometricPoint^>
			{
				public:
					GeometricPoint^ Origin;

					ComparerAngleCoords(GeometricPoint^ origin);
					virtual int Compare(GeometricPoint^ p1, GeometricPoint^ p2);

				private:
					double _oTest(double dp1x, double dp1y, double dp2x, double dp2y);
			};
#pragma endregion
	};
}