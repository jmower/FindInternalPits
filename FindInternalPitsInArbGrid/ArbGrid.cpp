#include "stdafx.h"
#include "ArbGrid.h"
#include "ArbGridFile.h"
#include <limits> // for min and max values
#include <iostream>
#include <vector>
#include <string>

ArbGrid::ArbGrid()
: m_controlPoints(0)
, m_minElevation(INT_MAX)
, m_maxElevation(INT_MIN)
{
}


ArbGrid::~ArbGrid()
{
}

ArbGrid::ArbGrid(ArbGridFile agf) {
	loadArbGrid(&agf);
	createDemVfromPNVertex(); // do this before pit processing happens so project pit method has original data

}
int ArbGrid::findInternalPits() {
	// For each internal point in grid, see if 8 surrounding neighbors are all higher than this cell. If so, then this is a pit. Report location and return number of pits
	int numPits = 0;
	for (int col = 1; col < m_cols - 1; col++) {
		for (int row = 1; row < m_rows - 1; row++) {
			int thisPNVi = col * m_rows + row;
			PNVertex thisPNV = m_PNVertex[thisPNVi];
			int upperPNVi = thisPNVi + 1;
			PNVertex upperPNV = m_PNVertex[upperPNVi];
			int lowerPNVi = thisPNVi - 1;
			PNVertex lowerPNV = m_PNVertex[lowerPNVi];
			int leftPNVi = thisPNVi - m_rows;
			PNVertex leftPNV = m_PNVertex[leftPNVi];
			int rightPNVi = thisPNVi + m_rows;
			PNVertex rightPNV = m_PNVertex[rightPNVi];
			int ulPNVi = leftPNVi + 1;
			PNVertex ulPNV = m_PNVertex[ulPNVi];
			int llPNVi = leftPNVi - 1;
			PNVertex llPNV = m_PNVertex[llPNVi];
			int urPNVi = rightPNVi + 1;
			PNVertex urPNV = m_PNVertex[urPNVi];
			int lrPNVi = rightPNVi - 1;
			PNVertex lrPNV = m_PNVertex[lrPNVi];
			if (thisPNV.m_pos.y < upperPNV.m_pos.y && thisPNV.m_pos.y < lowerPNV.m_pos.y && thisPNV.m_pos.y < leftPNV.m_pos.y && thisPNV.m_pos.y < rightPNV.m_pos.y && thisPNV.m_pos.y < ulPNV.m_pos.y
				&& thisPNV.m_pos.y < llPNV.m_pos.y && thisPNV.m_pos.y < urPNV.m_pos.y && thisPNV.m_pos.y < lrPNV.m_pos.y) {
				// found a pit--report
				numPits++;
				std::cout << "Found a pit at " << thisPNV.m_pos.x << ", " << thisPNV.m_pos.y << ", " << thisPNV.m_pos.z << std::endl;
			}
		}
	}
	return numPits;
}
void ArbGrid::createDemVfromPNVertex(void) {
	// rewrite m_PNVertex as a vector
	for (int i = 0; i < m_controlPoints; i++) {
		glm::vec3 pos = glm::vec3(m_PNVertex[i].m_pos); // make a copy so that there is no cross talk between pit finding and fixing methods
		m_demV.push_back(pos);
		std::cout << m_demV[i].x << ", " << m_demV[i].y << ", " << m_demV[i].z << std::endl;
	}
	std::cout << std::endl << "createDemVfromPNVertex(): added " << m_controlPoints << " records to m_demV" << std::endl;
}
bool ArbGrid::loadArbGrid(ArbGridFile* arbGridFile)
{
	// Read values from an ArbGrid file and load into m_samples[]. Later, m_sample is resampled into m_evalControlPoints whose size is dependent
	// on the current user viewport.
	m_minEasting = arbGridFile->getMinEasting();
	m_minNorthing = arbGridFile->getMinNorthing();
	m_maxEasting = arbGridFile->getMaxEasting();
	m_maxNorthing = arbGridFile->getMaxNorthing();
	m_samplingIntervalE = arbGridFile->getSamplingIntervalE();
	m_samplingIntervalN = arbGridFile->getSamplingIntervalN();
	m_rows = arbGridFile->getRows();
	m_cols = arbGridFile->getCols();

	int charsInProfile = m_rows * GRIDSAMPLECHARS;
	char *sBuffer = new char[charsInProfile + 1];
	m_controlPoints = m_rows * m_cols;
	// delete old data, if any
	if (m_PNVertex != NULL) {
		delete(m_PNVertex); // get rid of the list (elements of list are structs--they will delete when they go out of scope)
	}
	m_PNVertex = new PNVertex[m_controlPoints];
	int vIndex = 0;
	for (int c = 0; c < m_cols; c++) {
		// I'm not sure why I need to read 1 more char than charsInProfile but it definitely works right. Same deal in header.
		arbGridFile->getInFile()->get(sBuffer, charsInProfile + 1);
		std::string profile(sBuffer);
		//int index = 0;
		for (int r = 0; r < m_rows; r++) {
			float elevation = (float)parseElevation(profile, r);
			if (elevation >(float)m_maxElevation) {
				m_maxElevation = (int)elevation;
			}
			if (elevation < (float)m_minElevation) {
				m_minElevation = elevation;
			}
			float easting = m_minEasting + c * m_samplingIntervalE;
			float northing = m_minNorthing + r * m_samplingIntervalN;
			PNVertex* vertex = new PNVertex(easting, elevation, northing);
			m_PNVertex[vIndex++] = *vertex;
		}
	}
	findWorldCenter(); // get center of scene for world transformations, if necessary
	delete(sBuffer);

	// Create vertex normals using PN triangle data structure (see http://alex.vlachos.com/graphics/CurvedPNTriangles.pdf). 
	// Also see http://ogldev.atspace.co.uk/www/tutorial18/tutorial18.html.
	for (int c = 0; c < m_cols - 1; c++) {
		for (int r = 0; r < m_rows - 1; r++) {
			// get indices for upper left, lower left, upper right, lower right vertices in m_PNVertex
			// make sure that vertices bordering on world do not average in invalid vertices
			int iUL = c * m_rows + r;
			int iLL = iUL + 1;
			int iUR = iUL + m_rows;
			int iLR = iUR + 1;
			// create local copies of the vertices in the list. Use pointers so that object on list gets updated
			PNVertex* ul = &m_PNVertex[iUL];
			PNVertex* ur = &m_PNVertex[iUR];
			PNVertex* ll = &m_PNVertex[iLL];
			PNVertex* lr = &m_PNVertex[iLR];
			// create difference vectors for the top, left, bottom, and right sides of the cell defined by the vertices
			glm::vec3 top = ur->m_pos - ul->m_pos;
			glm::vec3 left = ll->m_pos - ul->m_pos;
			glm::vec3 bottom = lr->m_pos - ll->m_pos;
			glm::vec3 right = lr->m_pos - ur->m_pos;
			// create the normal vectors for the 2 triangular faces defined in this grid cell and normalize them
			// MAKE SURE THE CROSS PRODUCT ORDER IS CORRECT, OTHERWISE THE VALUES WILL HAVE THE WRONG SIGN
			// From http://en.wikipedia.org/wiki/Cross_product: cross product is anticommutative, a X b = -(a X b). My code is crossing sides
			// in clockwise order around the grid cell.
			glm::vec3 normFaceA = glm::cross(bottom, left);
			normFaceA = glm::normalize(normFaceA);
			glm::vec3 normFaceB = glm::cross(top, right);
			normFaceB = glm::normalize(normFaceB);
			// Add the normalized face normal vectors to the vertex normal components of the triangle vertices. The summed normals will be
			// normalized below to create 'vertex normals' for each vertex. These normals can be used for Phong shading or for PN triangle
			// tessellation.
			ul->m_normal += normFaceA;
			ll->m_normal += normFaceA;
			lr->m_normal += normFaceA;
			ul->m_normal += normFaceB;
			lr->m_normal += normFaceB;
			ur->m_normal += normFaceB;
			//std::cerr << std::endl << "breakpoint" << std::endl;
		}
	}
	// Normalize all the vertex normals in m_PNVertex. This 'averages' the component vectors.
	//std::cout << "loadArbGrid(): v = ";
	for (int v = 0; v < m_controlPoints; v++) {
		
		// Normalize the vertex normals. This has the effect of averaging the face normals associated with each vertex.
		m_PNVertex[v].m_normal = glm::normalize(m_PNVertex[v].m_normal);
	}

	//// NEW 3/4/2014: buildBSpline()
	//// Should not be building bspline here
	//if (buildBSpline()) {
	//	return true;
	//}

	return true;
}

