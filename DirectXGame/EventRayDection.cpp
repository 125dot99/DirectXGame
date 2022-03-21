#include "EventRayDection.h"

EventRayDection::EventRayDection()
{

}

void EventRayDection::Add(const Vector3& _checkPos, float radius)
{
	primitive::Sphere circle = { _checkPos, radius };
	vecCheckCircles.emplace_back(circle);
}

int EventRayDection::Update(GameObject* pObject, float angle, float rotationOffset)
{
	auto& pos1 = pObject->GetPosition();
	auto rot = Vector2::FromAngle(pObject->GetRotation().y + rotationOffset);
	for (int i = 0, end = (int)vecCheckCircles.size(); i < end; i++)
	{
		auto& pos2 = vecCheckCircles[i].center;
		Vector3 v = pos2 - pos1;
		if (v.Length() < vecCheckCircles[i].radius &&
			-Vector3(rot.y, 0, rot.x).Dot(v.Normalize()) > angle)
		{
			return i + 1;
		}
	}
	return 0;
}

Vector3 EventRayDection::GetEventPos(int index) const
{
	return index >= vecCheckCircles.size() ? Vector3::Zero() : vecCheckCircles[index].center;
}

