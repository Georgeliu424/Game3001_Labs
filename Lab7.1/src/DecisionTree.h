#pragma once
#ifndef __DECISION_TREE__
#define __DECISION_TREE__

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
	//constructors
	DecisionTree();//default
	DecisionTree(Agent* agent);

	//destructor
	~DecisionTree();
	//getters and setters
	Agent* GetAgent()const;
	void SetAgent(Agent* agent);

	//conditionNote Getters
	LOSCondition* GetLOSNode()const;
	RadiusCondition* GetRadiusNode()const;
	CloseCombatCondition* GetCloseCombatNode()const;

	//public utility
	TreeNode* AddNode(TreeNode * parent,TreeNode * child_node, TreeNodeType type);

	//life cycle Functions
	void Display()const;
	void Update()const;
	void Clean();

	// Main Public Method
	void MakeDecision()const;
private:
	//private data members
	Agent* m_agent{};
	LOSCondition* m_LOSNode{};
	RadiusCondition* m_RadiusNode{};
	CloseCombatCondition* m_CloseCombatNode{};
	//Tree Node list
	std::vector<TreeNode*> m_treeNodeList;

	//private methods
	void m_buildTree();
};





#endif /*defined (__DECISION_TREE__)*/


