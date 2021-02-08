#pragma once
#include <bullet/btBulletCollisionCommon.h>
#include <GLM/glm.hpp>

//Just used to convert glm to bullet and vice versa

static class BtToGlm
{
public:
	static glm::vec3 BTTOGLMV3(btVector3 input)
	{
		glm::vec3 output;
		output.x = input.getX();
		output.y = input.getY();
		output.z = input.getZ();
		return output;
	}

	static btVector3 GLMTOBTV3(glm::vec3 input)
	{
		btVector3 output;
		output.setX(input.x);
		output.setY(input.y);
		output.setZ(input.z);
		return output;
	}

};