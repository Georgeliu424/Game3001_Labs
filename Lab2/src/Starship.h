#pragma once
#ifndef __STARSHIP__
#define __STARSHIP__

#include "Agent.h"
class Starship : public Agent
{
public:

	Starship();

	~Starship() ;


	//LifeCycle Functions
	void Draw() override;
	void Update() override;
	void Clean() override;

	//getters and setters
	float GetMaxSpeed() const;
	float GetTurnRate() const;
	float GetAccelerationRate() const;
	glm::vec2 GetDesiredVelocity() const;

	void SetMaxSoeed(float Speed);
	void SetTurnRate(float Angle);
	void SetAccelerationRate(float rate);
	void SetDesiredVelocity(glm::vec2 target_postion);

	void seek();
	void Lookwhereyouregoing(glm::vec2 target_direction);


private:
	//private movement variable
	float m_maxSpeed;
	float m_turnRate;
	float m_accelerationRate;


	glm::vec2 m_desiredVelocity;

	void m_move();



};

#endif

