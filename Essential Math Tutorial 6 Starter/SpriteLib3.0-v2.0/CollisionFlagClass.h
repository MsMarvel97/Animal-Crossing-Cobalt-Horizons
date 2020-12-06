#pragma once
class CollisionFlagClass
{
public:
	void SetCollisionBlend(bool check);//setter
	bool GetCollisionBlend();

	void SetCollisionMilk(bool check);//setter
	bool GetCollisionMilk();

	void SetCollisionSugar(bool check);//setter
	bool GetCollisionSugar();

	void SetCollisionCups(bool check);//setter
	bool GetCollisionCups();

	void SetCollisionRegister(bool check);//setter
	bool GetCollisionRegister();

protected:
	bool m_collisionCheckBlend = false;
	bool m_collisionCheckMilk = false;
	bool m_collisionCheckSugar = false;
	bool m_collisionCheckCups = false;
	bool m_collisionCheckRegister = false;
};

