#pragma once

#include "Toolbox.h"
#include "ClosestPairOfPoints.h"
#include "FarthestPairOfPoints.h"
#include "PointInclusion.h"

namespace ComputationalGeometryLF
{
	using namespace System;
	using namespace System::Windows::Forms;
	using namespace System::Threading;

	using namespace CVNetControls;

	public ref class AlgorithmForm : public SecondaryForm
	{
		public:
			AlgorithmForm(LabelButton^ controlButton) : SecondaryForm(controlButton)
			{
				Text = "Algorithms";
				_initializeUI();
			}

			void setObjectIndex(Int32 i)
			{
				if(checkBoxes[0]->Checked) textBoxes[0]->ControlText = i.ToString();
				else textBoxes[1]->ControlText = i.ToString();
			}

		private:
			TitledTextBoxTitledList^ textBoxes;
			SingleCheckBoxList^ checkBoxes;
			LabelButtonList^ buttons;

			void Event_MouseDownOrientation(Object^ sender, MouseEventArgs^ e)
			{
				GeometricObject^ obj1;
				GeometricObject^ obj2;

				if(parseGeometricObjects(obj1, obj2)) {
					if(obj1->isPoint() && obj2->isSegment()) {
						double outcome = BasicAlgorithms::orientationTest(obj1->interpretAs<GeometricPoint>(), obj2->interpretAs<GeometricSegment>());
						MessageBox::Show(BasicAlgorithms::interpretPointOrientationTest(outcome));
					}
					else if(obj1->isSegment() && obj2->isPoint()) {
						double outcome = BasicAlgorithms::orientationTest(obj2->interpretAs<GeometricPoint>(), obj1->interpretAs<GeometricSegment>());
						MessageBox::Show(BasicAlgorithms::interpretPointOrientationTest(outcome));
					}
					else if(obj1->isSegment() && obj2->isSegment()) {
						Tuple<double, double>^ outcome = BasicAlgorithms::orientationTest(obj2->interpretAs<GeometricSegment>(), obj1->interpretAs<GeometricSegment>());
						MessageBox::Show(BasicAlgorithms::interpretSegmentOrientationTest(outcome));
					}
					else MessageBox::Show("Improper objects. Requires a POINT and a SEGMENT or TWO SEGMENTS.");
				}
			}

			void Event_MouseDownBelonging(Object^ sender, MouseEventArgs^ e)
			{
				GeometricObject^ obj1;
				GeometricObject^ obj2;

				if(parseGeometricObjects(obj1, obj2)) {
					if(obj1->isPoint() && obj2->isSegment()) {
						bool outcome = BasicAlgorithms::belongingnessTest(obj1->interpretAs<GeometricPoint>(), obj2->interpretAs<GeometricSegment>());
						MessageBox::Show((outcome) ? "The POINT BELONGS to the SEGMENT." : "The POINT does NOT BELONG to the SEGMENT.");
					}
					else if(obj1->isSegment() && obj2->isPoint()) {
						bool outcome = BasicAlgorithms::belongingnessTest(obj2->interpretAs<GeometricPoint>(), obj1->interpretAs<GeometricSegment>());
						MessageBox::Show((outcome) ? "The POINT BELONGS to the SEGMENT." : "The POINT does NOT BELONG to the SEGMENT.");
					}
					else if(obj1->isSegment() && obj2->isSegment()) {
						bool outcome1 = BasicAlgorithms::belongingnessTest(obj1->interpretAs<GeometricSegment>(), obj2->interpretAs<GeometricSegment>());
						bool outcome2 = BasicAlgorithms::belongingnessTest(obj2->interpretAs<GeometricSegment>(), obj1->interpretAs<GeometricSegment>());
						if(outcome1 && outcome2) MessageBox::Show("The SEGMENTS SHARE coordinates.");
						else if(outcome1) MessageBox::Show("The 1ST SEGMENT BELONGS to the 2ND SEGMENT.");
						else if(outcome2) MessageBox::Show("The 2ND SEGMENT BELONGS to the 1ST SEGMENT.");
						else MessageBox::Show("Neither of the SEGMENTS belongs to another.");
					}
					else if(obj1->isPoint() && obj2->isPoint()) {
						bool outcome = (obj1->interpretAs<GeometricPoint>()->Location == obj2->interpretAs<GeometricPoint>()->Location);
						MessageBox::Show((outcome) ? "The POINTS SHARE coordinates." : "The POINTS do NOT SHARE coordinates.");
					}
					else MessageBox::Show("Improper objects. Requires a POINT or a SEGMENT.");
				}
			}

			void Event_MouseDownContain(Object^ sender, MouseEventArgs^ e)
			{
				GeometricObject^ obj1;
				GeometricObject^ obj2;

				if(parseGeometricObjects(obj1, obj2)) {
					if(obj1->isRectangle()) {
						if(obj2->isPoint()) {
							bool outcome = BasicAlgorithms::pointContainmentTest(obj2->interpretAs<GeometricPoint>(), obj1);
							MessageBox::Show((outcome) ? "The POINT is CONTAINED inside the RECTANGLE." : "The POINT is NOT CONTAINED inside the RECTANGLE.");
						}
						else {
							bool outcome = BasicAlgorithms::containmentTest(obj2, obj1);
							MessageBox::Show((outcome) ? "The 2ND OBJECT is CONTAINED inside the RECTANGLE." : "The 2ND OBJECT is NOT CONTAINED inside the RECTANGLE.");
						}
					}
					else if(obj2->isRectangle()) {
						if(obj1->isPoint()) {
							bool outcome = BasicAlgorithms::pointContainmentTest(obj1->interpretAs<GeometricPoint>(), obj2);
							MessageBox::Show((outcome) ? "The POINT is CONTAINED inside the RECTANGLE." : "The POINT is NOT CONTAINED inside the RECTANGLE.");
						}
						else {
							bool outcome = BasicAlgorithms::containmentTest(obj1, obj2);
							MessageBox::Show((outcome) ? "The 1ST OBJECT is CONTAINED inside the RECTANGLE." : "The 1ST OBJECT is NOT CONTAINED inside the RECTANGLE.");
						}
					}
					else MessageBox::Show("Improper objects. Requires a RECTANGLE.");
				}
			}

			void Event_MouseDownBoxIntersect(Object^ sender, MouseEventArgs^ e)
			{
				GeometricObject^ obj1;
				GeometricObject^ obj2;

				if(parseGeometricObjects(obj1, obj2)) {
					if(!obj1->isPoint() && !obj2->isPoint()) {
						bool outcome = BasicAlgorithms::boundingBoxIntersectionTest(obj1, obj2);
						MessageBox::Show((outcome) ? "OBJECTS' bounding boxes INTERSECT." : "OBJECTS' bounding boxes do NOT INTERSECT.");
					}
					else MessageBox::Show("Improper objects. Requires TWO NON-POINT objects.");
				}
			}

			void Event_MouseDownIntersect(Object^ sender, MouseEventArgs^ e)
			{
				GeometricObject^ obj1;
				GeometricObject^ obj2;

				if(parseGeometricObjects(obj1, obj2)) {
					if(obj1->isSegment() && obj2->isSegment()) {
						bool outcome = BasicAlgorithms::intersectionTest(obj1->interpretAs<GeometricSegment>(), obj2->interpretAs<GeometricSegment>());
						MessageBox::Show((outcome) ? "Segments INTERSECT." : "Segments do NOT INTERSECT.");
					}
					else MessageBox::Show("Improper objects. Requires TWO SEGMENTS.");
				}
			}

			void Event_MouseDownIntersectPoint(Object^ sender, MouseEventArgs^ e)
			{
				GeometricObject^ obj1;
				GeometricObject^ obj2;

				if(parseGeometricObjects(obj1, obj2)) {
					if(obj1->isSegment() && obj2->isSegment()) {
						GeometricPoint^ intersection = BasicAlgorithms::getIntersectionPoint(obj1->interpretAs<GeometricSegment>(), obj2->interpretAs<GeometricSegment>());
						if(intersection != nullptr) {
							intersection->MainColor = Color::Red;
							Toolbox::addObject(intersection);
						}
						else MessageBox::Show("Segments do NOT INTERSECT.");
					}
					else MessageBox::Show("Improper objects. Requires TWO SEGMENTS.");
				}
			}

			void Event_MouseDownClosestPair(Object^ sender, MouseEventArgs^ e)
			{
				GeometricObject^ obj1;
				GeometricObject^ obj2;

				if(parseGeometricObjects(obj1, obj2)) {
					if(obj1->isRectangle()) {
						PointPair^ outcome = ClosestPairOfPoints::findByTree(obj1->interpretAs<GeometricRectangle>(), false);
						MessageBox::Show("A: " + outcome->Item1->ToString() + "\r\nB: " + outcome->Item2->ToString() + "\r\nDelta: " + outcome->Item3.ToString());
					}
					else MessageBox::Show("Improper object. ---");
				}
			}

			void Event_MouseDownConvexHull(Object^ sender, MouseEventArgs^ e)
			{
				GeometricObject^ obj1;
				GeometricObject^ obj2;

				if(parseGeometricObjects(obj1, obj2)) {
					if(obj1->isRectangle()) {
						GeometricPolygon^ hull = ConvexHull::quickHull(obj1->interpretAs<GeometricRectangle>(), false);
						if(hull->PointCount > 1) Toolbox::addObject(hull);
						//MessageBox::Show("A: " + outcome->Item1->ToString() + "\r\nB: " + outcome->Item2->ToString() + "\r\nDelta: " + outcome->Item3.ToString());
					}
					else MessageBox::Show("Improper object. ---");
				}
			}

			void Event_MouseDownFarthestPair(Object^ sender, MouseEventArgs^ e)
			{
				GeometricObject^ obj1;
				GeometricObject^ obj2;

				if(parseGeometricObjects(obj1, obj2)) {
					if(obj1->isRectangle()) {
						PointPair^ outcome = FarthestPairOfPoints::find(obj1->interpretAs<GeometricRectangle>(), false);
						MessageBox::Show("A: " + outcome->Item1->ToString() + "\r\nB: " + outcome->Item2->ToString() + "\r\nDelta: " + outcome->Item3.ToString());
					}
					else MessageBox::Show("Improper object. ---");
				}
			}

			void Event_MouseDownIntersectPoints(Object^ sender, MouseEventArgs^ e)
			{
				GeometricObject^ obj1;
				GeometricObject^ obj2;

				if(parseGeometricObjects(obj1, obj2)) {
					if(obj1->isRectangle()) {
						List<Intersection^>^ intersections = IntersectionPoints::solve(obj1->interpretAs<GeometricRectangle>(), true);
						List<GeometricObject^>^ points = gcnew List<GeometricObject^>(intersections->Count + 1);
						for(Int32 i = 0; i < intersections->Count; ++i) {
							points->Add(gcnew GeometricPoint(intersections[i]->Point));
							points[i]->MainColor = Color::Red;
						}
						Toolbox::addObjects(points);
						//MessageBox::Show("A: " + outcome->Item1->ToString() + "\r\nB: " + outcome->Item2->ToString() + "\r\nDelta: " + outcome->Item3.ToString());
					}
					else MessageBox::Show("Improper object. ---");
				}
			}

			void Event_MouseDownCcwPoly(Object^ sender, MouseEventArgs^ e)
			{
				GeometricObject^ obj1;
				GeometricObject^ obj2;

				if(parseGeometricObjects(obj1, obj2)) {
					if(obj1->isPolygon()) {
						GeometricPolygon^ p = obj1->interpretAs<GeometricPolygon>();
						p = Triangulation::GetCounterClockwiseOrdered(p);
						Toolbox::setObject(Int32::Parse(textBoxes[0]->ControlText) - 1, p);
					}
					else MessageBox::Show("Improper object. ---");
				}
			}

			void Event_MouseDownTriangulate(Object^ sender, MouseEventArgs^ e)
			{
				GeometricObject^ obj1;
				GeometricObject^ obj2;

				if(parseGeometricObjects(obj1, obj2)) {
					if(obj1->isPolygon()) {
						GeometricPolygon^ p = obj1->interpretAs<GeometricPolygon>();
						TriangulatedPolygon^ tp = Triangulation::TriangulateNaive(p);
						if(tp != nullptr) {
							List<GeometricObject^>^ objs = gcnew List<GeometricObject^>();
							for(Int32 i = 0; i < tp->Segments->Count; ++i) objs->Add(tp->Segments[i]);
							for(Int32 i = 0; i < objs->Count; ++i) objs[i]->MainColor = Color::Red;
							Toolbox::addObjects(objs);

							traverseGraph(tp->TriangleGraphRoot, nullptr);
							
						}
						else MessageBox::Show("Provided polygon is not simple.");
					}
					else MessageBox::Show("Improper object. ---");
				}
			}

			void traverseGraph(TriangulationRegion^ node, TriangulationRegion^ parent)
			{
				GeometricPolygon^ p = gcnew GeometricPolygon();
				p->addPoint(node->Points[0]);
				p->addPoint(node->Points[1]);
				p->addPoint(node->Points[2]);
				p->MainColor = Color::Blue;
				Toolbox::addObject(p);
				Thread::Sleep(2000);

				for(Int32 i = 0; i < node->Neighbours->Count; ++i) {
					if(node->Neighbours[i] != parent) {
						traverseGraph(node->Neighbours[i], node);
					}
				}
			}

			void Event_MouseDownTurnPoints(Object^ sender, MouseEventArgs^ e)
			{
				GeometricObject^ obj1;
				GeometricObject^ obj2;

				if(parseGeometricObjects(obj1, obj2)) {
					if(obj1->isPolygon()) {
						GeometricPolygon^ p = obj1->interpretAs<GeometricPolygon>();
						List<GeometricPoint^>^ pts = Triangulation::GetPolygonVertexTypes(p);
						List<GeometricObject^>^ objs = gcnew List<GeometricObject^>();
						for(Int32 i = 0; i < pts->Count; ++i) objs->Add(pts[i]);
						Toolbox::addObjects(objs);
					}
					else MessageBox::Show("Improper object. ---");
				}
			}

			void Event_MouseDownTriangMonotone(Object^ sender, MouseEventArgs^ e)
			{
				GeometricObject^ obj1;
				GeometricObject^ obj2;

				if(parseGeometricObjects(obj1, obj2)) {
					if(obj1->isPolygon()) {
						GeometricPolygon^ p = obj1->interpretAs<GeometricPolygon>();
						TriangulatedPolygon^ tp = Triangulation::TriangulateMonotoneY(p);
						if(tp != nullptr) {
							List<GeometricObject^>^ objs = gcnew List<GeometricObject^>();
							for(Int32 i = 0; i < tp->Segments->Count; ++i) objs->Add(tp->Segments[i]);
							for(Int32 i = 0; i < objs->Count; ++i) objs[i]->MainColor = Color::Red;
							Toolbox::addObjects(objs);
						}
						else MessageBox::Show("Provided polygon is not y-monotone.");
					}
					else MessageBox::Show("Improper object. ---");
				}
			}

			void Event_MouseDownTrichromatic(Object^ sender, MouseEventArgs^ e)
			{
				GeometricObject^ obj1;
				GeometricObject^ obj2;

				if(parseGeometricObjects(obj1, obj2)) {
					if(obj1->isPolygon()) {
						GeometricPolygon^ p = obj1->interpretAs<GeometricPolygon>();
						List<GeometricPoint^>^ chromatic = Triangulation::GetTrichromaticPoints(p);
						if(chromatic != nullptr) {
							List<GeometricObject^>^ objs = gcnew List<GeometricObject^>();
							for(Int32 i = 0; i < chromatic->Count; ++i) objs->Add(chromatic[i]);
							Toolbox::addObjects(objs);
						}
						else MessageBox::Show("Provided polygon is not simple.");
					}
					else MessageBox::Show("Improper object. ---");
				}
			}

			void Event_MouseDownPointInPoly(Object^ sender, MouseEventArgs^ e)
			{
				GeometricObject^ obj1;
				GeometricObject^ obj2;

				if(parseGeometricObjects(obj1, obj2)) {
					if(obj1->isPoint() && obj2->isPolygon()) {
						GeometricPoint^ p = obj1->interpretAs<GeometricPoint>();
						GeometricPolygon^ poly = obj2->interpretAs<GeometricPolygon>();
						bool outcome = PointInclusion::IsInsidePolygon(p, poly);
						MessageBox::Show((outcome) ? "Point IS inside the polygon." : "Point IS NOT inside the polygon.");
					}
					else if(obj1->isPolygon() && obj2->isPoint()) {
						GeometricPoint^ p = obj2->interpretAs<GeometricPoint>();
						GeometricPolygon^ poly = obj1->interpretAs<GeometricPolygon>();
						bool outcome = PointInclusion::IsInsidePolygon(p, poly);
						MessageBox::Show((outcome) ? "Point IS inside the polygon." : "Point IS NOT inside the polygon.");
					}
					else MessageBox::Show("Improper objects. ---");
				}
			}

			void Event_MouseDownPointInConvex(Object^ sender, MouseEventArgs^ e)
			{
				GeometricObject^ obj1;
				GeometricObject^ obj2;

				if(parseGeometricObjects(obj1, obj2)) {
					if(obj1->isPoint() && obj2->isPolygon()) {
						GeometricPoint^ p = obj1->interpretAs<GeometricPoint>();
						GeometricPolygon^ poly = obj2->interpretAs<GeometricPolygon>();
						bool outcome = PointInclusion::IsInsideConvexPolygon(p, poly);
						MessageBox::Show((outcome) ? "Point IS inside the polygon." : "Point IS NOT inside the polygon.");
					}
					else if(obj1->isPolygon() && obj2->isPoint()) {
						GeometricPoint^ p = obj2->interpretAs<GeometricPoint>();
						GeometricPolygon^ poly = obj1->interpretAs<GeometricPolygon>();
						bool outcome = PointInclusion::IsInsideConvexPolygon(p, poly);
						MessageBox::Show((outcome) ? "Point IS inside the polygon." : "Point IS NOT inside the polygon.");
					}
					else MessageBox::Show("Improper objects. ---");
				}
			}

			bool parseGeometricObjects(GeometricObject^% OUT_first, GeometricObject^% OUT_second)
			{
				Int32 i1, i2;
				if(Int32::TryParse(textBoxes[0]->ControlText, i1) && Int32::TryParse(textBoxes[1]->ControlText, i2)) {
					if(i1 != i2 && i1 >= 1 && i1 <= Toolbox::getObjectCount() && i2 >= 1 && i2 <= Toolbox::getObjectCount()) {
						OUT_first = Toolbox::getObject(i1 - 1);
						OUT_second = Toolbox::getObject(i2 - 1);
						return true;
					}
					else MessageBox::Show("Improper indexes.");
				}
				else MessageBox::Show("Accepts integers only.");
				return false;
			}

#pragma region Initialization
			void _initializeUI()
			{
				_initializeTextBoxes();
				_initializeCheckBoxes();
				_initializeButtons();
				_initializeEvents();
				ClientSize = System::Drawing::Size(buttons->Right + 10, buttons->Bottom + 10);
			}

			void _initializeTextBoxes()
			{
				textBoxes = gcnew TitledTextBoxTitledList(this);
				textBoxes->addTextBox("boxSelect1", "First object's index:", "");
				textBoxes->addTextBox("boxSelect2", "Second object's index:", "");

				textBoxes->IsListVertical = true;
				textBoxes->IsTitleVertical = true;
				textBoxes->Offset = Point(0, 5);
				textBoxes->SpacingY = -1;
				textBoxes->TitleText = "Selected Objects:";

				for(Int32 i = 0; i < textBoxes->Count; ++i) {
					textBoxes[i]->OffsetX = 15;
					textBoxes[i]->MaxLength = 7;
				}

				textBoxes->Location = Point(10, 10);
				textBoxes->performLayout();
				textBoxes->performHorizontalAlignment();
			}

			void _initializeCheckBoxes()
			{
				checkBoxes = gcnew SingleCheckBoxList(this);
				checkBoxes->addCheckBox("checkSelect1", true);
				checkBoxes->addCheckBox("checkSelect2", false);

				checkBoxes->disableMultiSelection(0);
				checkBoxes->IsListVertical = true;

				for(Int32 i = 0; i < checkBoxes->Count; ++i) {
					checkBoxes[i]->Left = textBoxes[i]->Right + 5;
					checkBoxes[i]->CentreY = textBoxes[i]->CentreY;
				}

				checkBoxes->updateBounds();
			}

			void _initializeButtons()
			{
				buttons = gcnew LabelButtonList(this);
				buttons->addButton("aOrientation", "ORIENTATION TEST");
				buttons->addButton("aBelongingness", "BELONGINGNESS TEST");
				buttons->addButton("aContainment", "CONTAINMENT TEST");
				buttons->addButton("aBoxIntersection", "BOX INTERSECTION TEST");
				buttons->addButton("aIntersection", "INTERSECTION TEST");
				buttons->addButton("aIntersectionPoint", "INTERSECTION POINT");
				buttons->addButton("aClosestPair", "CLOSEST PAIR OF POINTS");
				buttons->addButton("aFarthestPair", "FARTHEST PAIR OF POINTS");
				buttons->addButton("aConvexHull", "CONVEX HULL");
				buttons->addButton("aIntersectionPoints", "INTERSECTION POINTS");
				buttons->addButton("aCcwPoly", "CCW POLYGON");
				buttons->addButton("aTriangulate", "TRIANGULATE POLYGON");
				buttons->addButton("aTurnPoints", "POLYGON VERTEX TYPES");
				buttons->addButton("aTriangMonotone", "TRIANGULATE Y-MONO");
				buttons->addButton("aTrichromatic", "TRICHROMATIC GRAPH");
				buttons->addButton("aPointInPoly", "POINT IN POLYGON");
				buttons->addButton("aPointInConvex", "POINT IN CONVEX");

				LabelButtonScheme::Invert(buttons, false, Color::LightSeaGreen, Color::WhiteSmoke, 1.15f);
				buttons->BorderColor = Color::DarkGreen;
				buttons->IsListVertical = true;
				buttons->SpacingY = -1;
				buttons->Location = Point(checkBoxes->Right + 30, textBoxes->Top);
				
				for(Int32 i = 0; i < buttons->Count; ++i)
					buttons[i]->Width = buttons[7]->PreferredSize.Width;

				buttons->performLayout();
			}

			void _initializeEvents()
			{
				buttons[0]->addMouseDownEvent(gcnew MouseEventHandler(this, &AlgorithmForm::Event_MouseDownOrientation));
				buttons[1]->addMouseDownEvent(gcnew MouseEventHandler(this, &AlgorithmForm::Event_MouseDownBelonging));
				buttons[2]->addMouseDownEvent(gcnew MouseEventHandler(this, &AlgorithmForm::Event_MouseDownContain));
				buttons[3]->addMouseDownEvent(gcnew MouseEventHandler(this, &AlgorithmForm::Event_MouseDownBoxIntersect));
				buttons[4]->addMouseDownEvent(gcnew MouseEventHandler(this, &AlgorithmForm::Event_MouseDownIntersect));
				buttons[5]->addMouseDownEvent(gcnew MouseEventHandler(this, &AlgorithmForm::Event_MouseDownIntersectPoint));
				buttons[6]->addMouseDownEvent(gcnew MouseEventHandler(this, &AlgorithmForm::Event_MouseDownClosestPair));
				buttons[7]->addMouseDownEvent(gcnew MouseEventHandler(this, &AlgorithmForm::Event_MouseDownFarthestPair));
				buttons[8]->addMouseDownEvent(gcnew MouseEventHandler(this, &AlgorithmForm::Event_MouseDownConvexHull));
				buttons[9]->addMouseDownEvent(gcnew MouseEventHandler(this, &AlgorithmForm::Event_MouseDownIntersectPoints));
				buttons[10]->addMouseDownEvent(gcnew MouseEventHandler(this, &AlgorithmForm::Event_MouseDownCcwPoly));
				buttons[11]->addMouseDownEvent(gcnew MouseEventHandler(this, &AlgorithmForm::Event_MouseDownTriangulate));
				buttons[12]->addMouseDownEvent(gcnew MouseEventHandler(this, &AlgorithmForm::Event_MouseDownTurnPoints));
				buttons[13]->addMouseDownEvent(gcnew MouseEventHandler(this, &AlgorithmForm::Event_MouseDownTriangMonotone));
				buttons[14]->addMouseDownEvent(gcnew MouseEventHandler(this, &AlgorithmForm::Event_MouseDownTrichromatic));
				buttons[15]->addMouseDownEvent(gcnew MouseEventHandler(this, &AlgorithmForm::Event_MouseDownPointInPoly));
				buttons[16]->addMouseDownEvent(gcnew MouseEventHandler(this, &AlgorithmForm::Event_MouseDownPointInConvex));
			}
#pragma endregion
	};
}