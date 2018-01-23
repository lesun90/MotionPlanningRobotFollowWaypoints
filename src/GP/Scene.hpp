#ifndef GP__SCENE_HPP_
#define GP__SCENE_HPP_

#include "Utils/Grid.hpp"
#include "Utils/Polygon2D.hpp"
#include "Utils/Params.hpp"
#include "Utils/Misc.hpp"
#include "External/PQP/PQPTriMesh.hpp"
#include <vector>

namespace GP
{
    class Scene
    {
    public:
	Scene(void);
	
	virtual ~Scene(void)
	{
	    DeleteItems<Polygon2D*>(&m_obstacles);
	    DeleteItems<Polygon2D*>(&m_bounds);
	}

	virtual void SetupFromParams(Params * const p);
	
	virtual Grid* GetGrid(void)
	{
	    return &m_grid;
	}
		
	/**
        *@brief Read the obstacles of a scene from a file
	*@param fname the name of the file
	*@par   Description:
		Obstacles are stored in a vector as 2D polygons. 
		The procedure iterates over all polygons and reads the 
		x and y values for the coordinates of all vertices of the polygon
        */
	virtual void ReadObstacles(const char fname[]);

	virtual void ReadTerrain(const char fname[]);
	
	virtual void PrintObstacles(const char fname[]);
	
	/**
        *@brief Return the total number of obstacles composing the scene
	*@returns the size of the vector used to store 2D polygonial obstacles
        */
	virtual int GetNrObstacles(void) const
	{
	    return m_obstacles.size();
	}
	
	/**
        *@brief Returns the i-th obstacle
	*@param i the index of the obstacle
        */
	virtual const Polygon2D* GetObstacle(const int i) const
	{
	    return m_obstacles[i];
	}

	/**
        *@brief Returns the i-th obstacle
	*@param i the index of the obstacle
        */
	virtual Polygon2D* GetObstacle(const int i)
	{
	    return m_obstacles[i];
	}

	/**
        *@brief Computes if there is a collision between a segment and any obstacle
	*@param p1 the first end point of the segment
	*@param p2 the second end point of the segment
	*@par	Description:
		The procedure iterates over all obstacles and for each of them checks
		whether or not it intersect with the segment [p1 p2]
        */
	virtual bool SegmentObstaclesCollision(const double p1[2], const double p2[2]);
	
	/**
        *@brief Draws all obstacles in the scene which are represented as 2D polygons
        */
	virtual void DrawObstacles(void);

	virtual void DrawTerrain(void);
	
	virtual TriMesh* GetTriMeshObstacles(void)
	{
	    return &m_tmeshObstacles;
	}

	virtual void AdjustGrid(const double eps);

	virtual void AddBoundary(const double thick);

	virtual void AddObstacle(Polygon2D * const poly);
	
	virtual TriMesh* GetTerrain(void) 
	{
	    return &m_tmeshTerrain;
	}
	
    protected:
	Grid                    m_grid;
	std::vector<Polygon2D*> m_obstacles;
	std::vector<Polygon2D*> m_bounds;
	PQPTriMesh              m_tmeshTerrain;
	PQPTriMesh              m_tmeshObstacles;
	TriMesh                 m_tmeshObstaclesForDrawing;
	double                  m_hObstacles;
	double                  m_hBoundary;
    };
}

#endif



