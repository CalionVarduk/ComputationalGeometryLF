#pragma once

#include "CVNetControls.h"
#include "GeometricPoint.h"
#include "GeometricSegment.h"
#include "GeometricRectangle.h"
#include "GeometricEllipse.h"
#include "GeometricPolygon.h"

namespace ComputationalGeometryLF
{
	using namespace System;
	using namespace System::Drawing;
	using namespace System::Windows::Forms;
	using namespace System::Collections::Generic;

	using namespace CVNetControls;
	using namespace CVStructures;

	public ref class CartesianPlane
	{
		public:
		#pragma region Properties
			property bool LabelsVisible {
				public: bool get() { return _labelsY->Enabled; }
				public: void set(bool value)
						{
							_labelsY->Visible = value;
							_labelsX->Visible = value;
							_labelsY->Enabled = value;
							_labelsX->Enabled = value;
						}
			}

			property bool DrawGrid {
				public: bool get() { return _drawGridOn; }
				public: void set(bool value) { _drawGridOn = value; }
			}

			property Color PlaneBackgroundColor {
				public: Color get() { return _planeBackgroundColor; }
				public: void set(Color value) { _planeBackgroundColor = value; }
			}

			property Color LabelBackgroundColor {
				public: Color get() { return _labelBackgroundColor; }
				public: void set(Color value) { _labelBackgroundColor = value; }
			}

			property Color SmallGridColor {
				public: Color get() { return _smallGridColor; }
				public: void set(Color value) { _smallGridColor = value; }
			}

			property Color LargeGridColor {
				public: Color get() { return _largeGridColor; }
				public: void set(Color value) { _largeGridColor = value; }
			}

			property Color OriginGridColor {
				public: Color get() { return _originGridColor; }
				public: void set(Color value) { _originGridColor = value; }
			}

			property Color LabelTextColor {
				public: Color get() { return _labelTextColor; }
				public: void set(Color value) { _labelTextColor = value; }
			}

			property String^ LabelFontName {
				public: String^ get() { return _labelFont->Name; }
				public: void set(String^ value) { _labelFont = gcnew Font(value, _labelFont->Size); }
			}

			property Byte LabelBoxSpacing {
				public: Byte get() { return _labelBoxSpacing; }
				public: void set(Byte value)
						{
							_labelBoxSpacing = NumericLimiter<Byte>::max(value, 16);
							_labelsY->Height = _box->Height + (_labelBoxSpacing << 1);
							_labelsX->Width = _box->Width + (_labelBoxSpacing << 1);

							_graphicsX = _setBitmap(_labelsX);
							_graphicsY = _setBitmap(_labelsY);

							_labelsX->Top = _labelsY->Bottom;
							_box->Location = Point(_labelsY->Right + _labelBoxSpacing, _labelsY->Top + _labelBoxSpacing);
						}
			}

			property CVPoint<double> Origin {
				public: CVPoint<double> get() { return _origin; }
				public: void set(CVPoint<double> value)
						{
							OriginX = value.X;
							OriginY = value.Y;
						}
			}

			property double OriginX {
				public: double get() { return _origin.X; }
				public: void set(double value) { _origin.X = NumericLimiter<double>::minMax(value, -10000000, 10000000); }
			}

			property double OriginY {
				public: double get() { return _origin.Y; }
				public: void set(double value) { _origin.Y = NumericLimiter<double>::minMax(value, -10000000, 10000000); }
			}

			property Int32 SmallGridSpacingInPixels {
				public: Int32 get() { return _gridSpacingSmallPx; }
				public: void set(Int32 value)
						{
							_gridSpacingSmallPx = NumericLimiter<Int32>::minMax(value, 1, 1000000);
							_fixLargeGridSpacingInPixels();
						}
			}

			property Int32 LargeGridSpacing {
				public: Int32 get() { return _gridSpacingLarge; }
				public: void set(Int32 value)
						{
							_gridSpacingLarge = NumericLimiter<Int32>::minMax(value, 1, 2000);
							_fixLargeGridSpacingInPixels();
						}
			}

			property Byte LabelBoxGirth {
				public: Byte get() { return (Byte)_labelsY->Width; }
				public: void set(Byte value)
						{
							value = NumericLimiter<Byte>::minMax(value, 10, 100);
							
							Int32 offset = value - _labelsY->Width;
							_labelsY->Width += offset;
							_labelsX->Height += offset;
							_box->Left += offset;
							_labelsX->Left += offset;

							_graphicsX = _setBitmap(_labelsX);
							_graphicsY = _setBitmap(_labelsY);
						}
			}

			property CVRectangle<double> ViewRectangle {
				public: CVRectangle<double> get() { return CVRectangle<double>(-_origin.X, -_origin.Y, _box->Width, _box->Height); }
			}

			property Size BoxSize {
				public: Size get() { return _box->Size; }
				public: void set(Size value)
						{
							if(value.Width < 0) value.Width = 0;
							if(value.Height < 0) value.Height = 0;

							Int32 offset = value.Width - _box->Width;
							_box->Width += offset;
							_labelsX->Width += offset;

							offset = value.Height - _box->Height;
							_box->Height += offset;
							_labelsY->Height += offset;
							_labelsX->Top += offset;

							_graphics = _setBitmap(_box);
							_graphicsX = _setBitmap(_labelsX);
							_graphicsY = _setBitmap(_labelsY);
						}
			}

			property Int32 BoxWidth {
				public: Int32 get() { return _box->Width; }
				public: void set(Int32 value)
						{
							if(value < 0) value = 0;

							Int32 offset = value - _box->Width;
							_box->Width += offset;
							_labelsX->Width += offset;

							_graphics = _setBitmap(_box);
							_graphicsX = _setBitmap(_labelsX);
						}
			}

			property Int32 BoxHeight {
				public: Int32 get() { return _box->Height; }
				public: void set(Int32 value)
						{
							if(value < 0) value = 0;

							Int32 offset = value - _box->Height;
							_box->Height += offset;
							_labelsY->Height += offset;
							_labelsX->Top += offset;

							_graphics = _setBitmap(_box);
							_graphicsY = _setBitmap(_labelsY);
						}
			}

			property Size FullSize {
				public: Size get() { return Size(FullWidth, FullHeight); }
			}

			property Int32 FullWidth {
				public: Int32 get() { return Right - Left; }
			}

			property Int32 FullHeight {
				public: Int32 get() { return Bottom - Top; }
			}

			property Point Location {
				public: Point get() { return _labelsY->Location; }
				public: void set(Point value)
						{
							Left = value.X;
							Top = value.Y;
						}
			}

			property Int32 Left {
				public: Int32 get() { return _labelsY->Left; }
				public: void set(Int32 value)
						{
							Int32 offset = value - _labelsY->Left;
							_box->Left += offset;
							_labelsX->Left += offset;
							_labelsY->Left += offset;
						}
			}

			property Int32 Top {
				public: Int32 get() { return _labelsY->Top; }
				public: void set(Int32 value)
						{
							Int32 offset = value - _labelsY->Top;
							_box->Top += offset;
							_labelsX->Top += offset;
							_labelsY->Top += offset;
						}
			}

			property Int32 Right {
				public: Int32 get() { return _labelsX->Right; }
				public: void set(Int32 value)
						{
							Int32 offset = value - _labelsX->Right;
							_box->Left += offset;
							_labelsX->Left += offset;
							_labelsY->Left += offset;
						}
			}

			property Int32 Bottom {
				public: Int32 get() { return _labelsX->Bottom; }
				public: void set(Int32 value)
						{
							Int32 offset = value - _labelsX->Bottom;
							_box->Top += offset;
							_labelsX->Top += offset;
							_labelsY->Top += offset;
						}
			}

			property Int32 CentreX {
				public: Int32 get() { return Left + (FullWidth >> 1); }
				public: void set(Int32 value)
						{
							Int32 offset = value - CentreX;
							_box->Left += offset;
							_labelsX->Left += offset;
							_labelsY->Left += offset;
						}
			}

			property Int32 CentreY {
				public: Int32 get() { return Top + (FullHeight >> 1); }
				public: void set(Int32 value)
						{
							Int32 offset = value - CentreY;
							_box->Top += offset;
							_labelsX->Top += offset;
							_labelsY->Top += offset;
						}
			}

			property Control^ Parent {
			public: Control^ get() { return _box->Parent; }
			public: void set(Control^ value)
					{
						if(_box->Parent != nullptr) {
							_box->Parent->Controls->Remove(_labelsX);
							_box->Parent->Controls->Remove(_labelsY);
							_box->Parent->Controls->Remove(_box);
						}

						if(value != nullptr) {
							value->Controls->Add(_box);
							value->Controls->Add(_labelsX);
							value->Controls->Add(_labelsY);
						}
					}
			}

			property Point LastMouseLocation {
				public: Point get() { return _lastMouseLoc; }
			}
		#pragma endregion

			CartesianPlane();
			CartesianPlane(Control^ parent);

			void addMouseMoveEvent(MouseEventHandler^ handler);
			void addMouseDownEvent(MouseEventHandler^ handler);
			void addMouseUpEvent(MouseEventHandler^ handler);

			void startDraw();
			void endDraw();
			void drawObject(GeometricObject^ object);
			void drawObjects(array<GeometricObject^>^ objects);
			void drawObjects(List<GeometricObject^>^ objects);

		private:
			PictureBox^ _box;
			Graphics^ _graphics;
			SolidBrush^ _brush;
			Color _planeBackgroundColor;
			Pen^ _gridPen;
			Color _smallGridColor;
			Color _largeGridColor;
			Color _originGridColor;
			bool _drawGridOn;
			Point _lastMouseLoc;

			Byte _labelBoxSpacing;
			PictureBox^ _labelsX;
			PictureBox^ _labelsY;
			Graphics^ _graphicsX;
			Graphics^ _graphicsY;
			Color _labelBackgroundColor;
			Color _labelTextColor;
			StringFormat^ _labelFormat;
			Font^ _labelFont;
			
			CVPoint<double> _origin;
			Int32 _gridSpacingSmallPx;
			Int32 _gridSpacingLarge;
			Int32 _gridSpacingLargePx;

			Pen^ _objectPen;
			SolidBrush^ _objectBrush;

			void _fixLargeGridSpacingInPixels();

			double _interpretX(double x);
			double _interpretY(double y);
			double _flipY(double y);

			static double _getFirstGridID(double origin, Int32 spacing);
			static double _getFirstGridValue(double gridID, Int32 spacing);
			static Int32 _getFirstGridLocation(double origin, double gridVal);
			Int32 _getGridLargeCheck(double gridID);

			void _clearBackground();
			void _drawGridAndLabels();

			void _drawSmallSpacingGridX(Int32 gridLocation, Int32 largeGridCheck);
			void _drawSmallSpacingGridY(Int32 gridLocation, Int32 largeGridCheck);
			void _drawLargeSpacingGridX(Int32 gridLocation);
			void _drawLargeSpacingGridY(Int32 gridLocation);
			void _drawOriginGridX();
			void _drawOriginGridY();

			void _drawLabelsX(Int32 gridLocation, double gridValue);
			void _drawLabelsY(Int32 gridLocation, double gridValue);

			Graphics^ _setBitmap(PictureBox^ box);

			void _initialize();

			void Event_MouseMoveBox(Object^ sender, MouseEventArgs^ e);
	};
}