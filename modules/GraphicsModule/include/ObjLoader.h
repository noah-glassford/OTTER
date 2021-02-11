#pragma once
#include "MeshFactory.h"
struct Frame
{
	VertexBuffer::sptr m_Pos;
	VertexBuffer::sptr m_Col;
	VertexBuffer::sptr m_Normal;
	VertexBuffer::sptr m_UV;
};

class ObjLoader
{
public:
	static VertexArrayObject::sptr LoadFromFile(const std::string& filename, const glm::vec4& inColor = glm::vec4(1.0f));
	static Frame LoadFrame(std::string filename, glm::vec4 col);

protected:
	ObjLoader() = default;
	~ObjLoader() = default;
};