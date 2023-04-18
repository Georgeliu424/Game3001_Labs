#pragma once
#ifndef __ENEMY_HEALTH_CONDITION__
#define __ENEMY_HEALTH_CONDITION__

#include "ConditionNode.h"

class EnemyHealthCondition : public ConditionNode
{
public:
	EnemyHealthCondition(Agent * agent = nullptr, bool healthy = true);
	virtual ~EnemyHealthCondition();

	// Setter
	void SetHealth(bool state);

	virtual bool Condition() override;
private:
	bool m_isHealthy{}; 
};
#endif /* defined (__ENEMY_HEALTH_CONDITION__) */