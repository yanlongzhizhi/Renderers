#pragma once
#include "Object.h"
#include "Texture.h"
#include "Shader.h"

enum REFLECTTYPE
{
	DIFFUSE=0,
	SPECULAR,
	EMIT
};

class RMaterial :RObject
{
public:
	RMaterial(glm::vec4 color, REFLECTTYPE type = DIFFUSE, float roughness = 1.0, float metalness = 0.0);
	~RMaterial();

	inline float GetRoughness() { return m_roughness; };
	inline float GetMetalness() { return m_metalness; };
	inline void SetRoughness(float data) { m_roughness = data; };
	inline void SetMetalness(float data) { m_metalness = data; };

	inline glm::vec4 GetColor() { return m_color; };
	inline REFLECTTYPE GetType() { return m_type; };
	inline void SetColor(glm::vec4 data) { m_color = data; };
	inline void SetType(REFLECTTYPE data) { m_type = data; };

private:
	REFLECTTYPE m_type;
	float m_roughness;
	float m_metalness;

	glm::vec4 m_color;

	Texture* m_texture;
	Shader* shader;
};