#pragma once
#include <vector>
#include "PhysicsBody.h"

class Trigger
{
public:
	//Depreciated use OnEnter and OnExit instead
	virtual void OnTrigger();

	virtual void OnEnter();
	virtual void OnExit();

	void SetTriggerEntity(int triggerEnt);
	int GetTriggerEntity();

	void ClearTargetEntities();

	void AddTargetMoveX(float x);
	void AddTargetMoveY(float y);

	void SetScalar(float x);
	void SetFixtures(int y);
	void SetSpriteScale(std::string sprite);

	void SetVectorMove(b2Vec2 move);
	void TriggeredBodies(PhysicsBody body);

	void SetRotationAngle(float r);

	void AddTargetEntity(int entity);
	void SetTargetEntities(std::vector<int> entities);
	void SetTargetTrigger(int trigger);
	void SetTrueTarget(int target);

	std::vector<int> GetTargetEntities();
	b2Vec2 movement = (b2Vec2(0.f, 0.f));


protected:
	int m_targetTrigger = 0;
	int m_triggerEntity;
	//target of trigger being targeted
	int m_trueTarget = 0;
	float scalar = 0;
	float fixtures = 0;
	float rotation = 0;
	std::string spriteScale;
	std::vector<int> m_targetEntities;
	std::vector<PhysicsBody> m_bodies;
	std::vector<int> m_targetX;
	std::vector<int> m_targetY;
};

