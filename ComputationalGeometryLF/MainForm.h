#pragma once

#include "AddObjectForm.h"
#include "ModObjectForm.h"
#include "RemoveObjectForm.h"
#include "AlgorithmForm.h"
#include "SettingsForm.h"
#include "DataConverter.h"

#define SET_REDRAW 0x0B

namespace ComputationalGeometryLF
{
	using namespace System;
	using namespace System::Text;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	using namespace CVNetControls;
	using namespace CVFileIO;

	[System::Runtime::InteropServices::DllImport("user32.dll")]
	extern int SendMessage(IntPtr hWnd, Int32 wMsg, bool wParam, Int32 lParam);

	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm()
		{
			InitializeComponent();
			_initializeFileManager();
			_initializeUI();

			objects = gcnew List<GeometricObject^>();

			settingsForm = gcnew SettingsForm(menuButtons[3]);
			addObjectForm = gcnew AddObjectForm(menuButtons[4]);
			modObjectForm = gcnew ModObjectForm(menuButtons[5]);
			removeObjectForm = gcnew RemoveObjectForm(menuButtons[6]);
			algorithmForm = gcnew AlgorithmForm(menuButtons[7]);

			Toolbox::init(plane, objects, objectsInfoBox, refreshButton, selectedObject,
							gcnew ModObjectFormRefresher(modObjectForm, &ModObjectForm::refreshPropertyBoxes));
		}

	protected:
		~MainForm()
		{
			if(components)
				delete components;
		}

	private:
		AddObjectForm^ addObjectForm;
		ModObjectForm^ modObjectForm;
		RemoveObjectForm^ removeObjectForm;
		AlgorithmForm^ algorithmForm;
		SettingsForm^ settingsForm;

		Panel^ menuPanel;
		LabelButtonList^ menuButtons;

		Panel^ modePanel;
		LabelButtonTitledList^ modeChoice;
		TitledTextBoxTitledList^ originBoxes;
		LabelButton^ confirmOrigin;

		Panel^ drawPanel;
		LabelButtonTitledList^ objectChoice;
		LabelButtonTitledList^ styleChoice;
		TitledControl<Panel>^ colorPanel;
		LabelButton^ closePoly;

		TitledLabel^ mouseX;
		TitledLabel^ mouseY;
		TitledLabel^ selectedObject;

		TitledTextBox^ objectsInfoBox;
		LabelButton^ refreshButton;

		CartesianPlane^ plane;
		List<GeometricObject^>^ objects;

		FileManager^ fileManager;
		DataConverter^ converter;
		bool mouseDownLeft;
		bool mouseDownRight;
		Point lastMousePos;

		System::ComponentModel::Container ^components;

		Int32 getObjectIndex()
		{
			CVPoint<double> mouseLoc = CVPoint<double>(plane->LastMouseLocation.X, plane->LastMouseLocation.Y);
			for(Int32 i = objects->Count - 1; i >= 0; --i)
				if(BasicAlgorithms::pointContainmentTest(mouseLoc.X, mouseLoc.Y, objects[i]))
					return i + 1;
			return 0;
		}

		void startObjectDrawing()
		{
			if(objectChoice[0]->Activated) startPointDrawing(plane->LastMouseLocation);
			else if(objectChoice[1]->Activated) startSegmentDrawing(plane->LastMouseLocation);
			else if(objectChoice[2]->Activated) startRectangleDrawing(plane->LastMouseLocation);
			else startPolygonDrawing(plane->LastMouseLocation);
		}

		GeometricObject::DrawStyle getObjectDrawStyle()
		{
			GeometricObject::DrawStyle style;
			if(styleChoice[0]->Activated)
				style = (styleChoice[1]->Activated) ? GeometricObject::DrawStyle::DrawAndFill : GeometricObject::DrawStyle::Draw;
			else style = GeometricObject::DrawStyle::Fill;
			return style;
		}

		void startPointDrawing(Point mouseLoc)
		{
			GeometricPoint^ p = gcnew GeometricPoint(mouseLoc.X, mouseLoc.Y);
			p->MainColor = colorPanel->ControlBackColor;
			p->DrawStyleHint = getObjectDrawStyle();
			Toolbox::addObject(p);
		}

		void startSegmentDrawing(Point mouseLoc)
		{
			GeometricSegment^ s = gcnew GeometricSegment(mouseLoc.X, mouseLoc.Y, mouseLoc.X, mouseLoc.Y);
			s->MainColor = colorPanel->ControlBackColor;
			s->DrawStyleHint = getObjectDrawStyle();
			objects->Add(s);
		}

