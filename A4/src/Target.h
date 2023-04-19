#pragma once
#ifndef __TARGET__
#define __TARGET__

#include "Agent.h"

class Target final : public Agent {
public:
	Target();
	~Target() override;

	// Inherited via GameObject
	virtual void Draw() override;
	virtual void Update() override;
	virtual void Clean() override;

	void turnRight();
	void turnLeft();
	void moveForward();
	void moveBack();

	void move();

	float getMaxSpeed() const;
	glm::vec2 getOrientation() const;
	float getRotation();

	void setMaxSpeed(float newSpeed);
	void flipDbg();
	void setOrientation(glm::vec2 orientation);
	void setRotation(float angle);
	void setDestination(glm::vec2 destination);
	void setAnimating(bool state);

private:
	void Move();
	void CheckBounds();
	void Reset();

	glm::vec2 m_destination;
	glm::vec2 m_targetDirection;
	glm::vec2 m_orientation;

	float m_maxSpeed;
	float m_turnRate;
	float m_rotationAngle;

	bool m_dbgMode;
	bool m_isAnimating;

	int m_frameCounter;

	void m_Turn();
};


#endif /* defined (__TARGET__) */