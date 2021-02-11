#include <Player.h>
#include <RenderingManager.h>
#include <Timer.h>
#include <InstantiatingSystem.h>
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

		const float* pSource = (const float*)glm::value_ptr(tempView);
		for (int i = 0; i < 16; ++i)
			dArray[i] = pSource[i];

		glm::vec3 lookDir;
		lookDir.x = -dArray[2];
		lookDir.y = -dArray[6];
		lookDir.z = -dArray[10];

		//defaulting to 0 for player
		btVector3 playerPosition = RenderingManager::activeScene->FindFirst("Camera").get<PhysicsBody>().GetBody()->getCenterOfMassTransform().getOrigin();
		
		std::cout << lookDir.x << " " << lookDir.y << " " << lookDir.z << " \n";
		lookDir *= range;
		btVector3 to = BtToGlm::GLMTOBTV3(lookDir);
		to += playerPosition;
		btCollisionWorld::ClosestRayResultCallback Results(playerPosition, to);

		PhysicsSystem::GetWorld()->rayTest(playerPosition, to, Results);

		//std::cout << "Raycast ended at point: " << to.getX() << ", " << to.getY() << ", " << to.getZ() << std::endl;

		

		

		if (Results.hasHit()  && Results.m_collisionObject->getUserIndex() == 5)
		{
			//std::cout << Results.m_collisionObject->getUserIndex2();
			//ECS::Get<Enemy>(Results.m_collisionObject->getUserIndex2()).m_hp = ECS::Get<Enemy>(Results.m_collisionObject->getUserIndex2()).m_hp - m_Damage;
			//ECS::Get<Transform>(2).SetPosition(BtToGlm::BTTOGLMV3(Results.m_collisionObject->getWorldTransform().getOrigin()) + BtToGlm::BTTOGLMV3(playerPosition) * 0.2f);
			std::cout << "Hit Something\n";
			InstantiatingSystem::LoadPrefabFromFile(glm::vec3(BtToGlm::BTTOGLMV3(Results.m_collisionObject->getWorldTransform().getOrigin())),"node/Water_Proj.node");
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
	to.setY(to.getY() - 2.f);
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
