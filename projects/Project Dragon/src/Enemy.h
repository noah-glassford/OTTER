#pragma once
#include <Scene.h>
#include <GLM/glm.hpp>
#include <bullet/btBulletCollisionCommon.h>
#include <Timer.h>
#include <PhysicsBody.h>
#include <iostream>
//basically playerdata but for the enemy
class Enemy
{
public:
	//Enemy Stat Values
	float m_hp = 10.f;

	//Enemy(GameObject* ThisObj);

	//Internal Values
	glm::vec3 movementDirection = glm::vec3(0, 0, 0);
	btVector3 thisPosition;
	btVector3 distance;
	int entityNumber = 2;
	int lookRange = 5;
	float distanceNorm = 100.0f;
	float mTimer = 0.0f;
	float m_MovementSpeed = 100.f;

	float TimeBetweenHits = 1.f;
	float HitTimer = 0.f;
	bool canBeHit = false;

	//pass the gameobject into this entity on creation
	//PhysicsBody* m_This = nullptr;

	//External Values
	btVector3 playerPosition;

	bool shouldFuckingDie = false;

	void Update(PhysicsBody m_This);
<<<<<<< HEAD
	void TakeDamage(float damage) { m_hp -= damage;  };
	void Kill() { shouldFuckingDie = true; };
	

	//temp
	bool died = false;
=======
>>>>>>> parent of 8b27400 (Uh Shit Happened)

	//to check if the player should take damage
};