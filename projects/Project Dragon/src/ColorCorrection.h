#pragma once
#include <PostEffect.h>
#include <LUT.h>
class ColorCorrectionEffect : public PostEffect
{
public:
	void Init(unsigned width, unsigned height) override;
	void ApplyEffect(PostEffect* buffer) override;
	void LoadLUT(std::string path);
	void LoadLUT(std::string path, int index);
	//apply to screen

	//vector of luts
	std::vector<LUT3D> _LUTS;

	LUT3D _LUT;
};