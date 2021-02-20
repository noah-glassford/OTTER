#include <Player.h>
#include <BtToGlm.h>
#include <PhysicsSystem.h>
#include <Enemy.h>
#include <random>
/*
bool Weapon::Shoot(float range)
{
	
	//* Commented out for now because testing with a scene without enemies and crashed if I don't comment this out :)
	if (m_CanShoot )
	{
		m_Timer = 0.f;


		GameObject cameraObj = RenderingManager::activeScene->FindFirst("Camera");
		Transform t = cameraObj.get<Transform>();
		glm::mat4 tempView = glm::inverse(t.WorldTransform());

		double dArray[16] = { 0.0 };


		//This here gives us a lookAt vector
		const float* pSource = (const float*)glm::value_ptr(tempView);
		for (int i = 0; i < 16; ++i)
			dArray[i] = pSource[i];

		glm::vec3 lookDir;
		lookDir.x = -dArray[2];
		lookDir.y = -dArray[6];
		lookDir.z = -dArray[10];

		//Grabs player position
		btVector3 playerPosition = RenderingManager::activeScene->FindFirst("Camera").get<PhysicsBody>().GetBody()->getCenterOfMassTransform().getOrigin();
		
		//construct our raycast vector for shooting
		lookDir *= range;
		btVector3 to = BtToGlm::GLMTOBTV3(lookDir);
		to += playerPosition;

		btCollisionWorld::ClosestRayResultCallback Results(playerPosition, to);

		PhysicsSystem::GetWorld()->rayTest(playerPosition, to, Results);

		if (Results.hasHit()  && Results.m_collisionObject->getUserIndex() == 5) //if this is run you hit an enemy
		{
			//Instantiate projectile/marker of where you shot because hitscan
			InstantiatingSystem::LoadPrefabFromFile(glm::vec3(BtToGlm::BTTOGLMV3(Results.m_collisionObject->getWorldTransform().getOrigin())),"node/Water_Proj.node");

			//does damage to enemy
		
			Enemy* e = reinterpret_cast<Enemy*>(Results.m_collisionObject->getUserPointer());
			e->m_hp -= 1;

			return true;
		}
		else
		{
			InstantiatingSystem::LoadPrefabFromFile(BtToGlm::BTTOGLMV3(to), "node/Water_Proj.node");
			//ECS::Get<Transform>(2).SetPosition(BtToGlm::BTTOGLMV3(to));
			return false;
		}
	}
	else
	{
	//	std::cout << "Not Shot\n";
		return false;
		
	}
	
	return false;
}

void Weapon::Update()
{
	m_Timer += Timer::dt;
	
	if (m_Timer >= m_FireDelay)
		m_CanShoot = true;
	else
		m_CanShoot = false;
}

void Player::Update()
{
	m_Weapons[0].Update();
}

void Player::CheckJump()
{

	btVector3 playerPosition = RenderingManager::activeScene->FindFirst("Camera").get<PhysicsBody>().GetBody()->getCenterOfMassTransform().getOrigin();
	btVector3 to = playerPosition;
	to.setZ(to.getZ() - 2.f);
	//to += playerPosition;
	btCollisionWorld::ClosestRayResultCallback Results(playerPosition, to);

	

	PhysicsSystem::GetWorld()->rayTest(playerPosition, to, Results);

	if (Results.hasHit())
	{
		m_PlayerData.m_CanJump = true;
	}
	else
	{
		m_PlayerData.m_CanJump = false;
	}
}

PlayerData Player::GetPlayerData()
{
	return m_PlayerData;
}

void Player::SetPlayerData(PlayerData playdata)
{
	m_PlayerData = playdata;
}

Weapon Player::GetWeapon(int index)
{
	return m_Weapons[index];
}

std::vector<Weapon> Player::GetWeaponVector()
{
	return m_Weapons;
}

void Player::SetActiveWeapon(int activeweapon)
{
	m_ActiveWeapon = activeweapon;
}

int Player::GetActiveWeapon()
{
	return m_ActiveWeapon;
}
*/


void Player::InitWeapons()
{
	m_RightHandWeapons.push_back(new FireWeapon());
	m_RightHandWeapons.push_back(new WaterWeapon());
	m_LeftHandWeapons.push_back(new AirWeapon());
	m_LeftHandWeapons.push_back(new EarthWeapon());

	m_RightHandWeapons[1]->m_FireDelay = 3;
	m_LeftHandWeapons[0]->m_FireDelay = 2;
	m_LeftHandWeapons[1]->m_FireDelay = 0.000000000001;

}


