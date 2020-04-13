#include "pch.h"
#include "SceneManager.h"

SceneManager::SceneManager()
{

}

SceneManager::~SceneManager()
{

}

void SceneManager::AddNode(Node* node)
{
	for (int i = 0; i < m_nodes.size(); i++)
	{
		if (m_nodes[i]->GetName() == node->GetName())
		{
			return;
		}
	}

	m_nodes.push_back(node);
}