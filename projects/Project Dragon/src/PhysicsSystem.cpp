#include <PhysicsSystem.h>
#include <Timer.h>
#include <GLM/gtc/type_ptr.hpp>
#include <RenderingManager.h>
#include <BtToGlm.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

btDiscreteDynamicsWorld* PhysicsSystem::m_World;

std::vector<btRigidBody*> PhysicsSystem::m_bodies;


std::vector<btCollisionShape*> colShapes;

void PhysicsSystem::Init()
{
	//I STOLE THIS FROM BULLET GITHUB PAGE GOD THIS IS SO TEMPORARY

	///-----includes_end-----

	int i;
	///-----initialization_start-----

	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	m_World = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	m_World->setGravity(btVector3(0, -30, 0));
}

void PhysicsSystem::Update()
{
	auto reg = &RenderingManager::activeScene->Registry();

	m_World->stepSimulation(Timer::dt, 10);


	auto view = reg->view<PhysicsBody, Transform>();

	for (auto entity : view)
	{
		PhysicsBody& physBod = view.get<PhysicsBody>(entity);
		Transform& Trans = view.get<Transform>(entity);
		std::cout << "Transform" << Trans.GetLocalPosition().y << std::endl;
		std::cout << "Phys" << physBod.GetBody()->getCenterOfMassTransform().getOrigin() << std::endl;
		Trans.SetLocalPosition(BtToGlm::BTTOGLMV3(physBod.GetBody()->getCenterOfMassTransform().getOrigin()));

		Trans.UpdateWorldMatrix();
	}

}

void PhysicsSystem::ClearWorld()
{
	for (int i = 0; i < m_bodies.size(); i++)
		m_World->removeRigidBody(m_bodies[i]);
}

btDiscreteDynamicsWorld* PhysicsSystem::GetWorld()
{
	return m_World;
}

std::vector<btRigidBody*> PhysicsSystem::GetBodies()
{
	return m_bodies;
}