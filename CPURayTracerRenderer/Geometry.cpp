#include "stdafx.h"
#include "Geometry.h"
#include "erand48.h"

Ray::Ray()
{
	m_origin = glm::vec3(0.0, 0.0, 0.0);
	m_direction = glm::vec3(0.0, 0.0, 0.0);
}

Ray::Ray(glm::vec3 origin, glm::vec3 direction)
{
	m_origin = origin;
	m_direction = direction;
}

Ray::~Ray()
{

}

AABBBox::AABBBox()
{
	m_leftBottom = glm::vec3(0.0f, 0.0f, 0.0f);
	m_rightTop = glm::vec3(0.0f, 0.0f, 0.0f);
}

AABBBox::AABBBox(glm::vec3 leftBottom, glm::vec3 rightTop)
{
	m_leftBottom = leftBottom;
	m_rightTop = rightTop;
}

AABBBox::~AABBBox()
{

}

void AABBBox::Add(AABBBox box)
{
	if (box.m_leftBottom.x < m_leftBottom.x)
	{
		m_leftBottom.x = box.m_leftBottom.x;
	}
	if (box.m_leftBottom.y < m_leftBottom.y)
	{
		m_leftBottom.y = box.m_leftBottom.y;
	}
	if (box.m_leftBottom.z < m_leftBottom.z)
	{
		m_leftBottom.z = box.m_leftBottom.z;
	}

	if (box.m_rightTop.x > m_rightTop.x)
	{
		m_rightTop.x = box.m_rightTop.x;
	}
	if (box.m_rightTop.y > m_rightTop.y)
	{
		m_rightTop.y = box.m_rightTop.y;
	}
	if (box.m_rightTop.z > m_rightTop.z)
	{
		m_rightTop.z = box.m_rightTop.z;
	}
}

void AABBBox::Add(glm::vec3 point)
{
	if (point.x < m_leftBottom.x)
	{
		m_leftBottom.x = point.x;
	}
	if (point.y < m_leftBottom.y)
	{
		m_leftBottom.y = point.y;
	}
	if (point.z < m_leftBottom.z)
	{
		m_leftBottom.z = point.z;
	}
}

AXIS AABBBox::GetLongestAxis()
{
	glm::vec3 diagonal = m_rightTop - m_leftBottom;
	if (diagonal.x > diagonal.y && diagonal.x > diagonal.z)
	{
		return AXIS_X;
	}
	else if (diagonal.y > diagonal.x && diagonal.y > diagonal.z)
	{
		return AXIS_Y;
	}
	else
	{
		return AXIS_Z;
	}
}

bool AABBBox::IntersectionWithRay(Ray ray, double* distance)
{
	glm::vec3 invDirection;
	invDirection.x = 1.0f / ray.m_direction.x;
	invDirection.y = 1.0f / ray.m_direction.y;
	invDirection.z = 1.0f / ray.m_direction.z;

	double tx1 = (m_leftBottom.x - ray.m_origin.x)*invDirection.x;
	double tx2 = (m_rightTop.x - ray.m_origin.x)*invDirection.x;

	double tmin = min(tx1, tx2);
	double tmax = max(tx1, tx2);

	double ty1 = (m_leftBottom.y - ray.m_origin.y)*invDirection.y;
	double ty2 = (m_rightTop.y - ray.m_origin.y)*invDirection.y;

	tmin = max(tmin, min(ty1, ty2));
	tmax = min(tmax, max(ty1, ty2));

	double tz1 = (m_leftBottom.z - ray.m_origin.z)*invDirection.z;
	double tz2 = (m_rightTop.z - ray.m_origin.z)*invDirection.z;

	tmin = max(tmin, min(tz1, tz2));
	tmax = min(tmax, max(tz1, tz2));
	*distance = tmin;

	return tmax >= tmin;
}

Material::Material()
{
	m_type = DIFFUSE;
	m_color = glm::vec3(1.0, 1.0, 1.0);
	m_emission = glm::vec3(0.0, 0.0, 0.0);
	m_roughness = 1.0;
}

Material::Material(REFLECTTYPE type, glm::vec3 color, float roughness)
{
	m_type = type;
	m_color = color;
	m_emission = glm::vec3(0.0, 0.0, 0.0);
	m_roughness = roughness;
}

Material::Material(REFLECTTYPE type, glm::vec3 color, glm::vec3 emission, float roughness)
{
	m_type = type;
	m_color = color;
	m_emission = emission;
	m_roughness = roughness;
}

Material::~Material()
{

}

Ray Material::ReflectRay(Ray inRay, glm::vec3 posiotn, glm::vec3 normal, unsigned short* seed)
{
	if (m_type == DIFFUSE)
	{
		return BRDF_DIFFUSE(inRay, posiotn, normal, seed);
	}
	else if (m_type == SPECULAR)
	{
		return BRDF_SPECULAR(inRay, posiotn, normal, seed);
	}
	else
	{
		return Ray();
	}
}

