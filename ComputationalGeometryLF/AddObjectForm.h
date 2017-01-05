#pragma once

#include "Toolbox.h"

namespace ComputationalGeometryLF
{
	using namespace System;
	using namespace System::Windows::Forms;

	using namespace CVNetControls;

	public ref class AddObjectForm : public SecondaryForm
	{
		public:
			AddObjectForm(LabelButton^ controlButton) : SecondaryForm(controlButton)
			{
				Text = "Add Object";
				_initializeUI();
			}

		private:
			TitledCheckBoxTitledList^ checkBoxes;
			TitledTextBoxTitledList^ textBoxes;
			LabelButton^ confirm;

			void addPoint()
			{
				Int32 x, y;
				if(Int32::TryParse(textBoxes[0]->ControlText, x) && Int32::TryParse(textBoxes[1]->ControlText, y)) {
					GeometricObject^ p = gcnew GeometricPoint(x, y);
					p->MainColor = Color::Red;
					Toolbox::addObject(p);
				}
				else MessageBox::Show("Accepts integers only.");
			}

			void addSegment()
			{
				Int32 sx, sy, ex, ey;
				if(Int32::TryParse(textBoxes[0]->ControlText, sx) && Int32::TryParse(textBoxes[1]->ControlText, sy) &&
					Int32::TryParse(textBoxes[2]->ControlText, ex) && Int32::TryParse(textBoxes[3]->ControlText, ey)) {
					GeometricObject^ s = gcnew GeometricSegment(sx, sy, ex, ey);
					s->MainColor = Color::Blue;
					s->DrawStyleHint = GeometricObject::DrawStyle::DrawAndFill;
					Toolbox::addObject(s);
				}
				else MessageBox::Show("Accepts integers only.");
			}

			void addRectangle()
			{
				Int32 x, y, w, h;
				if(Int32::TryParse(textBoxes[0]->ControlText, x) && Int32::TryParse(textBoxes[1]->ControlText, y) &&
					Int32::TryParse(textBoxes[2]->ControlText, w) && Int32::TryParse(textBoxes[3]->ControlText, h)) {
					GeometricRectangle^ r = gcnew GeometricRectangle(x, y, w, h);

					if(r->Width < 0) {
						r->Width = -r->Width;
						r->Left -= r->Width;
					}

					if(r->Height < 0) {
						r->Height = -r->Height;
						r->Bottom -= r->Height;
					}

					r->MainColor = Color::ForestGreen;
					r->DrawStyleHint = GeometricObject::DrawStyle::DrawAndFill;
					Toolbox::addObject(r);
				}
				else MessageBox::Show("Accepts integers only.");
			}

			void Event_PointChecked(Object^ sender, MouseEventArgs^ e)
			{
				if(textBoxes[2]->Visible) {
					textBoxes[0]->TitleText = "Origin [X]:";
					textBoxes[1]->TitleText = "Origin [Y]:";

					textBoxes[2]->Visible = false;
					textBoxes[3]->Visible = false;
				}
			}

			void Event_SegmentChecked(Object^ sender, MouseEventArgs^ e)
			{
				textBoxes[0]->TitleText = "Start [X]:";
				textBoxes[1]->TitleText = "Start [Y]:";
				textBoxes[2]->TitleText = "End [X]:";
				textBoxes[3]->TitleText = "End [Y]:";

				textBoxes[2]->Visible = true;
				textBoxes[3]->Visible = true;
			}

			void Event_RectangleChecked(Object^ sender, MouseEventArgs^ e)
			{
				textBoxes[0]->TitleText = "Origin [X]:";
				textBoxes[1]->TitleText = "Origin [Y]:";
				textBoxes[2]->TitleText = "Width:";
				textBoxes[3]->TitleText = "Height:";

				textBoxes[2]->Visible = true;
				textBoxes[3]->Visible = true;
			}

			void Event_MouseDownConfirm(Object^ sender, MouseEventArgs^ e)
			{
				if(checkBoxes[0]->Checked) addPoint();
				else if(checkBoxes[1]->Checked) addSegment();
				else addRectangle();
			}

#pragma region Initialization
			void _initializeUI()
			{
				_initializeCheckBoxes();
				_initializeTextBoxes();
				_initializeConfirmButton();
				_initializeEvents();
				ClientSize = System::Drawing::Size(textBoxes->Right + 10, textBoxes->Bottom + 10);
			}

			void _initializeCheckBoxes()
			{
				checkBoxes = gcnew TitledCheckBoxTitledList(this);
				checkBoxes->addCheckBox("isPoint", "Point:", true);
				checkBoxes->addCheckBox("isSegment", "Segment:", false);
				checkBoxes->addCheckBox("isRectangle", "Rectangle:", false);
				checkBoxes->disableMultiSelection(0);

				checkBoxes->IsListVertical = true;
				checkBoxes->IsTitleVertical = true;
				checkBoxes->Offset = Point(0, 5);
				checkBoxes->TitleText = "Object type:";

				for(Int32 i = 0; i < checkBoxes->Count; ++i) {
					checkBoxes[i]->OffsetX = 16;
					checkBoxes[i]->OffsetY += checkBoxes[i]->MiddleAlignmentY + 1;
				}

				checkBoxes->Location = Point(10, 10);
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

				textBoxes->Location = Point(checkBoxes->Right + 40, checkBoxes->Top);
				textBoxes->performLayout();
				textBoxes->performHorizontalAlignment();
			}

			void _initializeConfirmButton()
			{
				confirm = gcnew LabelButton(this, "confirmButton", "ADD");
				LabelButtonScheme::Invert(confirm, false, Color::SteelBlue, Color::WhiteSmoke, 1.15f);
				confirm->BorderColor = Color::MidnightBlue;
				confirm->Location = Point(checkBoxes->Left, checkBoxes->Bottom + 10);
			}

			void _initializeEvents()
			{
				checkBoxes[0]->addMouseDownEvent(gcnew MouseEventHandler(this, &AddObjectForm::Event_PointChecked));
				checkBoxes[1]->addMouseDownEvent(gcnew MouseEventHandler(this, &AddObjectForm::Event_SegmentChecked));
				checkBoxes[2]->addMouseDownEvent(gcnew MouseEventHandler(this, &AddObjectForm::Event_RectangleChecked));
				confirm->addMouseDownEvent(gcnew MouseEventHandler(this, &AddObjectForm::Event_MouseDownConfirm));
			}
#pragma endregion
	};
}