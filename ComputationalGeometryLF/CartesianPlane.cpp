#include "CartesianPlane.h"

using namespace ComputationalGeometryLF;

CartesianPlane::CartesianPlane()
{ _initialize(); }

CartesianPlane::CartesianPlane(Control^ parent)
{
	_initialize();
	Parent = parent;
}

void CartesianPlane::addMouseMoveEvent(MouseEventHandler^ handler)
{ _box->MouseMove += handler; }

void CartesianPlane::addMouseDownEvent(MouseEventHandler^ handler)
{ _box->MouseDown += handler; }

void CartesianPlane::addMouseUpEvent(MouseEventHandler^ handler)
{ _box->MouseUp += handler; }

void CartesianPlane::startDraw()
{
	_clearBackground();
	_drawGridAndLabels();
}

void CartesianPlane::endDraw()
{
	_box->Refresh();
	_labelsX->Refresh();
	_labelsY->Refresh();
}

void CartesianPlane::drawObject(GeometricObject^ object)
{
	CVPoint<double> flippedOrigin = CVPoint<double>(_origin.X, _flipY(_origin.Y));
	
	if(object->IsDrawn && object->isInView(ViewRectangle)) {
		if(object->DrawStyleHint.HasFlag(GeometricObject::DrawStyle::Fill))
			object->fill(_graphics, _objectBrush, flippedOrigin);
		if(object->DrawStyleHint.HasFlag(GeometricObject::DrawStyle::Draw))
			object->draw(_graphics, _objectPen, flippedOrigin);
	}
}

void CartesianPlane::drawObjects(array<GeometricObject^>^ objects)
{
	CVRectangle<double> view = ViewRectangle;
	CVPoint<double> flippedOrigin = CVPoint<double>(_origin.X, _flipY(_origin.Y));
	
	for(Int32 i = 0; i < objects->Length; ++i) {
		GeometricObject^ obj = objects[i];

		if(obj->IsDrawn && obj->isInView(view)) {
			if(obj->DrawStyleHint.HasFlag(GeometricObject::DrawStyle::Fill))
				obj->fill(_graphics, _objectBrush, flippedOrigin);
			if(obj->DrawStyleHint.HasFlag(GeometricObject::DrawStyle::Draw))
				obj->draw(_graphics, _objectPen, flippedOrigin);
		}
	}
}

void CartesianPlane::drawObjects(List<GeometricObject^>^ objects)
{
	CVRectangle<double> view = ViewRectangle;
	CVPoint<double> flippedOrigin = CVPoint<double>(_origin.X, _flipY(_origin.Y));
	
	for(Int32 i = 0; i < objects->Count; ++i) {
		GeometricObject^ obj = objects[i];

		if(obj->IsDrawn && obj->isInView(view)) {
			if(obj->DrawStyleHint.HasFlag(GeometricObject::DrawStyle::Fill))
				obj->fill(_graphics, _objectBrush, flippedOrigin);
			if(obj->DrawStyleHint.HasFlag(GeometricObject::DrawStyle::Draw))
				obj->draw(_graphics, _objectPen, flippedOrigin);
		}
	}
}

inline void CartesianPlane::_fixLargeGridSpacingInPixels()
{ _gridSpacingLargePx = _gridSpacingSmallPx * _gridSpacingLarge; }

inline double CartesianPlane::_interpretX(double x)
{ return x - _origin.X; }

inline double CartesianPlane::_interpretY(double y)
{ return _flipY(y) - _origin.Y; }

inline double CartesianPlane::_flipY(double y)
{ return _box->Height - y - 1; }

inline double CartesianPlane::_getFirstGridID(double origin, Int32 spacing)
{ return Math::Ceiling(-origin / spacing); }

inline double CartesianPlane::_getFirstGridValue(double gridID, Int32 spacing)
{ return gridID * spacing; }

inline Int32 CartesianPlane::_getFirstGridLocation(double origin, double gridVal)
{ return (Int32)(origin + gridVal); }

inline Int32 CartesianPlane::_getGridLargeCheck(double gridID)
{
	Int32 lCheck = (Int32)(gridID - (Math::Floor(gridID / _gridSpacingLarge) * _gridSpacingLarge));
	return (lCheck <= 0) ? lCheck + _gridSpacingLarge : lCheck;
}