Ray Material::BRDF_DIFFUSE(Ray inRay, glm::vec3 posiotn, glm::vec3 normal, unsigned short* seed)
{
	glm::vec3 normalRectify = glm::dot(normal, inRay.m_direction) < 0 ? normal : normal * glm::vec3(-1.0);

	double r1 = 2 * M_PI * erand48(seed);
	double r2 = erand48(seed);
	double r2s = sqrt(r2);

	glm::vec3 yaw = fabs(normalRectify.x) > 0.1 ? glm::vec3(0.0, 1.0, 0.0) : glm::vec3(1.0, 0.0, 0.0);
	yaw = glm::cross(yaw, normalRectify);
	yaw = glm::normalize(yaw);

	glm::vec3 roll = glm::cross(normalRectify, yaw);

	glm::vec3 direction = (yaw * glm::vec3(cos(r1) * r2s) + roll * glm::vec3(sin(r1) * r2s) + normalRectify * glm::vec3(sqrt(1 - r2)));
	direction = glm::normalize(direction);

	return Ray(posiotn, direction);
}

Ray Material::BRDF_SPECULAR(Ray inRay, glm::vec3 posiotn, glm::vec3 normal, unsigned short* seed)
{
	glm::vec3 reflection = inRay.m_direction - normal * glm::vec3(2 * glm::dot(normal, inRay.m_direction));

	reflection.x = reflection.x + (erand48(seed) - 0.5) * m_roughness;
	reflection.y = reflection.y + (erand48(seed) - 0.5) * m_roughness;
	reflection.z = reflection.z + (erand48(seed) - 0.5) * m_roughness;
	reflection = glm::normalize(reflection);

	return Ray(posiotn, reflection);
}

Triangle::Triangle(glm::vec3 pos0, glm::vec3 pos1, glm::vec3 pos2, glm::vec3 edge0, glm::vec3 edge1, glm::vec3 normal, Material material, glm::vec3 color)
{
	m_pos0 = pos0;
	m_pos1 = pos1;
	m_pos2 = pos2;
	m_edge0 = edge0;
	m_edge1 = edge1;
	m_normal = normal;
	m_material = material;
	m_color = color;
}

Triangle::~Triangle()
{

}

AABBBox Triangle::GetBoundingBox()
{
	glm::vec3 leftBottom, rightTop;
	leftBottom.x = min(min(m_pos0.x, m_pos1.x), m_pos2.x);
	leftBottom.y = min(min(m_pos0.y, m_pos1.y), m_pos2.y);
	leftBottom.z = min(min(m_pos0.z, m_pos1.z), m_pos2.z);

	rightTop.x = max(max(m_pos0.x, m_pos1.x), m_pos2.x);
	rightTop.y = max(max(m_pos0.y, m_pos1.y), m_pos2.y);
	rightTop.z = max(max(m_pos0.z, m_pos1.z), m_pos2.z);

	return AABBBox(leftBottom, rightTop);
}

glm::vec3 Triangle::GetCenter()
{
	glm::vec3 center = (m_pos0 + m_pos1 + m_pos2) / 3.0f;
	return center;
}

bool Triangle::Intersect(Ray ray, double* distance, double* minDistance, glm::vec3* normal)
{
	double u, v, t_temp = 0;

	glm::vec3 pvec = glm::cross(ray.m_direction, m_edge1); 
	double det = glm::dot(m_edge0, pvec);
	if (det == 0)
	{
		return false;
	}
		
	double invDet = 1.0f / det;
	glm::vec3 tvec = ray.m_origin - m_pos0;
	u = glm::dot(tvec, pvec) * invDet;
	if (u < 0 || u > 1)
	{
		return false;
	}

	glm::vec3 qvec = glm::cross(tvec, m_edge0);
	v = glm::dot(ray.m_direction, qvec) * invDet;
	if (v < 0 || u + v > 1)
	{
		return false;
	}
	t_temp = glm::dot(m_edge1, qvec) * invDet;
	if (t_temp < *minDistance) 
	{
		if (t_temp > 1e-9) 
		{   
			*distance = t_temp;
			*normal = m_normal;
			return true;
		}
	}
	return false;
}

glm::vec3 Triangle::GetProjection(glm::vec3 point)
{
	glm::vec3 pos = point - m_pos0;
	double distance00 = glm::dot(m_edge0, m_edge0);
	double distance01 = glm::dot(m_edge0, m_edge1);
	double distance11 = glm::dot(m_edge1, m_edge1);
	double distance20 = glm::dot(pos, m_edge0);
	double distance21 = glm::dot(pos, m_edge1);

	double distance = distance00 * distance11 - distance01 * distance01;
	double v = (distance11*distance20 - distance01 * distance21) / distance;
	double w = (distance00*distance21 - distance01 * distance20) / distance;
	double u = 1 - v - w;
	return glm::vec3(u, v, w);
}