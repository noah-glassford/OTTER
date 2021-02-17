#pragma once
#include <GLM/glm.hpp>
class LightSource //defines a point light
{
public:
	glm::vec3 m_Ambient = glm::vec3(0.05f, 0.05f, 0.05f);//default to white
	glm::vec3 m_Diffuse = glm::vec3(0.8f, 0.8f, 0.8f);//default to white
	glm::vec3 m_Specular = glm::vec3(1.f, 1.f, 1.f);//default to white
};
