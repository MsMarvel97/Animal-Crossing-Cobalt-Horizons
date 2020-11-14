#include "ResizeTrigger.h"
#include "ECS.h"

void ResizeTrigger::OnEnter()
{
	Trigger::OnEnter();
	
	if (!triggered)
	{
		for (int i = 0; i < m_targetEntities.size(); i++)
		{
			float x;
			float y;
			const auto body = ECS::GetComponent<PhysicsBody>(m_targetEntities[i]).GetBodyType();

			if (body == BodyType::CIRCLE)
			{
				float scaledRadius = (ECS::GetComponent<Sprite>(m_targetEntities[i]).GetWidth() / 2.f) * scalar;
				x = ECS::GetComponent<Sprite>(m_targetEntities[i]).GetWidth() + scaledRadius;
				y = ECS::GetComponent<Sprite>(m_targetEntities[i]).GetHeight() + scaledRadius;
			}
			else
			{
				x = ECS::GetComponent<Sprite>(m_targetEntities[i]).GetWidth() * scalar;
				y = ECS::GetComponent<Sprite>(m_targetEntities[i]).GetHeight() * scalar;
			}

			std::string fileName = "BeachBall.png";

			ECS::GetComponent<PhysicsBody>(m_targetEntities[i]).ScaleBody(scalar, fixtures);
			ECS::GetComponent<Sprite>(m_targetEntities[i]).SetSizeScale(scalar);
			ECS::GetComponent<Sprite>(m_targetEntities[i]).LoadSprite(spriteScale, x, y);
			triggered = true;
		}
	}
}

void ResizeTrigger::OnExit()
{
	Trigger::OnExit();
}
