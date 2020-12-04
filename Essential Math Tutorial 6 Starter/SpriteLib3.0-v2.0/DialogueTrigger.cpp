#include "DialogueTrigger.h"
#include "ECS.h"

void DialogueTrigger::OnEnter()
{
	Trigger::OnEnter();
	switch (diaNum)
	{
	case(0):
		ECS::GetComponent<DialogueClass>(m_targetEntities[0]).SetKainat(true);
		break;
	case(1):
		ECS::GetComponent<DialogueClass>(m_targetEntities[0]).SetKyra(true);
		break;
	case(2):
		ECS::GetComponent<DialogueClass>(m_targetEntities[0]).SetMithunan(true);
		break;
	case(3):
		ECS::GetComponent<DialogueClass>(m_targetEntities[0]).SetStevie(true);
		break;
	case(4):
		ECS::GetComponent<DialogueClass>(m_targetEntities[0]).SetWinston(true);
		break;
	case(5):
		ECS::GetComponent<DialogueClass>(m_targetEntities[0]).SetBookOne(true);
		break;
	case(6):
		ECS::GetComponent<DialogueClass>(m_targetEntities[0]).SetBookTwo(true);
		break;
	case(7):
		ECS::GetComponent<DialogueClass>(m_targetEntities[0]).SetBookThree(true);
		break;
	}
}

void DialogueTrigger::OnExit()
{
	Trigger::OnExit();
	switch (diaNum)
	{
	case(0):
		ECS::GetComponent<DialogueClass>(m_targetEntities[0]).SetKainat(false);
		break;
	case(1):
		ECS::GetComponent<DialogueClass>(m_targetEntities[0]).SetKyra(false);
		break;
	case(2):
		ECS::GetComponent<DialogueClass>(m_targetEntities[0]).SetMithunan(false);
		break;
	case(3):
		ECS::GetComponent<DialogueClass>(m_targetEntities[0]).SetStevie(false);
		break;
	case(4):
		ECS::GetComponent<DialogueClass>(m_targetEntities[0]).SetWinston(false);
		break;
	case(5):
		ECS::GetComponent<DialogueClass>(m_targetEntities[0]).SetBookOne(false);
		break;
	case(6):
		ECS::GetComponent<DialogueClass>(m_targetEntities[0]).SetBookTwo(false);
		break;
	case(7):
		ECS::GetComponent<DialogueClass>(m_targetEntities[0]).SetBookThree(false);
		break;
	}
}