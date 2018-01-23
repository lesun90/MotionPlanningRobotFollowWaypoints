#include "GP/FollowPlanner.hpp"
#include "GP/GPConstants.hpp"
#include "Utils/GDraw.hpp"

namespace GP
{
    FollowPlanner::FollowPlanner(void) 
    {
	m_follow            = NULL;
	m_sim               = NULL;
	m_extendMaxNrSteps  = Constants::PLANNER_EXTEND_MAX_NR_STEPS;
	m_extendMinNrSteps  = Constants::PLANNER_EXTEND_MIN_NR_STEPS;
	m_probSelectNearestVertex = Constants::PLANNER_PROBABILITY_SELECT_NEAREST_VERTEX;
	m_vidSolved         = Constants::ID_UNDEFINED;
	m_dsel              = Constants::PLANNER_SELECTION_PENALTY;
	
    }
    
    FollowPlanner::~FollowPlanner(void)
    {
	DeleteItems<Vertex*>(&m_vertices);
    }
    
    void FollowPlanner::SetupFromParams(Params * const p)
    {
	m_extendMaxNrSteps  = p->GetValueAsInt("PlannerExtendMaxNrSteps", m_extendMaxNrSteps);
	m_extendMinNrSteps  = p->GetValueAsInt("PlannerExtendMinNrSteps", m_extendMinNrSteps);
	m_dsel              = p->GetValueAsDouble("PlannerSelectionPenalty", m_dsel);
	m_probSelectNearestVertex = p->GetValueAsDouble("PlannerProbabilitySelectNearestVertex", m_probSelectNearestVertex);

    }

    void FollowPlanner::SetSimulator(MPSimulator * sim)
    {
	m_sim = sim;
    }
    
    void FollowPlanner::SetFollow(Follow * follow)
    {
	m_follow = follow;
    }
    
    bool FollowPlanner::GetSolution(std::vector<int> * const path) const
    {
	if(m_vidSolved < 0)
	    return false;
	
	path->clear();
	
	int pid = m_vidSolved;
	while(pid >= 0)
	{
	    path->push_back(pid);

	    if(pid >= (int) m_vertices.size())
		printf("path is wrong ... pid = %d nv = %d size=%d\n", pid, (int) m_vertices.size(), (int) path->size());
	    
	    pid = m_vertices[pid]->m_parent;


	}

	ReverseItems<int>(path);
 
	return true;
    }
    

    void FollowPlanner::Initialize(void)
    {
	if(!(m_sim->IsStateValid()))
	{
	    printf("initial state is in collision\n");
	    exit(0);
	}
	
	Vertex *v   = new Vertex();
	if(AddVertex(v) < 0)
	{
	    printf("initial state could not be added\n");
	    delete v;
	    exit(0);
	}
    }
    

    void FollowPlanner::ExtendFrom(const int vid, const double target[])
    {
	//printf("extending from %d/%d\n", vid, m_vertices.size());
	
	int          parent  = vid;
	const double d       = m_sim->DistanceStates(m_vertices[vid]->m_state, target);
	const int    nrSteps = ((int) (d/m_sim->GetMinDistOneStep())) + RandomUniformInteger(m_extendMinNrSteps, m_extendMaxNrSteps);
	
	m_sim->SetState(m_vertices[vid]->m_state);

	m_sim->StartSteerToPosition(target);
	
	for(int i = 0; i < nrSteps && m_vidSolved < 0; ++i)
	{
	    m_sim->SteerToPosition(target);
	    m_sim->SimulateOneStep();
	    if(!m_sim->IsStateValid())
		return;
	    
	    Vertex *vnew = new Vertex();
	    vnew->m_parent = parent;

	    if(AddVertex(vnew) < 0)
	    {
		delete vnew;
		return;
	    }
		    
	    if(m_sim->HasReachedSteerPosition(target, m_follow->GetReachTolerance()))
		return;
	    parent = m_vertices.size() - 1;
	}
    }
    
    FollowPlanner::Group* FollowPlanner::SelectGroup(void)
    {
	double wmax = -HUGE_VAL;
	Group *gmax = NULL;
	
	for(auto & it : m_groups)
	    if(it.second->m_weight > wmax)
	    {
		wmax = it.second->m_weight;
		gmax = it.second;
	    }
	return gmax;
	
    }

    int FollowPlanner::SelectVertex(const Group * g, const double target[])
    {
	if(RandomUniformReal() < 1 - m_probSelectNearestVertex)
	    return g->m_vids[RandomUniformInteger(0, g->m_vids.size() - 1)];
	
	double dmin = HUGE_VAL;
	int    imin = -1;
	double d;
	
	for(auto &it : g->m_vids)
	    if((d = Algebra2D::PointDist(target, m_vertices[it]->m_state)) < dmin)
	    {
		dmin = d;
		imin = it;
	    }
	return imin;
	
    }
    
    
    void FollowPlanner::SelectTarget(FollowPlanner::Group *g, double target[])
    {
	m_follow->Sample(g->m_id, target);

    }
    

    int FollowPlanner::AddVertex(FollowPlanner::Vertex * const v)
    {
	v->m_state = m_sim->GetStateAllocator()->New();
	m_sim->GetState(v->m_state);

	v->m_nextWaypt = v->m_parent >= 0 ? m_vertices[v->m_parent]->m_nextWaypt : 0;
	if(m_follow->Reached(v->m_nextWaypt, v->m_state))
	    ++(v->m_nextWaypt);
	else if(m_follow->IsInside(v->m_nextWaypt, v->m_state) == false)
	    return Constants::ID_UNDEFINED;
	
	m_vertices.push_back(v);

	Group *g;
	auto it = m_groups.find(v->m_nextWaypt);
	if(it == m_groups.end())
	{
	    g = new Group();
	    g->m_id = v->m_nextWaypt;
	    g->m_weight = m_follow->Weight(g->m_id);
	    m_groups.insert(std::make_pair(g->m_id, g));
	}
	else
	    g = it->second;
	g->m_vids.push_back(m_vertices.size() - 1);

	if(v->m_nextWaypt >= m_follow->GetNrWaypts())
	    m_vidSolved = m_vertices.size() - 1;
	return m_vertices.size() - 1;	
    }
        
    void FollowPlanner::Run(const int nrIters)
    {
	if(m_vertices.size() == 0)
	{
	    Initialize();
	}
	
	Group *g;
	double target[100];
	int    vid;
	
	for(int i = 0; i < nrIters && m_vidSolved < 0; ++i)
	{
	    g = SelectGroup();
	    SelectTarget(g, target);
	    vid = SelectVertex(g, target);
	    ExtendFrom(vid, target);


	    g->m_weight *= m_dsel;
	    if(g->m_weight < Constants::EPSILON)
		for(auto & it : m_groups)
		    it.second->m_weight /= m_dsel;
	}
    }

    void FollowPlanner::Draw(void) const
    {
	
	for(int i = m_vertices.size() - 1; i >= 1; --i)
	    GDrawSegment2D(m_vertices[i]->m_state, m_vertices[m_vertices[i]->m_parent]->m_state);
    }
    
    

}


