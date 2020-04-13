#pragma once
#include "Geometry.h"

class KDTreeNode
{
public:
	KDTreeNode();
	~KDTreeNode();

	bool RayIntersect(Ray ray, double* distance, double* minDistance, glm::vec3* normal, glm::vec3* color);

	vector<Triangle*> m_triangles;
	AABBBox m_boundingBox;

	KDTreeNode* m_leftChild;
	KDTreeNode* m_rightChild;
};

class KDTree
{
public:

	KDTree();
	~KDTree();

	void BuildKDTree(vector<Triangle*>& triangles);
	void BuildKDNode(KDTreeNode* node, vector<Triangle*>& triangles, int depth);

	KDTreeNode* m_root;
	int m_maxDepth;
};