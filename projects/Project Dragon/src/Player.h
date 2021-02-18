#pragma once
#include <RenderingManager.h>
#include <Timer.h>
#include <InstantiatingSystem.h>
//base weapon class other weapons inherit from this
class Weapon
{
public:
	void Update() { m_Timer += Timer::dt; if (m_Timer > m_FireDelay) CanShoot = true; else CanShoot = false; };
	virtual bool Fire();
	bool m_FireDelay;
	float m_Timer;
	float m_Damage;
	bool CanShoot;
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
	bool Fire() override;
};
class Player
{
public:
	void InitWeapons();
	int m_Hp;
	std::vector<Weapon*> m_LeftHandWeapons;
	std::vector<Weapon*> m_RightHandWeapons;
	bool m_LeftEquiped;
	bool m_RightEquiped;

};
