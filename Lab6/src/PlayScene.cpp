#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include <windows.h>
// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

PlayScene::PlayScene()
{
	PlayScene::Start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::Draw()
{
	DrawDisplayList();

	SDL_SetRenderDrawColor(Renderer::Instance().GetRenderer(), 255, 255, 255, 255);
}

void PlayScene::Update()
{
	UpdateDisplayList();
	m_checkAgentLOS(m_pStarShip, m_pTarget);
}

void PlayScene::Clean()
{
	RemoveAllChildren();
}

void PlayScene::HandleEvents()
{
	EventManager::Instance().Update();

	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_ESCAPE))
	{
		Game::Instance().Quit();
	}

	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_1))
	{
		Game::Instance().ChangeSceneState(SceneState::START);
	}

	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_2))
	{
		Game::Instance().ChangeSceneState(SceneState::END);
	}
}



void PlayScene::Start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";

	//Game Objects
	m_pStarShip = new StarShip();
	m_pStarShip->GetTransform()->position = glm::vec2(150.0f, 300.0f);
	AddChild(m_pStarShip);

	m_pTarget = new Target();
	m_pTarget->GetTransform()->position = glm::vec2(600.0f, 300.0f);
	AddChild(m_pTarget);

	//Add obstacle
	BuildObstaclePool();

	m_pObstacles[0]->GetTransform()->position = glm::vec2(380.0f, 80.0f);
	m_pObstacles[0]->SetHeight(50);
	AddChild(m_pObstacles[0]);

	m_pObstacles[1]->GetTransform()->position = glm::vec2(380.0f, 280.0f);
	m_pObstacles[1]->SetWidth(100);
	AddChild(m_pObstacles[1]);

	m_pObstacles[2]->GetTransform()->position = glm::vec2(380.0f, 480.0f);
	AddChild(m_pObstacles[2]);

	//setup the Grid
	m_isGridEnabled = false;
	m_buildGrid();
	m_toggleGrid(m_isGridEnabled);

	// Preload Sounds

	SoundManager::Instance().Load("../Assets/Audio/yay.ogg", "yay", SoundType::SOUND_SFX);
	SoundManager::Instance().Load("../Assets/Audio/thunder.ogg", "thunder", SoundType::SOUND_SFX);

	

	ImGuiWindowFrame::Instance().SetGuiFunction(std::bind(&PlayScene::GUI_Function, this));
}

void PlayScene::GUI_Function()
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("GAME3001 - W2023 - Lab 6.1", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar );

	ImGui::Separator();

	// Debug Properties
	if(ImGui::Checkbox("Toggle Grid", &m_isGridEnabled))
	{
		m_toggleGrid(m_isGridEnabled);

	}

	ImGui::Separator();

	//static position variables 
	static int start_position[2] = {
		static_cast<int>(m_pStarShip->GetTransform()->position.x),
		static_cast<int>(m_pStarShip->GetTransform()->position.y) };

	static int goal_position[2] = {
		static_cast<int>(m_pTarget->GetTransform()->position.x),
		static_cast<int>(m_pTarget->GetTransform()->position.y) };

	
	ImGui::Separator();

	// StarShip Properties
	
	if(ImGui::SliderInt2("Starship Position", start_position, 0, 800))
	{
		m_pStarShip->GetTransform()->position.x = static_cast<float>(start_position[0]);
		m_pStarShip->GetTransform()->position.y = static_cast<float>(start_position[1]);
	}

	// allow the ship to rotate
	static int angle;
	if (ImGui::SliderInt("Ship Direction",&angle,-360,360))
	{
		m_pStarShip->SetCurrentHeading(static_cast<float>(angle));
	}

	ImGui::Separator();

	// Target Properties
	
	if (ImGui::SliderInt2("Target Position", goal_position, 0,800))
	{
		m_pTarget->GetTransform()->position.x = static_cast<float>(goal_position[0]);
		m_pTarget->GetTransform()->position.y = static_cast<float>(goal_position[1]);
	}

	ImGui::Separator();
	// Add obstacle position control
	for (unsigned i = 0; i < m_pObstacles.size(); ++i)
	{
		int obstacle_Position[] = { static_cast<int>(m_pObstacles[i]->GetTransform()->position.x),
		static_cast<int>(m_pObstacles[i]->GetTransform()->position.y) };
		std::string label = "Obstacle" + std::to_string(i + 1) + "Position";
		if (ImGui::SliderInt2(label.c_str(),obstacle_Position,0,800))
		{
			m_pObstacles[i]->GetTransform()->position.x = static_cast<float>(obstacle_Position[0]);
			m_pObstacles[i]->GetTransform()->position.y = static_cast<float>(obstacle_Position[1]);
			m_buildGrid();
		}
	}
	ImGui::End();
}

