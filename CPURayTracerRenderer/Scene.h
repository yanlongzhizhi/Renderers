#pragma once
#include "Geometry.h"
#include "KDTree.h"
#include "tiny_obj_loader.h"

class RayObjectIntersection 
{
public:
	RayObjectIntersection(bool hitFlag = false, double distance = 0, glm::vec3 normal = glm::vec3(0.0), Material material = Material());
	~RayObjectIntersection();

	bool m_isHit;
	double m_distance;
	glm::vec3 m_normal;
	Material m_material;
};


class Object 
{
public:
	Object();
	~Object();

	virtual RayObjectIntersection IntersectWithRay(Ray inRay) = 0;

	glm::vec3 m_worldPosition;
};


class Sphere : public Object 
{
public:
	Sphere();
	Sphere(glm::vec3 position, double radius, Material material);
	~Sphere();

	inline double GetRadius() { return m_radius;};
	inline Material GetMaterial() { return m_material; };

	virtual RayObjectIntersection IntersectWithRay(Ray inRay);

	double m_radius;
	Material m_material;
};


class Mesh : public Object 
{
public:
	Mesh();
	Mesh(glm::vec3 postion, const char* file_path, Material material);
	~Mesh();
	virtual RayObjectIntersection IntersectWithRay(Ray inRay);

private:
	std::vector<tinyobj::shape_t> m_shapes;
	std::vector<tinyobj::material_t> m_materials;
	std::vector<Material> materials;
	std::vector<Triangle*> m_triangles;
	Material m_material;	// Material
	KDTree* m_kdTree;

};

class Scene
{
public:
	Scene();
	~Scene();

	RayObjectIntersection SceneIntersect(Ray inRay);
	glm::vec3 PathTrace(Ray inRay, int depth, unsigned short* seed);

	std::vector<Object*> m_objects;
	int m_EConvDepth;
	float m_RRpro;
	glm::vec3 m_sky;
};