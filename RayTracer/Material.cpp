#include "pch.h"
#include "Material.h"

RMaterial::RMaterial(glm::vec4 color, REFLECTTYPE type, float roughness, float metalness)
{
	m_color = color;
	m_type = type;
	m_roughness = roughness;
	m_metalness = metalness;
}

RMaterial::~RMaterial()
{

}