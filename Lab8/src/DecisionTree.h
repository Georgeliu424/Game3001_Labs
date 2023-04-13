#pragma once
#ifndef __DECISION_TREE__
#define __DECISION_TREE__

#include "Agent.h"
#include "CloseCombatCondition.h"
#include "LOSCondition.h"
#include "RadiusCondition.h"
#include "RangedCombatCondition.h"
#include "TreeNode.h"
#include "TreeNodeType.h"

#include "EnemyHealthCondition.h"
#include "EnemyHitCondition.h"
#include "PlayerDetectedCondition.h"

#include <vector>

class DecisionTree
{
public:
	// constructor(s)
	DecisionTree(); // default / empty constructor
	DecisionTree(Agent* agent);

	// destructor
	~DecisionTree();

	// getters and setters
	[[nodiscard]] Agent* GetAgent() const;
	void SetAgent(Agent* agent);

	// ConditionNode Getters
	[[nodiscard]] LOSCondition* GetLOSNode() const;
	[[nodiscard]] RadiusCondition* GetRadiusNode() const;
	[[nodiscard]] CloseCombatCondition* GetCloseCombatNode() const;
	[[nodiscard]] RangedCombatCondition* GetRangedCombatNode() const;

	//New for Lab 7.2
	std::vector<TreeNode*>& GetTree();
	void SetLOSNode(LOSCondition* node);
	void SetRadiusNode(RadiusCondition* node);
	void SetCloseCombatNode(CloseCombatCondition* node);
	void SetRangedCombatNode(RangedCombatCondition* node);

	//New for Lab 7.3
	[[nodiscard]] EnemyHealthCondition* GetEnemyHealthNode() const;
	[[nodiscard]] EnemyHitCondition* GetEnemyHitNode() const;
	[[nodiscard]] PlayerDetectedCondition* GetPlayerDetectedNode() const;

	void SetEnemyHealthNode(EnemyHealthCondition* node);
	void SetEnemyHitNode(EnemyHitCondition* node);
	void SetPlayerDetectedNode(PlayerDetectedCondition* node);

	// Public Utility / Convenience functions
	TreeNode* AddNode(TreeNode* parent, TreeNode* child_node, TreeNodeType type);

	// Life Cycle Functions
	void Display() const;
	void Update() const;
	void Clean();

	// Main Public Method
	void MakeDecision() const; // in-order traversal of the tree
private:
	// Private data members
	Agent* m_agent{};
	LOSCondition* m_LOSNode{};
	RadiusCondition* m_RadiusNode{};
	CloseCombatCondition* m_CloseCombatNode{};

	//New for Lab 7.2
	RangedCombatCondition* m_RangedCombatNode{};

	// New for Lab 7.3
	EnemyHealthCondition* m_EnemyHealthNode{};
	EnemyHitCondition* m_EnemyHitNode{};
	PlayerDetectedCondition* m_PlayerDetectedNode{};

	// Tree Node List
	std::vector<TreeNode*> m_treeNodeList;
};

#endif /* defined (__DECISION_TREE__) */