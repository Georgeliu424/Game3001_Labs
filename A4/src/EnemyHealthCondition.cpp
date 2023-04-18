#include "EnemyHealthCondition.h"

EnemyHealthCondition::EnemyHealthCondition(Agent* agent,const bool healthy)
	:ConditionNode(agent)
{
	m_name = "Enemy Health Condition";
	SetHealth(healthy); 
}

EnemyHealthCondition::~EnemyHealthCondition()
= default;

void EnemyHealthCondition::SetHealth(const bool state)
{
	m_isHealthy = state;
}

bool EnemyHealthCondition::Condition()
{
	return m_isHealthy;
}
