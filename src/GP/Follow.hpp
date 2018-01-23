#ifndef GP__FOLLOW_HPP_
#define GP__FOLLOW_HPP_

#include "Utils/Params.hpp"
#include <vector>

namespace GP
{
    class Follow
    {
    public:
	Follow(void);
					
	virtual ~Follow(void)
	{
	}

	virtual void SetupFromParams(Params * const p);
		
	virtual void Draw(void);

	virtual void Clear(void);
	
	virtual void AddWaypt(const double px, const double py, const double r);
	
	
	virtual bool IsInside(const int i, const double p[]) const;
	
	virtual bool Reached(const int i, const double p[]) const;

	virtual int Nearest(const int i, const double p[]) const;

	virtual double Weight(const int i) const;
					
	virtual void Sample(const int i, double p[]);
	
	virtual int GetNrWaypts(void) const
	{
	    return m_waypts.size() / 3;
	}

	virtual const double* GetWaypt(const int i) const
	{
	    return &m_waypts[3 * i];
	}

	virtual double GetReachTolerance(void) const
	{
	    return m_tolReach;
	}
	
	
    protected: 
	std::vector<double> m_waypts;
	double              m_tolReach;
	double              m_wbase;
    };
}

#endif



