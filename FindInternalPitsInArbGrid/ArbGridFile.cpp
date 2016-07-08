#include "stdafx.h"
#include "ArbGridFile.h"
#include <iostream>
#include <stdlib.h>
#include <crtdbg.h>
#include <fstream>
#include <string>

ArbGridFile::ArbGridFile()
{
	m_fileName = "";
	m_arbGridFile = NULL;
	m_minEasting = UNSETfloat;
	m_minNorthing = UNSETfloat;
	m_maxEasting = UNSETfloat;
	m_maxNorthing = UNSETfloat;
	m_samplingIntervalE = UNSETfloat;
	m_samplingIntervalN = UNSETfloat;
	m_rows = UNSETint;
	m_cols = UNSETint;
}

ArbGridFile::ArbGridFile(std::string filename)
{
	m_fileName = filename;
	m_arbGridFile = NULL;
	m_minEasting = UNSETfloat;
	m_minNorthing = UNSETfloat;
	m_maxEasting = UNSETfloat;
	m_maxNorthing = UNSETfloat;
	m_samplingIntervalE = UNSETfloat;
	m_samplingIntervalN = UNSETfloat;
	m_rows = UNSETint;
	m_cols = UNSETint;
	if (!openArbGridFile(m_fileName)) {
		m_arbGridFile = NULL;
	}
}

ArbGridFile::~ArbGridFile()
{
	if (m_arbGridFile != NULL) {
		//delete m_arbGridFile;
	}
}

bool ArbGridFile::openArbGridFile(std::string arbGridFileName) 
{
	
	m_fileName = arbGridFileName;
	if (m_arbGridFile != NULL) {
		delete m_arbGridFile;
	}
	m_arbGridFile = new std::fstream(m_fileName);
	if (m_arbGridFile->good()) {
		std::cout << "reading " << m_fileName << "..." << std::endl;
	}
	else {
		std::cout << "couldn't find " << m_fileName << std::endl;
		return false;
	}
	readHeader();
	return true;
}
bool ArbGridFile::readHeader() 
{
	char *sBuffer = new char[ARBGRIDHEADERWIDTH+1];
	// It's unclear why I have to read an extra character but it definitely works right.
	m_arbGridFile->get(sBuffer, ARBGRIDHEADERWIDTH+1);
	std::string header(sBuffer);
	parseHeader(header);
	delete sBuffer;
	return true;
}
bool ArbGridFile::parseHeader(std::string csHeader)
{
	const int UTMWIDTH = 9; // number of chars in easting, northing, and sampling interval strings
	const int ROWCOLWIDTH = 5; // number of chars in row and col strings

	std::string csMinE, csMinN, csMaxE, csMaxN, csSampIntE, csSampIntN, csRows, csCols;
	//csMinE = csHeader.Mid(0, UTMWIDTH);
	csMinE = csHeader.substr(0, UTMWIDTH);
	csMinN = csHeader.substr(UTMWIDTH, UTMWIDTH);
	csMaxE = csHeader.substr(UTMWIDTH * 2, UTMWIDTH);
	csMaxN = csHeader.substr(UTMWIDTH * 3, UTMWIDTH);
	csSampIntE = csHeader.substr(UTMWIDTH * 4, UTMWIDTH);
	csSampIntN = csHeader.substr(UTMWIDTH * 5, UTMWIDTH);
	csRows = csHeader.substr (UTMWIDTH * 6, ROWCOLWIDTH);
	csCols = csHeader.substr(UTMWIDTH * 6 + ROWCOLWIDTH, ROWCOLWIDTH);
	std::string::size_type sz;
	//m_minEasting = std::stof(csMinE, &sz());
	m_minEasting = stof(csMinE, &sz);
	m_minNorthing = std::stof(csMinN, &sz);
	m_maxEasting = std::stof(csMaxE, &sz);
	m_maxNorthing = std::stof(csMaxN, &sz);
	m_samplingIntervalE = std::stof(csSampIntE, &sz);
	m_samplingIntervalN = std::stof(csSampIntN, &sz);
	m_rows = std::stoi(csRows, &sz);
	m_cols = std::stoi(csCols, &sz);
	return true;
}
std::fstream* ArbGridFile::getInFile(void)
{
	return m_arbGridFile;
}

float ArbGridFile::getMinEasting(void)
{
	return m_minEasting;
}

float ArbGridFile::getMinNorthing(void)
{
	return m_minNorthing;
}

float ArbGridFile::getMaxEasting(void)
{
	return m_maxEasting;
}

float ArbGridFile::getMaxNorthing(void)
{
	return m_maxNorthing;
}
float ArbGridFile::getSamplingIntervalE(void)
{
	return m_samplingIntervalE;
}
float ArbGridFile::getSamplingIntervalN(void)
{
	return m_samplingIntervalN;
}
int ArbGridFile::getRows(void) 
{
	return m_rows;
}
int ArbGridFile::getCols()
{
	return m_cols;
}

std::string ArbGridFile::getFileName(void)
{
	return m_fileName;
}

std::string ArbGridFile::getArbGridDir(void) 
{
	std::string dir;
	std::string path("");
	size_t lastForwardSlashIndex = m_fileName.find_last_of("/");
	if (lastForwardSlashIndex != std::string::npos) {
		path = m_fileName.substr(0, lastForwardSlashIndex);
		return path;
	} 
	size_t lastBackSlashIndex = m_fileName.find_last_of("\\");
	if (lastBackSlashIndex != std::string::npos) {
		path = m_fileName.substr(0, lastBackSlashIndex);
		return path;
	} 
	return "";
}