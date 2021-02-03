#pragma once
#include <vector>
#include <Texture2D.h>
#include <Shader.h>

struct DepthTarget
{
	//Deconstructor for Depth Target
	//*Unloads texture
	~DepthTarget();
	//deletes the texture of the depth target
	void Unload();
	//holds the depth texture
	Texture2D _texture;
};

struct ColorTarget
{
	//Deconstructor for Color Target
	//*Unloads all the color targets
	~ColorTarget();
	void Unload();
	std::vector<Texture2D> _textures;
	std::vector<GLenum> _formats;
	std::vector<GLenum> _buffers;

	unsigned int _numAttachments = 0;
};

class Framebuffer
{
public:
	Framebuffer();
	~Framebuffer();

	void Unload();

	virtual void Init(unsigned width, unsigned height);
	void Init();
	//Adds depth target
	//ONLY EVER ONE
	void AddDepthTarget();

	//Adds color targer
	//As many as you want
	void AddColorTarget(GLenum format);
	//binds
	void BindDepthAsTexture(int textureSlot) const;
	void BindColorAsTexture(unsigned colorBuffer, int textureSlot) const;
	void UnbindTexture(int textureSlot) const;

	void Reshape(unsigned width, unsigned height);
	void SetSize(unsigned width, unsigned height);

	void SetViewport() const;

	void Clear();
	bool CheckFBO();


	void Bind() const;

	void Unbind() const;

	//Renders to fullscreen quad
	void RenderToFSQ() const;

	//draws the contents of the framebuffer to the back buffer
	void DrawToBackBuffer();

	//initialize FSQ
	static void InitFullscreenQuad();

	static void DrawFullscreenQuad();

	unsigned int _width = 0;
	unsigned int _height = 0;


protected:
	//ogl framebuffer handle
	GLuint _FBO;
	//Depth attachment
	DepthTarget _depth;
	ColorTarget _color;

	GLbitfield _clearFlag = 0;

	GLenum _filter = GL_NEAREST;

	GLenum _wrap = GL_CLAMP_TO_EDGE;

	bool _isInit = false;
	//depth attachment?
	bool _depthActive = false;

	static GLuint _fullscreenQuadVBO;
	static GLuint _fullscreenQuadVAO;

	static int _maxColorAttachments;

	static bool _isInitFSQ;

};