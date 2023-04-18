#pragma once
#ifndef __DECISION_TREE__
#define  __DECISION_TREE__

#include "Agent.h"
#include "CloseCombatCondition.h"
#include "LOSCondition.h"
#include "RadiusCondition.h"
#include "TreeNode.h"
#include "TreeNodeType.h"

#include <vector>


class DecisionTree
{
public:
	// Constructors
	DecisionTree(); // Default Constructor
	DecisionTree(Agent* agent);

	// Destructor
	~DecisionTree();

	// Getters and Setters
	Agent* GetAgent() const;
	void SetAgent(Agent* agent);

	// Condition Node Setters
	LOSCondition* GetLOSNode() const;
	RadiusCondition* GetRadiusNode() const;
	CloseCombatCondition* GetCloseCombatNode() const;

	// Public Utility Methods
	TreeNode* AddNode(TreeNode* parent_node, TreeNode* child_node, TreeNodeType type);

	// Lifecycle Methods
	void Display() const;
	void Update();
	void Clean();

	// Main Public Method
	void MakeDecision() const; // in-order traversal

private:
	// Private Data Members
	Agent* m_agent{};
	LOSCondition* m_LOSNode{};
	RadiusCondition* m_RadiusNode{};
	CloseCombatCondition* m_CloseCombatNode{};

	// Tree Node List
	std::vector<TreeNode*> m_treeNodeList;

	// Private Methods
	void m_buildTree();
};

#endif
