#include "stdafx.h"
#include "Renderer.h"

CPURayTraceRenderer::CPURayTraceRenderer()
{
	m_samples = 0;
	m_width = 0;
	m_height = 0;
	m_frameBuffer.clear();
	m_scene = NULL;
	m_kdTree = NULL;

	m_cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
	m_cameraDir = glm::vec3(0.0f, 0.0f, 0.0f);
}

CPURayTraceRenderer::~CPURayTraceRenderer()
{
	m_frameBuffer.clear();
	if (m_kdTree)
	{
		delete m_kdTree;
		m_kdTree = NULL;
	}
	if (m_scene)
	{
		delete m_scene;
		m_scene = NULL;
	}
}

void CPURayTraceRenderer::Initialize(int width, int height, int samples)
{
	//m_kdTree = new KDTree();
	//m_kdTree->BuildKDTree(triangles);

	m_width = width;
	m_height = height;
	m_samples = samples;
	m_scene = new Scene();

	for (int i = 0; i < m_width * m_height; i++)
	{
		m_frameBuffer.push_back(0.0f);
		m_frameBuffer.push_back(0.0f);
		m_frameBuffer.push_back(0.0f);
		m_frameBuffer.push_back(1.0f);
	}
}

Ray CPURayTraceRenderer::GernerateRay(int x, int y, bool jitter, unsigned short* seed)
{
	//double random = erand48(seed);
	float ratio = (float)m_width / m_height;
	double m_x_spacing = (2.0f * ratio) / (double)m_width;
	double m_y_spacing = (double)2.0 / (double)m_height;

	glm::vec2 jitterScreenDir = glm::vec2(0.0f, 0.0f);
	glm::vec2 stepScreen = glm::vec2((2.0f * ratio) / (double)m_width, (double)2.0 / (double)m_height);
	glm::vec3 jitterWorldDir = m_cameraPos + m_cameraDir * 2.0f;
	glm::vec3 rightDirection = glm::normalize(glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), -1.0f * m_cameraDir));
	glm::vec3 upDirection = glm::normalize(glm::cross(rightDirection, m_cameraDir));


	if (jitter) 
	{
		jitterScreenDir.x = (erand48(seed) * stepScreen.x) - stepScreen.x * 0.5f;
		jitterScreenDir.y = (erand48(seed) * stepScreen.y) - stepScreen.y * 0.5f;
	}


	jitterWorldDir = jitterWorldDir + rightDirection * (ratio - ratio * (x * 2.0f / m_width + jitterScreenDir.x));
	jitterWorldDir = jitterWorldDir + upDirection * (1.0f - ((y * 2.0f) / m_height + +jitterScreenDir.y));

	return Ray(m_cameraPos, glm::normalize(jitterWorldDir - m_cameraPos));
}

glm::vec3 CPURayTraceRenderer::ReflectRay(Ray ray, int depth, unsigned short* seed)
{
	return glm::vec3(0.0);
}

void CPURayTraceRenderer::Render()
{
	for (int i = 0; i < m_height; i++)
	{
		printf("\rCPU RayTracer Rendering (%i samples): %.2f%% ", m_samples, (double)i / m_height * 100);
		unsigned short seed[3] = { 0, 0, i * i * i };
		for (int j = 0; j < m_width; j++)
		{
			glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f);

			for (int k = 0; k < m_samples; k++)
			{
				bool jitter = false;
				if (k > 0)
				{
					jitter = true;
				}

				Ray reflectRay = GernerateRay(j, i, jitter, seed);
				color = color + m_scene->PathTrace(reflectRay, 0, seed);
			}

			color = color / (float)m_samples;
			//color = glm::vec3(-1.0f, -1.0f, 1.0f);

			m_frameBuffer[(i * m_width + j) * 4] = color.x;
			m_frameBuffer[(i * m_width + j) * 4 + 1] = color.y;
			m_frameBuffer[(i * m_width + j) * 4 + 2] = color.z;
		}
	}
	printf("\rCPU RayTracer Rendering (%i samples): %.2f%% ", m_samples, 1.0 * 100);
	printf("\rSave Rendered Texture to image: ./renderImage.png");
	SaveImage("renderImage.png");
}

unsigned char CPURayTraceRenderer::FrameDataToColor(float data)
{
	if (data < 0.0f)
	{
		data = 0.0f;
	}
	if (data > 1.0)
	{
		data = 1.0f;
	}
	return int(data * 255);
}

void CPURayTraceRenderer::SaveImage(string path)
{
	vector<unsigned char> pixelBuffer;
	for (int i = 0; i < m_width * m_height * 4; i++)
	{
		pixelBuffer.push_back(FrameDataToColor(m_frameBuffer[i]));
	}
	lodepng::encode(path, pixelBuffer, m_width, m_height);
	pixelBuffer.clear();
}