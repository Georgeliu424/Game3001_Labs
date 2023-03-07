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
	AddChild(m_pStarShip);

	m_pTarget = new Target();
	AddChild(m_pTarget);


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
	
	ImGui::Begin("GAME3001 - W2023 - Lab5", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar );

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
		m_pStarShip->GetTransform()->position.x = start_position[0];
		m_pStarShip->GetTransform()->position.y = start_position[1];
	}

	ImGui::Separator();

	// Target Properties
	
	if (ImGui::SliderInt2("Target Position", goal_position, 0,800))
	{
		m_pTarget->GetTransform()->position.x = goal_position[0];
		m_pTarget->GetTransform()->position.y = goal_position[1];
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



void PlayScene::m_buildObstacles()
{
	for (int i=0;i<300;++i)
	{
		m_pObstacles.push_back(new Obstacle());
	}
}