int ArbGrid::parseElevation(std::string profile, int index)
{
	int start = index * GRIDSAMPLECHARS;
	int elevation = std::stoi(profile.substr(start, GRIDSAMPLECHARS));

	return elevation;
}
void ArbGrid::findWorldCenter(void) {
	float eastingCenter = (m_maxEasting - m_minEasting) / 2.0f + m_minEasting;
	float northingCenter = (m_maxNorthing - m_minNorthing) / 2.0f + m_minNorthing;
	float elevationCenter = (m_maxElevation - m_minElevation) / 2.0f + m_minElevation;
	m_worldCenter = glm::vec3(eastingCenter, elevationCenter, northingCenter);
}

std::vector<glm::vec3>& ArbGrid::getDemV() {
	return m_demV;
}
glm::vec3 ArbGrid::getWorldCenter(void) {
	return m_worldCenter;
}
float ArbGrid::getMinEasting(void)
{
	return m_minEasting;
}
float ArbGrid::getMinNorthing(void)
{
	return m_minNorthing;
}
float ArbGrid::getMaxEasting(void)
{
	return m_maxEasting;
}
float ArbGrid::getMaxNorthing(void)
{
	return m_maxNorthing;
}
float ArbGrid::getSamplingIntervalE(void)
{
	return m_samplingIntervalE;
}
float ArbGrid::getSamplingIntervalN(void)
{
	return m_samplingIntervalN;
}
int ArbGrid::getRows(void)
{
	return m_rows;
}

int ArbGrid::getCols(void)
{
	return m_cols;
}
int ArbGrid::getMinElevation()
{
	return m_minElevation;
}
int ArbGrid::getMaxElevation()
{
	return m_maxElevation;
}

int ArbGrid::getRowFromIndex(int index)
{
	return index % m_rows; // remainder after integer dividing by number of rows
}
//int ArbGrid::getECPRowFromIndex(int index)
//{
//	return index % m_ecpRows; // remainder after integer dividing by number of rows
//}
int ArbGrid::getColFromIndex(int index)
{
	return index / m_rows; // integer division
}
//int ArbGrid::getECPColFromIndex(int index)
//{
//	return index / m_ecpRows; // integer division
//}

float ArbGrid::getEastingFromIndex(int index)
{
	return m_minEasting + getColFromIndex(index) * m_samplingIntervalE;
}

float ArbGrid::getNorthingFromIndex(int index)
{
	return m_minNorthing + getRowFromIndex(index) * m_samplingIntervalN;
}

PNVertex* ArbGrid::getPNVertex(void)
{
	return m_PNVertex;
}

int ArbGrid::getNumControlPoints(void)
{
	return m_controlPoints;
}