bool FireWeapon::Fire()
{

	std::cout << "Shot FireWeapon\n";
	return false;
}

bool WaterWeapon::Fire()
{
	
	if (m_CanShoot)
	{
		m_CanShoot = false;
		m_Timer = 0.f;


		GameObject cameraObj = RenderingManager::activeScene->FindFirst("Camera");
		Transform t = cameraObj.get<Transform>();
		glm::mat4 tempView = glm::inverse(t.WorldTransform());

		double dArray[16] = { 0.0 };


		//This here gives us a lookAt vector
		const float* pSource = (const float*)glm::value_ptr(tempView);
		for (int i = 0; i < 16; ++i)
			dArray[i] = pSource[i];

		glm::vec3 lookDir;
		lookDir.x = -dArray[2];
		lookDir.y = -dArray[6];
		lookDir.z = -dArray[10];

		//Grabs player position
		btVector3 playerPosition = RenderingManager::activeScene->FindFirst("Camera").get<PhysicsBody>().GetBody()->getCenterOfMassTransform().getOrigin();

		//construct our raycast vector for shooting
		lookDir *= 50;
		btVector3 to = BtToGlm::GLMTOBTV3(lookDir);
		to += playerPosition;

		btCollisionWorld::ClosestRayResultCallback Results(playerPosition, to);

		PhysicsSystem::GetWorld()->rayTest(playerPosition, to, Results);

		if (Results.hasHit() && Results.m_collisionObject->getUserIndex() == 2) //if this is run you hit an enemy
		{
			//Instantiate projectile/marker of where you shot because hitscan
			InstantiatingSystem::LoadPrefabFromFile(glm::vec3(BtToGlm::BTTOGLMV3(Results.m_collisionObject->getWorldTransform().getOrigin())), "node/Water_Proj.node");

			//does damage to enemy

			Enemy* e = reinterpret_cast<Enemy*>(Results.m_collisionObject->getUserPointer());
			e->m_hp -= 1;
			std::cout << e->m_hp;

			return true;
		}
		else
		{
			InstantiatingSystem::LoadPrefabFromFile(BtToGlm::BTTOGLMV3(to), "node/Water_Proj.node");
			//ECS::Get<Transform>(2).SetPosition(BtToGlm::BTTOGLMV3(to));
			return false;
		}
	}
	else
	{
		//	std::cout << "Not Shot\n";
		return false;

	}

	return false;
}

bool AirWeapon::Fire()
{
	if (m_CanShoot)
	{
		m_CanShoot = false;
		m_Timer = 0.f;
		std::cout << "Shot AirWeapon\n";


		GameObject cameraObj = RenderingManager::activeScene->FindFirst("Camera");
		Transform t = cameraObj.get<Transform>();
		glm::mat4 tempView = glm::inverse(t.WorldTransform());

		double dArray[16] = { 0.0 };


		//This here gives us a lookAt vector
		const float* pSource = (const float*)glm::value_ptr(tempView);
		for (int i = 0; i < 16; ++i)
			dArray[i] = pSource[i];

		glm::vec3 lookDir;
		lookDir.x = -dArray[2];
		lookDir.y = -dArray[6];
		lookDir.z = -dArray[10];

		//Grabs player position
		btVector3 playerPosition = RenderingManager::activeScene->FindFirst("Camera").get<PhysicsBody>().GetBody()->getCenterOfMassTransform().getOrigin();

		//construct our raycast vector for shooting
		lookDir *= 10;
		btVector3 to = BtToGlm::GLMTOBTV3(lookDir);
		to += playerPosition;

		//since a shotgun shoots multiple pellets in a spread, this loop will run a set number of raytests and use rand to make the trajectory spread
		int i = 0;
		while (i < 10)
		{
			btVector3 tempVec = to;
			std::cout << i;
			float PathDeviation = static_cast<float>(rand()) / static_cast<float>(RAND_MAX); //this returns a random float between 0 and 1
			//lets pretend this is something fancy
			
			if (i % 1 == 0)
			tempVec.setX(tempVec.getX() + PathDeviation * 2);
			if (i % 2 == 0)
			tempVec.setY(tempVec.getY() + PathDeviation * 2);
			if (i % 3 == 0)
			tempVec.setZ(tempVec.getZ() + PathDeviation * 2);


			btCollisionWorld::ClosestRayResultCallback Results(playerPosition, tempVec);

			PhysicsSystem::GetWorld()->rayTest(playerPosition, tempVec, Results);

			if (Results.hasHit() && Results.m_collisionObject->getUserIndex() == 2) //if this is run you hit an enemy
			{
				//Instantiate projectile/marker of where you shot because hitscan
				InstantiatingSystem::LoadPrefabFromFile(glm::vec3(BtToGlm::BTTOGLMV3(Results.m_collisionObject->getWorldTransform().getOrigin())), "node/Water_Proj.node");

				//does damage to enemy

				Enemy* e = reinterpret_cast<Enemy*>(Results.m_collisionObject->getUserPointer());
				e->m_hp -= 1;
				std::cout << e->m_hp;

				//return true;
			}
			else
			{
				InstantiatingSystem::LoadPrefabFromFile(BtToGlm::BTTOGLMV3(tempVec), "node/Water_Proj.node");
				//ECS::Get<Transform>(2).SetPosition(BtToGlm::BTTOGLMV3(to));
				//return false;
			}
			i++;
		}
	}
	else
	{
		//	std::cout << "Not Shot\n";
		return false;

	}

	return false;
}

