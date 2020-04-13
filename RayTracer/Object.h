#pragma once
#include<iostream>
#include<vector>
#include "glm/glm.hpp"
using namespace std;

class RObject{

public:
	int m_id;
	string m_typeName;

	RObject(int id = 0, string typeName = "");
	~RObject();

	int GetId();
	string GetTypeName();

	virtual void Update() = 0;
};