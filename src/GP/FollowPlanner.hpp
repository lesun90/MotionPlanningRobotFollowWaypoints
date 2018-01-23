#ifndef GP__FOLLOW_PLANNER_HPP_
#define GP__FOLLOW_PLANNER_HPP_

#include "GP/Follow.hpp"
#include "GP/MPSimulator.hpp"
#include "Utils/Params.hpp"
#include "Utils/Misc.hpp"
#include "Utils/Map.hpp"
#include <vector>

namespace GP
{
    class FollowPlanner
    {
    public:
	FollowPlanner(void);
	
	virtual ~FollowPlanner(void);

	virtual void SetupFromParams(Params * const p);
	
	virtual void SetSimulator(MPSimulator * sim);
	
	virtual void SetFollow(Follow * follow);
	
	virtual void Run(const int nrIters);

 	virtual bool GetSolution(std::vector<int> * const rpath) const;
	
	virtual const double* GetState(const int i) const
	{
	    return m_vertices[i]->m_state;
	}
	
	virtual void Draw(void) const;
	
    protected:
	struct Vertex
	{  
	    Vertex(void)
	    {
		m_parent       = Constants::ID_UNDEFINED;
		m_state        = NULL;
		m_nextWaypt    = Constants::ID_UNDEFINED;
	    }
	    
	    virtual ~Vertex(void)
	    {
		if(m_state)
		    delete[] m_state;
	    }
	    
	    int     m_parent;
	    double *m_state;
	    int     m_nextWaypt;
	    
	};

	struct Group
	{
	    Group(void) :
		m_id(Constants::ID_UNDEFINED),
		m_weight(0.0)
	    {
	    }
	    
	    int              m_id;
	    double           m_weight;
	    std::vector<int> m_vids;
	};
		    
	
	virtual void Initialize(void);
	
	virtual void ExtendFrom(const int vid, const double target[]);
	
	virtual int AddVertex(Vertex * const v);

	virtual Group* SelectGroup(void);

	virtual void SelectTarget(Group *g, double target[]);

	virtual int SelectVertex(const Group * g, const double target[]);
	
		    
	Follow               *m_follow;
	MPSimulator          *m_sim;
	std::vector<Vertex*>  m_vertices;
	int                   m_vidSolved;
	int                   m_extendMinNrSteps;
	int                   m_extendMaxNrSteps;
	double                m_probSelectNearestVertex;	
	UseMap(int, Group*)   m_groups;
	double                m_dsel;
	
	
    };    
}

#endif

