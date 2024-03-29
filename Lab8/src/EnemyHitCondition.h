#pragma once
#ifndef __ENEMY_HIT_CONDITION__
#define __ENEMY_HIT_CONDITION__ 

#include "ConditionNode.h"

class EnemyHitCondition : public ConditionNode
{
public:
	EnemyHitCondition(Agent* agent = nullptr, bool hit = false);
	virtual ~EnemyHitCondition();

	// Setter
	void SetIsHit(bool state); 

	virtual bool Condition() override;
private:
	bool m_isHit{};  
};
#endif /* defined (__ENEMY_HIT_CONDITION__) */