		void startRectangleDrawing(Point mouseLoc)
		{
			GeometricRectangle^ r = gcnew GeometricRectangle(mouseLoc.X, mouseLoc.Y, 0, 0);
			r->MainColor = colorPanel->ControlBackColor;
			r->DrawStyleHint = getObjectDrawStyle();
			objects->Add(r);
		}

		void startPolygonDrawing(Point mouseLoc)
		{
			if(closePoly->Visible) {
				GeometricPolygon^ p = objects[objects->Count - 1]->interpretAs<GeometricPolygon>();
				p->addPoint(mouseLoc.X, mouseLoc.Y);
				Toolbox::redrawPlane();
			}
			else {
				GeometricPolygon^ p = gcnew GeometricPolygon(mouseLoc.X, mouseLoc.Y);
				p->addPoint(mouseLoc.X, mouseLoc.Y);
				p->MainColor = colorPanel->ControlBackColor;
				p->DrawStyleHint = getObjectDrawStyle();
				objects->Add(p);
				closePoly->Visible = true;
				closePoly->refresh();
			}
		}

		void modifyObject(Int32 i)
		{
			if(i > 0) {
				GeometricObject^ obj = objects[i - 1];
				obj->MainColor = colorPanel->ControlBackColor;
				obj->DrawStyleHint = getObjectDrawStyle();
				Toolbox::redrawPlane();
			}
		}

		void removeObject(Int32 i)
		{
			if(i > 0) Toolbox::removeObject(i - 1);
		}

		void selectObject(Int32 i)
		{
			if(i > 0) Toolbox::selectObject(i - 1);
		}

		void moveViewOrigin(Point mouseLoc)
		{
			plane->OriginX += mouseLoc.X - lastMousePos.X;
			plane->OriginY -= mouseLoc.Y - lastMousePos.Y;
			lastMousePos = mouseLoc;
			Toolbox::redrawPlane();
		}

		void updateSegmentDrawing(Point mouseLoc)
		{
			GeometricSegment^ s = objects[objects->Count - 1]->interpretAs<GeometricSegment>();
			s->EndX = plane->LastMouseLocation.X;
			s->EndY = plane->LastMouseLocation.Y;
			Toolbox::redrawPlane();
		}

		void updateRectangleDrawing(Point mouseLoc)
		{
			GeometricRectangle^ r = objects[objects->Count - 1]->interpretAs<GeometricRectangle>();
			CVRectangle<double> rect = CVRectangle<double>(r->Left, r->Bottom, mouseLoc.X - r->Left, mouseLoc.Y - r->Bottom);

			if(rect.Width < 0) {
				r->Width = -rect.Width;
				r->Left = mouseLoc.X;
			}
			else r->Width = rect.Width;

			if(rect.Height < 0) {
				r->Height = -rect.Height;
				r->Bottom = mouseLoc.Y;
			}
			else r->Height = rect.Height;

			Toolbox::redrawPlane();

			r->Left = rect.Left;
			r->Bottom = rect.Top;
			r->Width = rect.Width;
			r->Height = rect.Height;
		}

		void updatePolygonDrawing(Point mouseLoc)
		{
			GeometricPolygon^ p = objects[objects->Count - 1]->interpretAs<GeometricPolygon>();
			p->modPointAt(p->PointCount - 1, plane->LastMouseLocation.X, plane->LastMouseLocation.Y);
			Toolbox::redrawPlane();
		}

		void updateMouseLabels()
		{
			mouseX->ControlText = plane->LastMouseLocation.X.ToString();
			mouseY->ControlText = plane->LastMouseLocation.Y.ToString();
			mouseX->toLabel()->Refresh();
			mouseY->toLabel()->Refresh();
		}

		void stopSegmentDrawing(Point mouseLoc)
		{
			GeometricSegment^ s = objects[objects->Count - 1]->interpretAs<GeometricSegment>();
			s->EndX = mouseLoc.X;
			s->EndY = mouseLoc.Y;
			if(s->Start == s->End) objects->RemoveAt(objects->Count - 1);
			else if(Toolbox::getAutoRefreshInfoBox()) Toolbox::refreshInfoBox();
			Toolbox::redrawPlane();
		}

		void stopRectangleDrawing(Point mouseLoc)
		{
			GeometricRectangle^ r = objects[objects->Count - 1]->interpretAs<GeometricRectangle>();
			r->Width = mouseLoc.X - r->Left;
			r->Height = mouseLoc.Y - r->Bottom;

			if(r->Width < 0) {
				r->Width = -r->Width;
				r->Left = mouseLoc.X;
			}

			if(r->Height < 0) {
				r->Height = -r->Height;
				r->Bottom = mouseLoc.Y;
			}

			if(r->Width == 0 || r->Height == 0) objects->RemoveAt(objects->Count - 1);
			else if(Toolbox::getAutoRefreshInfoBox()) Toolbox::refreshInfoBox();
			Toolbox::redrawPlane();
		}

