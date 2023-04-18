#pragma once
#ifndef __PLAYER__
#define __PLAYER__

#include "PlayerAnimationState.h"
#include "Sprite.h"

class Player final : public Sprite
{
public:
	Player();
	~Player() override;

	// Life Cycle Methods
	virtual void Draw() override;
	virtual void Update() override;
	virtual void Clean() override;

	bool IsColliding=false;
	// setters
	void SetAnimationState(PlayerAnimationState new_state);
	void SetCurrentDirection(glm::vec2 new_direction);

	void SetCurrentHeading(float heading);
	float GetCurrentHeading() const;
	glm::vec2 GetCurrentDirection() const;
	glm::vec2 m_currentDirection;
	

private:
	void BuildAnimations();

	PlayerAnimationState m_currentAnimationState;
};

#endif /* defined (__PLAYER__) */