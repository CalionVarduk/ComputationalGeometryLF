#pragma once

#include "Toolbox.h"

namespace ComputationalGeometryLF
{
	using namespace System;
	using namespace System::Windows::Forms;

	using namespace CVNetControls;

	public ref class SettingsForm : public SecondaryForm
	{
		public:
			SettingsForm(LabelButton^ controlButton) : SecondaryForm(controlButton)
			{
				Text = "Settings";
				_initializeUI();
			}

		private:
			TitledCheckBoxList^ checkBoxes;
			ControlTitledList<TitledControl<Panel>>^ colorPanels;
			TitledTextBoxList^ textBoxes;
			LabelButton^ confirm;

			void Event_MouseDownConfirm(Object^ sender, MouseEventArgs^ e)
			{
				Toolbox::setAutoRefreshInfoBox(checkBoxes[0]->Checked);
				Toolbox::setGridDrawing(checkBoxes[1]->Checked);
				Toolbox::setPlaneLabelDrawing(checkBoxes[2]->Checked);
				Toolbox::setBackgroundColor(colorPanels[0]->ControlBackColor);
				Toolbox::setLargeGridColor(colorPanels[1]->ControlBackColor);
				Toolbox::setSmallGridColor(colorPanels[2]->ControlBackColor);
				Toolbox::setOriginGridColor(colorPanels[3]->ControlBackColor);

				Int32 small, large;
				if(Int32::TryParse(textBoxes[0]->ControlText, small) && Int32::TryParse(textBoxes[1]->ControlText, large)) {
					if(small < 5) small = 5;
					Toolbox::setSmallGridSpacing(small);
					Toolbox::setLargeGridSpacing(large);
				}
				else MessageBox::Show("Accepts integers only.");

				Toolbox::redrawPlane();
			}

			void Event_MouseDownBackground(Object^ sender, MouseEventArgs^ e)
			{
				ColorDialog^ dialog = gcnew ColorDialog();
				if(dialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
					colorPanels[0]->ControlBackColor = dialog->Color;
			}

			void Event_MouseDownPrimary(Object^ sender, MouseEventArgs^ e)
			{
				ColorDialog^ dialog = gcnew ColorDialog();
				if(dialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
					colorPanels[1]->ControlBackColor = dialog->Color;
			}

			void Event_MouseDownSecondary(Object^ sender, MouseEventArgs^ e)
			{
				ColorDialog^ dialog = gcnew ColorDialog();
				if(dialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
					colorPanels[2]->ControlBackColor = dialog->Color;
			}

			void Event_MouseDownOrigin(Object^ sender, MouseEventArgs^ e)
			{
				ColorDialog^ dialog = gcnew ColorDialog();
				if(dialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
					colorPanels[3]->ControlBackColor = dialog->Color;
			}

#pragma region Initialization
			void _initializeUI()
			{
				_initializeTextBoxes();
				_initializePanels();
				_initializeCheckBoxes();
				_initializeConfirmButton();
				_initializeEvents();
				ClientSize = System::Drawing::Size(confirm->Right + 10, confirm->Bottom + 10);
			}

			void _initializeTextBoxes()
			{
				textBoxes = gcnew TitledTextBoxList(this);
				textBoxes->addTextBox("secondaryGrid", "Secondary grid spacing (in pixels):", "10");
				textBoxes->addTextBox("primaryGrid", "Primary grid spacing (in secondary grid count):", "10");

				textBoxes->IsListVertical = true;
				textBoxes->SpacingY = -1;

				for(Int32 i = 0; i < textBoxes->Count; ++i) {
					textBoxes[i]->OffsetX = 42;
					textBoxes[i]->MaxLength = 7;
				}

				textBoxes->Location = Point(10, 10);
				textBoxes->performLayout();
				textBoxes->performHorizontalAlignment();
			}
			
			void _initializePanels()
			{
				colorPanels = gcnew ControlTitledList<TitledControl<Panel>>(this);
				colorPanels->add(gcnew TitledControl<Panel>());
				colorPanels->add(gcnew TitledControl<Panel>());
				colorPanels->add(gcnew TitledControl<Panel>());
				colorPanels->add(gcnew TitledControl<Panel>());

				colorPanels->TitleText = "Plane settings:";
				colorPanels->IsListVertical = true;
				colorPanels->IsTitleVertical = true;
				colorPanels->Offset = Point(0, 8);
				colorPanels->Spacing = Point(0, -1);

				colorPanels[0]->TitleText = "Background color:";
				colorPanels[1]->TitleText = "Primary grid color:";
				colorPanels[2]->TitleText = "Secondary grid color:";
				colorPanels[3]->TitleText = "Origin grid color:";

				colorPanels[0]->ControlBackColor = Color::White;
				colorPanels[1]->ControlBackColor = Color::DarkGray;
				colorPanels[2]->ControlBackColor = Color::LightGray;
				colorPanels[3]->ControlBackColor = Color::Black;

				for(Int32 i = 0; i < colorPanels->Count; ++i) {
					colorPanels[i]->OffsetX = 10;
					((Panel^)colorPanels[i]->fetchWinControl())->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
					colorPanels[i]->ControlSize = System::Drawing::Size(55, 25);
					colorPanels[i]->OffsetY += colorPanels[i]->MiddleAlignmentY + 1;
				}

				colorPanels->Location = Point(10, textBoxes->Bottom + 15);
				colorPanels->performLayout();
				colorPanels->performHorizontalAlignment();
			}

			void _initializeCheckBoxes()
			{
				checkBoxes = gcnew TitledCheckBoxList(this);
				checkBoxes->addCheckBox("autoRefreshBox", "Auto refresh the list of objects:", true);
				checkBoxes->addCheckBox("drawGrid", "Draw grid on the plane:", true);
				checkBoxes->addCheckBox("drawLabels", "Draw the plane's axis labels:", true);

				checkBoxes->Spacing = Point(0, 5);
				checkBoxes->enableMultiSelection();

				for(Int32 i = 0; i < checkBoxes->Count; ++i) {
					checkBoxes[i]->OffsetX = 10;
					checkBoxes[i]->OffsetY += checkBoxes[i]->MiddleAlignmentY + 1;
				}

				checkBoxes->Location = Point(colorPanels->Right + 25, colorPanels[0]->TitleTop);
				checkBoxes->performLayout();
				checkBoxes->performHorizontalAlignment();
			}

			void _initializeConfirmButton()
			{
				confirm = gcnew LabelButton(this, "confirmButton", "CONFIRM");
				LabelButtonScheme::Invert(confirm, false, Color::LightSeaGreen, Color::WhiteSmoke, 1.15f);
				confirm->BorderColor = Color::DarkGreen;
				confirm->Right = checkBoxes->Right;
				confirm->Bottom = colorPanels->Bottom;
			}

			void _initializeEvents()
			{
				confirm->addMouseDownEvent(gcnew MouseEventHandler(this, &SettingsForm::Event_MouseDownConfirm));
				colorPanels[0]->fetchWinControl()->MouseDown += gcnew MouseEventHandler(this, &SettingsForm::Event_MouseDownBackground);
				colorPanels[1]->fetchWinControl()->MouseDown += gcnew MouseEventHandler(this, &SettingsForm::Event_MouseDownPrimary);
				colorPanels[2]->fetchWinControl()->MouseDown += gcnew MouseEventHandler(this, &SettingsForm::Event_MouseDownSecondary);
				colorPanels[3]->fetchWinControl()->MouseDown += gcnew MouseEventHandler(this, &SettingsForm::Event_MouseDownOrigin);
			}
#pragma endregion
	};
}