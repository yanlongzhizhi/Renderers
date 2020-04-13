#pragma once
#include<iostream>
#include<algorithm>
#include<vector>
#include "glm/glm.hpp"

#define MAX_IDSTANCE 0xFFFFFFFF
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;

class Ray;
class AABBBox;
class Triangle;

enum REFLECTTYPE
{
	DIFFUSE=0,
	SPECULAR,
	LIGHTSOURCE
};

enum AXIS
{
	AXIS_X = 0,
	AXIS_Y,
	AXIS_Z
};

class Ray
{
public:
	Ray();
	Ray(glm::vec3 origin, glm::vec3 direction);
	~Ray();

	glm::vec3 m_origin;
	glm::vec3 m_direction;
};

class AABBBox
{
public:
	AABBBox();
	AABBBox(glm::vec3 leftBottom, glm::vec3 rightTop);
	~AABBBox();

	void Add(AABBBox box);
	void Add(glm::vec3 point);
	AXIS GetLongestAxis();
	bool IntersectionWithRay(Ray ray, double* distance);

	glm::vec3 m_leftBottom;
	glm::vec3 m_rightTop;
};

class Material
{
public:
	Material();
	Material(REFLECTTYPE type, glm::vec3 color, float roughness);
	Material(REFLECTTYPE type, glm::vec3 color, glm::vec3 emission, float roughness);
	~Material();

	Ray ReflectRay(Ray inRay, glm::vec3 posiotn, glm::vec3 normal, unsigned short* seed);
	Ray BRDF_DIFFUSE(Ray inRay, glm::vec3 posiotn, glm::vec3 normal, unsigned short* seed);
	Ray BRDF_SPECULAR(Ray inRay, glm::vec3 posiotn, glm::vec3 normal, unsigned short* seed);

	REFLECTTYPE m_type;
	glm::vec3 m_color;
	glm::vec3 m_emission;
	float m_roughness;
};

class Triangle
{
public:
	Triangle(glm::vec3 pos0, glm::vec3 pos1, glm::vec3 pos2, glm::vec3 edge0, glm::vec3 edge1, glm::vec3 normal, Material material, glm::vec3 color);
	~Triangle();

	AABBBox GetBoundingBox();
	glm::vec3 GetCenter();
	bool Intersect(Ray ray, double* distance, double* minDistance, glm::vec3* normal);
	glm::vec3 GetProjection(glm::vec3 point);

	glm::vec3 m_pos0;
	glm::vec3 m_pos1;
	glm::vec3 m_pos2;
	glm::vec3 m_edge0;
	glm::vec3 m_edge1;
	glm::vec3 m_normal;

	Material m_material;
	glm::vec3 m_color;
};