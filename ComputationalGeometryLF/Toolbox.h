#pragma once

#include "CVNetControls.h"
#include "CartesianPlane.h"

namespace ComputationalGeometryLF
{
	using namespace System;
	using namespace System::Text;
	using namespace System::Drawing;
	using namespace System::Windows::Forms;

	public delegate void ModObjectFormRefresher();

	public ref class Toolbox abstract
	{
		public:
			static void init(CartesianPlane^ plane, List<GeometricObject^>^ objects, TitledTextBox^ objectsInfoBox, LabelButton^ refreshButton, TitledLabel^ selectedObject, ModObjectFormRefresher^ modFormRefresher)
			{
				Toolbox::modFormRefresher += modFormRefresher;
				Toolbox::objectsInfoBox = objectsInfoBox;
				Toolbox::refreshButton = refreshButton;
				Toolbox::selectedObject = selectedObject;
				linkPlane(plane, objects);
			}

			static void linkPlane(CartesianPlane^ plane, List<GeometricObject^>^ objects)
			{
				Toolbox::plane = plane;
				Toolbox::objects = objects;
				if(objects->Count > 0) selectObject(-1);
				refreshInfoBox();
				modFormRefresher->Invoke();
				redrawPlane();
			}

			static void redrawPlane()
			{
				plane->startDraw();
				plane->drawObjects(objects);
				plane->endDraw();
			}

			static void setAutoRefreshInfoBox(bool on)
			{
				autoRefresh = on;
				refreshButton->Visible = !autoRefresh;
				if(autoRefresh) refreshInfoBox();
			}

			static bool getAutoRefreshInfoBox()
			{
				return autoRefresh;
			}

			static void setGridDrawing(bool on)
			{
				plane->DrawGrid = on;
			}

			static void setPlaneLabelDrawing(bool on)
			{
				plane->LabelsVisible = on;
			}

			static void refreshInfoBox()
			{
				StringBuilder^ sb = gcnew StringBuilder();
				for(Int32 i = 0; i < objects->Count; ++i)
					sb->Append((i + 1).ToString() + ". " + objects[i]->ToString() + "\r\n");
				objectsInfoBox->ControlText = sb->ToString();
			}

			static void setBackgroundColor(Color color)
			{
				plane->PlaneBackgroundColor = color;
			}

			static void setSmallGridColor(Color color)
			{
				plane->SmallGridColor = color;
			}

			static void setLargeGridColor(Color color)
			{
				plane->LargeGridColor = color;
			}

			static void setOriginGridColor(Color color)
			{
				plane->OriginGridColor = color;
			}

			static void setSmallGridSpacing(Int32 spacing)
			{
				plane->SmallGridSpacingInPixels = spacing;
			}

			static void setLargeGridSpacing(Int32 spacing)
			{
				plane->LargeGridSpacing = spacing;
			}

			static Int32 moveObjectTo(Int32 iObj, Int32 iDest)
			{
				if(iDest < 0) iDest = 0;
				else if(iDest >= objects->Count) iDest = objects->Count - 1;

				if(iObj != iDest) {
					GeometricObject^ obj = objects[iObj];
					objects->RemoveAt(iObj);
					objects->Insert(iDest, obj);

					Int32 iSelected = getSelectedObjectIndex() - 1;
					if(iSelected == iObj) selectObject(iDest);
					else if(iObj < iDest) {
						if(iSelected > iObj && iSelected <= iDest)
							selectObject(iSelected - 1);
					}
					else if(iSelected >= iDest && iSelected < iObj)
						selectObject(iSelected + 1);

					if(autoRefresh) refreshInfoBox();
				}
				return iDest;
			}

			static void setObject(Int32 i, GeometricObject^ obj)
			{
				objects[i] = obj;
				if(autoRefresh) refreshInfoBox();
				redrawPlane();
			}

			static void addObject(GeometricObject^ obj)
			{
				objects->Add(obj);
				if(autoRefresh) refreshInfoBox();
				redrawPlane();
			}

			static void addObjects(List<GeometricObject^>^ objs)
			{
				for(Int32 i = 0; i < objs->Count; ++i) objects->Add(objs[i]);
				if(autoRefresh) refreshInfoBox();
				redrawPlane();
			}

			static void removeObject(Int32 i)
			{
				objects->RemoveAt(i);
				if(autoRefresh) refreshInfoBox();
				_fixSelectWhenRemove(i);
				modFormRefresher->Invoke();
				redrawPlane();
			}

			static void removePoints()
			{
				for(Int32 i = objects->Count - 1; i >= 0; --i) {
					if(objects[i]->isPoint()) {
						objects->RemoveAt(i);
						_fixSelectWhenRemove(i);
					}
				}
				if(autoRefresh) refreshInfoBox();
				modFormRefresher->Invoke();
				redrawPlane();
			}

			static void removeSegments()
			{
				for(Int32 i = objects->Count - 1; i >= 0; --i) {
					if(objects[i]->isSegment()) {
						objects->RemoveAt(i);
						_fixSelectWhenRemove(i);
					}
				}
				if(autoRefresh) refreshInfoBox();
				modFormRefresher->Invoke();
				redrawPlane();
			}

			static void removeRectangles()
			{
				for(Int32 i = objects->Count - 1; i >= 0; --i) {
					if(objects[i]->isRectangle()) {
						objects->RemoveAt(i);
						_fixSelectWhenRemove(i);
					}
				}
				if(autoRefresh) refreshInfoBox();
				modFormRefresher->Invoke();
				redrawPlane();
			}

			static void removeAll()
			{
				for(Int32 i = objects->Count - 1; i >= 0; --i)
					objects->RemoveAt(i);

				selectObject(-1);
				if(autoRefresh) refreshInfoBox();
				modFormRefresher->Invoke();
				redrawPlane();
			}

			static void selectObject(Int32 i)
			{
				if(i >= 0) selectedObject->ControlText = "index [" + (i + 1).ToString() + "], " + objects[i]->ToString();
				else selectedObject->ControlText = "--";
				selectedObject->Right = plane->Right;
			}

			static GeometricObject^ getObject(Int32 i)
			{
				return objects[i];
			}

			static Int32 getObjectCount()
			{
				return objects->Count;
			}

			static Int32 getSelectedObjectIndex()
			{
				if(selectedObject->ControlText->Length > 2) {
					Int32 i = 7;
					StringBuilder^ index = gcnew StringBuilder(8);
					while(Char::IsNumber(selectedObject->ControlText[i]))
						index->Append(selectedObject->ControlText[i++]);
					return Int32::Parse(index->ToString());
				}
				return 0;
			}

		private:
			static ModObjectFormRefresher^ modFormRefresher;
			static bool autoRefresh = true;
			static List<GeometricObject^>^ objects;
			static CartesianPlane^ plane;
			static TitledTextBox^ objectsInfoBox;
			static LabelButton^ refreshButton;
			static TitledLabel^ selectedObject;

			static void _fixSelectWhenRemove(Int32 i)
			{
				Int32 iSelected = getSelectedObjectIndex() - 1;
				if(iSelected >= 0) {
					if(iSelected > i) selectObject(iSelected - 1);
					else if(iSelected == i) selectObject(-1);
				}
			}
	};
}