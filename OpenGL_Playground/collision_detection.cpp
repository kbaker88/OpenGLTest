#include "collision_detection.h"

glm::vec3 Collision_NormalizeVector(glm::vec3 Vector)
{
	return (Vector / glm::dot(Vector, Vector));
}

glm::vec3 Collision_GetPlaneNormal(glm::vec3 PointConnect,
	glm::vec3 PointIndex, glm::vec3 PointMiddle)
{
	glm::vec3 LineA = PointIndex - PointConnect;
	glm::vec3 LineB = PointMiddle - PointConnect;

	return glm::cross(LineA, LineB);
}

glm::vec3 Collision_FindClosestPlanePt(glm::vec3 CheckPoint,
	glm::vec3 PlanePt, glm::vec3 PlaneNormal)
{
	float d = glm::dot(PlaneNormal, PlanePt);
	float t = (glm::dot(PlaneNormal, CheckPoint) - d ) /
		dot(PlaneNormal, PlaneNormal);
	return CheckPoint - (t * PlaneNormal);
}

float Collision_DistancePtFromPlane(glm::vec3 ChoicePoint,
	glm::vec3 PlanePt, glm::vec3 PlaneNormal)
{
	float d = glm::dot(PlaneNormal, PlanePt);
	return (glm::dot(PlaneNormal, ChoicePoint) - d) / 
		dot(PlaneNormal, PlaneNormal);
}