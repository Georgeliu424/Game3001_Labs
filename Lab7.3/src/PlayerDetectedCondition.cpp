#include "PlayerDetectedCondition.h"

PlayerDetectedCondition::PlayerDetectedCondition(Agent * agent,const bool detected)
	:ConditionNode(agent)
{
	m_name = "Player Detected Condition";
	SetDetected(detected);
}

PlayerDetectedCondition::~PlayerDetectedCondition()
= default;

bool PlayerDetectedCondition::GetDetected() const
{
	return m_isDetected;
}

void PlayerDetectedCondition::SetDetected(const bool state)
{
	m_isDetected = state;
}

bool PlayerDetectedCondition::Condition()
{
	return m_isDetected;
}
