#pragma once
#include "Object.h"
#include "glm/glm.hpp"

class Node :RObject
{
public:

	Node();
	Node(glm::vec3 position);
	~Node();

	inline string GetName() { return m_name; };
	inline glm::vec3 GetPosition() { return m_position; };
	inline void SetName(string data) { m_name = data; };
	inline void SetPosition(glm::vec3 data) { m_position = data; };

	glm::vec3 m_position;
	string m_name;
};