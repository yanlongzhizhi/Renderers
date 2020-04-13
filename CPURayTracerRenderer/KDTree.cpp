#include "stdafx.h"
#include "KDTree.h"


KDTreeNode::KDTreeNode()
{
	m_triangles.clear();

	m_leftChild = NULL;
	m_rightChild = NULL;
}

KDTreeNode::~KDTreeNode()
{
	m_triangles.clear();

	if (m_leftChild)
	{
		delete m_leftChild;
		m_leftChild = NULL;
	}
	if (m_rightChild)
	{
		delete m_rightChild;
		m_rightChild = NULL;
	}
}

bool KDTreeNode::RayIntersect(Ray ray, double* distance, double* minDistance, glm::vec3* normal, glm::vec3* color)
{
	double distance0 = MAX_IDSTANCE;
	if (!m_boundingBox.IntersectionWithRay(ray, &distance0))
	{
		return false;
	}
	if (distance0 > * minDistance)
	{
		return false;
	}

	bool hitLeft = false;
	bool hitRight = false;
	bool hit = false;
	if (!m_rightChild && !m_leftChild)
	{
		for (int i = 0; i < m_triangles.size(); i++)
		{
			if (m_triangles[i]->Intersect(ray, distance, minDistance, normal))
			{
				hit = true;
				*minDistance = *distance;
				*color = m_triangles[i]->m_material.m_color;
			}
		}
		if (hit)
		{
			return true;
		}
		else
		{
			return false;
		}

	}
	hitLeft = m_leftChild ? m_leftChild->RayIntersect(ray, distance, minDistance, normal, color) : false;
	hitRight = m_rightChild ? m_rightChild->RayIntersect(ray, distance, minDistance, normal, color) : false;

	hit = hitLeft || hitRight;

	return hit;
}

KDTree::KDTree()
{
	m_root = NULL;
	m_maxDepth = 25;
}

KDTree::~KDTree()
{
	if (m_root)
	{
		delete m_root;
		m_root = NULL;
	}
}

void KDTree::BuildKDTree(vector<Triangle*>& triangles)
{
	if (triangles.size() < 1)
	{
		return;
	}

	m_root = new KDTreeNode();
	BuildKDNode(m_root, triangles, 0);
}

void KDTree::BuildKDNode(KDTreeNode* node, vector<Triangle*>& triangles, int depth)
{
	glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f);
	AABBBox box = triangles[0]->GetBoundingBox();
	for (int i = 1; i < triangles.size(); i++)
	{
		box.Add(triangles[i]->GetBoundingBox());
		center += triangles[i]->GetCenter() / (float)triangles.size();
	}

	node->m_triangles = triangles;
	node->m_boundingBox = box;

	if (depth == m_maxDepth || triangles.size() < 6)
	{
		return;
	}

	AXIS axis = box.GetLongestAxis();
	vector<Triangle*> leftTriangles;
	vector<Triangle*> rightTriangles;

	for (int i = 0; i < triangles.size(); i++)
	{
		if (axis == AXIS_X)
		{
			center.x < triangles[i]->GetCenter().x ? leftTriangles.push_back(triangles[i]) : rightTriangles.push_back(triangles[i]);
		}
		else if (axis == AXIS_Y)
		{
			center.y < triangles[i]->GetCenter().y ? leftTriangles.push_back(triangles[i]) : rightTriangles.push_back(triangles[i]);
		}
		else
		{
			center.z < triangles[i]->GetCenter().z ? leftTriangles.push_back(triangles[i]) : rightTriangles.push_back(triangles[i]);
		}
	}

	if (leftTriangles.size() >= 1)
	{
		node->m_leftChild = new KDTreeNode();
		BuildKDNode(node->m_leftChild, leftTriangles, depth + 1);
	}
	if (rightTriangles.size() >= 1)
	{
		node->m_rightChild = new KDTreeNode();
		BuildKDNode(node->m_rightChild, rightTriangles, depth + 1);
	}
	//BuildKDNode(node->m_leftChild, leftTriangles, depth + 1);
	//BuildKDNode(node->m_rightChild, rightTriangles, depth + 1);
}