		void stopPolygonDrawing(Point mouseLoc)
		{
			GeometricPolygon^ p = objects[objects->Count - 1]->interpretAs<GeometricPolygon>();
			p->modPointAt(p->PointCount - 1, plane->LastMouseLocation.X, plane->LastMouseLocation.Y);
			if(Toolbox::getAutoRefreshInfoBox()) Toolbox::refreshInfoBox();
			Toolbox::redrawPlane();
		}

		void enableDrawPanel()
		{
			SendMessage(Handle, SET_REDRAW, false, 0);
			refreshButton->Top += drawPanel->Height;
			objectsInfoBox->ControlHeight -= drawPanel->Height;
			objectsInfoBox->Top += drawPanel->Height;
			plane->BoxHeight -= drawPanel->Height;
			plane->Top += drawPanel->Height;
			drawPanel->Visible = true;
			SendMessage(Handle, SET_REDRAW, true, 0);
			Toolbox::redrawPlane();
			Refresh();
		}

		void disableDrawPanel()
		{
			drawPanel->Visible = false;
			SendMessage(Handle, SET_REDRAW, false, 0);
			refreshButton->Top -= drawPanel->Height;
			objectsInfoBox->ControlHeight += drawPanel->Height;
			objectsInfoBox->Top -= drawPanel->Height;
			plane->BoxHeight += drawPanel->Height;
			plane->Top -= drawPanel->Height;
			SendMessage(Handle, SET_REDRAW, true, 0);
			Toolbox::redrawPlane();
			Refresh();
		}

		void Event_MouseDownPlane(Object^ sender, MouseEventArgs^ e)
		{
			if(e->Button == System::Windows::Forms::MouseButtons::Right) {
				mouseDownRight = true;
				lastMousePos = e->Location;
			}
			else if(e->Button == System::Windows::Forms::MouseButtons::Left) {
				mouseDownLeft = true;
				Int32 i = 0;
				if(modeChoice[3]->Activated) {
					i = getObjectIndex();
					if(i > 0) {
						if(algorithmForm->IsShown) algorithmForm->setObjectIndex(i);
						if(removeObjectForm->IsShown) removeObjectForm->setObjectIndex(i);
						if(modObjectForm->IsShown) modObjectForm->setObjectIndex(i);
					}
				}
				if(modeChoice[0]->Activated) startObjectDrawing();
				else if(modeChoice[1]->Activated) modifyObject((i > 0) ? i : getObjectIndex());
				else if(modeChoice[2]->Activated) removeObject((i > 0) ? i : getObjectIndex());
				else if(modeChoice[3]->Activated) selectObject((i > 0) ? i : getObjectIndex());
			}
		}

		void Event_MouseMovePlane(Object^ sender, MouseEventArgs^ e)
		{	
			if(mouseDownRight)
				moveViewOrigin(e->Location);
			if(mouseDownLeft && modeChoice[0]->Activated) {
				if(objectChoice[1]->Activated) updateSegmentDrawing(plane->LastMouseLocation);
				else if(objectChoice[2]->Activated) updateRectangleDrawing(plane->LastMouseLocation);
				else if(objectChoice[3]->Activated) updatePolygonDrawing(plane->LastMouseLocation);
			}
			updateMouseLabels();
		}

		void Event_MouseUpPlane(Object^ sender, MouseEventArgs^ e)
		{
			if(e->Button == System::Windows::Forms::MouseButtons::Left) {
				mouseDownLeft = false;
				if(modeChoice[0]->Activated) {
					if(objectChoice[1]->Activated) stopSegmentDrawing(plane->LastMouseLocation);
					else if(objectChoice[2]->Activated) stopRectangleDrawing(plane->LastMouseLocation);
					else if(objectChoice[3]->Activated) stopPolygonDrawing(plane->LastMouseLocation);
				}
			}
			else if(e->Button == System::Windows::Forms::MouseButtons::Right) mouseDownRight = false;
		}

		void Event_MouseMove(Object^ sender, MouseEventArgs^ e)
		{
			mouseX->ControlText = "--";
			mouseY->ControlText = "--";
		}

		void Event_MouseDownRefresh(Object^ sender, MouseEventArgs^ e)
		{
			Toolbox::refreshInfoBox();
		}

