#pragma once
#include "Node.h"

class vertex
{
public:
	glm::vec3 m_position;
	glm::vec3 m_normal;
	glm::vec2 m_uv;
};

class Model :Node
{
	string m_modelName;

public:
	Model();
	~Model();

	void LoadModel(string filePath);

	vector<vertex> m_vertex;
};