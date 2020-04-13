#include "stdafx.h"
#include "Scene.h"
#include "erand48.h"

Object::Object()
{
	m_worldPosition = glm::vec3(0.0);
}

Object::~Object()
{

}

Sphere::Sphere()
{
	m_worldPosition = glm::vec3(0.0);
	m_radius = 0.0;
	m_material = Material();
}

Sphere::Sphere(glm::vec3 position, double radius, Material material)
{
	m_worldPosition = position;
	m_radius = radius;
	m_material = material;
}

Sphere::~Sphere()
{

}

RayObjectIntersection Sphere::IntersectWithRay(Ray inRay)
{
	// Solve t^2*d.d + 2*t*(o-p).d + (o-p).(o-p)-R^2 = 0
	bool hit = false;
	double distance = 0.0;
	glm::vec3 normal = glm::vec3(0.0);

	glm::vec3 RayToPos = m_worldPosition - inRay.m_origin;
	double tip = 0.0;
	double epsion = 1e-4;
	double b = glm::dot(RayToPos, inRay.m_direction);
	double det = b * b - glm::dot(RayToPos, RayToPos) + m_radius * m_radius;
	if (det < 0)
	{
		return RayObjectIntersection(hit, distance, normal, m_material);
	}
	else
	{
		det = sqrt(det);
	}
	distance = (tip = b - det) > epsion ? tip : ((tip = b + det) > epsion ? tip : 0);
	if (fabs(distance) > 0.001)
	{
		hit = true;
	}
	normal = glm::normalize((inRay.m_origin + inRay.m_direction * glm::vec3(distance)) - m_worldPosition);

	return RayObjectIntersection(hit, distance, normal, m_material);
}

Mesh::Mesh()
{
	m_worldPosition = glm::vec3(0.0);
	m_material = Material();
	m_kdTree = NULL;
	m_shapes.empty();
	m_materials.empty();
	materials.empty();
	m_triangles.empty();
}

Mesh::Mesh(glm::vec3 postion, const char* file_path, Material material)
{
	m_worldPosition = postion;
	m_material = material;
	m_kdTree = NULL;

	m_shapes.empty();
	m_materials.empty();
	materials.empty();
	m_triangles.empty();

	std::string mtlbasepath;
	std::string inputfile = file_path;
	unsigned long pos = inputfile.find_last_of("/");
	mtlbasepath = inputfile.substr(0, pos + 1);

	std::string err = tinyobj::LoadObj(m_shapes, m_materials, inputfile.c_str(), inputfile.c_str());

	if (!err.empty()) {
		std::cerr << err << std::endl;
		exit(1);
	}

	long shapes_size = 0;
	long indices_size = 0;
	long materials_size = 0;
	shapes_size = m_shapes.size();
	materials_size = m_materials.size();

	for (int i = 0; i < materials_size; i++) 
	{
		materials.push_back(Material(DIFFUSE, glm::vec3(1.0), 0.8));
	}

	for (int i = 0; i < shapes_size; i++) 
	{
		indices_size = m_shapes[i].mesh.indices.size() / 3;
		for (size_t f = 0; f < indices_size; f++) 
		{
			glm::vec3 v0 = glm::vec3(
				m_shapes[i].mesh.positions[m_shapes[i].mesh.indices[3 * f] * 3],
				m_shapes[i].mesh.positions[m_shapes[i].mesh.indices[3 * f] * 3 + 1],
				m_shapes[i].mesh.positions[m_shapes[i].mesh.indices[3 * f] * 3 + 2]
			) + m_worldPosition;

			glm::vec3 v1 = glm::vec3(
				m_shapes[i].mesh.positions[m_shapes[i].mesh.indices[3 * f + 1] * 3],
				m_shapes[i].mesh.positions[m_shapes[i].mesh.indices[3 * f + 1] * 3 + 1],
				m_shapes[i].mesh.positions[m_shapes[i].mesh.indices[3 * f + 1] * 3 + 2]
			) + m_worldPosition;

			glm::vec3 v2 = glm::vec3(
				m_shapes[i].mesh.positions[m_shapes[i].mesh.indices[3 * f + 2] * 3],
				m_shapes[i].mesh.positions[m_shapes[i].mesh.indices[3 * f + 2] * 3 + 1],
				m_shapes[i].mesh.positions[m_shapes[i].mesh.indices[3 * f + 2] * 3 + 2]
			) + m_worldPosition;

			glm::vec3 edg0, edg1, normal;
			edg0 = v1 - v0;
			edg1 = v2 - v0;
			normal = glm::normalize(glm::cross(edg0, edg1));

			m_triangles.push_back(new Triangle(v0, v1, v2, edg0, edg1, normal, m_material, m_material.m_color));
		}
	}

	// Clean up
	m_shapes.clear();
	m_materials.clear();
	m_kdTree = new KDTree();
	m_kdTree->BuildKDTree(m_triangles);
	printf("\n");
}

