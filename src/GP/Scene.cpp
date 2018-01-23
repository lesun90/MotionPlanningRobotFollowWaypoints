#include "GP/Scene.hpp"
#include "GP/GPConstants.hpp"
#include "Utils/TriMeshReader.hpp"
#include "Utils/Misc.hpp"
#include "Utils/Geometry.hpp"
#include "Utils/PrintMsg.hpp"
#include "Utils/GDraw.hpp"


namespace GP
{
    Scene::Scene(void)
    {
	m_grid.Setup2D(Constants::SCENE_GRID_DIMX,
		       Constants::SCENE_GRID_DIMY,
		       Constants::SCENE_GRID_MINX,
		       Constants::SCENE_GRID_MINY,
		       Constants::SCENE_GRID_MAXX,
		       Constants::SCENE_GRID_MAXY);

	m_hObstacles = Constants::SCENE_OBSTACLES_HEIGHT;
	m_hBoundary  = Constants::SCENE_BOUNDARY_HEIGHT;	
    }
    
    void Scene::SetupFromParams(Params * const p)
    {
	const int    dimsx = p->GetValueAsInt("SceneGridDimX", m_grid.GetDims()[0]);
	const int    dimsy = p->GetValueAsInt("SceneGridDimY", m_grid.GetDims()[1]);
	const double minx  = p->GetValueAsDouble("SceneGridMinX", m_grid.GetMin()[0]);
	const double miny  = p->GetValueAsDouble("SceneGridMinY", m_grid.GetMin()[1]);
	const double maxx  = p->GetValueAsDouble("SceneGridMaxX", m_grid.GetMax()[0]);
	const double maxy  = p->GetValueAsDouble("SceneGridMaxY", m_grid.GetMax()[1]);

	m_grid.Setup2D(dimsx, dimsy, minx, miny, maxx, maxy);

	m_hObstacles = p->GetValueAsDouble("SceneObstaclesHeight", m_hObstacles);
	m_hBoundary  = p->GetValueAsDouble("SceneBoundaryHeight", m_hBoundary);
    }

    void Scene::AddObstacle(Polygon2D * const poly)
    {
	m_obstacles.push_back(poly);
	m_tmeshObstacles.AddPolygon(poly);
	m_tmeshObstaclesForDrawing.AddExtrudedPolygon(poly, 0, m_hObstacles);
    }
    
    void Scene::ReadObstacles(const char fname[])
    {	
	FILE *in = fopen(fname, "r");
	if(!in)
	{
	    OnInputError(printf("could not open <%s> for reading\n", fname));
	}
	
	ReadPolygons2D(in, &m_obstacles);
	fclose(in);

	m_tmeshObstacles.Clear();
	for(int i = m_obstacles.size() - 1; i >= 0; --i)
	{
	    m_tmeshObstacles.AddPolygon(m_obstacles[i]);
	    m_tmeshObstaclesForDrawing.AddExtrudedPolygon(m_obstacles[i], 0, m_hObstacles);
	}
	
	printf("added obstacles: %d\n", m_obstacles.size());
	
    }


    void Scene::ReadTerrain(const char fname[])
    {	
	TriMeshReader(fname, &m_tmeshTerrain);
    }

    void Scene::PrintObstacles(const char fname[])
    {
	FILE *out = fopen(fname, "w");
	fprintf(out, "%d\n", m_obstacles.size());
	for(int i = 0; i < (int) m_obstacles.size(); ++i)
	    m_obstacles[i]->Print(out);
	fclose(out);
    }
    

    bool Scene::SegmentObstaclesCollision(const double p1[2], const double p2[2])
    {	
	const int n = m_obstacles.size();
	for(int i = 0; i < n; ++i)
	    if(m_obstacles[i]->SegmentCollision(p1, p2))
		return true;
	return false;
    }

