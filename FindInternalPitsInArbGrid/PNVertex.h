#pragma once
#include <glm/glm.hpp>

struct PNVertex {
	glm::vec3 m_pos;
	glm::vec2 m_texture; // not used at the moment
	glm::vec3 m_normal;
	PNVertex()
		: m_pos(0, 0, 0)
		, m_texture(0, 0)
		, m_normal(0, 0, 0)
	{
	}
	PNVertex(float easting, float elevation, float northing)
		: m_texture(0, 0)
		, m_normal(0, 0, 0)
	{
		m_pos = glm::vec3(easting, elevation, northing);
	}
};
