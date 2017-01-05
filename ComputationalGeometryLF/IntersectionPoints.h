#pragma once

#include "BasicAlgorithms.h"

namespace ComputationalGeometryLF
{
	using namespace System;
	using namespace CVStructures;

	public ref class NodeSegment
	{
		public:
			static const double CompareError = 0.000001;
			GeometricSegment^ Segment;

			NodeSegment(GeometricSegment^ s)
				: Segment(s)
			{
				a = (s->EndY - s->StartY) / (s->EndX - s->StartX);
				b = s->StartY - (s->StartX * a);
			}

			static void SetSweepLine(CVPoint<double> p)
			{ sweepLineY = p.Y - CompareError; }

			static bool operator> (NodeSegment^ lhs, NodeSegment^ rhs)
			{
				if(lhs == rhs) return false;
				double x1 = (sweepLineY - lhs->b) / lhs->a;
				double x2 = (sweepLineY - rhs->b) / rhs->a;
				return (x1 > x2);
			}

			static bool operator< (NodeSegment^ lhs, NodeSegment^ rhs)
			{
				if(lhs == rhs) return false;
				double x1 = (sweepLineY - lhs->b) / lhs->a;
				double x2 = (sweepLineY - rhs->b) / rhs->a;
				return (x1 < x2);
			}

		private:
			static double sweepLineY;
			double a, b;
	};

	public ref class EventPoint : public IComparable<EventPoint^>
	{
		public:
			property CVPoint<double> Point { public: CVPoint<double> get() { return _p; } }
			property Int32 SegmentCount { public: Int32 get() { return (AsUpper->Count + AsLower->Count + AsIntersect->Count); } }
			List<NodeSegment^>^ AsUpper;
			List<NodeSegment^>^ AsLower;
			List<NodeSegment^>^ AsIntersect;

			static EventPoint^ CreateAsUpper(NodeSegment^ s)
			{
				EventPoint^ ep = gcnew EventPoint(s->Segment->Upper);
				ep->AsUpper->Add(s);
				return ep;
			}

			static EventPoint^ CreateAsLower(NodeSegment^ s)
			{
				EventPoint^ ep = gcnew EventPoint(s->Segment->Lower);
				ep->AsLower->Add(s);
				return ep;
			}

			static EventPoint^ CreateAsIntersect(CVPoint<double> p, NodeSegment^ s1, NodeSegment^ s2)
			{
				EventPoint^ ep = gcnew EventPoint(p);
				ep->AsIntersect->Add(s1);
				ep->AsIntersect->Add(s2);
				return ep;
			}

			bool mergeWith(EventPoint^ ep)
			{
				//if(_p != ep->_p) return false;
				AsUpper->AddRange(ep->AsUpper);
				AsLower->AddRange(ep->AsLower);
				for(Int32 i = 0; i < ep->AsIntersect->Count; ++i) {
					NodeSegment^ s = ep->AsIntersect[i];
					if(!AsIntersect->Contains(s) && !AsUpper->Contains(s) && !AsLower->Contains(s))
						AsIntersect->Add(s);
				}
				return true;
			}

			static bool operator> (EventPoint^ lhs, EventPoint^ rhs)
			{
				bool equalCheckPassed = (Math::Abs(lhs->_p.Y - rhs->_p.Y) > NodeSegment::CompareError);
				if(equalCheckPassed) return (lhs->_p.Y < rhs->_p.Y);
				equalCheckPassed = (Math::Abs(lhs->_p.X - rhs->_p.X) > NodeSegment::CompareError);
				return (equalCheckPassed) ? (lhs->_p.X > rhs->_p.X) : false;
			}

			static bool operator< (EventPoint^ lhs, EventPoint^ rhs)
			{
				bool equalCheckPassed = (Math::Abs(lhs->_p.Y - rhs->_p.Y) > NodeSegment::CompareError);
				if(equalCheckPassed) return (lhs->_p.Y > rhs->_p.Y);
				equalCheckPassed = (Math::Abs(lhs->_p.X - rhs->_p.X) > NodeSegment::CompareError);
				return (equalCheckPassed) ? (lhs->_p.X < rhs->_p.X) : false;
			}

			virtual Int32 CompareTo(EventPoint^ other)
			{
				if(_p.Y < other->_p.Y) return 1;
				if(_p.Y > other->_p.Y) return -1;
				if(_p.X > other->_p.X) return 1;
				if(_p.X < other->_p.X) return -1;
				return 0;
			}

		private:
			CVPoint<double> _p;

			EventPoint(CVPoint<double> p)
				: _p(p), AsUpper(gcnew List<NodeSegment^>()), AsLower(gcnew List<NodeSegment^>()), AsIntersect(gcnew List<NodeSegment^>())
			{}
	};

	public ref class Intersection
	{
		public:
			CVPoint<double> Point;
			List<GeometricSegment^>^ Segments;

			Intersection(EventPoint^ ep)
				: Point(ep->Point), Segments(gcnew List<GeometricSegment^>(ep->SegmentCount))
			{
				for(Int32 j = 0; j < ep->AsUpper->Count; ++j) Segments->Add(ep->AsUpper[j]->Segment);
				for(Int32 j = 0; j < ep->AsLower->Count; ++j) Segments->Add(ep->AsLower[j]->Segment);
				for(Int32 j = 0; j < ep->AsIntersect->Count; ++j) Segments->Add(ep->AsIntersect[j]->Segment);
			}

			static bool operator> (Intersection^ lhs, Intersection^ rhs)
			{
				return (lhs->Point.Y < rhs->Point.Y) || (lhs->Point.Y == rhs->Point.Y && lhs->Point.X > rhs->Point.X);
			}

			static bool operator< (Intersection^ lhs, Intersection^ rhs)
			{
				return (lhs->Point.Y > rhs->Point.Y) || (lhs->Point.Y == rhs->Point.Y && lhs->Point.X < rhs->Point.X);
			}
	};

	public ref class IntersectionPoints abstract
	{
		public:
			static List<Intersection^>^ solve(GeometricRectangle^ bounds, bool containedOnly);
			static bool isPolygonSimple(GeometricPolygon^ p);

		private:
			static BSTree<EventPoint^>^ _initEventQueue(List<GeometricSegment^>^ segments);

			static void _handleSegmentRemoval(EventPoint^ p, BSTree<NodeSegment^>^ SL);
			static void _handleEventPoint(EventPoint^ p, BSTree<EventPoint^>^ EQ, BSTree<NodeSegment^>^ SL);
			static void _handleLowerPoint(EventPoint^ p, BSTree<EventPoint^>^ EQ, BSTree<NodeSegment^>^ SL);

			static void _addNewEvent(EventPoint^ currentE, EventPoint^ newE, BSTree<EventPoint^>^ EQ);
			static EventPoint^ _tryIntersection(NodeSegment^ s1, NodeSegment^ s2);

			static void _recSortedToTree(BSTree<EventPoint^>^ tree, List<EventPoint^>^ sorted, Int32 l, Int32 r);
			static List<NodeSegment^>^ _getNeighbours(NodeSegment^ s, BSTree<NodeSegment^>^ SL);
	};
}