#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include <windows.h>

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"
#include <fstream>

PlayScene::PlayScene()
{
	PlayScene::Start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::Draw()
{
	DrawDisplayList();

	if(m_isGridEnabled)
	{
		// Draws collision bounds for each obstacle 
		for (const auto obstacle : m_pObstacles)
		{
			Util::DrawRect(obstacle->GetTransform()->position - glm::vec2(obstacle->GetWidth() * 0.5f,
				obstacle->GetHeight() * 0.5f), obstacle->GetWidth(), obstacle->GetHeight());
		}

		//new for Lab 7.3 - Draw Detection radius
		auto detected = m_pStarship->GetTree()->GetPlayerDetectedNode()->GetDetected();
		Util::DrawCircle(m_pStarship->GetTransform()->position, 300.0f, detected ? glm::vec4(0, 1, 0, 1) : glm::vec4(1, 0, 0, 1));
	}

	SDL_SetRenderDrawColor(Renderer::Instance().GetRenderer(), 255, 255, 255, 255);
}

void PlayScene::Update()
{
	UpdateDisplayList();

	//Setup Ranged Combat Enemy
	m_pStarship->GetTree()->GetEnemyHealthNode()->SetHealth(m_pStarship->GetHealth() > 25);
	m_pStarship->GetTree()->GetEnemyHitNode()->SetIsHit(false);
	m_pStarship->CheckAgentLOSToTarget(m_pTarget, m_pObstacles);
	//Distance check between the Starship and Target for detection radius
	const float distance = Util::Distance(m_pStarship->GetTransform()->position, m_pTarget->GetTransform()->position);

	//Radius Detection...just outside of LOS Range (around 300px)
	m_pStarship->GetTree()->GetPlayerDetectedNode()->SetDetected(distance < 300);

	//Within LOS distance... but not close (optimum firing range)
	m_pStarship->GetTree()->GetRangedCombatNode()->SetIsWithinCombatRange(distance >= 200 && distance <= 350);

	////Close Range
	//m_pStarship->GetTree()->GetRangedCombatNode()->SetIsWithinCombatRange(distance >= 0 && distance <= 100);


	switch(m_LOSMode)
	{
	case LOSMode::TARGET:
		m_checkAllNodesWithTarget(m_pTarget);
		break;
	case LOSMode::SHIP:
		m_checkAllNodesWithTarget(m_pStarship);
		break;
	case LOSMode::BOTH:
		m_checkAllNodesWithBoth();
		break;
	}

	//collision check between TorpedoK and target
	for (const auto torpedo :m_pTorpedoesK )
	{
		CollisionManager::CircleAABBCheck(torpedo, m_pTarget);
	}
	
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
	if (EventManager::Instance().KeyPressed(SDL_SCANCODE_F))
	{//Set Spawn Point(front of our d7)

		// Torpedo will fire
		m_pTorpedoes.push_back(new Torpedo(5.0f)); // Instantiate a torpedo
		m_pTarget->SetCurrentDirection(glm::vec2(1.0f, 0.0f));
		m_pTorpedoes.back()->GetTransform()->position = m_pTarget->GetTransform()->position + m_pTarget->GetCurrentDirection() * 30.0f ; // Spawn point
		SoundManager::Instance().SetSoundVolume(50);
		SoundManager::Instance().PlaySoundFX("torpedo");
		AddChild(m_pTorpedoes.back(), 2);
	}
	if (EventManager::Instance().KeyPressed(SDL_SCANCODE_K))
	{
		m_pStarship->TakeDamage(10); // StarShip takes damage.
		m_pStarship->GetTree()->GetEnemyHitNode()->SetIsHit(true);
		std::cout << "Starship at" << m_pStarship->GetHealth() << " %." << std::endl;
	}
	if (EventManager::Instance().KeyPressed(SDL_SCANCODE_R))
	{
		m_pStarship->SetHealth(100); 
		m_pStarship->GetTree()->GetEnemyHitNode()->SetIsHit(false);
		m_pStarship->GetTree()->GetPlayerDetectedNode()->SetDetected(false);
		m_pStarship->GetTransform()->position = glm::vec2(40.0f, 40.0f);
		std::cout << "Conditions Reset" << std::endl;
	}
}

void PlayScene::Start()
{
	// Set GUI Title
	m_guiTitle = "Lab 8";

	// Setup a few more fields
	m_LOSMode = LOSMode::TARGET;
	m_pathNodeLOSDistance = 1000; // 1000px distance
	m_setPathNodeLOSDistance(m_pathNodeLOSDistance);

	// Add Game Objects
	m_pBackground = new Background();
	AddChild(m_pBackground, 0);

	m_pTarget = new Target();
	m_pTarget->GetTransform()->position = glm::vec2(500.0f, 300.0f);
	AddChild(m_pTarget, 2);

	//m_pStarship = new CloseCombatEnemy(this);
	m_pStarship = new RangedCombatEnemy(this);
	m_pStarship->GetTransform()->position = glm::vec2(400.0f, 40.0f);
	AddChild(m_pStarship, 2);

	// Add Obstacles
	BuildObstaclePool();

	// Setup the Grid
	m_isGridEnabled = false;
	m_buildGrid();
	m_toggleGrid(m_isGridEnabled);



	// Preload Sounds

	SoundManager::Instance().Load("../Assets/Audio/yay.ogg", "yay", SoundType::SOUND_SFX);
	SoundManager::Instance().Load("../Assets/Audio/thunder.ogg", "thunder", SoundType::SOUND_SFX);
	SoundManager::Instance().Load("../Assets/Audio/torpedo.ogg", "torpedo", SoundType::SOUND_SFX);
	SoundManager::Instance().Load("../Assets/Audio/torpedo_k.ogg", "torpedo_k", SoundType::SOUND_SFX);

	// Preload Music
	SoundManager::Instance().Load("../Assets/Audio/Mutara.mp3", "mutara", SoundType::SOUND_MUSIC);
	SoundManager::Instance().Load("../Assets/Audio/Klingon.mp3", "klingon", SoundType::SOUND_MUSIC);
	SoundManager::Instance().SetMusicVolume(16);

	// Play Music
	SoundManager::Instance().PlayMusic("klingon");

	ImGuiWindowFrame::Instance().SetGuiFunction(std::bind(&PlayScene::GUI_Function, this));
}

void PlayScene::SpawnEnemyTorpedo()
{
	//Set Spawn Point(front of our d7)
	glm::vec2 spawn_point = m_pStarship->GetTransform()->position + m_pStarship->GetCurrentDirection() * 30.0f;

	//Set the direction of the Torpedo(Normalized)
	glm::vec2 torpedo_direction = Util::Normalize(m_pTarget->GetTransform()->position - spawn_point);

	//Spawn the Torpedo
	m_pTorpedoesK.push_back(new TorpedoK(5.0f, torpedo_direction)); //instantias a torpedo and adds it to the vector
	m_pTorpedoesK.back()->GetTransform()->position = spawn_point; // Set the initial position of the torpedo
	SoundManager::Instance().SetSoundVolume(50);
	SoundManager::Instance().PlaySoundFX("torpedo_k"); //Play the Torpedo Sound
	AddChild(m_pTorpedoesK.back(), 2);
}

Target* PlayScene::GetTarget() const
{
	return m_pTarget;
}

void PlayScene::GUI_Function()
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin(m_guiTitle.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar );

	ImGui::Separator();

	// Debug Properties
	if(ImGui::Checkbox("Toggle Grid", &m_isGridEnabled))
	{
		m_toggleGrid(m_isGridEnabled);
	}

	ImGui::Separator();

	static int LOS_mode = static_cast<int>(m_LOSMode);
	ImGui::Text("Path Node LOS");
	ImGui::RadioButton("Target", &LOS_mode, static_cast<int>(LOSMode::TARGET)); ImGui::SameLine();
	ImGui::RadioButton("StarShip", &LOS_mode, static_cast<int>(LOSMode::SHIP)); ImGui::SameLine();
	ImGui::RadioButton("Both Target & StarShip", &LOS_mode, static_cast<int>(LOSMode::BOTH));

	m_LOSMode = static_cast<LOSMode>(LOS_mode);

	ImGui::Separator();

	if(ImGui::SliderInt("Path Node LOS Distance", &m_pathNodeLOSDistance, 0, 1000))
	{
		m_setPathNodeLOSDistance(m_pathNodeLOSDistance);
	}

	ImGui::Separator();

	// StarShip Properties
	static int shipPosition[] = { static_cast<int>(m_pStarship->GetTransform()->position.x),
		static_cast<int>(m_pStarship->GetTransform()->position.y) };
	if(ImGui::SliderInt2("Ship Position", shipPosition, 0, 800))
	{
		m_pStarship->GetTransform()->position.x = static_cast<float>(shipPosition[0]);
		m_pStarship->GetTransform()->position.y = static_cast<float>(shipPosition[1]);
	}

	// allow the ship to rotate
	static int angle;
	if(ImGui::SliderInt("Ship Direction", &angle, -360, 360))
	{
		m_pStarship->SetCurrentHeading(static_cast<float>(angle));
	}

	ImGui::Separator();

	// Target Properties
	static int targetPosition[] = { static_cast<int>(m_pTarget->GetTransform()->position.x),
		static_cast<int>(m_pTarget->GetTransform()->position.y) };
	if (ImGui::SliderInt2("Target Position", targetPosition, 0, 800))
	{
		m_pTarget->GetTransform()->position.x = static_cast<float>(targetPosition[0]);
		m_pTarget->GetTransform()->position.y = static_cast<float>(targetPosition[1]);
	}

	ImGui::Separator();

	// Add Obstacle position control for each obstacle
	for(unsigned i = 0; i < m_pObstacles.size(); ++i)
	{
		int obstaclePosition[] = { static_cast<int>(m_pObstacles[i]->GetTransform()->position.x),
		static_cast<int>(m_pObstacles[i]->GetTransform()->position.y) };
		std::string label = "Obstacle " + std::to_string(i + 1) + " Position";
		if(ImGui::SliderInt2(label.c_str(), obstaclePosition, 0, 800))
		{
			m_pObstacles[i]->GetTransform()->position.x = static_cast<float>(obstaclePosition[0]);
			m_pObstacles[i]->GetTransform()->position.y = static_cast<float>(obstaclePosition[1]);
			m_buildGrid();
		}
	}


	ImGui::End();
}

