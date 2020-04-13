#pragma once
#include "Node.h"
#include "Geometry.h"

class Camera:Node 
{
public:

	Camera();
	Camera(glm::vec3 position, glm::vec3 direction);
	Camera(glm::vec3 position, glm::vec3 direction, int width, int height, float fov, float ratio);
	~Camera();

	inline int GetWidth() { return m_width; };
	inline int GetHeight() { return m_height; };
	inline float GetFov() { return m_fov; };
	inline float GetRatio() { return m_ratio; };
	inline void SetFov(float data) { m_fov = data; };
	inline void SetRatio(float data) { m_ratio = data; };

private:
	int m_width;
	int m_height;
	float m_fov;
	float m_ratio;
	glm::vec3 m_direction;
};