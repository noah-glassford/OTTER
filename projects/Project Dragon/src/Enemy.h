#pragma once
#include <Scene.h>
#include <GLM/glm.hpp>
#include <bullet/btBulletCollisionCommon.h>
#include <Timer.h>
#include <PhysicsBody.h>
//basically playerdata but for the enemy
class Enemy
{
public:
	//Enemy Stat Values
	int m_hp = 3;

	//Enemy(GameObject* ThisObj);

	//Internal Values
	glm::vec3 movementDirection = glm::vec3(0, 0, 0);
	btVector3 thisPosition;
	btVector3 distance;
	int entityNumber = 2;
	int lookRange = 5;
	float distanceNorm = 100.0f;
	float mTimer = 0.0f;
	float m_MovementSpeed = 10.f;

	float TimeBetweenHits = 1.f;
	float HitTimer = 0.f;
	bool canBeHit = false;

	//pass the gameobject into this entity on creation
	//PhysicsBody* m_This = nullptr;

	//External Values
	btVector3 playerPosition;

	void Update(PhysicsBody m_This);

	//to check if the player should take damage
};