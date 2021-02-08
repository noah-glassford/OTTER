#pragma once
//This is the class for ECS
//using basically only AABB
//Add this to the entitites you want to have physics
#include <GLM/glm.hpp>
#include <vector>
#include <bullet/btBulletDynamicsCommon.h>
#include <Transform.h>

class PhysicsBody
{
public:

	btRigidBody* GetBody();
	void SetBody(btRigidBody* body);
	//for now all bodies will be boxes, will change later but really I just want to get
	void Update();

	void SetLinearVelocity(btVector3 direction);
	void ApplyForce(btVector3 direction);
	void AddBody(float mass, btVector3 origin, btVector3 size);
	void AddBody(float mass, btVector3 origin, btVector3 size, float friction);

	void SetUserData(int data);
	void SetUserData2(int data);

	int m_BodyId;//this for da world thing
	int m_Entity;//attach this entitys id to itself so I can call the appropriate transform in update.

private:
	//pass this to this components transform in the update

	btRigidBody* m_Body = nullptr;
	

};
