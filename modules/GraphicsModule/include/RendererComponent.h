#pragma once
#include <VertexArrayObject.h>
#include <ShaderMaterial.h>

class RendererComponent {
public:
	VertexArrayObject::sptr Mesh;
	ShaderMaterial::sptr    Material;
	bool CastShadows = true;

	RendererComponent& SetMesh(const VertexArrayObject::sptr& mesh) { Mesh = mesh; return *this; }
	RendererComponent& SetMaterial(const ShaderMaterial::sptr& material) { Material = material; return *this; }
};