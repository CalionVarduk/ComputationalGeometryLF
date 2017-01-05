#pragma once

#include "CV++ Utilities\CVStructures.h"

namespace ComputationalGeometryLF
{
	using namespace System;
	using namespace System::Drawing;
	using namespace System::Windows::Forms;
	using namespace CVStructures;

	public ref class GeometricObject abstract
	{
		public:
			enum class DrawStyle { Draw = 1, Fill = 2, DrawAndFill = 3 };

		#pragma region Properties
			property DrawStyle DrawStyleHint {
				public: DrawStyle get() { return _style; }
				public: void set(DrawStyle value) { _style = value; }
			}

			property bool IsDrawn {
				public: bool get() { return _isDrawn; }
				public: void set(bool value) { _isDrawn = value; }
			}

			property Color MainColor {
				public: Color get() { return _mainColor; }
				public: void set(Color value) { _mainColor = value; }
			}

			property CVRectangle<double> BoundingBox {
				public: CVRectangle<double> get() { return _boundingBox; }
			}

			property CVPoint<double> BoundingLocation {
				public: CVPoint<double> get() { return _boundingBox.Location; }
			}

			property double BoundingLeft {
				public: double get() { return _boundingBox.Left; }
			}

			property double BoundingRight {
				public: double get() { return _boundingBox.Right; }
			}

			property double BoundingCentreX {
				public: double get() { return _boundingBox.CentreX; }
			}

			property double BoundingTop {
				public: double get() { return _boundingBox.Bottom; }
			}

			property double BoundingBottom {
				public: double get() { return _boundingBox.Top; }
			}

			property double BoundingCentreY {
				public: double get() { return _boundingBox.CentreY; }
			}

			property double BoundingWidth {
				public: double get() { return _boundingBox.Width; }
			}

			property double BoundingHeight {
				public: double get() { return _boundingBox.Height; }
			}
		#pragma endregion

			GeometricObject();

			virtual void draw(Graphics^ graphics, Pen^ pen, CVPoint<double>% translation) = 0;
			virtual void fill(Graphics^ graphics, SolidBrush^ brush, CVPoint<double>% translation) = 0;

			template <class T>
			T^ interpretAs()
			{ return (T^)this; }

			virtual bool isPoint();
			virtual bool isSegment();
			virtual bool isRectangle();
			virtual bool isEllipse();
			virtual bool isPolygon();

			bool isInView(CVRectangle<double>% view);

		protected:
			bool _isDrawn;
			Color _mainColor;
			
			CVRectangle<double> _boundingBox;
			DrawStyle _style;
	};
}