bool EarthWeapon::Fire()
{

	if (m_CanShoot)
	{
		std::cout << "Shot Earth\n";
		m_CanShoot = false;
		m_Timer = 0.f;


		GameObject cameraObj = RenderingManager::activeScene->FindFirst("Camera");
		Transform t = cameraObj.get<Transform>();
		glm::mat4 tempView = glm::inverse(t.WorldTransform());

		double dArray[16] = { 0.0 };


		//This here gives us a lookAt vector
		const float* pSource = (const float*)glm::value_ptr(tempView);
		for (int i = 0; i < 16; ++i)
			dArray[i] = pSource[i];

		glm::vec3 lookDir;
		lookDir.x = -dArray[2];
		lookDir.y = -dArray[6];
		lookDir.z = -dArray[10];

		//Grabs player position
		btVector3 playerPosition = RenderingManager::activeScene->FindFirst("Camera").get<PhysicsBody>().GetBody()->getCenterOfMassTransform().getOrigin();

		//construct our raycast vector for shooting
		lookDir *= 30;
		btVector3 to = BtToGlm::GLMTOBTV3(lookDir);
		to += playerPosition;

		btCollisionWorld::ClosestRayResultCallback Results(playerPosition, to);

		PhysicsSystem::GetWorld()->rayTest(playerPosition, to, Results);

		if (Results.hasHit() && Results.m_collisionObject->getUserIndex() == 2) //if this is run you hit an enemy
		{
			//Instantiate projectile/marker of where you shot because hitscan
			InstantiatingSystem::LoadPrefabFromFile(glm::vec3(BtToGlm::BTTOGLMV3(Results.m_collisionObject->getWorldTransform().getOrigin())), "node/Water_Proj.node");

			//does damage to enemy

			Enemy* e = reinterpret_cast<Enemy*>(Results.m_collisionObject->getUserPointer());
			e->m_hp -= 1;
			std::cout << e->m_hp;

			return true;
		}
		else
		{
			InstantiatingSystem::LoadPrefabFromFile(BtToGlm::BTTOGLMV3(to), "node/Water_Proj.node");
			//ECS::Get<Transform>(2).SetPosition(BtToGlm::BTTOGLMV3(to));
			return false;
		}
	}
	else
	{
		//	std::cout << "Not Shot\n";
		return false;

	}

	return false;
	return false;
}

bool hasInit = false;
void Player::Update()
{
	if (!hasInit)
	{
		InitWeapons();
		hasInit = 1;
	}


	btVector3 playerPosition = RenderingManager::activeScene->FindFirst("Camera").get<PhysicsBody>().GetBody()->getCenterOfMassTransform().getOrigin();
	btVector3 to = playerPosition;
	to.setZ(to.getZ() - 2.f);
	//to += playerPosition;
	btCollisionWorld::ClosestRayResultCallback Results(playerPosition, to);



	PhysicsSystem::GetWorld()->rayTest(playerPosition, to, Results);

	if (Results.hasHit())
	{
		 m_CanJump = true;
	}
	else
	{
		m_CanJump = false;
	}

	m_LeftHandWeapons[m_LeftEquiped]->Update();
	m_RightHandWeapons[m_RightEquiped]->Update();

}

void Player::SwitchLeftHand()
{
	m_LeftEquiped = !m_LeftEquiped;
}

void Player::SwitchRightHand()
{
	m_RightEquiped = !m_RightEquiped;
}

void Player::LeftHandShoot()
{
	m_LeftHandWeapons[m_LeftEquiped]->Fire();
}

void Player::RightHandShoot()
{
	m_RightHandWeapons[m_RightEquiped]->Fire();
}

bool Weapon::Fire()
{
	return false;
}
