#include "Kinematics.h"
#include "ECS.h"

void Kinematics::SetPosition()
{
	float parentX = ECS::GetComponent<PhysicsBody>(kinParent).GetPosition().x;
	float parentY = ECS::GetComponent<PhysicsBody>(kinParent).GetPosition().y;

	float childX = ECS::GetComponent<Transform>(kinChild).GetPosition().x;
	float childY = ECS::GetComponent<Transform>(kinChild).GetPosition().y;
	float childZ = ECS::GetComponent<Transform>(kinChild).GetPosition().z;



	ECS::GetComponent<Transform>(kinChild).SetPosition(vec3(parentX + offsetX, parentY + offsetY, childZ));
	std::cout << ECS::GetComponent<Transform>(kinChild).GetPosition().x << "|" << ECS::GetComponent<Transform>(kinChild).GetPosition().y << "|" << ECS::GetComponent<Transform>(kinChild).GetPosition().z << std::endl;
}

void Kinematics::SetParent(int parent)
{
	kinParent = parent;
}

void Kinematics::SetChild(int child)
{
	kinChild = child;
}

void Kinematics::SetOffset(float x, float y)
{
	offsetX = x;
	offsetY = y;
}
