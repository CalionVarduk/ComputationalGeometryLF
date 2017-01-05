#pragma once

#include "Toolbox.h"

namespace ComputationalGeometryLF
{
	using namespace System;
	using namespace System::Windows::Forms;

	using namespace CVNetControls;

	public ref class RemoveObjectForm : public SecondaryForm
	{
		public:
			RemoveObjectForm(LabelButton^ controlButton) : SecondaryForm(controlButton)
			{
				Text = "Remove Object";
				_initializeUI();
			}

			void setObjectIndex(Int32 i)
			{
				box->ControlText = i.ToString();
			}

		private:
			TitledTextBox^ box;
			LabelButtonTitledList^ buttons;
			LabelButton^ confirm;

			void Event_MouseDownConfirm(Object^ sender, MouseEventArgs^ e)
			{
				Int32 i;
				if(Int32::TryParse(box->ControlText, i)) {
					if(i >= 1 && i <= Toolbox::getObjectCount()) Toolbox::removeObject(i - 1);
					else MessageBox::Show("Index does not exist.");
				}
				else MessageBox::Show("Accepts integers only.");
			}

			void Event_MouseDownPoint(Object^ sender, MouseEventArgs^ e)
			{
				Toolbox::removePoints();
			}

			void Event_MouseDownSegment(Object^ sender, MouseEventArgs^ e)
			{
				Toolbox::removeSegments();
			}

			void Event_MouseDownRectangle(Object^ sender, MouseEventArgs^ e)
			{
				Toolbox::removeRectangles();
			}

			void Event_MouseDownAll(Object^ sender, MouseEventArgs^ e)
			{
				Toolbox::removeAll();
			}

#pragma region Initialization
			void _initializeUI()
			{
				_initializeBox();
				_initializeButtons();
				_initializeConfirmButton();
				_initializeEvents();
				ClientSize = System::Drawing::Size(buttons->Right + 10, buttons->Bottom + 10);
			}

			void _initializeBox()
			{
				box = gcnew TitledTextBox(this, "tBox", "Object's index:");
				box->IsTitleVertical = false;
				box->Offset = Point(25, 0);
				box->MaxLength = 7;
				box->Location = Point(10, 10);
			}

			void _initializeButtons()
			{
				buttons = gcnew LabelButtonTitledList(this);
				buttons->TitleText = "Group removal:";
				buttons->IsTitleVertical = true;
				buttons->Offset = Point(0, 5);
				buttons->IsListHorizontal = true;
				buttons->SpacingX = -1;
			
				buttons->addButton("removePoints", "POINTS");
				buttons->addButton("removeSegments", "SEGMENTS");
				buttons->addButton("removeRectangles", "RECTANGLES");
				buttons->addButton("removeAll", "ALL");

				LabelButtonScheme::Invert(buttons, false, Color::LightSeaGreen, Color::WhiteSmoke, 1.15f);
				buttons->BorderColor = Color::DarkGreen;
				buttons->AutoSize = false;
				
				for(Int32 i = 0; i < buttons->Count; ++i)
					buttons[i]->Width = 120;

				buttons->Location = Point(10, box->Bottom + 15);
				buttons->performLayout();
				buttons[3]->Location = Point(buttons[1]->Left, buttons[1]->Bottom - 1);
				buttons->updateBounds();
			}

			void _initializeConfirmButton()
			{
				confirm = gcnew LabelButton(this, "confirmButton", "REMOVE");
				LabelButtonScheme::Invert(confirm, false, Color::SteelBlue, Color::WhiteSmoke, 1.15f);
				confirm->BorderColor = Color::MidnightBlue;
				confirm->Top = box->Top;
				confirm->Right = buttons->Right;
			}

			void _initializeEvents()
			{
				buttons[0]->addMouseDownEvent(gcnew MouseEventHandler(this, &RemoveObjectForm::Event_MouseDownPoint));
				buttons[1]->addMouseDownEvent(gcnew MouseEventHandler(this, &RemoveObjectForm::Event_MouseDownSegment));
				buttons[2]->addMouseDownEvent(gcnew MouseEventHandler(this, &RemoveObjectForm::Event_MouseDownRectangle));
				buttons[3]->addMouseDownEvent(gcnew MouseEventHandler(this, &RemoveObjectForm::Event_MouseDownAll));
				confirm->addMouseDownEvent(gcnew MouseEventHandler(this, &RemoveObjectForm::Event_MouseDownConfirm));
			}
#pragma endregion
	};
}