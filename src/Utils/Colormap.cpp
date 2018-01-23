#include "Utils/Colormap.hpp"
#include "Utils/PrintMsg.hpp"
#include <cstdio>

namespace GP
{
    Colormap* Colormap::m_singleton = new Colormap();
    
    double Colormap::GetColor(const int n, const double vals[], const double val) const
    {

/*
t = (val - start) / range = (val - pos * range) / range = val / range - pos = val * (n-1) - pos
 */
	const int pos = val * (n - 1);

	if(pos < 0)
	    return vals[0];
	else if(pos >= n - 1)
	    return vals[n - 1];
	else
	{
	    const double t = val * (n - 1.0) - pos;
	    return (1 - t) * vals[pos] + t * vals[pos + 1];
	}
    }

    void Colormap::Read(const char fname[])
    {
	FILE *in = fopen(fname, "r");
	
	if(!in)
	    OnInputError(printf("could not open file <%s> for reading colormap\n", fname));
	
	double r, g, b;
	
	m_red.clear();
	m_green.clear();
	m_blue.clear();

	while(fscanf(in, "%lf %lf %lf", &r, &g, &b) == 3)
	{
	    m_red.push_back(r);
	    m_green.push_back(g);
	    m_blue.push_back(b);
	}

	fclose(in);
	
    }
}


    
    







