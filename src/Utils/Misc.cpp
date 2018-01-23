#include "Utils/Misc.hpp"
#include "Utils/PrintMsg.hpp"

namespace GP
{
    void PointAtDistanceAlongPath(const int n, const double pts[], const double d, const int dim, double p[])
    {
	double dtot = 0.0;
	double dcurr= 0.0;
	
	for(int i = 1; i < n; ++i)
	{
	    dcurr = Algebra::PointDist(dim, &pts[dim * (i-1)], &pts[dim * i]);
	    dtot += dcurr;
	    if(dtot > d)
	    {
		const double *p1 = &pts[dim * (i-1)];
		const double *p2 = &pts[dim * i];
		const double  t = (dtot - d) / dcurr;
		for(int j = 0; j < dim; ++j)
		    p[j] = (1 - t) * p1[j] + t * p2[j];
		return;
	    }
	}
	for(int j = 0; j < dim; ++j)
	    p[j] = pts[dim * (n - 1) + j];
    }

    void GenerateGaps(const int nrGaps,
		      const double duration,
		      const double minGap,
		      const double maxGap,
		      const double minNonGap,
		      std::vector<double> * const gaps)
    {
	double start = 0.0;

	gaps->clear();
	
	for(int n = nrGaps; n > 0; --n)
	{
	    double remaining = duration - start -  n * minNonGap - maxGap * (n - 1);
	    double g1Time    = RandomUniformReal(minGap, maxGap);

	    if(minNonGap >= remaining - g1Time)
	    {
		OnInputError(printf("check your arguments...smth is wrong\n"));
		return;
		
	    }
	    double g1Start   = RandomUniformReal(minNonGap, remaining - g1Time);
	    gaps->push_back(start + g1Start);
	    gaps->push_back(start + g1Start + g1Time);
	    start   += g1Start + g1Time;
	}
    
    }
    
    
}	



