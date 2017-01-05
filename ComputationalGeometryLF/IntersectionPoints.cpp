#include "IntersectionPoints.h"

using namespace ComputationalGeometryLF;

List<Intersection^>^ IntersectionPoints::solve(GeometricRectangle^ bounds, bool containedOnly)
{
	List<GeometricSegment^>^ segments = BasicAlgorithms::getSegmentsWithinBounds(bounds, containedOnly);

	BSTree<EventPoint^>^ EQ = _initEventQueue(segments);
	BSTree<NodeSegment^>^ SL = gcnew BSTree<NodeSegment^>();
	List<Intersection^>^ IL = gcnew List<Intersection^>();

	while(!EQ->IsEmpty) {
		EventPoint^ p = EQ->Min;
		EQ->remove(p);

		if(p->SegmentCount > 1) IL->Add(gcnew Intersection(p));
		_handleSegmentRemoval(p, SL);
		_handleEventPoint(p, EQ, SL);
	}
	return IL;
}

bool IntersectionPoints::isPolygonSimple(GeometricPolygon^ p)
{
	List<GeometricSegment^>^ segments = BasicAlgorithms::getPolygonSegments(p);

	BSTree<EventPoint^>^ EQ = _initEventQueue(segments);
	BSTree<NodeSegment^>^ SL = gcnew BSTree<NodeSegment^>();

	while(!EQ->IsEmpty) {
		EventPoint^ p = EQ->Min;
		EQ->remove(p);

		if(p->AsIntersect->Count > 0) return false;
		_handleSegmentRemoval(p, SL);
		_handleEventPoint(p, EQ, SL);
	}
	return true;
}

BSTree<EventPoint^>^ IntersectionPoints::_initEventQueue(List<GeometricSegment^>^ segments)
{
	Int32 i, j;
	List<NodeSegment^>^ NodeSegments = gcnew List<NodeSegment^>(segments->Count);
	for(i = 0; i < segments->Count; ++i)
		NodeSegments->Add(gcnew NodeSegment(segments[i]));

	segments->Clear();
	segments->TrimExcess();

	List<EventPoint^>^ ePoints = gcnew List<EventPoint^>(NodeSegments->Count << 1);
	for(i = 0; i < NodeSegments->Count; ++i) {
		ePoints->Add(EventPoint::CreateAsUpper(NodeSegments[i]));
		ePoints->Add(EventPoint::CreateAsLower(NodeSegments[i]));
	}

	NodeSegments->Clear();
	NodeSegments->TrimExcess();
	ePoints->Sort();

	List<EventPoint^>^ trimmedEvents = gcnew List<EventPoint^>(ePoints->Count);
	for(i = 0, j = 0; i < ePoints->Count; ) {
		trimmedEvents->Add(ePoints[i++]);
		while(i < ePoints->Count && trimmedEvents[j]->Point == ePoints[i]->Point)
			trimmedEvents[j]->mergeWith(ePoints[i++]);
		++j;
	}

	ePoints->Clear();
	ePoints->TrimExcess();

	BSTree<EventPoint^>^ tree = gcnew BSTree<EventPoint^>();
	_recSortedToTree(tree, trimmedEvents, 0, trimmedEvents->Count - 1);
	trimmedEvents->Clear();
	trimmedEvents->TrimExcess();
	return tree;
}

void IntersectionPoints::_handleSegmentRemoval(EventPoint^ p, BSTree<NodeSegment^>^ SL)
{
	Int32 i;
	for(i = 0; i < p->AsLower->Count; ++i)
		SL->remove(p->AsLower[i]);

	for(i = 0; i < p->AsIntersect->Count; ++i)
		SL->remove(p->AsIntersect[i]);
}

void IntersectionPoints::_handleEventPoint(EventPoint^ p, BSTree<EventPoint^>^ EQ, BSTree<NodeSegment^>^ SL)
{
	NodeSegment::SetSweepLine(p->Point);

	if(p->AsUpper->Count + p->AsIntersect->Count > 0) {
		Int32 i;
		NodeSegment^ s1 = (p->AsUpper->Count > 0) ? p->AsUpper[0] : p->AsIntersect[0];
		NodeSegment^ s2 = s1;
		for(i = 0; i < p->AsUpper->Count; ++i) {
			NodeSegment^ s = p->AsUpper[i];
			if(s1 > s) s1 = s;
			if(s2 < s) s2 = s;
		}
		for(i = 0; i < p->AsIntersect->Count; ++i) {
			NodeSegment^ s = p->AsIntersect[i];
			if(s1 > s) s1 = s;
			if(s2 < s) s2 = s;
		}

		List<NodeSegment^>^ n1 = SL->getRange(s2, 1, 0);
		if(n1->Count > 0) {
			EventPoint^ ep = _tryIntersection(n1[0], s1);
			if(ep != nullptr) _addNewEvent(p, ep, EQ);
		}

		List<NodeSegment^>^ n2 = SL->getRange(s1, 0, 1);
		if(n2->Count > 0) {
			EventPoint^ ep = _tryIntersection(s2, n2[0]);
			if(ep != nullptr) _addNewEvent(p, ep, EQ);
		}

		for(i = 0; i < p->AsUpper->Count; ++i)
			SL->insert(p->AsUpper[i]);

		for(i = 0; i < p->AsIntersect->Count; ++i)
			SL->insert(p->AsIntersect[i]);
	}
	else _handleLowerPoint(p, EQ, SL);
}

void IntersectionPoints::_handleLowerPoint(EventPoint^ p, BSTree<EventPoint^>^ EQ, BSTree<NodeSegment^>^ SL)
{
	Int32 i;
	NodeSegment^ s1 = p->AsLower[0];
	NodeSegment^ s2 = s1;
	for(i = 1; i < p->AsLower->Count; ++i) {
		NodeSegment^ s = p->AsLower[i];
		if(s1 > s) s1 = s;
		if(s2 < s) s2 = s;
	}

	List<NodeSegment^>^ n1 = SL->getRange(s2, 1, 0);
	List<NodeSegment^>^ n2 = SL->getRange(s1, 0, 1);

	if(n1->Count > 0 && n2->Count > 0) {
		EventPoint^ ep = _tryIntersection(n1[0], n2[0]);
		if(ep != nullptr) _addNewEvent(p, ep, EQ);
	}
}

void IntersectionPoints::_addNewEvent(EventPoint^ currentE, EventPoint^ newE, BSTree<EventPoint^>^ EQ)
{
	if(EQ->exists(newE)) EQ->get(newE)->mergeWith(newE);
	else if(newE > currentE) EQ->insert(newE);
}

EventPoint^ IntersectionPoints::_tryIntersection(NodeSegment^ s1, NodeSegment^ s2)
{
	GeometricPoint^ ip = BasicAlgorithms::getIntersectionPoint(s1->Segment, s2->Segment);
	return (ip != nullptr) ? EventPoint::CreateAsIntersect(ip->Location, s1, s2) : nullptr;
}

void IntersectionPoints::_recSortedToTree(BSTree<EventPoint^>^ tree, List<EventPoint^>^ sorted, Int32 l, Int32 r)
{
	if(l <= r) {
		Int32 m = l + ((r - l) >> 1);
		tree->insert(sorted[m]);
		_recSortedToTree(tree, sorted, l, m - 1);
		_recSortedToTree(tree, sorted, m + 1, r);
	}
}

List<NodeSegment^>^ IntersectionPoints::_getNeighbours(NodeSegment^ s, BSTree<NodeSegment^>^ SL)
{
	return SL->getRange(s, 1, 1);
}