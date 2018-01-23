#include "Utils/GManager.hpp"
#include "GP/Setup.hpp"
#include "GP/GPConstants.hpp"
#include "Utils/Params.hpp"
#include "Utils/Misc.hpp"
#include "Utils/Stats.hpp"
#include "Utils/Colormap.hpp"
#include "Utils/GTexture.hpp"

namespace GP
{	
    class GRunFollowPlannerManager : public GManager
    {
    public:
	enum
	    {
		FLAG_DRAW_SCENE          = 1,
		FLAG_DRAW_SIMULATOR      = 2,
		FLAG_DRAW_PLANNER        = 4,
		FLAG_DRAW_PATH           = 8,
		FLAG_PAUSE               = 16,
		FLAG_RUN                 = 32
	    };
	
	GRunFollowPlannerManager(void) : GManager()
	{
	    m_flags  = AddFlag(0, FLAG_DRAW_SCENE |  FLAG_DRAW_PLANNER | FLAG_DRAW_SIMULATOR | FLAG_DRAW_PATH);
	    m_pathPos = 0;
	}
	
	virtual ~GRunFollowPlannerManager(void)
	{
	}

	virtual void HandleEventOnDisplay(void)
	{
	    GManager::HandleEventOnDisplay();
	    const bool    is2D = GDrawIs2D();
	    const double *pmin = m_data.m_scene->GetGrid()->GetMin();
	    const double *pmax = m_data.m_scene->GetGrid()->GetMax();
	    const double  off  = 0.002;
	    double        rgb[3];
	    GMaterial     gmat;
	    
	    
	    SetValue(INDEX_MINX, pmin[0] - off * (pmax[0] - pmin[0]));
	    SetValue(INDEX_MINY, pmin[1] - off * (pmax[1] - pmin[1]));
	    SetValue(INDEX_MAXX, pmax[0] + off * (pmax[0] - pmin[0]));
	    SetValue(INDEX_MAXY, pmax[1] + off * (pmax[1] - pmin[1]));

	    if(HasFlag(m_flags, FLAG_DRAW_SCENE))
	    {
		GDrawColor(0.2, 0.2, 0.2);
		GDrawWireframe(false);
		gmat.SetObsidian();
		gmat.SetDiffuse(0.8, 0.8, 0.8);
		GDrawMaterial(&gmat);
		m_gtex.AutomaticCoords();
		m_gtex.Use();
		m_data.m_scene->DrawObstacles();
	    }



	    GDraw2D();
	    GDrawColor(1, 0, 1);
	    GDrawLineWidth(2.0);
	    m_follow.Draw();
	    if(!is2D)
		GDraw3D();
	    

	    if(m_path.size() == 0 && HasFlag(m_flags, FLAG_DRAW_PLANNER))
	    {
		GDraw2D();
		GDrawLineWidth(4.0);
		GDrawColor(0, 0, 0);
		GDrawPushTransformation();
		GDrawMultTrans(0, 0, Constants::DRAW_ZORDER_PLANNER);
		m_data.m_planner->Draw();
		GDrawPopTransformation();
		if(!is2D)
		    GDraw3D();
	    }

	    if(m_path.size() > 0)
	    {
		GDraw2D();
		GDrawLineWidth(4.0);
		GDrawColor(0, 0, 1);
		
		for(int i = 1; i < (int) m_path.size(); ++i)
		    GDrawSegment2D(m_data.m_planner->GetState(m_path[i-1]),
				   m_data.m_planner->GetState(m_path[i]));
		if(!is2D)
		    GDraw3D();
		
	    }


	    if(HasFlag(m_flags, FLAG_DRAW_SIMULATOR))
	    {
		if(HasFlag(GManager::m_flags, FLAG_3D))
		    GDraw3D();
		else
		{
		    GDrawColor(1, 0, 0);
		    GDraw2D();
		}
		
		GDrawPushTransformation();
		GDrawMultTrans(0, 0, Constants::DRAW_ZORDER_ROBOT);
		
		
		gmat.SetObsidian();
		gmat.SetDiffuse(0.8, 0.8, 0.8);
		GDrawMaterial(&gmat);
        	m_gtex.AutomaticCoords();
		m_gtex.Use();
		
		if(m_path.size() > 0)
		    m_data.m_sim->SetState(m_data.m_planner->GetState(m_path[m_pathPos]));
		m_data.m_sim->Draw();
		
		
		GDrawPopTransformation();
		
	    }
	    
	    
	}	
	
