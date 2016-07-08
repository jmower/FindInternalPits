//
//  GridDEM.hpp
//  PlaneProjection
//
//  Created by Jim Mower on 5/31/16.
//  Copyright © 2016 Jim Mower. All rights reserved.
//

#ifndef GridDEM_hpp
#define GridDEM_hpp

#include <stdio.h>

#endif /* GridDEM_hpp */

#include <glm/glm.hpp>
#include <vector>
#include "Patch.hpp"

class GridDEM {
// vector container for a grid DEM
private:
    int cols;
    int rows;
    std::vector<Patch> patch; // the grid DEM stored as a vector
    std::vector<glm::vec3> dem;
public:
    GridDEM(int _cols, int _rows, std::vector<glm::vec3> _dem, std::vector<Patch> _patch);
	GridDEM(int _cols, int _rows, std::vector<glm::vec3> _dem);
	int getCols(void);
    int getRows(void);
    std::vector<glm::vec3> getDem(void);
    std::vector<Patch> getPatch(void);
    int fixPits();
    bool isPit(int col, int row);
    void projectPit(int col, int row);
};