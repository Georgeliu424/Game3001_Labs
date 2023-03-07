#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Target.h"
#include "StarShip.h"
#include "Obstacle.h"
#include "PathNode.h"

class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene() override;

	// Scene LifeCycle Functions
	virtual void Draw() override;
	virtual void Update() override;
	virtual void Clean() override;
	virtual void HandleEvents() override;
	virtual void Start() override;
private:
	// IMGUI Function
	void GUI_Function();
	std::string m_guiTitle;
	glm::vec2 m_mousePosition;

	// Game Objects for the Scene
	Target* m_pTarget;
	StarShip* m_pStarShip;

	// Debugging Variables
	bool m_bDebugView;
	bool m_isGridEnabled;
	
	// obstacle list
	std::vector<Obstacle*> m_pObstacles;
	void BuildObstaclePool();

	//Division scheme
	std::vector<PathNode*>m_pGrid;
	void m_buildGrid();
	void m_toggleGrid(bool state)const;
	void m_clearNodes();

	//LOS Functions
	void m_checkShipLOS(DisplayObject*target_object) const;

};

#endif /* defined (__PLAY_SCENE__) */