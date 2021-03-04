#include <PhysicsSystem.h>
#include <Timer.h>
#include <GLM/gtc/type_ptr.hpp>
#include <RenderingManager.h>
#include <BtToGlm.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <Enemy.h>
#include <InstantiatingSystem.h>

btDiscreteDynamicsWorld* PhysicsSystem::m_World;

std::vector<btRigidBody*> PhysicsSystem::m_bodies;


std::vector<btCollisionShape*> colShapes;

bool callbackFunc(btManifoldPoint& cp, const btCollisionObjectWrapper* obj1, int id1, int index1, const btCollisionObjectWrapper* obj2, int id2, int index2);



void PhysicsSystem::Init()
{
	

	//code sourced from bullet github page

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

	m_World->setGravity(btVector3(0, 0, -30));

	gContactAddedCallback = callbackFunc;

}

void PhysicsSystem::Update()
{
	
	m_World->stepSimulation(Timer::dt, 1);


	auto reg = &RenderingManager::activeScene->Registry();

	/*
	entt::basic_group<entt::entity, entt::exclude_t<>, entt::get_t<Transform>, PhysicsBody> Phys =
		reg->group<PhysicsBody>(entt::get_t<Transform>());
	
	Phys.each([&](entt::entity e, PhysicsBody& bod, Transform& transform) 
		{
			transform.SetLocalPosition(BtToGlm::BTTOGLMV3(bod.GetBody()->getCenterOfMassTransform().getOrigin()));
			std::cout << transform.GetLocalPosition().y;
			std::cout << bod.GetBody()->getCenterOfMassTransform().getOrigin().getY();
		
		});


	*/

	auto& view = reg->view<Transform, PhysicsBody>();
	for (auto entity : view)
	{
		
		PhysicsBody& physBod = view.get<PhysicsBody>(entity);
		Transform& Trans = view.get<Transform>(entity);
		
		Trans.SetLocalPosition(BtToGlm::BTTOGLMV3(physBod.GetBody()->getCenterOfMassTransform().getOrigin()));
		Trans.Recalculate();
		//std::cout << Trans.GetLocalPosition().y;

		
	}


	
}

void PhysicsSystem::ClearWorld()
{
	for (int i = 0; i < m_bodies.size(); i++)
		m_World->removeRigidBody(m_bodies[i]);
}

//idk this probably works or smth
bool hitByFireProj = false;

bool callbackFunc(btManifoldPoint& cp, const btCollisionObjectWrapper* obj1, int id1, int index1,  const btCollisionObjectWrapper* obj2, int id2, int index2)
{

	//for direct hit with fire attack
	if (obj1->getCollisionObject()->getUserIndex() == 3 && obj2->getCollisionObject()->getUserIndex() == 2)
	{	
			PhysicsBody* p = reinterpret_cast<PhysicsBody*>(obj1->getCollisionObject()->getUserPointer());
		
			entt::registry& reg = RenderingManager::activeScene->Registry();
			reg.destroy((entt::entity)obj1->getCollisionObject()->getUserIndex3());
			
			reg.get<Enemy>((entt::entity)obj2->getCollisionObject()->getUserIndex3()).m_hp -= 1;
			


		
	}
	if (obj2->getCollisionObject()->getUserIndex() == 3 && obj1->getCollisionObject()->getUserIndex() == 2)
	{
		
	
		entt::registry& reg = RenderingManager::activeScene->Registry();
		reg.destroy((entt::entity)obj2->getCollisionObject()->getUserIndex3());

		reg.get<Enemy>((entt::entity)obj1->getCollisionObject()->getUserIndex3()).m_hp -= 1;
	}

	//Fire attack explosion
	//this gets called whenever the fire attack projectile hits anything
	if (obj1->getCollisionObject()->getUserIndex() == 3)
	{
		//loop through all the bodies in the world
		for (int i = 0; i < PhysicsSystem::m_bodies.size(); i++)
		{
			//we are only interested in bodies that are enemies
			if (PhysicsSystem::m_bodies[i]->getUserIndex() == 2)
			{
				//now that we have it narrowed down to our enemies we can do more computationally expensive stuff
				//set the positions
				btVector3 ProjectilePosition, enemyPosition;
				//due to the way collision call backs work we have to run this twice because it is equally likely the projectile is obj1 or obj2
				ProjectilePosition = obj1->getWorldTransform().getOrigin();
				enemyPosition = PhysicsSystem::m_bodies[i]->getCenterOfMassTransform().getOrigin();
				PhysicsBody* p = reinterpret_cast<PhysicsBody*>(obj1->getCollisionObject()->getUserPointer());
				PhysicsSystem::m_World->removeRigidBody(p->GetBody());
				btVector3 Diff;
				Diff = ProjectilePosition - enemyPosition;
				float length = glm::length(BtToGlm::BTTOGLMV3(Diff));
				if (length < 10.f)
				{
					entt::registry& reg = RenderingManager::activeScene->Registry();
					if (PhysicsSystem::m_bodies[i]->getUserIndex2() == 2)
						reg.get<Enemy>((entt::entity)obj2->getCollisionObject()->getUserIndex3()).m_hp -= 5;
					else
						reg.get<Enemy>((entt::entity)obj2->getCollisionObject()->getUserIndex3()).m_hp -= 2;
				}
			}
			
		}
	}
	if (obj2->getCollisionObject()->getUserIndex() == 3)
	{
		//loop through all the bodies in the world
		for (int i = 0; i < PhysicsSystem::m_bodies.size(); i++)
		{
			//we are only interested in bodies that are enemies
			if (PhysicsSystem::m_bodies[i]->getUserIndex() == 2)
			{
				//now that we have it narrowed down to our enemies we can do more computationally expensive stuff
				//set the positions
				btVector3 ProjectilePosition, enemyPosition;
				//due to the way collision call backs work we have to run this twice because it is equally likely the projectile is obj1 or obj2
				ProjectilePosition = obj2->getWorldTransform().getOrigin();
				enemyPosition = PhysicsSystem::m_bodies[i]->getCenterOfMassTransform().getOrigin();
				PhysicsBody* p = reinterpret_cast<PhysicsBody*>(obj2->getCollisionObject()->getUserPointer());
				PhysicsSystem::m_World->removeRigidBody(p->GetBody());

				btVector3 Diff;
				Diff = ProjectilePosition - enemyPosition;
				float length = glm::length(BtToGlm::BTTOGLMV3(Diff));
				if (length < 10.f)
				{
					Enemy* e = reinterpret_cast<Enemy*>(PhysicsSystem::m_bodies[i]->getUserPointer());
					if (PhysicsSystem::m_bodies[i]->getUserIndex2() == 2)
						e->m_hp -= 10;
					else
						e->m_hp -= 7;
				}
			}
			
		}
	}

	return false;
}

btDiscreteDynamicsWorld* PhysicsSystem::GetWorld()
{
	return m_World;
}

std::vector<btRigidBody*> PhysicsSystem::GetBodies()
{
	return m_bodies;
}