void PlayScene::m_buildGrid()
{
	constexpr  auto tile_size = Config::TILE_SIZE;//Tile size alias
	constexpr auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);

	m_clearNodes(); //We will need to clear the nodes because we will rebuild our grid every time we move an obstacle

	// layout a grid of path_nodes
	for (int row = 0; row < Config::ROW_NUM; ++row)
	{
		for (int col = 0; col < Config::COL_NUM; ++col)
		{
			auto path_node = new PathNode();
			//node placement 
			path_node->GetTransform()->position = glm::vec2(static_cast<float>(col) * tile_size + offset.x,
				static_cast<float>(row) * tile_size + offset.y);

			//only show the grid where there are no obstacles
			bool keep_node = true;
			for (const auto obstacle : m_pObstacles)
			{
				//determine which path_node to keep
				if (CollisionManager::AABBCheck(path_node, obstacle))
				{
					keep_node = false;
				}
			}
			if (keep_node)
			{
				AddChild(path_node);
				m_pGrid.push_back(path_node);
			}
			else
			{
				delete path_node;
			}
		}
	}
	m_toggleGrid(m_isGridEnabled);

}

void PlayScene::m_toggleGrid(const bool state) const
{
	for (const auto path_node : m_pGrid)
	{
		path_node->SetVisible(state);
		
	}
}

void PlayScene::m_clearNodes()
{
	m_pGrid.clear();
	for (const auto display_object : GetDisplayList())
	{
		if (display_object->GetType() == GameObjectType::PATH_NODE)
		{
			RemoveChild(display_object);
		}
	}
}

void PlayScene::m_checkShipLOS(DisplayObject* target_object) const
{
	m_pStarShip->SetHasLOS(false);//default state - No LOS

	//if ship to target distance is less than or equal to the LOS distance, we're in the range
	const auto ship_to_range = Util::GetClosestEdge(m_pStarShip -> GetTransform() ->position, target_object);
	if (ship_to_range<=m_pStarShip->GetLOSDistance())
	{
		std::vector<DisplayObject*> contact_list;
		for (auto display_object : GetDisplayList())
		{
			if (display_object -> GetType() == GameObjectType::PATH_NODE){continue;} // ignore these
			if ((display_object->GetType() != m_pStarShip->GetType()) && 
				(display_object->GetType() != target_object->GetType())) 
			{
				// check if another display_object is closer to the startship than the target
				const auto ship_to_object_distance = Util::GetClosestEdge(m_pStarShip->GetTransform()->position, display_object);
				if (ship_to_object_distance <= ship_to_range ) 
				{
					contact_list.push_back(display_object);
				}
			}
		}
		const auto has_LOS = CollisionManager::LOSCheck(m_pStarShip,
			m_pStarShip->GetTransform()->position + m_pStarShip->GetCurrentDirection() * m_pStarShip->GetLOSDistance(),
			contact_list, target_object);
		m_pStarShip->SetHasLOS(has_LOS);
	}
}

bool PlayScene::m_checkAgentLOS(Agent* agent, DisplayObject* target_object) const
{
	bool has_LOS = false;//default state - No LOS
	agent->SetHasLOS(has_LOS);

	//if ship to target distance is less than or equal to the LOS distance, we're in the range
	const auto agent_to_range = Util::GetClosestEdge(m_pStarShip->GetTransform()->position, target_object);
	if (agent_to_range <= agent->GetLOSDistance())
	{
		//we are in within LOS Distance
		std::vector<DisplayObject*> contact_list;
		for (auto display_object : GetDisplayList())
		{
			const auto agent_to_object_distance = Util::GetClosestEdge(agent->GetTransform()->position, display_object);
			if(agent_to_object_distance>agent_to_range){continue;} // target is out of range
			if((display_object->GetType()!= GameObjectType::AGENT)
				&&(display_object->GetType() != GameObjectType::PATH_NODE)
				&& (display_object->GetType() != GameObjectType::TARGET))
			{
				contact_list.push_back(display_object);
			}
		}
		const glm::vec2 agent_LOS_endPoint = agent->GetTransform()->position + agent->GetCurrentDirection() * agent->GetLOSDistance();
		has_LOS = CollisionManager::LOSCheck(agent, agent_LOS_endPoint, contact_list,target_object);

		const auto LOSColour = (target_object->GetType() == GameObjectType::AGENT) ? glm::vec4(0, 0, 1, 1) : glm::vec4(0, 1, 0, 1);
		agent->SetHasLOS(has_LOS, LOSColour);
	}
	return has_LOS;
}

bool PlayScene::m_checkPathNodeLOS(PathNode* path_node, DisplayObject* target_object)
{
	return false;
}

void PlayScene::m_checkAllNodesWithTarget(DisplayObject* target_object)
{
}

void PlayScene::m_checkAllNodesWithBoth()
{
}

void PlayScene::m_setPathNodeLOSDistance(int distance)
{
}




void PlayScene::BuildObstaclePool()
{
	for (int i = 0 ; i < 3; ++i)
	{
		m_pObstacles.push_back(new Obstacle());
	}
}










