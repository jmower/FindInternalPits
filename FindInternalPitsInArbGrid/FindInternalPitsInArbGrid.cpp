// FindInternalPitsInArbGrid.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "ArbGridFile.h"
#include "ArbGrid.h"
#include "Patch.hpp"
#include "GridDEM.hpp"
using namespace std;

// CREATE ANOTHER VERSION OF buildPNTriGrid() that takes an arbgrid and turns it into a vector list
void buildPNTriGrid() {
	const int COLS = 5;
	const int ROWS = 5;
	glm::vec3 dem[COLS][ROWS];
	std::vector<glm::vec3> demV;

	dem[0][0] = glm::vec3(579900, 200, 4719900);
	dem[0][1] = glm::vec3(579900, 250, 4720000);
	dem[0][2] = glm::vec3(579900, 300, 4720100);
	dem[0][3] = glm::vec3(579900, 350, 4720200);
	dem[0][4] = glm::vec3(579900, 400, 4720300);
	dem[1][0] = glm::vec3(580000, 200, 4719900);
	dem[1][1] = glm::vec3(580000, 250, 4720000); // sw
	dem[1][2] = glm::vec3(580000, 300, 4720100); // w
	dem[1][3] = glm::vec3(580000, 350, 4720200); // nw
	dem[1][4] = glm::vec3(580000, 400, 4720300);
	dem[2][0] = glm::vec3(580100, 200, 4719900);
	dem[2][1] = glm::vec3(580100, 250, 4720000); // s
	dem[2][2] = glm::vec3(580100, 200, 4720100); // center
	dem[2][3] = glm::vec3(580100, 350, 4720200); // n
	dem[2][4] = glm::vec3(580100, 400, 4720300);
	dem[3][0] = glm::vec3(580200, 200, 4719900);
	dem[3][1] = glm::vec3(580200, 250, 4720000); // se
	dem[3][2] = glm::vec3(580200, 300, 4720100); // e
	dem[3][3] = glm::vec3(580200, 350, 4720200); // ne
	dem[3][4] = glm::vec3(580200, 400, 4720300);
	dem[4][0] = glm::vec3(580300, 200, 4719900);
	dem[4][1] = glm::vec3(580300, 250, 4720000);
	dem[4][2] = glm::vec3(580300, 300, 4720100);
	dem[4][3] = glm::vec3(580300, 350, 4720200);
	dem[4][4] = glm::vec3(580300, 400, 4720300);

	std::vector<Patch> patchV;
	for (int c = 0; c < 5; c++) {
		for (int r = 0; r < 5; r++) {
			demV.push_back(dem[c][r]);
		}
	}
	for (int c = 0; c < 4; c++) {
		for (int r = 0; r < 4; r++) {
			//            demV.push_back(dem[c][r]);
			// TRACE THROUGH AND MAKE SURE NORMALS ARE CORRECT
			patchV.push_back(Patch(dem[c + 1][r + 1], dem[c + 1][r], dem[c][r]));
			patchV.push_back(Patch(dem[c][r + 1], dem[c + 1][r + 1], dem[c][r]));
		}
	}
	GridDEM theGridDem(5, 5, demV, patchV);
	std::cout << "breakpoint" << std::endl;
}


int main(int argc,      // Number of strings in array argv
	char *argv[],   // Array of command-line argument strings
	char *envp[])  // Array of environment variable strings)
{
	// Read an arbgrid file and report the number of internal pits
	string path = "";
	cout << "Path to ArbGrid file: ";
	//cin >> path;
	if (argc < 2) {
		cout << endl << "Please type the path to an ArbGrid file after the command name" << endl;
		return 0;
	}
	path = string(argv[1]);
	//// For now, add explicit path here
	//path = "c:\\users\\jmower\\Documents\\urhere\\src\\data\\Hemisphere\\SidePit9x9.ag";
	ArbGridFile agf(path);
	if (agf.getInFile() == NULL) {
		cout << endl << "problem reading " << path << endl;
		return 0;
	}
	ArbGrid ag(agf);
	cout << "found " << ag.findInternalPits() << " pits in " << agf.getFileName() << endl;

	// Now call a version of buildPNTriGrid() that takes ag.getPNVertex(), turns it into a vector list, and runs it through the project pit code
	//ag.createDemVfromPNVertex(); // now doing this in ArbGrid constructor right after loading data into PNVertex structure but before fixing pits
	GridDEM theGridDEM(ag.getCols(), ag.getRows(), ag.getDemV());
	return 0;
}

