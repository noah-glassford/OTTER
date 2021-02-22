#pragma once
#include <RenderingManager.h>
#include <Timer.h>
#include <InstantiatingSystem.h>
//base weapon class other weapons inherit from this
class Weapon
{
public:
	virtual void Update() { m_Timer += Timer::dt; if (m_Timer > m_FireDelay) m_CanShoot = true; else m_CanShoot = false; };
	virtual bool Fire();
	bool m_FireDelay;
	float m_Timer;
	float m_Damage;
	bool m_CanShoot;
};

class FireWeapon : public Weapon
{
public:
	bool Fire() override;
};

class WaterWeapon :public Weapon
{
public:
	bool Fire() override;
};

class AirWeapon : public Weapon
{
public:
	bool Fire() override;
};
class EarthWeapon : public Weapon
{
public:
	void Update() override;
	bool Fire() override;
};
class Player
{
public:
	void InitWeapons();
	void Update();
	void SwitchLeftHand();
	void SwitchRightHand();
	void LeftHandShoot();
	void RightHandShoot();
	int m_Hp;
	std::vector<Weapon*> m_LeftHandWeapons;
	std::vector<Weapon*> m_RightHandWeapons;
	bool m_LeftEquiped;
	bool m_RightEquiped;
	bool m_CanJump;

};
