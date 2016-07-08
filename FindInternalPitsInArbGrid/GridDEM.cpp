//
//  GridDEM.cpp
//  PlaneProjection
//
//  Created by Jim Mower on 5/31/16.
//  Copyright © 2016 Jim Mower. All rights reserved.
//
#include "stdafx.h"
#include "GridDEM.hpp"
#include <iostream>
#include <iomanip>

GridDEM::GridDEM(int _cols, int _rows, std::vector<glm::vec3> _dem, std::vector<Patch> _patch) {
    cols = _cols;
    rows = _rows;
    dem = std::vector<glm::vec3>(_dem); // the DEM as a vector of glm::vec3 samples
    patch = std::vector<Patch>(_patch); // patches constructed from the samples in dem
    // Run fixPits() in a loop until no more pits are found. This could be an infinite loop so limit number of
    // times that it can go.
    int foundPits = 0, pass = 0;
    const int MAXPASSES = 50;
    while ((foundPits = fixPits()) != 0) {
        std::cout << "fixed " << foundPits << " pits on pass " << pass << std::endl;
        if (pass > MAXPASSES) {
            break;
        }
        pass++;
    }
}
GridDEM::GridDEM(int _cols, int _rows, std::vector<glm::vec3> _dem) {
	cols = _cols;
	rows = _rows;
	dem = std::vector<glm::vec3>(_dem); // the DEM as a vector of glm::vec3 samples
										// Run fixPits() in a loop until no more pits are found. This could be an infinite loop so limit number of
										// times that it can go.
	std::cout << std::endl << "Beginning pit fixing through point projection" << std::endl;
	int foundPits = 0, pass = 0;
	const int MAXPASSES = 50;
	while ((foundPits = fixPits()) != 0) {
		std::cout << "fixed " << foundPits << " pits on pass " << pass << std::endl;
		if (pass > MAXPASSES) {
			break;
		}
		pass++;
	}

}
bool GridDEM::isPit(int col, int row) {
    int sampleIndex = col * rows + row;
    for (int c = col - 1; c <= col + 1; c++) {
        for (int r = row - 1; r <= row + 1; r++) {
            int neighIndex = c * rows + r;
            if (dem.at(sampleIndex).y >= dem.at(neighIndex).y) {
                return false;
            }
        }
    }
    std::cout << "isPit(): found pit at col " << col << " and row " << row << std::endl;
    return true;
}

void GridDEM::projectPit(int col, int row) {
    // projects an identified pit in a grid to the plane described by its ne, nw, and sw neighbors
    // DOES IT MAKE SENSE to construct the plane from the 3 hisghest surrounding point or use some other criteria?
    int pitIndex = col * cols + row;
    int neIndex = pitIndex + rows + 1;
    int nwIndex = pitIndex - rows + 1;
    int swIndex = pitIndex - rows - 1;
    glm::vec3 top = dem.at(neIndex) - dem.at(nwIndex); // build difference vectors
    glm::vec3 left = dem.at(nwIndex) - dem.at(swIndex);
    glm::vec3 planeNormal = glm::cross(top, left); // get the normal of the plane defined by ne, nw, sw
    planeNormal = glm::normalize(planeNormal); // normalize the normal
    // project the pit to the plane described by planeNormal
    glm::vec3 projectedPoint = Patch::projectToPlane(dem.at(pitIndex), dem.at(neIndex), planeNormal);
    std::cout << std::setprecision(9) << "projected pit at " << dem.at(pitIndex).x << ", " << dem.at(pitIndex).y << ", " << dem.at(pitIndex).z  << " to " << projectedPoint.x << ", " << projectedPoint.y << ", " << projectedPoint.z << std:: endl;
    // Replace the pit in the DEM with the projected value
    dem.at(pitIndex).x = projectedPoint.x;
    dem.at(pitIndex).y = projectedPoint.y;
    dem.at(pitIndex).z = projectedPoint.z;
}

int GridDEM::fixPits() {
    // find pits in dem and project them to plane of 3 surrounding samples
	std::cout << std::endl << "in fixPits()" << std::endl;
    int foundPits = 0; // for reporting
    for (int c = 1; c < cols - 1; c++) {
        for (int r = 1; r < cols - 1; r++) {
            if (isPit(c, r)) { // if this is a pit, then project to plane of its ne, nw, sw grid neighbors
                projectPit(c,r);
                foundPits++;
            }
        }
    }
	std::cout << "fixPits(): found " << foundPits << " pits" << std::endl;
    return foundPits;
}

int GridDEM::getCols() {
    return cols;
}

int GridDEM::getRows() {
    return rows;
}
std::vector<Patch> GridDEM::getPatch() {
    return patch;
}
std::vector<glm::vec3> GridDEM::getDem() {
    return dem;
}
