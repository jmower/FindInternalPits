#pragma once
class FrustumBounds
{
public:
	FrustumBounds(void);
	FrustumBounds(float minE, float maxE, float minElev, float maxElev, float minN, float maxN);
	~FrustumBounds(void);
	float getMinE();
	float getMaxE();
	float getMinElev();
	float getMaxElev();
	float getMinN();
	float getMaxN();
private:
	float m_minE;
	float m_maxE;
	float m_minElev;
	float m_maxElev;
	float m_minN;
	float m_maxN;
};

