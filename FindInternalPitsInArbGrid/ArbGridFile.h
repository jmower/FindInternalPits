#pragma once
#include <string>
class ArbGridFile
{
public:
	ArbGridFile();
	ArbGridFile(std::string filename);
	~ArbGridFile();
public:
	bool openArbGridFile(std::string arbGridFileName);
	bool readHeader(void);
	bool parseHeader(std::string csHeader);
	std::fstream* getInFile(void);
	void parseFileName(void);
	float getMinEasting(void);
	float getMinNorthing(void);
	float getMaxEasting(void);
	float getMaxNorthing(void);
	float getSamplingIntervalE(void);
	float getSamplingIntervalN(void);
	int getRows(void);
	int getCols(void);
	const float UNSETGLFLOAT = -1.0f;
	const float UNSETfloat = -1.0f;
	const int UNSETGLINT = -1;
	const int UNSETint = -1;
	const int ARBGRIDHEADERWIDTH = 64;

private:
	std::fstream* m_arbGridFile;
	std::string m_fileName;
	float m_minEasting, m_minNorthing, m_maxEasting, m_maxNorthing;
	float m_samplingIntervalE, m_samplingIntervalN;
	int m_rows, m_cols;
public:
	std::string getFileName(void);
	std::string getArbGridDir(void);
};

