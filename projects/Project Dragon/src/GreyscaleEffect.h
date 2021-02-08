#pragma once

#include "PostEffect.h"

class GreyscaleEffect : public PostEffect
{
public:
	void Init(unsigned width, unsigned height) override;
	void ApplyEffect(PostEffect* buffer) override;

	//apply to screen
	void DrawToScreen() override;

	float GetIntensity() const;

	void SetIntensity(float intensity);
private:
	float _intensity = 1.f;

};