void PlayScene::BuildObstaclePool()
{
	std::ifstream inFile("../Assets/data/obstacles.txt");

	while(!inFile.eof())
	{
		std::cout << "Obstacle " << std::endl;
		auto obstacle = new Obstacle();
		float x, y, w, h; // declare variables the same as how the file is organized
		inFile >> x >> y >> w >> h; // read data from file line by line
		obstacle->GetTransform()->position = glm::vec2(x, y);
		obstacle->SetWidth(static_cast<int>(w));
		obstacle->SetHeight(static_cast<int>(h));
		AddChild(obstacle, 0);
		m_pObstacles.push_back(obstacle);
	}
	inFile.close();
}

void PlayScene::m_buildGrid()
{
	constexpr auto tile_size = Config::TILE_SIZE;
	constexpr auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);

	m_clearNodes(); // we will need to clear nodes because we will rebuild/redraw the grid if we move an obstacle

	// lay out a grid of path_nodes
	for (int row = 0; row < Config::ROW_NUM; ++row)
	{
		for (int col = 0; col < Config::COL_NUM; ++col)
		{
			auto path_node = new PathNode();
			path_node->GetTransform()->position = glm::vec2(static_cast<float>(col) * tile_size + offset.x,
				static_cast<float>(row) * tile_size + offset.y);

			// only show grid where there are no obstacles
			bool keep_node = true;
			for (const auto obstacle : m_pObstacles)
			{
				// add the Obstacle Buffer TODO: this can be improved
				const auto buffer = new Obstacle();
				buffer->GetTransform()->position = obstacle->GetTransform()->position;
				buffer->SetWidth(obstacle->GetWidth() + 40);
				buffer->SetHeight(obstacle->GetHeight() + 40);

				// determine which path_nodes to keep
				if(CollisionManager::AABBCheck(path_node, buffer))
				{
					keep_node = false;
				}
			}
			if(keep_node)
			{
				AddChild(path_node);
				m_pGrid.push_back(path_node);
			}
			else
			{
				delete path_node;
			}
		}

		// if Grid is supposed to be hidden - make it so!
		m_toggleGrid(m_isGridEnabled);
	}
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
		if(display_object->GetType() == GameObjectType::PATH_NODE)
		{
			RemoveChild(display_object);
		}
	}
}

