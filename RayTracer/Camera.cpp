#include "pch.h"
#include "Camera.h"

Camera::Camera()
{
	m_position = glm::vec3(0.0, 0.0, 0.0);
	m_direction = glm::vec3(0.0, 0.0, -1.0);
	m_width = 620;
	m_height = 480;
	m_fov = 0.5;
	m_ratio = (float)m_width / (float)m_height;;
}

Camera::Camera(glm::vec3 position, glm::vec3 direction)
{
	m_position = position;
	m_direction = direction;
	m_width = 620;
	m_height = 480;
	m_fov = 0.5;
	m_ratio = (float)m_width/(float)m_height;
}

Camera::Camera(glm::vec3 position, glm::vec3 direction, int width, int height, float fov, float ratio)
{
	m_position = position;
	m_direction = direction;
	m_width = width;
	m_height = height;
	m_fov = fov;
	m_ratio = ratio;
}

Camera::~Camera()
{

}