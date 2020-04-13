#pragma once
#include "erand48.h"
#include "KDTree.h"
#include "Scene.h"
#include "lodepng.h"

class CPURayTraceRenderer
{

public:
	CPURayTraceRenderer();
	~CPURayTraceRenderer();

	inline void SetSamples(int data) { m_samples = data; };
	inline int GetSamples() { return m_samples; };
	inline void SetWidth(int data) { m_width = data; };
	inline int GetWidth() { return m_width; };
	inline void SetHeight(int data) { m_height = data; };
	inline int GetHeight() { return m_height; };
	inline void SetCameraPos(glm::vec3 data) { m_cameraPos = data; };
	inline glm::vec3 GetCameraPos() { return m_cameraPos; };
	inline void SetCameraDir(glm::vec3 data) { m_cameraDir = data; };
	inline glm::vec3 GetCameraDir() { return m_cameraDir; };

	unsigned char FrameDataToColor(float data);
	void Initialize(int width, int height, int samples);
	Ray GernerateRay(int x, int y, bool jitter, unsigned short* seed);
	glm::vec3 ReflectRay(Ray ray, int depth, unsigned short* seed);
	void Render();
	void SaveImage(string path);

	Scene* m_scene;

private:
	int m_samples;
	int m_width;
	int m_height;
	glm::vec3 m_cameraPos;
	glm::vec3 m_cameraDir;
	vector<float> m_frameBuffer;
	KDTree* m_kdTree;
};