bool PlayScene::m_checkAgentLOS(Agent* agent, DisplayObject* target_object) const
{
	bool has_LOS = false; // default - No LOS
	agent->SetHasLOS(has_LOS);

	// if ship to target distance is less than or equal to the LOS Distance (Range)
	const auto agent_to_range = Util::GetClosestEdge(agent->GetTransform()->position, target_object);
	if (agent_to_range <= agent->GetLOSDistance())
	{
		// we are in within LOS Distance 
		std::vector<DisplayObject*> contact_list;
		for (auto display_object : GetDisplayList())
		{
			const auto agent_to_object_distance = Util::GetClosestEdge(agent->GetTransform()->position, display_object);
			if (agent_to_object_distance > agent_to_range) { continue;  } // target is out of range
			if((display_object->GetType() != GameObjectType::AGENT)
				&& (display_object->GetType() != GameObjectType::PATH_NODE)
				&& (display_object->GetType() != GameObjectType::TARGET))
			{
				contact_list.push_back(display_object);
			}
		}

		const glm::vec2 agent_LOS_endPoint = agent->GetTransform()->position + agent->GetCurrentDirection() * agent->GetLOSDistance();
		has_LOS = CollisionManager::LOSCheck(agent, agent_LOS_endPoint, contact_list, target_object);

		const auto LOSColour = (target_object->GetType() == GameObjectType::AGENT) ? glm::vec4(0, 0, 1, 1) : glm::vec4(0, 1, 0, 1);
		agent->SetHasLOS(has_LOS, LOSColour);
	}
	return has_LOS;
}

