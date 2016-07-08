#pragma once
#include <fstream>
#include <vector>
#include "ArbGridFile.h"
#include "FrustumBounds.h"
#include <glm/glm.hpp>
#include "PNVertex.h"
#include "Patch.hpp"

class ArbGrid
{
public:
	ArbGrid();
	ArbGrid(ArbGridFile agf);
	~ArbGrid();
	bool loadArbGrid(ArbGridFile* arbGridFile);
	int parseElevation(std::string profile, int index);
	float getMinEasting(void);
	float getMinNorthing(void);
	float getMaxEasting(void);
	float getMaxNorthing(void);
	float getSamplingIntervalE(void);
	float getSamplingIntervalN(void);
	int getRows(void);
	int getCols(void);
	int getMinElevation(void);
	int getMaxElevation(void);
	int getRowFromIndex(int index);
	int getColFromIndex(int index);
	float getEastingFromIndex(int index);
	float getNorthingFromIndex(int index);
	int getNumControlPoints(void);
	glm::vec3 getWorldCenter(void); // getter for m_worldCenter
	const int GRIDSAMPLECHARS = 6; 
	PNVertex* getPNVertex(void);
	std::vector<glm::vec3>& getDemV();
	int findInternalPits(void);
	void createDemVfromPNVertex(void); // converts PnVertex array to Patch vector
private:
	PNVertex* m_PNVertex;
	float m_minEasting, m_minNorthing, m_maxEasting, m_maxNorthing;
	float m_samplingIntervalE, m_samplingIntervalN;
	int m_rows, m_cols;
	int m_minElevation, m_maxElevation;
	int m_controlPoints; // m_rows * m_cols
	glm::vec3 m_worldCenter;
	void findWorldCenter(void); // get center of scene relative to bounding coordinates
	std::vector<glm::vec3> m_demV;
};

