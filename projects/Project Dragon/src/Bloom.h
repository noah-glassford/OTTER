#pragma once
#include <PostEffect.h>
#include <LUT.h>
class BloomEffect : public PostEffect
{
public:
	void Init(unsigned width, unsigned height) override;
	void ApplyEffect(PostEffect* buffer) override;

	//Reshapes the buffers
	void Reshape(unsigned width, unsigned height) override;

	//Getters
	float GetDownscale() const;
	float GetThreshold() const;
	unsigned GetPasses() const;

	//Setters
	void SetDownscale(float downscale);
	void SetThreshold(float threshold);
	void SetPasses(unsigned passes);
	
	//apply to screen
private:
	float m_downscale = 2.f;
	float m_threshold = 0.01f;
	unsigned m_passes = 10;
	glm::vec2 m_pixelSize;

};