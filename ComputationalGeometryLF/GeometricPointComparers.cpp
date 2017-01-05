#include "GeometricPoint.h"

using namespace ComputationalGeometryLF;

GeometricPoint::ComparerX::ComparerX() {}
int GeometricPoint::ComparerX::Compare(GeometricPoint^ p1, GeometricPoint^ p2)
{ return (p1->X > p2->X) ? 1 : (p1->X < p2->X) ? -1 : 0; }

GeometricPoint::ComparerY::ComparerY() {}
int GeometricPoint::ComparerY::Compare(GeometricPoint^ p1, GeometricPoint^ p2)
{ return (p1->Y > p2->Y) ? 1 : (p1->Y < p2->Y) ? -1 : 0; }

GeometricPoint::ComparerAngle::ComparerAngle(GeometricPoint^ origin) : Origin(origin) {}
int GeometricPoint::ComparerAngle::Compare(GeometricPoint^ p1, GeometricPoint^ p2)
{
	double dp1y = p1->Y - Origin->Y;
	double dp2y = p2->Y - Origin->Y;
	double dp1x = p1->X - Origin->X;
	double dp2x = p2->X - Origin->X;

	double halfCheckY = dp1y * dp2y;						// sprawdzenie czy p1 i p2 sa w tej samej polowce wzgledem osi y
	if(halfCheckY > 0) {
		double orient = _oTest(dp1x, dp1y, dp2x, dp2y);		// jezeli tak, wykonujemy test orientacji
		if(orient > 0) return 1;							// p1 lezy na lewo od p2 - p1 jest wiekszy
		if(orient < 0) return -1;							// p1 lezy na prawo od p2 - p1 jest mniejszy
		return 0;
	}
	if(halfCheckY < 0) return (dp1y > 0) ? -1 : 1;			// jezeli p1 i p2 leza w innych polowkach - mniejszy jest punkt lezacy w polowce +

	if(dp1y == 0) {
		if(dp2y == 0) {										// jezeli dla p1 i p2 y = 0
			double halfCheckX = dp1x * dp2x;				// sprawdzenie czy p1 i p2 sa w tej samej polowce wzgledem osi x
			if(halfCheckX > 0) return 0;					// jezeli tak, sa rowne
			if(halfCheckX < 0) return (dp1x > 0) ? -1 : 1;	// jezeli nie, mniejszy jest punkt lezacy w polowce +

			if(dp1x == 0) {
				if(dp2x >= 0) return 0;						// jezeli p1.x = 0 i p2.x >= 0 to sa rowne
				return -1;									// jezeli p2.x < 0 to p1 jest mniejszy
			}
			if(dp1x < 0) return 1;							// jezeli p2.x = 0 i p1.x < 0 to p1 jest wiekszy
			return 0;										// jezeli p1.x > 0 to sa rowne
		}
		if(dp2y < 0) return -1;								// jezeli y1 = 0 i y2 < 0 - p1 jest mniejszy
		return (dp1x >= 0) ? -1 : 1;						// jezeli y1 = 0 i y2 > 0 - p1 jest mniejszy gdy x1 jest nieujemne
	}
	if(dp1y < 0) return 1;									// jezeli y1 < 0 i y2 = 0 - p1 jest wiekszy
	return (dp2x >= 0) ? 1 : -1;							// jezeli y1 > 0 i y2 = 0 - p1 jest wiekszy gdy x2 jest ujemne
}

double GeometricPoint::ComparerAngle::_oTest(double dp1x, double dp1y, double dp2x, double dp2y)
{ return (-dp1x * (dp2y - dp1y)) - ((dp2x - dp1x) * -dp1y); }

GeometricPoint::ComparerDistance::ComparerDistance(GeometricPoint^ origin) : Origin(origin) {}
int GeometricPoint::ComparerDistance::Compare(GeometricPoint^ p1, GeometricPoint^ p2)
{
	double dp1y = p1->Y - Origin->Y;
	double dp2y = p2->Y - Origin->Y;
	double dp1x = p1->X - Origin->X;
	double dp2x = p2->X - Origin->X;

	double d1sq = (dp1x * dp1x) + (dp1y * dp1y);
	double d2sq = (dp2x * dp2x) + (dp2y * dp2y);		// oblicza kwadraty dlugosci wektorow (o,p1) i (o,p2)
	if(d1sq > d2sq) return 1;							// (o,p1) jest dluzszy od (o,p2) - p1 jest wiekszy
	if(d1sq < d2sq) return -1;							// (o,p1) jest mniejszy od (o,p2) - p1 jest mniejszy
	return 0;											// (o,p1) jest rowny (o,p2) - p1 i p2 sa takie same
}

