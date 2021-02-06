#pragma once
#include <PostEffect.h>
#include <LUT.h>
class ColorCorrectionEffect : public PostEffect
{
public:
	void Init(unsigned width, unsigned height) override;
	void ApplyEffect(PostEffect* buffer) override;
	void LoadLUT(std::string path);
	//apply to screen


	LUT3D _LUT;
};