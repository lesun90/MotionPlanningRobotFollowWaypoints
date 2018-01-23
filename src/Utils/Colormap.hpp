#ifndef GP__COLORMAP_HPP_
#define GP__COLORMAP_HPP_

#include "Utils/Definitions.hpp"
#include <vector>

namespace GP
{
    
    class Colormap
    {
    public:
	Colormap(void)
	{   
	    Read("cmaps/jet.cmap");
	}
	
	virtual ~Colormap(void)
	{
	}	

	double GetRed(const double val) const
	{
	    return GetColor(m_red.size(), &m_red[0], val);
	}

	double GetGreen(const double val) const
	{
	    return GetColor(m_green.size(), &m_green[0], val);
	}

	double GetBlue(const double val) const
	{
	    return GetColor(m_blue.size(), &m_blue[0], val);
	}

	void GetRGB(const double val, double rgb[]) const
	{
	    rgb[0] = GetRed(val);
	    rgb[1] = GetGreen(val);
	    rgb[2] = GetBlue(val);
	}

	void Read(const char fname[]);

	static Colormap* GetSingleton(void)
	{
	    return m_singleton;
	}
	
	
    protected:	
	static Colormap *m_singleton;

	double GetColor(const int n, const double vals[], const double val) const;

	std::vector<double> m_red;
	std::vector<double> m_green;
	std::vector<double> m_blue;
    };
    
	
}

#endif
    
    
    
    







