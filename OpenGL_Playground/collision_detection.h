#ifndef COLLISION_DETECTION
#define COLLISION_DETECTION

#include <glm/glm.hpp>

glm::vec3 Collision_NormalizeVector(glm::vec3 Vector);
glm::vec3 Collision_GetPlaneNormal(glm::vec3 PointA,
	glm::vec3 PointB, glm::vec3 PointC);

glm::vec3 Collision_FindClosestPlanePt(glm::vec3 Point,
	glm::vec3 PlanePt, glm::vec3 PlaneNormal);

float Collision_DistancePtFromPlane(glm::vec3 ChoicePoint,
	glm::vec3 PlanePt, glm::vec3 PlaneNormal);

#endif
