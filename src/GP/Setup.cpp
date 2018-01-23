#include "GP/Setup.hpp"
#include "GP/GPConstants.hpp"
#include "GP/FollowPlanner.hpp"
#include "GP/MPSnakeSimulator.hpp"
#include "Utils/PrintMsg.hpp"
#include <cstring>

namespace GP
{
    void SetupData::SetupFromParams(Params * const p)
    {
	if(m_scene == NULL)
	    m_scene = new Scene();
	if(m_sim == NULL)
	    m_sim = new MPSnakeSimulator();
	m_planner = new FollowPlanner();

	
//setup scene	
	m_scene->SetupFromParams(p);
	m_scene->ReadObstacles(p->GetValue("SceneObstaclesFile", "data/scene.txt"));
	m_scene->AdjustGrid(2 * Constants::EPSILON);
	printf("adjusted grid: min = %f %f  max = %f %f\n", 
	       m_scene->GetGrid()->GetMin()[0],
	       m_scene->GetGrid()->GetMin()[1],
	       m_scene->GetGrid()->GetMax()[0],
	       m_scene->GetGrid()->GetMax()[1]);
	
	m_scene->AddBoundary(p->GetValueAsDouble("SceneBoundaryThickness", Constants::SCENE_BOUNDARY_THICKNESS));
//setup start
	double startx = p->GetValueAsDouble("RobotStartX", HUGE_VAL);
	double starty = p->GetValueAsDouble("RobotStartY", HUGE_VAL);


//setup simulator
	m_sim->SetupFromParams(p);
	m_sim->SetScene(m_scene);
	m_sim->CompleteSetup();

//setup planner
	m_planner->SetSimulator(m_sim);
	m_planner->SetupFromParams(p);

		
	if(startx != HUGE_VAL && starty != HUGE_VAL)
	{
	    double *s = m_sim->GetStateAllocator()->New();
	    for(int i = 0; i < m_sim->GetStateAllocator()->GetDim(); ++i)
		s[i] = 0.0;
	    s[MPSnakeSimulator::STATE_X] = startx;
	    s[MPSnakeSimulator::STATE_Y] = starty;
	    m_sim->SetState(s);
	    m_sim->GetStateAllocator()->Delete(s);
	}
    }
    
}











