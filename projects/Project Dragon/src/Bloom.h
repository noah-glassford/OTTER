#pragma once
#include <PostEffect.h>
#include <LUT.h>
class BloomEffect : public PostEffect
{
public:
	void Init(unsigned width, unsigned height) override;
	void ApplyEffect(PostEffect* buffer) override;
	
	//apply to screen

};