GeometricPoint::ComparerAngleCoords::ComparerAngleCoords(GeometricPoint^ origin) : Origin(origin) {}
int GeometricPoint::ComparerAngleCoords::Compare(GeometricPoint^ p1, GeometricPoint^ p2)
{
	double dp1y = p1->Y - Origin->Y;						// transformacja p1 i p2 do ukladu z poczatkiem w Origin (o)
	double dp2y = p2->Y - Origin->Y;
	double dp1x = p1->X - Origin->X;
	double dp2x = p2->X - Origin->X;

	double halfCheckY = dp1y * dp2y;						// sprawdzenie czy p1 i p2 sa w tej samej polowce wzgledem osi y
	if(halfCheckY > 0) {
		double orient = _oTest(dp1x, dp1y, dp2x, dp2y);		// jezeli tak, wykonujemy test orientacji
		if(orient > 0) return 1;							// p1 lezy na lewo od p2 - p1 jest wiekszy
		if(orient < 0) return -1;							// p1 lezy na prawo od p2 - p1 jest mniejszy

		double d1sq = (dp1x * dp1x) + (dp1y * dp1y);		// jezeli p1 i p2 leza na tym samym kacie
		double d2sq = (dp2x * dp2x) + (dp2y * dp2y);		// to oblicza kwadraty dlugosci wektorow (o,p1) i (o,p2)
		if(d1sq > d2sq) return 1;							// (o,p1) jest dluzszy od (o,p2) - p1 jest wiekszy
		if(d1sq < d2sq) return -1;							// (o,p1) jest mniejszy od (o,p2) - p1 jest mniejszy
		return 0;											// (o,p1) jest rowny (o,p2) - p1 i p2 sa takie same
	}
	if(halfCheckY < 0) return (dp1y > 0) ? -1 : 1;			// jezeli p1 i p2 leza w innych polowkach - mniejszy jest punkt lezacy w polowce +
						
	if(dp1y == 0) {
		if(dp2y == 0) {										// jezeli dla p1 i p2 y = 0
			double halfCheckX = dp1x * dp2x;				// sprawdzenie czy p1 i p2 sa w tej samej polowce wzgledem osi x
			if(halfCheckX > 0) {
				double abs1 = Math::Abs(dp1x);				// jezeli tak, mniejszy jest ten o mniejszym dystansie od o.x
				double abs2 = Math::Abs(dp2x);
				if(abs1 > abs2) return 1;
				if(abs1 < abs2) return -1;
				return 0;
			}
			if(halfCheckX < 0) return (dp1x > 0) ? -1 : 1;	// jezeli nie, mniejszy jest punkt lezacy w polowce +

			if(dp1x == 0) return (dp2x == 0) ? 0 : -1;		// jezeli p1 i p2 x = 0 to sa rowne, jezeli p2.x != 0 to p1 jest mniejszy
			return 1;										// jezeli p1.x != 0 i p2.x = 0 to p1 jest wiekszy
		}
		if(dp2y < 0) return -1;								// jezeli y1 = 0 i y2 < 0 - p1 jest mniejszy
		return (dp1x >= 0) ? -1 : 1;						// jezeli y1 = 0 i y2 > 0 - p1 jest mniejszy gdy x1 jest nieujemne
	}
	if(dp1y < 0) return 1;									// jezeli y1 < 0 i y2 = 0 - p1 jest wiekszy
	return (dp2x >= 0) ? 1 : -1;							// jezeli y1 > 0 i y2 = 0 - p1 jest wiekszy gdy x2 jest ujemne
}

double GeometricPoint::ComparerAngleCoords::_oTest(double dp1x, double dp1y, double dp2x, double dp2y)
{ return (-dp1x * (dp2y - dp1y)) - ((dp2x - dp1x) * -dp1y); }