    void Scene::DrawObstacles(void)
    {
	m_tmeshObstaclesForDrawing.Draw();
	
	//for(int i = m_obstacles.size() - 1; i >= 0; --i)
	//  GDrawPolygon2D(m_obstacles[i]);
    }

    void Scene::DrawTerrain(void)
    {
	m_tmeshTerrain.Draw();
    }
    

    void Scene::AdjustGrid(const double eps)
    {
	double pmin[2];
	double pmax[2];
	
	pmin[0] = pmin[1] = HUGE_VAL;
	pmax[0] = pmax[1] = -HUGE_VAL;
	
	for(int i = m_obstacles.size() - 1; i >= 0; --i)
	{
	    const double *bbox = m_obstacles[i]->GetBoundingBox();
	    if(bbox[0] < pmin[0])
		pmin[0] = bbox[0];
	    if(bbox[1] < pmin[1])
		pmin[1] = bbox[1];
	    if(bbox[2] > pmax[0])
		pmax[0] = bbox[2];
	    if(bbox[3] > pmax[1])
		pmax[1] = bbox[3];
	    
	}
	pmin[0] -= eps;
	pmin[1] -= eps;
	pmax[0] += eps;
	pmax[1] += eps;

	const double *gmin = m_grid.GetMin();
	const double *gmax = m_grid.GetMax();
	
	if(gmin[0] < pmin[0])
	    pmin[0] = gmin[0];
	if(gmin[1] < pmin[1])
	    pmin[1] = gmin[1];
	
	if(gmax[0] > pmax[0])
	    pmax[0] = gmax[0];
	if(gmax[1] > pmax[1])
	    pmax[1] = gmax[1];
	
	m_grid.Setup(2, m_grid.GetDims(), pmin, pmax);
       
    }

    void Scene::AddBoundary(const double thick)
    {
	const double *pmin = m_grid.GetMin();
	const double *pmax = m_grid.GetMax();
	
	double     poly[8];	
	Polygon2D *bound;

	AABoxAsPolygon2D(pmin[0] - thick, pmin[1] - thick,
			 pmax[0] + thick, pmin[1], poly);
	bound = new Polygon2D();
	bound->AddVertex(poly[0], poly[1]);
	bound->AddVertex(poly[2], poly[3]);
	bound->AddVertex(poly[4], poly[5]);
	bound->AddVertex(poly[6], poly[7]);
	m_bounds.push_back(bound);

	AABoxAsPolygon2D(pmin[0] - thick, pmax[1],
			 pmax[0] + thick, pmax[1] + thick, poly);
	bound = new Polygon2D();
	bound->AddVertex(poly[0], poly[1]);
	bound->AddVertex(poly[2], poly[3]);
	bound->AddVertex(poly[4], poly[5]);
	bound->AddVertex(poly[6], poly[7]);
	m_bounds.push_back(bound);

	AABoxAsPolygon2D(pmin[0] - thick, pmin[1],
			 pmin[0], pmax[1], poly);
	bound = new Polygon2D();
	bound->AddVertex(poly[0], poly[1]);
	bound->AddVertex(poly[2], poly[3]);
	bound->AddVertex(poly[4], poly[5]);
	bound->AddVertex(poly[6], poly[7]);
	m_bounds.push_back(bound);


	AABoxAsPolygon2D(pmax[0], pmin[1],
			 pmax[0] + thick, pmax[1], poly);
	bound = new Polygon2D();
	bound->AddVertex(poly[0], poly[1]);
	bound->AddVertex(poly[2], poly[3]);
	bound->AddVertex(poly[4], poly[5]);
	bound->AddVertex(poly[6], poly[7]);
	m_bounds.push_back(bound);

	for(int i = m_bounds.size() - 1; i >= 0; --i)
	{
	    m_tmeshObstacles.AddPolygon(m_bounds[i]);
	    m_tmeshObstaclesForDrawing.AddExtrudedPolygon(m_bounds[i], 0, m_hBoundary);
	}
    }
    
    
}

