#include "PosCheck.h"
#include "ECS.h"

void PosCheck::OnEnter()
{
	Trigger::OnEnter();
	switch (flag)
	{
	case(0):
		ECS::GetComponent<CollisionFlagClass>(m_targetEntities[0]).SetCollisionBlend(true);
		break;
	case(1):
		ECS::GetComponent<CollisionFlagClass>(m_targetEntities[0]).SetCollisionMilk(true);
		break;
	case(2):
		ECS::GetComponent<CollisionFlagClass>(m_targetEntities[0]).SetCollisionSugar(true);
		break;
	case(3):
		ECS::GetComponent<CollisionFlagClass>(m_targetEntities[0]).SetCollisionCups(true);
		break;
	case(4):
		ECS::GetComponent<CollisionFlagClass>(m_targetEntities[0]).SetCollisionRegister(true);
		break;
	}
}

void PosCheck::OnExit()
{
	Trigger::OnExit();
	switch (flag)
	{
	case(0):
		ECS::GetComponent<CollisionFlagClass>(m_targetEntities[0]).SetCollisionBlend(false);
		break;
	case(1):
		ECS::GetComponent<CollisionFlagClass>(m_targetEntities[0]).SetCollisionMilk(false);
		break;
	case(2):
		ECS::GetComponent<CollisionFlagClass>(m_targetEntities[0]).SetCollisionSugar(false);
		break;
	case(3):
		ECS::GetComponent<CollisionFlagClass>(m_targetEntities[0]).SetCollisionCups(false);
		break;
	case(4):
		ECS::GetComponent<CollisionFlagClass>(m_targetEntities[0]).SetCollisionRegister(false);
		break;
	}
}


