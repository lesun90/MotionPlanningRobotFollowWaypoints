#ifndef GP__SETUP_HPP_
#define GP__SETUP_HPP_

#include "GP/FollowPlanner.hpp"
#include "GP/Scene.hpp"
#include "GP/MPSimulator.hpp"
#include "Utils/Params.hpp"

namespace GP
{
    struct SetupData
    {
	SetupData(void) : m_scene(NULL),
			  m_sim(NULL),
			  m_planner(NULL)
	{
	}
	
	virtual ~SetupData(void)
	{
	    if(m_planner)
		delete m_planner;
	    if(m_sim)
		delete m_sim;
	    if(m_scene)
		delete m_scene;
	}

	virtual void SetupFromParams(Params * const p);
		
	Scene                  *m_scene;
	MPSimulator            *m_sim;
        FollowPlanner          *m_planner;
    };
    
    
}

#endif



    







