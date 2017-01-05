#pragma once

#include "GeometricPoint.h"

namespace ComputationalGeometryLF
{
	public ref class GeometricRectangle : public GeometricObject
	{
		public:
		#pragma region Properties
			property CVPoint<double> Origin {
				public: CVPoint<double> get() { return CVPoint<double>(Left, Bottom); }
				public: void set(CVPoint<double> value)
						{
							Left = value.X;
							Bottom = value.Y;
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

			property double CentreX {
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

			property double CentreY {
				public: double get() { return _boundingBox.CentreY; }
				public: void set(double value) { _boundingBox.CentreY = value; }
			}

			property CVSize<double> Size {
				public: CVSize<double> get() { return _boundingBox.Size; }
				public: void set(CVSize<double> value) { _boundingBox.Size = value; }
			}

			property double Width {
				public: double get() { return _boundingBox.Width; }
				public: void set(double value) { _boundingBox.Width = value; }
			}

			property double Height {
				public: double get() { return _boundingBox.Height; }
				public: void set(double value) { _boundingBox.Height = value; }
			}
		#pragma endregion

			GeometricRectangle();
			GeometricRectangle(CVPoint<double> origin, CVSize<double> size);
			GeometricRectangle(double left, double bottom, double width, double height);
			GeometricRectangle(CVPoint<double> bottomLeft, CVPoint<double> topRight);

			void set(CVPoint<double> origin, CVSize<double> size);
			void set(double left, double bottom, double width, double height);
			void set(CVPoint<double> bottomLeft, CVPoint<double> topRight);

			void offset(GeometricPoint^ dpt);
			void offset(CVPoint<double> dpt);
			void offset(double dx, double dy);

			virtual bool isRectangle() override;

			virtual void draw(Graphics^ graphics, Pen^ pen, CVPoint<double>% translation) override;
			virtual void fill(Graphics^ graphics, SolidBrush^ brush, CVPoint<double>% translation) override;
			virtual String^ ToString() override;
	};
}