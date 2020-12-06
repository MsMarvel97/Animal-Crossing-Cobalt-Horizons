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
	case(8):
		ECS::GetComponent<DialogueClass>(m_targetEntities[0]).SetRollOne(true);
		break;
	case(9):
		ECS::GetComponent<DialogueClass>(m_targetEntities[0]).SetRollTwo(true);
		break;
	case(10):
		ECS::GetComponent<DialogueClass>(m_targetEntities[0]).SetRollThree(true);
		break;
	case(11):
		ECS::GetComponent<DialogueClass>(m_targetEntities[0]).SetInstrumentOne(true);
		break;
	case(12):
		ECS::GetComponent<DialogueClass>(m_targetEntities[0]).SetInstrumentTwo(true);
		break;
	case(13):
		ECS::GetComponent<DialogueClass>(m_targetEntities[0]).SetInstrumentThree(true);
		break;
	case(14):
		ECS::GetComponent<DialogueClass>(m_targetEntities[0]).SetCafe(true);
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
	case(8):
		ECS::GetComponent<DialogueClass>(m_targetEntities[0]).SetRollOne(false);
		break;
	case(9):
		ECS::GetComponent<DialogueClass>(m_targetEntities[0]).SetRollTwo(false);
		break;
	case(10):
		ECS::GetComponent<DialogueClass>(m_targetEntities[0]).SetRollThree(false);
		break;
	case(11):
		ECS::GetComponent<DialogueClass>(m_targetEntities[0]).SetInstrumentOne(false);
		break;
	case(12):
		ECS::GetComponent<DialogueClass>(m_targetEntities[0]).SetInstrumentTwo(false);
		break;
	case(13):
		ECS::GetComponent<DialogueClass>(m_targetEntities[0]).SetInstrumentThree(false);
		break;
	case(14):
		ECS::GetComponent<DialogueClass>(m_targetEntities[0]).SetCafe(false);
		break;
	}
}