inline void CartesianPlane::_clearBackground()
{
	_graphics->Clear(_planeBackgroundColor);
	if(LabelsVisible) {
		_graphicsX->Clear(_labelBackgroundColor);
		_graphicsY->Clear(_labelBackgroundColor);
	}
}

inline void CartesianPlane::_drawGridAndLabels()
{
	double firstIdX = _getFirstGridID(Origin.X, _gridSpacingLargePx);
	double firstIdY = _getFirstGridID(Origin.Y, _gridSpacingLargePx);

	double lGridValX = _getFirstGridValue(firstIdX, _gridSpacingLargePx);
	double lGridValY = _getFirstGridValue(firstIdY, _gridSpacingLargePx);

	Int32 lGridLocX = _getFirstGridLocation(Origin.X, lGridValX);
	Int32 lGridLocY = (Int32)_flipY(_getFirstGridLocation(Origin.Y, lGridValY));

	if(LabelsVisible) {
		_brush->Color = _labelTextColor;
		_drawLabelsX(lGridLocX, lGridValX);
		_drawLabelsY(lGridLocY, lGridValY);
	}
	if(_drawGridOn) {
		firstIdX = _getFirstGridID(Origin.X, _gridSpacingSmallPx);
		firstIdY = _getFirstGridID(Origin.Y, _gridSpacingSmallPx);

		double sGridValX = _getFirstGridValue(firstIdX, _gridSpacingSmallPx);
		double sGridValY = _getFirstGridValue(firstIdY, _gridSpacingSmallPx);

		Int32 sGridLocX = _getFirstGridLocation(Origin.X, sGridValX);
		Int32 sGridLocY = (Int32)_flipY(_getFirstGridLocation(Origin.Y, sGridValY));

		Int32 lCheckX = _getGridLargeCheck(firstIdX);
		Int32 lCheckY = _getGridLargeCheck(firstIdY);

		_gridPen->Color = _smallGridColor;
		_drawSmallSpacingGridX(sGridLocX, lCheckX);
		_drawSmallSpacingGridY(sGridLocY, lCheckY);

		_gridPen->Color = _largeGridColor;
		_drawLargeSpacingGridX(lGridLocX);
		_drawLargeSpacingGridY(lGridLocY);

		_gridPen->Color = _originGridColor;
		_drawOriginGridX();
		_drawOriginGridY();
	}
}

inline void CartesianPlane::_drawSmallSpacingGridX(Int32 gridLocation, Int32 largeGridCheck)
{
	while(gridLocation < _box->Width) {
		if(largeGridCheck++ == _gridSpacingLarge) largeGridCheck = 1;
		else _graphics->DrawLine(_gridPen, gridLocation, 0, gridLocation, _box->Height);
		gridLocation += _gridSpacingSmallPx;
	}
}

inline void CartesianPlane::_drawSmallSpacingGridY(Int32 gridLocation, Int32 largeGridCheck)
{
	while(gridLocation >= 0) {
		if(largeGridCheck++ == _gridSpacingLarge) largeGridCheck = 1;
		else _graphics->DrawLine(_gridPen, 0, gridLocation, _box->Width, gridLocation);
		gridLocation -= _gridSpacingSmallPx;
	}
}

inline void CartesianPlane::_drawLargeSpacingGridX(Int32 gridLocation)
{
	while(gridLocation < _box->Width) {
		_graphics->DrawLine(_gridPen, gridLocation, 0, gridLocation, _box->Height);
		gridLocation += _gridSpacingLargePx;
	}
}

inline void CartesianPlane::_drawLargeSpacingGridY(Int32 gridLocation)
{
	while(gridLocation >= 0) {
		_graphics->DrawLine(_gridPen, 0, gridLocation, _box->Width, gridLocation);
		gridLocation -= _gridSpacingLargePx;
	}
}

inline void CartesianPlane::_drawOriginGridX()
{
	if(_origin.X >= 0 && _origin.X < _box->Width)
		_graphics->DrawLine(_gridPen, (Int32)_origin.X, 0, (Int32)_origin.X, _box->Height);
}