		void Event_MouseDownColorChange(Object^ sender, MouseEventArgs^ e)
		{
			ColorDialog^ dialog = gcnew ColorDialog();
			if(dialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
				((Control^)sender)->BackColor = dialog->Color;
		}

		void Event_MouseDownModeDraw(Object^ sender, MouseEventArgs^ e)
		{
			if(modeChoice[0]->Activated) {
				modeChoice[1]->deactivate();
				modeChoice[2]->deactivate();
				modeChoice[3]->deactivate();

				if(!drawPanel->Visible) enableDrawPanel();
				if(!objectChoice[0]->Visible) {
					objectChoice->Visible = true;
					styleChoice->Left = objectChoice->Right + 30;
					colorPanel->Left = styleChoice->Right + 30;
				}
			}
			else if(drawPanel->Visible) disableDrawPanel();
		}

		void Event_MouseDownModeMod(Object^ sender, MouseEventArgs^ e)
		{
			if(modeChoice[1]->Activated) {
				modeChoice[0]->deactivate();
				modeChoice[2]->deactivate();
				modeChoice[3]->deactivate();
				closePoly->Visible = false;

				if(!drawPanel->Visible) enableDrawPanel();
				if(objectChoice[0]->Visible) {
					objectChoice->Visible = false;
					styleChoice->Left = 5;
					colorPanel->Left = styleChoice->Right + 30;
				}
			}
			else if(drawPanel->Visible) disableDrawPanel();
		}

		void Event_MouseDownModeRemove(Object^ sender, MouseEventArgs^ e)
		{
			if(modeChoice[2]->Activated) {
				modeChoice[0]->deactivate();
				modeChoice[1]->deactivate();
				modeChoice[3]->deactivate();
			}
			if(drawPanel->Visible) disableDrawPanel();
		}

		void Event_MouseDownModeSelect(Object^ sender, MouseEventArgs^ e)
		{
			if(modeChoice[3]->Activated) {
				modeChoice[0]->deactivate();
				modeChoice[1]->deactivate();
				modeChoice[2]->deactivate();
			}
			if(drawPanel->Visible) disableDrawPanel();
		}

		void Event_MouseDownMoveTo(Object^ sender, MouseEventArgs^ e)
		{
			Int32 x, y;
			if(Int32::TryParse(originBoxes[0]->ControlText, x) && Int32::TryParse(originBoxes[1]->ControlText, y)) {
				plane->Origin = CVPoint<double>((plane->BoxWidth >> 1) - x, (plane->BoxHeight >> 1) - y);
				Toolbox::redrawPlane();
			}
			else MessageBox::Show("Accepts integers only.");
		}

		void Event_MouseDownClosePolygon(Object^ sender, MouseEventArgs^ e)
		{
			closePoly->Visible = false;
		}

		void Event_MouseDownPoint(Object^ sender, MouseEventArgs^ e)
		{
			objectChoice[1]->deactivate();
			objectChoice[2]->deactivate();
			objectChoice[3]->deactivate();
			closePoly->Visible = false;
		}

		void Event_MouseDownSegment(Object^ sender, MouseEventArgs^ e)
		{
			objectChoice[0]->deactivate();
			objectChoice[2]->deactivate();
			objectChoice[3]->deactivate();
			closePoly->Visible = false;
		}

		void Event_MouseDownRectangle(Object^ sender, MouseEventArgs^ e)
		{
			objectChoice[0]->deactivate();
			objectChoice[1]->deactivate();
			objectChoice[3]->deactivate();
			closePoly->Visible = false;
		}

		void Event_MouseDownPolygon(Object^ sender, MouseEventArgs^ e)
		{
			objectChoice[0]->deactivate();
			objectChoice[1]->deactivate();
			objectChoice[2]->deactivate();
		}

		void Event_MouseDownStyleDraw(Object^ sender, MouseEventArgs^ e)
		{
			if(!styleChoice[1]->Activated)
				styleChoice[0]->activate();
		}

		void Event_MouseDownStyleFill(Object^ sender, MouseEventArgs^ e)
		{
			if(!styleChoice[0]->Activated)
				styleChoice[1]->activate();
		}

		void Event_MouseDownSave(Object^ sender, MouseEventArgs^ e)
		{
			if(objects->Count > 0) {
				SaveFileDialog^ dialog = gcnew SaveFileDialog();
				dialog->Filter = "Computational Geometry |*.cvcg";
				dialog->ShowDialog();

				if(dialog->FileName != "") {
					converter->DataObject = objects;
					array<Byte>^ data;
					converter->dataToBytes(data);
					fileManager->save(dialog->FileName, data);
					MessageBox::Show("Data saved successfully!");
				}
			}
			else MessageBox::Show("Must have at least one object!");
		}

		void Event_MouseDownLoad(Object^ sender, MouseEventArgs^ e)
		{
            OpenFileDialog^ dialog = gcnew OpenFileDialog();
            dialog->Filter = "Computational Geometry |*.cvcg";
            dialog->ShowDialog();

            if(dialog->FileName != "") {
				array<Byte>^ data;
				if(fileManager->load(dialog->FileName, data)) {
					converter->ByteArray = data;
					List<GeometricObject^>^ list;
					if(converter->bytesToData(list)) {
						objects = list;
						Toolbox::linkPlane(plane, objects);
						MessageBox::Show("Data loaded successfully!");
					}
					else MessageBox::Show("Data loading unsuccessful.");
				}
				else MessageBox::Show("Data loading unsuccessful.");
			}
		}

		void Event_MouseDownAdd(Object^ sender, MouseEventArgs^ e)
		{
            OpenFileDialog^ dialog = gcnew OpenFileDialog();
            dialog->Filter = "Computational Geometry |*.cvcg";
            dialog->ShowDialog();

            if(dialog->FileName != "") {
				array<Byte>^ data;
				if(fileManager->load(dialog->FileName, data)) {
					converter->ByteArray = data;
					List<GeometricObject^>^ list;
					if(converter->bytesToData(list)) {
						objects->AddRange(list);
						Toolbox::linkPlane(plane, objects);
						MessageBox::Show("Data added successfully!");
					}
					else MessageBox::Show("Data adding unsuccessful.");
				}
				else MessageBox::Show("Data adding unsuccessful.");
			}
		}

		void Event_Resize(Object^ sender, EventArgs^ e)
		{
			Int32 drawPanelOffset = (drawPanel->Visible) ? drawPanel->Height : 0;
			menuPanel->Width = ClientSize.Width + 2;
			modePanel->Width = ClientSize.Width + 2;
			drawPanel->Width = ClientSize.Width + 2;
			plane->BoxSize = System::Drawing::Size(ClientSize.Width - 400, ClientSize.Height - 150 - drawPanelOffset);
			mouseX->Top = plane->Bottom + 10;
			mouseY->Top = mouseX->Bottom + 5;
			selectedObject->Right = plane->Right;
			selectedObject->Top = mouseX->Top;
			objectsInfoBox->ControlHeight = plane->FullHeight - 10 - drawPanelOffset;
			objectsInfoBox->Left = plane->Right + 15;
			refreshButton->Right = objectsInfoBox->Right - 17;
			Toolbox::redrawPlane();
		}

		void Event_OnPaintModePanel(Object^ sender, PaintEventArgs^ e)
		{
			Int32 x = modeChoice->Right + 15;
			e->Graphics->DrawLine(Pens::SteelBlue, x, 2, x, modePanel->Height - 5);
		}

#pragma region Initialization
		void _initializeUI()
		{
			mouseDownLeft = mouseDownRight = false;
			_initializeMenuPanel();
			_initializeModePanel();
			_initializeDrawPanel();
			_initializePlane();
			_initializeInfoBox();
			_initializeEvents();
		}

		void _initializeMenuPanel()
		{
			menuPanel = gcnew Panel();
			menuPanel->Size = System::Drawing::Size(ClientSize.Width + 2, 26);
			menuPanel->Location = Point(-1, -1);
			menuPanel->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			menuPanel->BackColor = Color::SteelBlue;
			Controls->Add(menuPanel);

			menuButtons = gcnew LabelButtonList(menuPanel);
			menuButtons->IsListHorizontal = true;
			menuButtons->SpacingX = 10;
			
			menuButtons->addButton("saveFile", "SAVE TO FILE");
			menuButtons->addButton("loadFile", "LOAD FROM FILE");
			menuButtons->addButton("addFile", "ADD FROM FILE");
			menuButtons->addButton("settings", "SETTINGS");
			menuButtons->addButton("addObject", "ADD OBJECT");
			menuButtons->addButton("modObject", "MODIFY OBJECT");
			menuButtons->addButton("removeObject", "REMOVE OBJECT");
			menuButtons->addButton("algorithms", "ALGORITHMS");

			LabelButtonScheme::Invert(menuButtons, false, menuPanel->BackColor, Color::WhiteSmoke, 1.15f);
			menuButtons->AutoSize = true;
			menuButtons->BorderStyle = gcnew RectangularBorder();
			menuButtons->Font = gcnew System::Drawing::Font("Microsoft Sans Serif", 10);
			menuButtons->Location = Point(5, 3);
			menuButtons->performLayout();
		}

		void _initializeModePanel()
		{
			modePanel = gcnew Panel();
			modePanel->Size = System::Drawing::Size(ClientSize.Width + 2, 21);
			modePanel->Location = Point(-1, menuPanel->Bottom - 1);
			modePanel->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			modePanel->BackColor = Color::LightSteelBlue;
			Controls->Add(modePanel);

			modeChoice = gcnew LabelButtonTitledList(modePanel);
			modeChoice->TitleText = "Operating mode:";
			modeChoice->TitleForeColor = Color::SteelBlue;

			modeChoice->addButton("modeAdd", "DRAW");
			modeChoice->addButton("modeMod", "MODIFY");
			modeChoice->addButton("modeRemove", "REMOVE");
			modeChoice->addButton("modeSelect", "SELECT");

			modeChoice->IsListHorizontal = true;
			modeChoice->SpacingX = -1;

			LabelButtonScheme::Invert(modeChoice, true, ColorFactory::Scale(Color::LightCoral, 0.9f), Color::WhiteSmoke, 1.15f);
			modeChoice->OnClickDeactivatable = true;
			modeChoice->BorderColor = Color::Firebrick;
			modeChoice->AutoSize = false;
			modeChoice->Font = gcnew System::Drawing::Font("Microsoft Sans Serif", 9);
			modeChoice->TitleFont = modeChoice[0]->Font;
			modeChoice->Offset = Point(5, 0);

			for(Int32 i = 0; i < modeChoice->Count; ++i)
				modeChoice[i]->Size = System::Drawing::Size(70, 17);

			modeChoice->performLayout();

			originBoxes = gcnew TitledTextBoxTitledList(modePanel);
			originBoxes->TitleText = "Move to point:";
			originBoxes->TitleForeColor = Color::SteelBlue;

			originBoxes->addTextBox("tBoxX", "[X]:", "0");
			originBoxes->addTextBox("tBoxY", "[Y]:", "0");
			originBoxes->IsListHorizontal = true;
			originBoxes->IsTitleHorizontal = true;
			originBoxes->TitleFont = modeChoice[0]->Font;
			originBoxes->Offset = Point(8, 0);
			originBoxes->SpacingX = 10;

			System::Drawing::Font^ font = gcnew System::Drawing::Font("Microsoft Sans Serif", 6.5f);
			for(Int32 i = 0; i < originBoxes->Count; ++i) {
				originBoxes[i]->ControlFont = font;
				originBoxes[i]->TitleFont = modeChoice[0]->Font;
				originBoxes[i]->ControlSize = System::Drawing::Size(55, 17);
				originBoxes[i]->TitleForeColor = Color::SteelBlue;
				originBoxes[i]->ControlBackColor = Color::WhiteSmoke;
				originBoxes[i]->ControlForeColor = Color::SteelBlue;
				originBoxes[i]->OffsetX = 2;
				originBoxes[i]->MaxLength = 8;
			}

			originBoxes->performLayout();

			confirmOrigin = gcnew LabelButton(modePanel, "confirmOrigin", "MOVE TO");
			confirmOrigin->Font = modeChoice[0]->Font;
			confirmOrigin->Size = System::Drawing::Size(70, 17);
			LabelButtonScheme::Invert(confirmOrigin, false, Color::LightSeaGreen, Color::WhiteSmoke, 1.15f);
			confirmOrigin->BorderColor = Color::ForestGreen;

			modeChoice->Location = Point(5, 1);
			originBoxes->Location = Point(modeChoice->Right + 30, 1);
			confirmOrigin->Location = Point(originBoxes->Right + 30, 1);
		}

		void _initializeDrawPanel()
		{
			drawPanel = gcnew Panel();
			drawPanel->Size = System::Drawing::Size(ClientSize.Width + 2, 21);
			drawPanel->Location = Point(-1, modePanel->Bottom - 1);
			drawPanel->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			drawPanel->BackColor = Color::LightSteelBlue;
			Controls->Add(drawPanel);

			objectChoice = gcnew LabelButtonTitledList(drawPanel);
			objectChoice->TitleText = "Object type:";
			objectChoice->TitleForeColor = Color::SteelBlue;

			objectChoice->addButton("drawPoint", "POINT");
			objectChoice->addButton("drawSegment", "SEGMENT");
			objectChoice->addButton("drawRectangle", "RECTANGLE");
			objectChoice->addButton("drawPolygon", "POLYGON");

			objectChoice->IsListHorizontal = true;
			objectChoice->SpacingX = -1;

			objectChoice[0]->activate();
			LabelButtonScheme::Invert(objectChoice, true, Color::LightSeaGreen, Color::WhiteSmoke, 1.15f);
			objectChoice->BorderColor = Color::ForestGreen;
			objectChoice->AutoSize = false;
			objectChoice->Font = modeChoice[0]->Font;
			objectChoice->TitleFont = modeChoice[0]->Font;
			objectChoice->Offset = Point(5, 0);

			for(Int32 i = 0; i < objectChoice->Count; ++i)
				objectChoice[i]->Size = System::Drawing::Size(100, 17);

			objectChoice->performLayout();

			styleChoice = gcnew LabelButtonTitledList(drawPanel);
			styleChoice->TitleText = "Draw style:";
			styleChoice->TitleForeColor = Color::SteelBlue;

			styleChoice->addButton("styleDraw", "DRAW");
			styleChoice->addButton("styleFill", "FILL");

			styleChoice->IsListHorizontal = true;
			styleChoice->SpacingX = -1;

			styleChoice[0]->activate();
			LabelButtonScheme::Invert(styleChoice, true, ColorFactory::Scale(Color::SkyBlue, 0.9f), Color::WhiteSmoke, 1.15f);
			styleChoice->OnClickDeactivatable = true;
			styleChoice->BorderColor = Color::SteelBlue;
			styleChoice->AutoSize = false;
			styleChoice->Font = modeChoice[0]->Font;
			styleChoice->TitleFont = modeChoice[0]->Font;
			styleChoice->Offset = Point(5, 0);

			for(Int32 i = 0; i < styleChoice->Count; ++i)
				styleChoice[i]->Size = System::Drawing::Size(60, 17);

			styleChoice->performLayout();

			colorPanel = gcnew TitledControl<Panel>();
			colorPanel->Parent = drawPanel;
			colorPanel->TitleText = "Color:";
			colorPanel->TitleForeColor = Color::SteelBlue;
			colorPanel->TitleFont = modeChoice[0]->Font;
			colorPanel->ControlSize = System::Drawing::Size(17, 17);
			colorPanel->OffsetX = 5;
			colorPanel->OffsetY += colorPanel->MiddleAlignmentY + 1;
			((Panel^)colorPanel->fetchWinControl())->BorderStyle = System::Windows::Forms::BorderStyle::None;
			colorPanel->ControlBackColor = Color::Blue;

			closePoly = gcnew LabelButton(drawPanel, "closePoly", "CLOSE POLYGON");
			LabelButtonScheme::Invert(closePoly, false, ColorFactory::Scale(Color::LightCoral, 0.9f), Color::WhiteSmoke, 1.15f);
			closePoly->BorderColor = Color::Firebrick;
			closePoly->AutoSize = false;
			closePoly->Font = modeChoice[0]->Font;
			closePoly->Size = System::Drawing::Size(130, 17);
			closePoly->Visible = false;

			objectChoice->Location = Point(5, 1);
			styleChoice->Location = Point(objectChoice->Right + 30, 1);
			colorPanel->Location = Point(styleChoice->Right + 30, 1);
			closePoly->Location = Point(colorPanel->Right + 30, 1);

			drawPanel->Visible = false;
		}

		void _initializePlane()
		{
			plane = gcnew CartesianPlane(this);
			plane->BoxSize = System::Drawing::Size(ClientSize.Width - 400, ClientSize.Height - 150);
			plane->Location = Point(10, modePanel->Bottom + 10);
			plane->Origin = CVPoint<double>(plane->BoxWidth >> 1, plane->BoxHeight >> 1);

			mouseX = gcnew TitledLabel(this, "mouseX", "Mouse X:", "--");
			mouseY = gcnew TitledLabel(this, "mouseY", "Mouse Y:", "--");
			mouseX->TitleFont = gcnew System::Drawing::Font("Microsoft Sans Serif", 7);
			mouseX->ControlFont = mouseX->TitleFont;
			mouseY->TitleFont = mouseX->TitleFont;
			mouseY->ControlFont = mouseX->TitleFont;
			mouseX->OffsetX = 10;
			mouseY->OffsetX = 10;
			mouseX->Location = Point(plane->Left + plane->LabelBoxGirth + plane->LabelBoxSpacing, plane->Bottom + 10);
			mouseY->Location = Point(mouseX->Left, mouseX->Bottom + 5);

			selectedObject = gcnew TitledLabel(this, "selectedObject", "Selected object:", "--");
			selectedObject->TitleFont = mouseX->TitleFont;
			selectedObject->ControlFont = mouseX->TitleFont;
			selectedObject->TextAlign = ContentAlignment::MiddleRight;
			selectedObject->OffsetX = 10;
			selectedObject->Right = plane->Right;
			selectedObject->Top = mouseX->Top;
		}

		void _initializeInfoBox()
		{
			objectsInfoBox = gcnew TitledTextBox(this, "objectsInfoBox", "Objects:");
			objectsInfoBox->IsTitleVertical = true;
			objectsInfoBox->Offset = Point(0, 5);
			objectsInfoBox->ReadOnly = true;
			objectsInfoBox->ScrollBars = ScrollBars::Both;
			objectsInfoBox->Multiline = true;
			objectsInfoBox->WordWrap = false;
			objectsInfoBox->TextAlign = HorizontalAlignment::Left;
			objectsInfoBox->ControlFont = gcnew System::Drawing::Font("Microsoft Sans Serif", 8);
			objectsInfoBox->ControlSize = System::Drawing::Size(ClientSize.Width - plane->Right - 30, plane->FullHeight - 10);
			objectsInfoBox->Location = Point(plane->Right + 15, plane->Top);

			refreshButton = gcnew LabelButton(this, "refreshButton", "REFRESH");
			LabelButtonScheme::Invert(refreshButton, false, Color::SteelBlue, Color::WhiteSmoke, 1.15f);
			refreshButton->Right = objectsInfoBox->Right - 17;
			refreshButton->Bottom = objectsInfoBox->ControlTop + 1;
			refreshButton->Visible = false;
		}

		void _initializeEvents()
		{
			Resize += gcnew EventHandler(this, &MainForm::Event_Resize);
			MouseMove += gcnew MouseEventHandler(this, &MainForm::Event_MouseMove);
			modePanel->Paint += gcnew PaintEventHandler(this, &MainForm::Event_OnPaintModePanel);

			plane->addMouseMoveEvent(gcnew MouseEventHandler(this, &MainForm::Event_MouseMovePlane));
			plane->addMouseDownEvent(gcnew MouseEventHandler(this, &MainForm::Event_MouseDownPlane));
			plane->addMouseUpEvent(gcnew MouseEventHandler(this, &MainForm::Event_MouseUpPlane));

			refreshButton->addMouseDownEvent(gcnew MouseEventHandler(this, &MainForm::Event_MouseDownRefresh));
			menuButtons[0]->addMouseDownEvent(gcnew MouseEventHandler(this, &MainForm::Event_MouseDownSave));
			menuButtons[1]->addMouseDownEvent(gcnew MouseEventHandler(this, &MainForm::Event_MouseDownLoad));
			menuButtons[2]->addMouseDownEvent(gcnew MouseEventHandler(this, &MainForm::Event_MouseDownAdd));
			modeChoice[0]->addMouseDownEvent(gcnew MouseEventHandler(this, &MainForm::Event_MouseDownModeDraw));
			modeChoice[1]->addMouseDownEvent(gcnew MouseEventHandler(this, &MainForm::Event_MouseDownModeMod));
			modeChoice[2]->addMouseDownEvent(gcnew MouseEventHandler(this, &MainForm::Event_MouseDownModeRemove));
			modeChoice[3]->addMouseDownEvent(gcnew MouseEventHandler(this, &MainForm::Event_MouseDownModeSelect));
			confirmOrigin->addMouseDownEvent(gcnew MouseEventHandler(this, &MainForm::Event_MouseDownMoveTo));
			objectChoice[0]->addMouseDownEvent(gcnew MouseEventHandler(this, &MainForm::Event_MouseDownPoint));
			objectChoice[1]->addMouseDownEvent(gcnew MouseEventHandler(this, &MainForm::Event_MouseDownSegment));
			objectChoice[2]->addMouseDownEvent(gcnew MouseEventHandler(this, &MainForm::Event_MouseDownRectangle));
			objectChoice[3]->addMouseDownEvent(gcnew MouseEventHandler(this, &MainForm::Event_MouseDownPolygon));
			closePoly->addMouseDownEvent(gcnew MouseEventHandler(this, &MainForm::Event_MouseDownClosePolygon));
			styleChoice[0]->addMouseDownEvent(gcnew MouseEventHandler(this, &MainForm::Event_MouseDownStyleDraw));
			styleChoice[1]->addMouseDownEvent(gcnew MouseEventHandler(this, &MainForm::Event_MouseDownStyleFill));
			colorPanel->fetchWinControl()->MouseDown += gcnew MouseEventHandler(this, &MainForm::Event_MouseDownColorChange);
		}

		void _initializeFileManager()
		{
			fileManager = gcnew FileManager();
			fileManager->setSignature("CVCG");
			fileManager->setEncryptKey(	"Czas utworzenia klucza: 2016-02-22 22:36;" + //41
										"Autor algorytmu szyfrujacego i wlasciciel klucza: Lukasz Furlepa (aka CalionVarduk);" + //84
										"Nazwa aplikacji: Computational Geometry~;" + //41
										"Aplikacja stworzona na przedmiot 'Geometria Obliczeniowa', Uniwersytet Slaski - KatowiceIM", //90
										true);
			fileManager->setCompressState(true);
			fileManager->setEncryptionState(true);

			converter = gcnew DataConverter();
		}
#pragma endregion

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->SuspendLayout();
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::ButtonFace;
			this->ClientSize = System::Drawing::Size(1124, 768);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::Sizable;
			this->MinimumSize = this->Size;
			this->Name = L"MainForm";
			this->Text = L"Computational Geometry";
			this->ResumeLayout(false);

		}
#pragma endregion
	};
}
