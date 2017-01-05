#pragma once

#include "Toolbox.h"

namespace ComputationalGeometryLF
{
	using namespace System;
	using namespace System::Windows::Forms;

	using namespace CVNetControls;

	public ref class ModObjectForm : public SecondaryForm
	{
		public:
			ModObjectForm(LabelButton^ controlButton) : SecondaryForm(controlButton)
			{
				Text = "Modify Object";
				_initializeUI();
			}

			void setObjectIndex(Int32 i)
			{
				box->ControlText = i.ToString();
			}

			void refreshPropertyBoxes()
			{
				Int32 i = getChosenIndex();
				if(i >= 0 && i < Toolbox::getObjectCount()) {
					GeometricObject^ obj = Toolbox::getObject(i);
					if(obj->isPoint()) pointChosen(obj);
					else if(obj->isSegment()) segmentChosen(obj);
					else rectangleChosen(obj);
				}
				else {
					upDown->ControlText = "0";
					chosenObject->ControlText = "--";
					textBoxes[0]->TitleText = "Origin [X]:";
					textBoxes[1]->TitleText = "Origin [Y]:";
					textBoxes[0]->ControlText = "";
					textBoxes[1]->ControlText = "";
					textBoxes[2]->Visible = false;
					textBoxes[3]->Visible = false;
				}
			}

		private:
			TitledTextBox^ box;
			TitledLabel^ chosenObject;
			LabelButtonTitledList^ upDownButtons;
			TitledLabel^ upDown;
			TitledCheckBoxTitledList^ checkBoxes;
			SingleCheckBox^ checkBox;
			TitledTextBoxTitledList^ textBoxes;
			TitledControl<Panel>^ colorPanel;
			LabelButton^ confirm;
			LabelButton^ modify;

			Int32 getChosenIndex()
			{
				return (chosenObject->ControlText->Equals("--")) ? -1 : Int32::Parse(chosenObject->ControlText) - 1;
			}

			GeometricObject::DrawStyle getObjectDrawStyle()
			{
				GeometricObject::DrawStyle style;
				if(checkBoxes[0]->Checked)
					style = (checkBoxes[1]->Checked) ? GeometricObject::DrawStyle::DrawAndFill : GeometricObject::DrawStyle::Draw;
				else style = GeometricObject::DrawStyle::Fill;
				return style;
			}

			void pointChosen(GeometricObject^ obj)
			{
				GeometricPoint^ p = obj->interpretAs<GeometricPoint>();
				textBoxes[0]->TitleText = "Origin [X]:";
				textBoxes[1]->TitleText = "Origin [Y]:";
				textBoxes[0]->ControlText = p->X.ToString();
				textBoxes[1]->ControlText = p->Y.ToString();
				textBoxes[2]->Visible = false;
				textBoxes[3]->Visible = false;
			}

			void segmentChosen(GeometricObject^ obj)
			{
				GeometricSegment^ s = obj->interpretAs<GeometricSegment>();
				textBoxes[0]->TitleText = "Start [X]:";
				textBoxes[1]->TitleText = "Start [Y]:";
				textBoxes[2]->TitleText = "End [X]:";
				textBoxes[3]->TitleText = "End [Y]:";
				textBoxes[0]->ControlText = s->StartX.ToString();
				textBoxes[1]->ControlText = s->StartY.ToString();
				textBoxes[2]->ControlText = s->EndX.ToString();
				textBoxes[3]->ControlText = s->EndY.ToString();
				textBoxes[2]->Visible = true;
				textBoxes[3]->Visible = true;
			}

			void rectangleChosen(GeometricObject^ obj)
			{
				GeometricRectangle^ r = obj->interpretAs<GeometricRectangle>();
				textBoxes[0]->TitleText = "Origin [X]:";
				textBoxes[1]->TitleText = "Origin [Y]:";
				textBoxes[2]->TitleText = "Width:";
				textBoxes[3]->TitleText = "Height:";
				textBoxes[0]->ControlText = r->Left.ToString();
				textBoxes[1]->ControlText = r->Bottom.ToString();
				textBoxes[2]->ControlText = r->Width.ToString();
				textBoxes[3]->ControlText = r->Height.ToString();
				textBoxes[2]->Visible = true;
				textBoxes[3]->Visible = true;
			}

			void modPointCoords(GeometricObject^ obj)
			{
				Int32 x, y;
				if(Int32::TryParse(textBoxes[0]->ControlText, x) && Int32::TryParse(textBoxes[1]->ControlText, y)) {
					GeometricPoint^ p = obj->interpretAs<GeometricPoint>();
					p->X = x;
					p->Y = y;
				}
				else MessageBox::Show("Accepts integers only.");
			}

			void modSegmentCoords(GeometricObject^ obj)
			{
				Int32 sx, sy, ex, ey;
				if(Int32::TryParse(textBoxes[0]->ControlText, sx) && Int32::TryParse(textBoxes[1]->ControlText, sy) &&
					Int32::TryParse(textBoxes[2]->ControlText, ex) && Int32::TryParse(textBoxes[3]->ControlText, ey)) {
					GeometricSegment^ s = obj->interpretAs<GeometricSegment>();
					s->Start = CVPoint<double>(sx, sy);
					s->End = CVPoint<double>(ex, ey);
				}
				else MessageBox::Show("Accepts integers only.");
			}

			void modRectangleCoords(GeometricObject^ obj)
			{
				Int32 x, y, w, h;
				if(Int32::TryParse(textBoxes[0]->ControlText, x) && Int32::TryParse(textBoxes[1]->ControlText, y) &&
					Int32::TryParse(textBoxes[2]->ControlText, w) && Int32::TryParse(textBoxes[3]->ControlText, h)) {
					GeometricRectangle^ r = obj->interpretAs<GeometricRectangle>();
					r->Left = x;
					r->Bottom = y;
					r->Width = w;
					r->Height = h;

					if(r->Width < 0) {
						r->Width = -r->Width;
						r->Left -= r->Width;
					}

					if(r->Height < 0) {
						r->Height = -r->Height;
						r->Bottom -= r->Height;
					}
				}
				else MessageBox::Show("Accepts integers only.");
			}

			void Event_MouseDownConfirm(Object^ sender, MouseEventArgs^ e)
			{
				Int32 i;
				if(Int32::TryParse(box->ControlText, i)) {
					if(i >= 1 && i <= Toolbox::getObjectCount()) {
						chosenObject->ControlText = i.ToString();
						GeometricObject^ obj = Toolbox::getObject(i - 1);

						upDown->ControlText = "0";
						if(obj->isPoint()) pointChosen(obj);
						else if(obj->isSegment()) segmentChosen(obj);
						else rectangleChosen(obj);
					}
					else MessageBox::Show("Index does not exist.");
				}
				else MessageBox::Show("Accepts integers only.");
			}

			void Event_MouseDownModify(Object^ sender, MouseEventArgs^ e)
			{
				Int32 i = getChosenIndex();
				if(i >= 0 && i < Toolbox::getObjectCount()) {
					GeometricObject^ obj = Toolbox::getObject(i);
					obj->DrawStyleHint = getObjectDrawStyle();
					obj->MainColor = colorPanel->ControlBackColor;

					if(textBoxes[0]->Enabled) {
						if(obj->isPoint()) modPointCoords(obj);
						else if(obj->isSegment()) modSegmentCoords(obj);
						else modRectangleCoords(obj);
					}

					Int32 j = Int32::Parse(upDown->ControlText);
					chosenObject->ControlText = (Toolbox::moveObjectTo(i, i + j) + 1).ToString();
					upDown->ControlText = "0";
					Toolbox::redrawPlane();
				}
				else MessageBox::Show("Index does not exist.");
			}

			void Event_MouseDownDraw(Object^ sender, MouseEventArgs^ e)
			{
				if(!checkBoxes[1]->Checked)
					checkBoxes[0]->Checked = true;
			}

			void Event_MouseDownFill(Object^ sender, MouseEventArgs^ e)
			{
				if(!checkBoxes[0]->Checked)
					checkBoxes[1]->Checked = true;
			}

			void Event_MouseDownColor(Object^ sender, MouseEventArgs^ e)
			{
				ColorDialog^ dialog = gcnew ColorDialog();
				if(dialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
					colorPanel->ControlBackColor = dialog->Color;
			}

			void Event_MouseDownCheck(Object^ sender, MouseEventArgs^ e)
			{
				textBoxes->Enabled = checkBox->Checked;
			}

			void Event_MouseDownUp(Object^ sender, MouseEventArgs^ e)
			{
				if(chosenObject->ControlText != "--")
					upDown->ControlText = (Int32::Parse(upDown->ControlText) + 1).ToString();
			}

			void Event_MouseDownDown(Object^ sender, MouseEventArgs^ e)
			{
				if(chosenObject->ControlText != "--")
					upDown->ControlText = (Int32::Parse(upDown->ControlText) - 1).ToString();
			}

			void Event_OnPaint(Object^ sender, PaintEventArgs^ e)
			{
				Int32 y = box->Bottom + 12;
				e->Graphics->DrawLine(Pens::Black, 15, y, ClientSize.Width - 15, y);
				y = chosenObject->Bottom + 12;
				e->Graphics->DrawLine(Pens::Black, 15, y, ClientSize.Width - 15, y);
			}

#pragma region Initialization
			void _initializeUI()
			{
				_initializeBox();
				_initializeConfirmButton();
				_initializeChosenObject();
				_initializeUpDown();
				_initializeCheckBoxes();
				_initializeTextBoxes();
				_initializeColorPanel();
				_initializeModifyButton();
				_initializeEvents();
				ClientSize = System::Drawing::Size(confirm->Right + 10, modify->Bottom + 10);
			}

			void _initializeBox()
			{
				box = gcnew TitledTextBox(this, "tBox", "Object's index:");
				box->IsTitleVertical = false;
				box->Offset = Point(15, 0);
				box->MaxLength = 7;
				box->Location = Point(10, 10);
			}

			void _initializeConfirmButton()
			{
				confirm = gcnew LabelButton(this, "confirmButton", "CONFIRM");
				LabelButtonScheme::Invert(confirm, false, Color::SteelBlue, Color::WhiteSmoke, 1.15f);
				confirm->BorderColor = Color::MidnightBlue;
				confirm->Location = Point(box->Right + 20, box->Top);
			}

			void _initializeChosenObject()
			{
				chosenObject = gcnew TitledLabel(this, "chosenObject", "Chosen object's index:", "--");
				chosenObject->OffsetX = 10;
				chosenObject->Location = Point(10, box->Bottom + 25);
			}

			void _initializeUpDown()
			{
				upDownButtons = gcnew LabelButtonTitledList(this);
				upDownButtons->TitleText = "Move the chosen object";

				upDownButtons->addButton("buttonUp", "UP");
				upDownButtons->addButton("buttonDown", "DOWN");

				upDownButtons->IsListHorizontal = true;
				upDownButtons->SpacingX = -1;

				LabelButtonScheme::Invert(upDownButtons, false, Color::LightSeaGreen, Color::WhiteSmoke, 1.15f);
				upDownButtons->BorderColor = Color::DarkGreen;
				upDownButtons->AutoSize = false;
				upDownButtons->Offset = Point(37, 0);

				for(Int32 i = 0; i < upDownButtons->Count; ++i)
					upDownButtons[i]->Size = System::Drawing::Size(75, 20);

				upDownButtons->Location = Point(10, chosenObject->Bottom + 25);
				upDownButtons->performLayout();

				upDown = gcnew TitledLabel(this, "upDown", "on the object's list by:", "0");
				upDown->OffsetX = 48;
				upDown->Location = Point(10, upDownButtons->Bottom + 5);
			}

			void _initializeCheckBoxes()
			{
				checkBoxes = gcnew TitledCheckBoxTitledList(this);
				checkBoxes->addCheckBox("isDraw", "Draw:", true);
				checkBoxes->addCheckBox("isFill", "Fill:", false);
				checkBoxes->enableMultiSelection();

				checkBoxes->IsListVertical = true;
				checkBoxes->IsTitleVertical = true;
				checkBoxes->Offset = Point(0, 5);
				checkBoxes->TitleText = "Draw style:";

				for(Int32 i = 0; i < checkBoxes->Count; ++i) {
					checkBoxes[i]->OffsetX = 16;
					checkBoxes[i]->OffsetY += checkBoxes[i]->MiddleAlignmentY + 1;
				}

				checkBoxes->Location = Point(10, upDown->Bottom + 20);
				checkBoxes->performLayout();
				checkBoxes->performHorizontalAlignment();
			}

			void _initializeTextBoxes()
			{
				textBoxes = gcnew TitledTextBoxTitledList(this);
				textBoxes->addTextBox("tBoxOriginX", "Origin [X]:", "");
				textBoxes->addTextBox("tBoxOriginY", "Origin [Y]:", "");
				textBoxes->addTextBox("tBoxAux1", "End [X]:", "");
				textBoxes->addTextBox("tBoxAux2", "End [Y]:", "");

				textBoxes[2]->Visible = false;
				textBoxes[3]->Visible = false;

				textBoxes->IsListVertical = true;
				textBoxes->IsTitleVertical = true;
				textBoxes->Offset = Point(0, 5);
				textBoxes->SpacingY = -1;
				textBoxes->TitleText = "Properties:";

				for(Int32 i = 0; i < textBoxes->Count; ++i) {
					textBoxes[i]->OffsetX = 20;
					textBoxes[i]->MaxLength = 8;
				}

				textBoxes->Location = Point(checkBoxes->Right + 75, checkBoxes->Top);
				textBoxes->performLayout();
				textBoxes->performHorizontalAlignment();
				textBoxes->Enabled = false;

				checkBox = gcnew SingleCheckBox(this, "checkBox", false);
				checkBox->ForeColor = ColorFactory::Scale(Color::LightGreen, 0.85f);
				checkBox->Left = textBoxes[0]->ControlLeft;
				checkBox->Bottom = textBoxes->TitleBottom;
			}

			void _initializeColorPanel()
			{
				colorPanel = gcnew TitledControl<Panel>();
				colorPanel->Parent = this;
				colorPanel->TitleText = "Color:";
				colorPanel->ControlSize = System::Drawing::Size(17, 17);
				colorPanel->OffsetX = 0;
				colorPanel->OffsetY += colorPanel->MiddleAlignmentY + 1;
				colorPanel->ControlBackColor = Color::Blue;
				((Panel^)colorPanel->fetchWinControl())->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
				colorPanel->Left = checkBoxes->Left;
				colorPanel->Top = checkBoxes->Bottom + 3;
				colorPanel->OffsetX = checkBoxes[0]->ControlLeft - (colorPanel->ControlWidth - checkBoxes[0]->ControlWidth) - colorPanel->TitleRight;
			}

			void _initializeModifyButton()
			{
				modify = gcnew LabelButton(this, "modifyButton", "MODIFY");
				LabelButtonScheme::Invert(modify, false, Color::SteelBlue, Color::WhiteSmoke, 1.15f);
				modify->BorderColor = Color::MidnightBlue;
				modify->Left = 10;
				modify->Bottom = textBoxes->Bottom;
			}

			void _initializeEvents()
			{
				Paint += gcnew PaintEventHandler(this, &ModObjectForm::Event_OnPaint);

				checkBoxes[0]->addMouseDownEvent(gcnew MouseEventHandler(this, &ModObjectForm::Event_MouseDownDraw));
				checkBoxes[1]->addMouseDownEvent(gcnew MouseEventHandler(this, &ModObjectForm::Event_MouseDownFill));
				upDownButtons[0]->addMouseDownEvent(gcnew MouseEventHandler(this, &ModObjectForm::Event_MouseDownUp));
				upDownButtons[1]->addMouseDownEvent(gcnew MouseEventHandler(this, &ModObjectForm::Event_MouseDownDown));
				colorPanel->fetchWinControl()->MouseDown += gcnew MouseEventHandler(this, &ModObjectForm::Event_MouseDownColor);
				checkBox->addMouseDownEvent(gcnew MouseEventHandler(this, &ModObjectForm::Event_MouseDownCheck));
				modify->addMouseDownEvent(gcnew MouseEventHandler(this, &ModObjectForm::Event_MouseDownModify));
				confirm->addMouseDownEvent(gcnew MouseEventHandler(this, &ModObjectForm::Event_MouseDownConfirm));
			}
#pragma endregion
	};
}