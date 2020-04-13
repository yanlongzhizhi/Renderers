#include "pch.h"
#include "Object.h"

RObject::RObject(int id, string typeName)
{
	m_id = id;
	m_typeName = typeName;
}

RObject::~RObject()
{

}

int RObject::GetId()
{
	return m_id;
}

string RObject::GetTypeName()
{
	return m_typeName;
}