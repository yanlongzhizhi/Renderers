#pragma once
#include "Object.h"

class Renderer :RObject
{
public:
	Renderer();
	~Renderer();

	virtual void Initialize() = 0;
	virtual void Render() = 0;
};