	virtual bool HandleEventOnNormalKeyPress(const int key)
	{
	    if(key == 'p')
		m_flags = FlipFlag(m_flags, FLAG_PAUSE);
	    else if(key == 'r')
	    {
		m_flags = AddFlag(m_flags, FLAG_RUN);
		
		m_data.m_planner->Run(100);
		if(m_data.m_planner->GetSolution(&m_path))
		{
		    m_pathPos = 0;
		    if(m_path.size() > 0)
			m_data.m_sim->SetState(m_data.m_planner->GetState(m_path[0]));
		}
	    }
	    else if(key == '+')
	    {
		if(m_pathPos < m_path.size() - 1)
		{
		    ++m_pathPos;
		    m_data.m_sim->SetState(m_data.m_planner->GetState(m_path[m_pathPos]));
		}
	    }
	    
	    else if(key == '-')
	    {
		if(m_pathPos > 0)
		{
		    --m_pathPos;
		    m_data.m_sim->SetState(m_data.m_planner->GetState(m_path[m_pathPos]));
		}
	    }
	    
	    
	    return GManager::HandleEventOnNormalKeyPress(key);
	}
	
	virtual void HandleEventOnTimer(void)
	{
	    if(HasFlag(m_flags, FLAG_PAUSE))
		return;

	    if(m_data.m_planner && HasFlag(m_flags, FLAG_RUN))
	    {
		if(m_path.size() == 0 && m_data.m_planner->GetSolution(&m_path))
		{
		    m_pathPos = 0;
		    if(m_path.size() > 0)
			m_data.m_sim->SetState(m_data.m_planner->GetState(m_path[0]));
		}
		else if(m_path.size() == 0)
		    m_data.m_planner->Run(50);
	    }

	    if(m_path.size() > 0 && m_pathPos < (int) m_path.size() - 1)
	    {
		++m_pathPos;
		m_data.m_sim->SetState(m_data.m_planner->GetState(m_path[m_pathPos]));
	    }
	}


	virtual bool HandleEventOnMouseLeftBtnDown(const int x, const int y)
	{
	    double pos[3];
	    MousePosFromScreenToWorld(x, y, &pos[0], &pos[1], &pos[2]);
	    m_follow.AddWaypt(pos[0], pos[1], 3.0);
	    
	    
	    return false;
	}

	SetupData        m_data;
	Flags            m_flags;
	GTexture         m_gtex;
	Follow           m_follow;
	
    protected:
	std::vector<int> m_path;
	int              m_pathPos;
	
    };
};


extern "C" int GRunFollowPlanner(int argc, char **argv)
{
    GP::GRunFollowPlannerManager gManager;
    
    printf("usage: ./bin/Runner GRunFollowPlanner fnameParams\n");
    
    if(argc < 1)
	return 0;
    //   GP::SetDefaultValuesParams();

    FILE *in = fopen(argv[1], "r");
    GP::Params::GetSingleton()->Read(in);
    fclose(in);
    GP::Params::GetSingleton()->Print(stdout);

    gManager.SetupFromParams(GP::Params::GetSingleton());
    GP::GDrawSetupFromParams(GP::Params::GetSingleton());    
    gManager.m_data.SetupFromParams(GP::Params::GetSingleton());
    gManager.m_data.m_planner->SetFollow(&gManager.m_follow);
    

    gManager.m_gtex.SetFileName(GP::Params::GetSingleton()->GetValue("GraphicsTextureObstacles"));
    
    gManager.Help();
    gManager.MainLoop("GRunFollowPlanner", 
		      GP::Params::GetSingleton()->GetValueAsInt("GraphicsResolutionX", GP::Constants::GRAPHICS_RESOLUTION_X),
		      GP::Params::GetSingleton()->GetValueAsInt("GraphicsResolutionY", GP::Constants::GRAPHICS_RESOLUTION_Y));
    

    return 0;
}
