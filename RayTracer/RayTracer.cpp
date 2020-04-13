// RayTracer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include "tinyxml.h"
#include "CPURenderer.h"

int main()
{
	CPURayTraceRenderer* CPURenderer = NULL;

	TiXmlDocument* config = new TiXmlDocument("RenderConfig.xml");
	config->LoadFile();
	TiXmlElement* rootElement = config->RootElement();
	if (rootElement)
	{
		//Render config
		TiXmlElement* renderer = rootElement->FirstChildElement("Renderer");
		TiXmlElement* child = renderer->FirstChildElement();
		
		while (child)
		{
			const char* name = child->Value();

			if (!strncmp(name, "parameter", 9))
			{
				TiXmlAttribute* pAttribute = child->FirstAttribute();

				while (pAttribute)
				{
					const char* name = pAttribute->Name();
					const char* value = pAttribute->Value();

					if (!strncmp(value, "Type", 4))
					{
						pAttribute = pAttribute->Next();
						const char* name = pAttribute->Name();
						const char* value = pAttribute->Value();

						if (!strncmp(value, "CPURayTracer", 12))
						{
							CPURenderer = new CPURayTraceRenderer();
						}
						pAttribute = pAttribute->Next();
					}
					else if (!strncmp(value, "RectSamples", 11))
					{
						pAttribute = pAttribute->Next();
						const char* name = pAttribute->Name();
						const char* value = pAttribute->Value();

						int width, height, samples;
						sscanf(value, "%d %d %d", &width, &height, &samples);
						CPURenderer->Initialize(width, height, samples);
						pAttribute = pAttribute->Next();
					}

					//pAttribute = pAttribute->Next();
					//int i = 0;
					//i++;
				}

				child = child->NextSiblingElement();
			}
			else if (!strncmp(name, "Camera", 6))
			{
				//Camera config
				TiXmlElement* camera = child->FirstChildElement();

				while (camera)
				{
					TiXmlAttribute* pAttribute = camera->FirstAttribute();

					while (pAttribute)
					{
						const char* name = pAttribute->Name();
						const char* value = pAttribute->Value();

						if (!strncmp(value, "Position", 8))
						{
							pAttribute = pAttribute->Next();
							const char* name = pAttribute->Name();
							const char* value = pAttribute->Value();

							glm::vec3 position;
							sscanf(value, "%f %f %f", &position.x, &position.y, &position.z);
							CPURenderer->SetCameraPos(position);
						}
						else if (!strncmp(value, "Direction", 9))
						{
							pAttribute = pAttribute->Next();
							const char* name = pAttribute->Name();
							const char* value = pAttribute->Value();

							glm::vec3 direction;
							sscanf(value, "%f %f %f", &direction.x, &direction.y, &direction.z);
							CPURenderer->SetCameraDir(glm::normalize(direction));
						}

						pAttribute = pAttribute->Next();
					}
					camera = camera->NextSiblingElement();
				}
				child = child->NextSiblingElement();
			}
			else if (!strncmp(name, "Scene", 4))
			{
				//Scene config
				//TiXmlElement* scene = renderer->FirstChildElement("Scene");
				TiXmlElement* sceneObject = child->FirstChildElement();

				while (sceneObject)
				{
					bool sphere = false;
					bool mesh = false;
					Material* material = NULL;
					char filePath[50] = { 0 };
					glm::vec3 position;
					float radius;

					TiXmlElement* sceneChild = sceneObject->FirstChildElement();

					while (sceneChild)
					{
						TiXmlAttribute* pAttribute = sceneChild->FirstAttribute();
						if (!pAttribute)
						{
							material = new Material();
							glm::vec3 color;
							glm::vec3 emission;
							float gloss;
							//Material config
							TiXmlElement* materialNode = sceneChild->FirstChildElement();
							while (materialNode)
							{

								TiXmlAttribute* pAttribute = materialNode->FirstAttribute();
								while (pAttribute)
								{
									const char* name = pAttribute->Name();
									const char* value = pAttribute->Value();

									if (!strncmp(value, "Type", 8))
									{
										pAttribute = pAttribute->Next();
										const char* name = pAttribute->Name();
										const char* value = pAttribute->Value();

										if (!strncmp(value, "Diffuse", 7))
										{
											material->m_type = DIFFUSE;
										}
										if (!strncmp(value, "Specular", 8))
										{
											material->m_type = SPECULAR;
										}
										if (!strncmp(value, "Light", 5))
										{
											material->m_type = LIGHTSOURCE;
										}
									}
									else if (!strncmp(value, "Color", 5))
									{
										pAttribute = pAttribute->Next();
										const char* name = pAttribute->Name();
										const char* value = pAttribute->Value();

										sscanf(value, "%f %f %f", &color.x, &color.y, &color.z);
										material->m_color = color;
									}
									else if (!strncmp(value, "Gloss", 5))
									{
										pAttribute = pAttribute->Next();
										const char* name = pAttribute->Name();
										const char* value = pAttribute->Value();

										sscanf(value, "%f", &gloss);
										material->m_roughness = gloss;
									}
									else if (!strncmp(value, "Emission", 8))
									{
										pAttribute = pAttribute->Next();
										const char* name = pAttribute->Name();
										const char* value = pAttribute->Value();

										sscanf(value, "%f %f %f", &emission.x, &emission.y, &emission.z);
										material->m_emission = emission;
									}

									pAttribute = pAttribute->Next();
								}
								materialNode = materialNode->NextSiblingElement();
							}
						}
						else
						{
							//TiXmlElement* parameter = sceneChild->FirstChildElement();
							//TiXmlAttribute* pAttribute = parameter->FirstAttribute();
							while (pAttribute)
							{
								const char* name = pAttribute->Name();
								const char* value = pAttribute->Value();

								if (!strncmp(value, "Type", 8))
								{
									pAttribute = pAttribute->Next();
									const char* name = pAttribute->Name();
									const char* value = pAttribute->Value();

									if (!strncmp(value, "Sphere", 5))
									{
										sphere = true;
									}
									else if (!strncmp(value, "Mesh", 4))
									{
										mesh = true;
									}
								}
								else if (!strncmp(value, "Position", 8))
								{
									pAttribute = pAttribute->Next();
									const char* name = pAttribute->Name();
									const char* value = pAttribute->Value();

									sscanf(value, "%f %f %f", &position.x, &position.y, &position.z);

								}
								else if (!strncmp(value, "Radius", 6))
								{
									pAttribute = pAttribute->Next();
									const char* name = pAttribute->Name();
									const char* value = pAttribute->Value();

									sscanf(value, "%f", &radius);
								}
								else if (!strncmp(value, "FileName", 8))
								{
									pAttribute = pAttribute->Next();
									const char* name = pAttribute->Name();
									const char* value = pAttribute->Value();

									strcpy(filePath, value);
								}

								pAttribute = pAttribute->Next();
							}
						}

						sceneChild = sceneChild->NextSiblingElement();
					}

					if (sphere)
					{
						CPURenderer->m_scene->m_objects.push_back(dynamic_cast<Object*>(new Sphere(position, radius, *material)));
					}
					else if (mesh)
					{
						CPURenderer->m_scene->m_objects.push_back(dynamic_cast<Object*>(new Mesh(position, filePath, *material)));
					}

					sceneObject = sceneObject->NextSiblingElement();
				}

				child = child->NextSiblingElement();
			}
		}
	}

	CPURenderer->Render();
}
