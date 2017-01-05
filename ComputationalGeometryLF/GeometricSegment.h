#pragma once

#include "GeometricPoint.h"

namespace ComputationalGeometryLF
{
	public ref class GeometricSegment : public GeometricObject
	{
		public:
		#pragma region Properties
			property CVPoint<double> Start {
				public: CVPoint<double> get() { return _start; }
				public: void set(CVPoint<double> value)
						{
							_start = value;
							_fixHorizontal();
							_fixVertical();
						}
			}

			property double StartX {
				public: double get() { return _start.X; }
				public: void set(double value)
						{
							_start.X = value;
							_fixHorizontal();
						}
			}

			property double StartY {
				public: double get() { return _start.Y; }
				public: void set(double value)
						{
							_start.Y = value;
							_fixVertical();
						}
			}

			property CVPoint<double> End {
				public: CVPoint<double> get() { return _end; }
				public: void set(CVPoint<double> value)
						{
							_end = value;
							_fixHorizontal();
							_fixVertical();
						}
			}

			property double EndX {
				public: double get() { return _end.X; }
				public: void set(double value)
						{
							_end.X = value;
							_fixHorizontal();
						}
			}

			property double EndY {
				public: double get() { return _end.Y; }
				public: void set(double value)
						{
							_end.Y = value;
							_fixVertical();
						}
			}

			property double UpperX {
				public: double get() { return (_start.Y > _end.Y) ? _start.X : (_start.Y < _end.Y) ? _end.X : (_start.X < _end.X) ? _start.X : _end.X; }
			}

			property double UpperY {
				public: double get() { return (_start.Y > _end.Y) ? _start.Y : (_start.Y < _end.Y) ? _end.Y : (_start.X < _end.X) ? _start.Y : _end.Y; }
			}

			property CVPoint<double> Upper {
				public: CVPoint<double> get() { return (_start.Y > _end.Y) ? _start : (_start.Y < _end.Y) ? _end : (_start.X < _end.X) ? _start : _end; }
			}

			property double LowerX {
				public: double get() { return (_start.Y > _end.Y) ? _end.X : (_start.Y < _end.Y) ? _start.X : (_start.X < _end.X) ? _end.X : _start.X; }
			}

			property double LowerY {
				public: double get() { return (_start.Y > _end.Y) ? _end.Y : (_start.Y < _end.Y) ? _start.Y : (_start.X < _end.X) ? _end.Y : _start.Y; }
			}

			property CVPoint<double> Lower {
				public: CVPoint<double> get() { return (_start.Y > _end.Y) ? _end : (_start.Y < _end.Y) ? _start : (_start.X < _end.X) ? _end : _start; }
			}

			property double LengthSquared {
				public: double get()
						{
							double dX = DeltaX;
							double dY = DeltaY;
							return (dX * dX) + (dY * dY);
						}
			}

			property double Length {
				public: double get() { return Math::Sqrt(LengthSquared); }
			}

			property CVPoint<double> Delta {
				public: CVPoint<double> get() { return CVPoint<double>(DeltaX, DeltaY); }
			}

			property double DeltaX {
				public: double get() { return _end.X - _start.X; }
			}

			property double DeltaY {
				public: double get() { return _end.Y - _start.Y; }
			}
		#pragma endregion

			GeometricSegment();
			GeometricSegment(CVPoint<double> start, CVPoint<double> end);
			GeometricSegment(double startX, double startY, double endX, double endY);

			void set(CVPoint<double> start, CVPoint<double> end);
			void set(double startX, double startY, double endX, double endY);

			void offset(GeometricPoint^ dpt);
			void offset(CVPoint<double> dpt);
			void offset(double dx, double dy);

			virtual bool isSegment() override;

			virtual void draw(Graphics^ graphics, Pen^ pen, CVPoint<double>% translation) override;
			virtual void fill(Graphics^ graphics, SolidBrush^ brush, CVPoint<double>% translation) override;
			virtual String^ ToString() override;

		private:
			CVPoint<double> _start;
			CVPoint<double> _end;

			void _fixHorizontal();
			void _fixVertical();
	};
}