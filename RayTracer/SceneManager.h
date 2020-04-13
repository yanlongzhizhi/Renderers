#pragma once
#include "Node.h"

class SceneManager :RObject
{
	vector<Node*> m_nodes;

public:
	SceneManager();
	~SceneManager();

	void AddNode(Node* node);
};