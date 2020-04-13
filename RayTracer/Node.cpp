#include "pch.h"
#include "Node.h"

Node::Node()
{
	m_position = glm::vec3(0.0, 0.0, 0.0);
}

Node::Node(glm::vec3 position)
{
	m_position = position;
}

Node::~Node()
{

}