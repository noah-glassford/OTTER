#include <PhysicsBody.h>
#include <Timer.h>
#include <PhysicsSystem.h>

btRigidBody* PhysicsBody::GetBody()
{
	return m_Body;
}

void PhysicsBody::SetBody(btRigidBody* body)
{
	m_Body = body;
}

void PhysicsBody::Update()
{
	
}

void PhysicsBody::AddBody(float mass, btVector3 origin, btVector3 size)
{
	btCollisionShape* colShape = new btBoxShape(size);

	btTransform startTransform;
	startTransform.setIdentity();

	bool isDynamic = true;
	if (mass == 0.f)
		isDynamic = false;

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		colShape->calculateLocalInertia(mass, localInertia);

	startTransform.setOrigin(origin);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
	m_Body= new btRigidBody(rbInfo);

	m_Body->setAngularFactor(btVector3(0.0f, 0.0f, 0.0f));
	
	PhysicsSystem::m_World->addRigidBody(m_Body);
	PhysicsSystem::m_bodies.push_back(m_Body);
	m_BodyId = PhysicsSystem::m_bodies.size() - 1;
	m_Body->setFriction(1.f);

	

}


void PhysicsBody::AddBody(float mass, btVector3 origin, btVector3 size, float friction)
{
	btCollisionShape* colShape = new btBoxShape(size);

	btTransform startTransform;
	startTransform.setIdentity();

	bool isDynamic = true;
	if (mass == 0.f)
		isDynamic = false;

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		colShape->calculateLocalInertia(mass, localInertia);

	startTransform.setOrigin(origin);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
	m_Body = new btRigidBody(rbInfo);

	m_Body->setAngularFactor(btVector3(0.0f, 0.0f, 0.0f));

	PhysicsSystem::m_World->addRigidBody(m_Body);
	PhysicsSystem::m_bodies.push_back(m_Body);
	m_BodyId = PhysicsSystem::m_bodies.size() - 1;
	m_Body->setFriction(friction);


}

void PhysicsBody::SetUserData(int data)
{
	m_Body->setUserIndex(data);
}

void PhysicsBody::SetUserData2(int data)
{
	m_Body->setUserIndex2(data);
}

void PhysicsBody::SetLinearVelocity(btVector3 direction)
{
	m_Body->setActivationState(1);
	m_Body->setLinearVelocity(direction);
}

void PhysicsBody::ApplyForce(btVector3 direction)
{
	m_Body->activate(1);
	m_Body->applyCentralImpulse(direction);

}
