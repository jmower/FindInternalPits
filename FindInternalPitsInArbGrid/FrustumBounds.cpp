#include "stdafx.h"
#include "FrustumBounds.h"


FrustumBounds::FrustumBounds()
	: m_minE(0)
	, m_maxE(0)
	, m_minElev(0)
	, m_maxElev(0)
	, m_minN(0)
	, m_maxN(0)
{
}


FrustumBounds::~FrustumBounds()
{
}

FrustumBounds::FrustumBounds(float minE, float maxE, float minElev, float maxElev, float minN, float maxN) {
	m_minE = minE;
	m_maxE = maxE;
	m_minElev = minElev;
	m_maxElev = maxElev;
	m_minN = minN;
	m_maxN = maxN;
}

float FrustumBounds::getMinE() { return m_minE; }
float FrustumBounds::getMaxE() { return m_maxE; }
float FrustumBounds::getMinElev() { return m_minElev; }
float FrustumBounds::getMaxElev() { return m_maxElev; }
float FrustumBounds::getMinN() { return m_minN; }
float FrustumBounds::getMaxN() { return m_maxN; }
