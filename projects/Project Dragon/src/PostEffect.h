#pragma once

#include <Framebuffer.h>
#include "Shader.h"

class PostEffect
{
public:
	virtual void Init(unsigned width, unsigned height);

	virtual void ApplyEffect(PostEffect* previousBuffer);
	virtual void DrawToScreen();

	//reshapes
	virtual void Reshape(unsigned width, unsigned height);

	//clears
	void Clear();

	void Unload();

	void BindBuffer(int index);
	void UnBindBuffer();

	void BindColorAsTexture(int index, int colorBuffer, int textureSlot);
	void BindDepthAsTexture(int index, int textureSlot);
	void UnbindTexture(int textureSlot);

	void BindShader(int index);
	void UnbindShader();

protected:
	std::vector<Framebuffer*> _buffers;
	std::vector<Shader::sptr> _shaders;

};