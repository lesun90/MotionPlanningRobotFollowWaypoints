#include "GP/Follow.hpp"
#include "GP/GPConstants.hpp"
#include "Utils/Geometry.hpp"
#include "Utils/Algebra2D.hpp"
#include "Utils/GDraw.hpp"

namespace GP
{
    Follow::Follow(void)
    {
	m_wbase    = Constants::FOLLOW_WEIGHT_BASE;
	m_tolReach = Constants::FOLLOW_REACH_TOLERANCE;
	
	Clear();
    }
		
    void Follow::SetupFromParams(Params * const p)
    {
    }
    
    void Follow::Clear(void)
    {
	m_waypts.clear();
    }
    
    void Follow::AddWaypt(const double px, const double py, const double r)
    {
	m_waypts.push_back(px);
	m_waypts.push_back(py);
	m_waypts.push_back(r);
    }
    

    bool Follow::IsInside(const int i, const double p[]) const
    {

	double        pmin[2];
	const double *wpt = GetWaypt(i);

	if(i == 0)
	    return Algebra2D::PointDistSquared(p, wpt) <= wpt[2] * wpt[2];
	else
	{
	    return DistSquaredPointSegment2D(p, GetWaypt(i-1), wpt, pmin) 
	    <= wpt[2] * wpt[2];
	}
    }
    

    bool Follow::Reached(const int i, const double p[]) const
    {

	return Algebra2D::PointDistSquared(GetWaypt(i), p) <= m_tolReach * m_tolReach;
    }


    int Follow::Nearest(const int k, const double p[]) const
    {	
	const int n    = GetNrWaypts();
	int       imin = -1;
	double    dmin = HUGE_VAL;
	double    d;
	
	for(int i = std::max(0, k - 1); i <= k; ++i)
	    if((d = Algebra2D::PointDistSquared(p, GetWaypt(i))) < dmin)
	    {	
		dmin = d;
		imin = i;
	    }
	return imin;
    }

    double Follow::Weight(const int i) const
    {
	return pow(m_wbase, ((double) i) / (GetNrWaypts() - 1));
    }
    

    void Follow::Sample(const int i, double p[])
    {
	const double *wpt = GetWaypt(i);
	
	if(RandomUniformReal() < 0.1)
	    SampleRandomPointInsideCircle2D(wpt, m_tolReach, p);
	else
	    SampleRandomPointInsideCircle2D(wpt, wpt[2], p);
    }

    void Follow::Draw(void)
    {
	const int n = GetNrWaypts();
	for(int i = 0; i < n - 1; ++i)
	{
	    const double *p1   = GetWaypt(i);
	    const double *p2   = GetWaypt(i + 1);
	    const double  d    = p1[2];
	    const double  vx   = p2[0] - p1[0];
	    const double  vy   = p2[1] - p1[1];
	    const double  norm = sqrt(vx * vx + vy * vy);
	    const double  ux   = -vy * d / norm;
	    const double  uy   =  vx * d/ norm;
	    
	    
	    GDrawSegment2D(p1, p2);
	    GDrawSegment2D(p1[0] + ux, p1[1] + uy, p2[0] + ux, p2[1] + uy);
	    GDrawSegment2D(p1[0] - ux, p1[1] - uy, p2[0] - ux, p2[1] - uy);
	    
	}

	GDrawWireframe(true);	
	for(int i = 0; i < n; ++i)
	    GDrawCircle2D(GetWaypt(i), GetWaypt(i)[2]);
	GDrawWireframe(false);

	for(int i = 0; i < n; ++i)
	    GDrawCircle2D(GetWaypt(i), m_tolReach);

	char msg[100];
	
	GDrawColor(0, 0, 0);
	for(int i = 0; i < n; ++i)
	{
	    sprintf(msg, "%d", i);
	    GDrawString2D(msg, GetWaypt(i)[0], GetWaypt(i)[1]);
	}
	
    }
    
}

