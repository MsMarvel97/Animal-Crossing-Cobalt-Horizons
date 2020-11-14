#include "Trigger.h"

#include "ECS.h"

//depreciated
void Trigger::OnTrigger()
{
	//make the object linked transparent
	ECS::GetComponent<Sprite>(m_triggerEntity).SetTransparency(0.2f);
}

void Trigger::OnEnter()
{	//Tests the trigger working and displays using T
	ECS::GetComponent<PhysicsBody>(m_triggerEntity).SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
}

void Trigger::OnExit()
{
	//make the object linked opaque
	ECS::GetComponent<PhysicsBody>(m_triggerEntity).SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
}

void Trigger::SetTriggerEntity(int triggerEnt)
{
	m_triggerEntity = triggerEnt;
}

int Trigger::GetTriggerEntity()
{
	return m_triggerEntity;
}



void Trigger::AddTargetMoveX(float x)
{
	m_targetX.push_back(x);
}

void Trigger::AddTargetMoveY(float y)
{
	m_targetY.push_back(y);
}

void Trigger::SetScalar(float x)
{
	scalar = x;
}

void Trigger::SetFixtures(int y)
{
	fixtures = y;
}

void Trigger::SetSpriteScale(std::string sprite)
{
	spriteScale = sprite;
}

void Trigger::SetVectorMove(b2Vec2 move)
{
	movement = move;
}

void Trigger::TriggeredBodies(PhysicsBody body)
{
	m_bodies.push_back(body);
}

void Trigger::SetRotationAngle(float r)
{
	rotation = r;
}

void Trigger::AddTargetEntity(int entity)
{
	m_targetEntities.push_back(entity);
}

void Trigger::SetTargetEntities(std::vector<int> entities)
{
	m_targetEntities = entities;
}

std::vector<int> Trigger::GetTargetEntities()
{
	return m_targetEntities;
}
