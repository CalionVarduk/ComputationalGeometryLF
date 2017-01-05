#pragma once

#include "GeometricObject.h"

namespace ComputationalGeometryLF
{
	public ref class GeometricEllipse : public GeometricObject
	{
		public:
		#pragma region Properties
			property CVPoint<double> Origin {
				public: CVPoint<double> get() { return CVPoint<double>(X, Y); }
				public: void set(CVPoint<double> value)
						{
							X = value.X;
							Y = value.Y;
						}
			}

			property double Left {
				public: double get() { return _boundingBox.Left; }
				public: void set(double value) { _boundingBox.Left = value; }
			}

			property double Right {
				public: double get() { return _boundingBox.Right; }
				public: void set(double value) { _boundingBox.Right = value; }
			}

			property double X {
				public: double get() { return _boundingBox.CentreX; }
				public: void set(double value) { _boundingBox.CentreX = value; }
			}

			property double Top {
				public: double get() { return _boundingBox.Bottom; }
				public: void set(double value) { _boundingBox.Bottom = value; }
			}

			property double Bottom {
				public: double get() { return _boundingBox.Top; }
				public: void set(double value) { _boundingBox.Top = value; }
			}

			property double Y {
				public: double get() { return _boundingBox.CentreY; }
				public: void set(double value) { _boundingBox.CentreY = value; }
			}

			property CVSize<double> Size {
				public: CVSize<double> get() { return _boundingBox.Size; }
				public: void set(CVSize<double> value)
						{
							Width = value.Width;
							Height = value.Height;
						}
			}

			property double Width {
				public: double get() { return _boundingBox.Width; }
				public: void set(double value)
						{
							double offset = value - _boundingBox.Width;
							_boundingBox.Width += offset;
							_boundingBox.Left -= offset * 0.5;
						}
			}

			property double Height {
				public: double get() { return _boundingBox.Height; }
				public: void set(double value)
						{
							double offset = value - _boundingBox.Height;
							_boundingBox.Height += offset;
							_boundingBox.Top -= offset * 0.5;
						}
			}
		#pragma endregion

			GeometricEllipse();
			GeometricEllipse(CVPoint<double> origin, CVSize<double> size);
			GeometricEllipse(double x, double y, double width, double height);

			virtual bool isEllipse() override;

			virtual void draw(Graphics^ graphics, Pen^ pen, CVPoint<double>% translation) override;
			virtual void fill(Graphics^ graphics, SolidBrush^ brush, CVPoint<double>% translation) override;
			virtual String^ ToString() override;
	};
}