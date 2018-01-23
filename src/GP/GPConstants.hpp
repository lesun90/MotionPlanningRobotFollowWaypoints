#ifndef GP__GPCONSTANTS_HPP_
#define GP__GPCONSTANTS_HPP_

#include "Utils/Constants.hpp"
#include <cmath>

namespace GP
{
    namespace Constants
    {
		
	const int    MEDIAL_AXIS_GRID_DIMX = 512;
	const int    MEDIAL_AXIS_GRID_DIMY = 512;
	const int    SCENE_GRID_DIMX = 128;
	const int    SCENE_GRID_DIMY = 128;
	const double SCENE_GRID_MINX = -40.0;
	const double SCENE_GRID_MINY = -40.0;
	const double SCENE_GRID_MAXX =  40.0;
	const double SCENE_GRID_MAXY =  40.0;
	const double SCENE_OBSTACLES_HEIGHT = 2.0;
	const double SCENE_BOUNDARY_HEIGHT  = 3.0;
	const double SCENE_BOUNDARY_THICKNESS = 0.4;
	
	const double SIMULATOR_TIME_STEP             = 0.1;
	const double SIMULATOR_MIN_DISTANCE_ONE_STEP = 0.5;
	const double SIMULATOR_MAX_DISTANCE_ONE_STEP = 0.8;

	const bool   ROBOT_FRONT_WHEEL_DRIVING = true;
	const double ROBOT_BODY_LENGTH         = 1.75;
	const double ROBOT_BODY_WIDTH          = 1.0;
	const double ROBOT_MIN_STEER_ANGLE     = -85 * Constants::DEG2RAD;
	const double ROBOT_MAX_STEER_ANGLE     =  85 * Constants::DEG2RAD;
	const double ROBOT_MIN_VELOCITY        = -2.0;
	const double ROBOT_MAX_VELOCITY        =  2.0;
	const double ROBOT_MIN_ACCELERATION    = -1.0;
	const double ROBOT_MAX_ACCELERATION    =  1.0;
	const double ROBOT_MIN_STEER_VELOCITY  = -2.7;
	const double ROBOT_MAX_STEER_VELOCITY  =  2.7;
	const double SNAKE_NR_LINKS            = 5;
	const double SNAKE_ATTACH_DISTANCE     = 0.01;
	
	const double PLANNER_SELECTION_PENALTY    = 0.99;
	const double PLANNER_PROBABILITY_SELECT_NEAREST_VERTEX = 0.9;
	const int    PLANNER_EXTEND_MIN_NR_STEPS  = 40;
	const int    PLANNER_EXTEND_MAX_NR_STEPS  = 80;

	
	const double FOLLOW_WEIGHT_BASE = 100000000.0;
	const double FOLLOW_REACH_TOLERANCE = 1.0;
	
	
	
	const double DRAW_ZORDER_GOAL          = 0.001;
	const double DRAW_ZORDER_ROBOT         = 0.001;
	const double DRAW_ZORDER_PLANNER       = 0.001;
	const double DRAW_ZORDER_REGION_CENTER = 0.002;
	
    }
}

#endif
    
    
    
    







