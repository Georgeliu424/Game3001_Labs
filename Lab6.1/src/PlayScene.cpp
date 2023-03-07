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

	ImGui::Separator();

	// Target Properties
	
	if (ImGui::SliderInt2("Target Position", goal_position, 0,800))
	{
		m_pTarget->GetTransform()->position.x = static_cast<float>(goal_position[0]);
		m_pTarget->GetTransform()->position.y = static_cast<float>(goal_position[1]);
	}

	ImGui::End();
}

void PlayScene::m_buildGrid()
{
	const auto tile_size = Config::TILE_SIZE;

	// layout a grid of tiles
	for (int row = 0; row < Config::ROW_NUM; ++row)
	{
		for (int col = 0; col < Config::COL_NUM; ++col)
		{
			
		}
	}

}

void PlayScene::m_toggleGrid(bool state) const
{
}

void PlayScene::m_clearNodes()
{
}

void PlayScene::m_checkShipLOS(DisplayObject* target_object) const
{
}


void PlayScene::BuildObstaclePool()
{
	for (int i = 0 ; i < 3; ++i)
	{
		m_pObstacles.push_back(new Obstacle());
	}
}