bool PlayScene::m_checkPathNodeLOS(PathNode* path_node, DisplayObject* target_object) const
{
	// check angle to the target so we can still use LOS Distance for path_nodes
	const auto target_direction = target_object->GetTransform()->position - path_node->GetTransform()->position;
	const auto normalized_direction = Util::Normalize(target_direction); // changes direction vector to a unit vector (magnitude of 1)
	path_node->SetCurrentDirection(normalized_direction);
	return m_checkAgentLOS(path_node, target_object);
}

void PlayScene::m_checkAllNodesWithTarget(DisplayObject* target_object) const
{
	for (const auto path_node : m_pGrid)
	{
		m_checkPathNodeLOS(path_node, target_object);
	}
}

void PlayScene::m_checkAllNodesWithBoth() const
{
	for (const auto path_node : m_pGrid)
	{
		const bool LOSWithStarShip = m_checkPathNodeLOS(path_node, m_pStarship);
		const bool LOSWithTarget = m_checkPathNodeLOS(path_node, m_pTarget);
		path_node->SetHasLOS(LOSWithStarShip && LOSWithTarget, glm::vec4(0, 1, 1, 1));
	}
}

void PlayScene::m_setPathNodeLOSDistance(const int distance) const
{
	for (const auto path_node : m_pGrid)
	{
		path_node->SetLOSDistance(static_cast<float>(distance));
	}
}
