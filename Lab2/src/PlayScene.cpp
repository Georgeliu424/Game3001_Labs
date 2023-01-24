#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"

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

	// handle player movement if no Game Controllers found
	
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
	m_bDebugView = false; // turn off debug collider
	//ADD Target
	m_pTarget = new Target();//instantiate an object of type target
	m_pTarget->GetTransform()->position = glm::vec2(100.0f, 400.0f);
	m_pStarship->SetTargetPosition(m_pTarget->GetTransform()->position);
	m_pStarship->SetCurrentDirection(glm::vec2(1.0f, 0.0f));
	//m_pStarship->SetEnabled(false);

	AddChild(m_pTarget);

	m_pStarship = new Starship;
	AddChild(m_pStarship);
	m_pStarship->GetTransform()->position = glm::vec2(400.0f, 300.0f);

	ImGuiWindowFrame::Instance().SetGuiFunction(std::bind(&PlayScene::GUI_Function, this));
}

void PlayScene::GUI_Function() const
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("Game 3001 Lab2", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar );

	ImGui::Separator();
	//Debug
	static bool toggleDebug = false;

	if (ImGui::Checkbox("Toggle Debug view", &toggleDebug))
	{
		m_bDebugView = toggleDebug;
	}


	//Target properties
	static float postion[2] = { m_pTarget->GetTransform()->position.x,
	m_pTarget->GetTransform()->position.y };
	if (ImGui::SliderFloat("Target Position",postion,0.0f,800.0f))
	{
		m_pTarget->GetTransform()->position = glm::vec2(postion[0], postion[1]);
	}

	ImGui::Separator();
	static bool toggleSeek = m_pStarship->IsEnabled();
	if (ImGui::Checkbox("Toggle seek",&toggleSeek))
	{
		m_pStarship->SetEnabled((toggleSeek));
	}

	
	ImGui::End();
}