Mesh::~Mesh()
{
	m_shapes.empty();
	m_materials.empty();
	materials.empty();

	for (int i = 0; i < m_triangles.size(); i++)
	{
		delete m_triangles[i];
		m_triangles[i] = NULL;
	}
	m_triangles.empty();

	if (m_kdTree)
	{
		delete m_kdTree;
		m_kdTree = NULL;
	}
}

RayObjectIntersection Mesh::IntersectWithRay(Ray inRay)
{
	double t = 0, tmin = INFINITY;
	glm::vec3 normal = glm::vec3();
	glm::vec3 color = glm::vec3();
	bool hit = m_kdTree->m_root->RayIntersect(inRay, &t, &tmin, &normal, &color);

	return RayObjectIntersection(hit, tmin, normal, Material(DIFFUSE, color, 0.8));
}

RayObjectIntersection::RayObjectIntersection(bool hitFlag, double distance, glm::vec3 normal, Material material)
{
	m_isHit = hitFlag;
	m_distance = distance;
	m_normal = normal;
	m_material = material;
}

RayObjectIntersection::~RayObjectIntersection()
{

}

Scene::Scene()
{
	m_objects.empty();

	m_EConvDepth = 5;
	m_RRpro = 0.9;
	m_sky = glm::vec3(0.5294, 0.8078, 0.9503);
}

Scene::~Scene()
{
	for (int i = 0; i < m_objects.size(); i++)
	{
		delete m_objects[i];
		m_objects[i] = NULL;
	}
	m_objects.empty();
}

RayObjectIntersection Scene::SceneIntersect(Ray inRay)
{
	RayObjectIntersection rayIntsect = RayObjectIntersection();
	RayObjectIntersection rayIntsectTemp;
	long size = m_objects.size();

	for (int i = 0; i < size; i++) 
	{
		rayIntsectTemp = m_objects[i]->IntersectWithRay(inRay);

		if (rayIntsectTemp.m_isHit)
		{
			if (fabs(rayIntsect.m_distance) < 0.0001 || rayIntsectTemp.m_distance < rayIntsect.m_distance)
			{
				rayIntsect = rayIntsectTemp;
			}
		}
	}
	return rayIntsect;
}

glm::vec3 Scene::PathTrace(Ray inRay, int depth, unsigned short* seed)
{
	RayObjectIntersection rayIntsect = SceneIntersect(inRay);

	if (!rayIntsect.m_isHit)
	{
		return m_sky;
	}

	if (rayIntsect.m_material.m_type == LIGHTSOURCE)
	{
		return rayIntsect.m_material.m_emission;
	}

	glm::vec3 color = rayIntsect.m_material.m_color;
	double terminate = max(max(color.x, color.y), color.z);

	double rnd = erand48(seed);

	//Guass Distribution
	double u = 0.0;
	double a = 1.0;
	double ret = (1.0 / (a * sqrtf(2.0 * 3.1415926))) * exp(-1.0 * (rnd - u) * (rnd - u) / (2 * a * a));
	ret = rnd;

	if (depth > m_EConvDepth)
	{
		if (ret < terminate * m_RRpro)
		{
			float decrese = m_RRpro / terminate;
			color = color * glm::vec3(decrese);
		}
		else 
		{
			return rayIntsect.m_material.m_emission;
		}
	}

	glm::vec3 x = inRay.m_origin + inRay.m_direction * glm::vec3(rayIntsect.m_distance);
	Ray reflected = rayIntsect.m_material.ReflectRay(inRay, x, rayIntsect.m_normal, seed);
	depth++;
	return color * PathTrace(reflected, depth, seed);
}