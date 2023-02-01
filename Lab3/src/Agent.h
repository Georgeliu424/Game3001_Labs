#pragma once
#ifndef __AGENT__
#define __AGENT__

#include <glm/vec4.hpp>
#include "NavigationObject.h"

class Agent : public NavigationObject
{
public:
	Agent();
	~Agent();

	// Inherited via GameObject
	void Draw() override;
	void Update() override;
	void Clean() override;

	// getters
	[[nodiscard]] glm::vec2 GetTargetPosition() const;
	[[nodiscard]] glm::vec2 GetCurrentDirection() const;
	[[nodiscard]] float GetLOSDistance() const;
	[[nodiscard]] bool HasLOS() const;
	[[nodiscard]] float GetCurrentHeading() const;
	[[nodiscard]] glm::vec4 GetLOSColour() const;

	[[nodiscard]] glm::vec2 getLeftLOSEndPoint() const;
	[[nodiscard]] glm::vec2 getMiddleLOSEndPoint() const;
	[[nodiscard]] glm::vec2 getRightLOSEndPoint() const;
	bool* GetCollisionWhiskers();
	glm::vec4 GetLineColour(int index)const;
	float GetWhiskerAngle() const;




	// setters
	void SetTargetPosition(glm::vec2 new_position);
	void SetCurrentDirection(glm::vec2 new_direction);
	void SetLOSDistance(float distance);
	void SetHasLOS(bool state);
	void SetCurrentHeading(float heading);
	void SetLOSColour(glm::vec4 colour);

	void SetLeftLOSEndPoint(glm::vec2 point);
	void SetMiddleLOSEndPoint(glm::vec2 point);
	void SetRightLOSEndPoint(glm::vec2 point);
	void SetLineColour(int index, glm::vec4 colour);
	void SetWhiskerAngle(float Angle);

	void UpdateWhiskerAngle(float Angle);

private:
	void ChangeDirection();
	float m_currentHeading;
	glm::vec2 m_currentDirection;
	glm::vec2 m_targetPosition;

	// LOS
	float m_LOSDistance;
	bool m_hasLOS;
	glm::vec4 m_LOSColour;

	//Whisker
	glm::vec2 m_leftLOSEndPoint;
	glm::vec2 m_middleLOSEndPoint;
	glm::vec2 m_rightLOSEndPoint;
	glm::vec4 m_lineColour[3];
	bool m_collisionWhiskers[3];
	float m_WhiskerAngle;

};



#endif /* defined ( __AGENT__) */