inline void CartesianPlane::_drawOriginGridY()
{
	if(_origin.Y >= 0 && _origin.Y < _box->Height) {
		Int32 flippedOriginY = (Int32)_flipY(_origin.Y);
		_graphics->DrawLine(_gridPen, 0, flippedOriginY, (Int32)_box->Width, flippedOriginY);
	}
}

inline void CartesianPlane::_drawLabelsX(Int32 gridLocation, double gridValue)
{
	float labelX = (float)(_labelBoxSpacing + gridLocation);
	float labelY = _labelsX->Height * 0.5f;

	while(gridLocation < _box->Width) {
		_graphicsX->DrawString(gridValue.ToString("0.###"), _labelFont, _brush, labelX, labelY, _labelFormat);
		gridLocation += _gridSpacingLargePx;
		gridValue += _gridSpacingLargePx;
		labelX += _gridSpacingLargePx;
	}
}

inline void CartesianPlane::_drawLabelsY(Int32 gridLocation, double gridValue)
{
	float labelX = _labelsY->Width * 0.5f;
	float labelY = (float)(_labelBoxSpacing + gridLocation);

	while(gridLocation >= 0) {
		_graphicsY->DrawString(gridValue.ToString("0.###"), _labelFont, _brush, labelX, labelY, _labelFormat);
		gridLocation -= _gridSpacingLargePx;
		gridValue += _gridSpacingLargePx;
		labelY -= _gridSpacingLargePx;
	}
}

inline Graphics^ CartesianPlane::_setBitmap(PictureBox^ box)
{
	if(box->Width > 0 && box->Height > 0)
		box->Image = gcnew Bitmap(box->Width, box->Height, System::Drawing::Imaging::PixelFormat::Format32bppArgb);
	return Graphics::FromImage(box->Image);
}

void CartesianPlane::_initialize()
{
	_box = gcnew PictureBox();
	_box->Size = Size(200, 200);
	_graphics = _setBitmap(_box);

	_brush = gcnew SolidBrush(Color::White);
	_planeBackgroundColor = Color::White;
	_gridPen = gcnew Pen(Color::Black, 1);
	_smallGridColor = Color::LightGray;
	_largeGridColor = Color::DarkGray;
	_originGridColor = Color::Black;
	_drawGridOn = true;
	_lastMouseLoc = Point(-1, -1);

	_labelBoxSpacing = 8;
	_labelsX = gcnew PictureBox();
	_labelsX->BorderStyle = BorderStyle::FixedSingle;
	_labelsX->Size = Size(_box->Width + (_labelBoxSpacing << 1), 25);
	_graphicsX = _setBitmap(_labelsX);

	_labelsY = gcnew PictureBox();
	_labelsY->BorderStyle = BorderStyle::FixedSingle;
	_labelsY->Size = Size(25, _box->Height + (_labelBoxSpacing << 1));
	_graphicsY = _setBitmap(_labelsY);

	_labelBackgroundColor = Color::White;
	_labelTextColor = Color::Black;
	_labelFormat = gcnew StringFormat();
	_labelFormat->Alignment = StringAlignment::Center;
	_labelFormat->LineAlignment = StringAlignment::Center;
	_labelFont = gcnew Font("Microsoft Sans Serif", 5.5f);

	_origin = CVPoint<double>(_box->Width >> 1, _box->Height >> 1);
	_gridSpacingSmallPx = 10;
	_gridSpacingLarge = 10;
	_fixLargeGridSpacingInPixels();

	_objectPen = gcnew Pen(Color::White);
	_objectBrush = gcnew SolidBrush(Color::White);

	_labelsY->Location = Point(0, 0);
	_labelsX->Location = Point(_labelsY->Right, _labelsY->Bottom);
	_box->Location = Point(_labelsY->Right + _labelBoxSpacing, _labelsY->Top + _labelBoxSpacing);

	_box->MouseMove += gcnew MouseEventHandler(this, &CartesianPlane::Event_MouseMoveBox);
}

void CartesianPlane::Event_MouseMoveBox(Object^ sender, MouseEventArgs^ e)
{
	_lastMouseLoc.X = (Int32)_interpretX(e->X);
	_lastMouseLoc.Y = (Int32)_interpretY(e->Y);
}