#include "CoconutTrigger.h"
#include "ECS.h"

void CoconutTrigger::OnEnter()
{
	Trigger::OnEnter();
	
	for (int i = 0; i < m_targetEntities.size(); i++)
	{
		ECS::GetComponent<PlayerPoints>(MainEntities::MainPlayer()).SetStun(true);

		PhysicsBody::m_bodiesToDelete.push_back(m_targetEntities[i]);
		//if (PhysicsBody::m_bodiesToDelete.size() > 0)
		//{
		//	std::cout << "It works!";
		//}
		m_targetEntities.clear();
	}


}

void CoconutTrigger::OnExit()
{
	Trigger::OnExit();
}
