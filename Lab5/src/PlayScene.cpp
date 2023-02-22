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

template <typename T>
void PlayScene::m_addObjectToGrid(T*& object, const int col, const int row, const TileStatus status)
{
	constexpr auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
	//added the T type object to the scene
	object = new T();//instantiate an object of type T 
	object->GetTransform()->position = m_getTile(col, row)->GetTransform()->position + offset;
	object->SetGridPosition(static_cast<float>(col), static_cast<float>(row));//Record grid space position
	m_getTile(col, row)->SetTileStatus(status);
	AddChild(object);
}
template<typename T>
void PlayScene::m_moveGameObject(T*& object, const int col, const int row, TileStatus status)
{
	constexpr auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);

	if (m_getTile(object->GetGridPosition())->GetTileStatus()!=TileStatus::IMPASSABLE)
	{
		m_getTile(object->GetGridPosition())->SetTileStatus(TileStatus::UNVISITED);
		m_updateTileMap(object->GetGridPosition(), TileStatus::UNVISITED);
	}
	object->GetTransform()->position = m_getTile(col, row)->GetTransform()->position + offset;
	object->SetGridPosition(static_cast<float>(col),static_cast<float>(row));

	if (m_getTile(object->GetGridPosition())->GetTileStatus() != TileStatus::IMPASSABLE)
	{
		m_getTile(object->GetGridPosition())->SetTileStatus(status);
		m_updateTileMap(col, row, status);
	}
	
}

void PlayScene::Start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";

	m_buildObstacles();
	m_buildGrid();
	m_currentHeuristic = Heuristic::MANHATTAN;

	m_initializeTileMap();
	m_buildTileMap();

	m_computeTileCosts();

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
	static bool toggle_grid = false;
	if(ImGui::Checkbox("Toggle Grid", &toggle_grid))
	{
		m_isGridEnabled = toggle_grid;
		m_setGridEnabled(m_isGridEnabled);
	}

	ImGui::Separator();

	static int radio = static_cast<int>(m_currentHeuristic);
	ImGui::Text("Heuristic Type");
	ImGui::RadioButton("Manhattan", &radio, static_cast<int>(Heuristic::MANHATTAN));
	ImGui::SameLine();
	ImGui::RadioButton("Euclidean", &radio, static_cast<int>(Heuristic::EUCLIDEAN));

	// check if  current heuristic is not the same as the selection
	if(m_currentHeuristic != static_cast<Heuristic>(radio))
	{
		m_currentHeuristic = static_cast<Heuristic>(radio);
		m_computeTileCosts();
	}

	ImGui::Separator();

	// StarShip Properties
	static int start_position[2] = {
		static_cast<int>(m_pStarShip->GetGridPosition().x),
		static_cast<int>(m_pStarShip->GetGridPosition().y) };
	if(ImGui::SliderInt2("Start Position", start_position, 0, Config::COL_NUM - 1))
	{
		// constrain the object within max rows
		if(start_position[1] > Config::ROW_NUM - 1)
		{
			start_position[1] = Config::ROW_NUM - 1;
		}
		m_moveGameObject(m_pStarShip, start_position[0], start_position[1], TileStatus::START);
	}

	ImGui::Separator();

	// Target Properties
	static int goal_position[2] = {
		static_cast<int>(m_pTarget->GetGridPosition().x),
		static_cast<int>(m_pTarget->GetGridPosition().y) };
	if (ImGui::SliderInt2("Goal Position", goal_position, 0, Config::COL_NUM - 1))
	{
		// constrain the object within max rows
		if (goal_position[1] > Config::ROW_NUM - 1)
		{
			goal_position[1] = Config::ROW_NUM - 1;
		}

		m_moveGameObject(m_pTarget, goal_position[0], goal_position[1], TileStatus::GOAL);
		m_computeTileCosts();
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
			Tile* tile = new Tile();
			// world space position
			tile->GetTransform()->position = glm::vec2(col * tile_size, row * tile_size);
			tile->SetGridPosition(col, row); // Grid Position
			tile->SetParent(this);
			tile->AddLabels();
			AddChild(tile);
			tile->SetEnabled(false);
			m_pGrid.push_back(tile);
		}
	}

	// setup the neighbour references for each tile in the grid
	// tiles = nodes in our graph

	for (int row = 0; row < Config::ROW_NUM; ++row)
	{
		for (int col = 0; col < Config::COL_NUM; ++col)
		{
			Tile* tile = m_getTile(col, row);

			// TopMost Neighbour
			if(row == 0)
			{
				tile->SetNeighbourTile(NeighbourTile::TOP_TILE, nullptr);
			}
			else
			{
				tile->SetNeighbourTile(NeighbourTile::TOP_TILE, m_getTile(col, row - 1));
			}

			// RightMost Neighbour
			if(col == Config::COL_NUM - 1)
			{
				tile->SetNeighbourTile(NeighbourTile::RIGHT_TILE, nullptr);
			}
			else
			{
				tile->SetNeighbourTile(NeighbourTile::RIGHT_TILE, m_getTile(col + 1,row));
			}

			// BottomMost Neighbour
			if(row == Config::ROW_NUM - 1)
			{
				tile->SetNeighbourTile(NeighbourTile::BOTTOM_TILE, nullptr);
			}
			else
			{
				tile->SetNeighbourTile(NeighbourTile::BOTTOM_TILE, m_getTile(col, row + 1));
			}

			// LeftMost Neighbour
			if (col == 0)
			{
				tile->SetNeighbourTile(NeighbourTile::LEFT_TILE, nullptr);
			}
			else
			{
				tile->SetNeighbourTile(NeighbourTile::LEFT_TILE, m_getTile(col - 1, row));
			}
		}
	}
}

bool PlayScene::m_getGridEnabled() const
{
	return m_isGridEnabled;
}

void PlayScene::m_setGridEnabled(const bool state)
{
	m_isGridEnabled = state;

	for (const auto tile : m_pGrid)
	{
		tile->SetEnabled(m_isGridEnabled); // enables each tile object
		tile->SetLabelsEnabled(m_isGridEnabled); // enables the corresponding labels
	}
}

void PlayScene::m_computeTileCosts()
{
	float distance = 0.0f;
	float dx = 0.0f;
	float dy = 0.0f;

	// for each tile in the grid, loop
	for (const auto tile : m_pGrid)
	{
		// compute the distance from each tile to the goal tile
		// distance (f) = tile cost (g) + heuristic estimate (h)
		switch(m_currentHeuristic)
		{
		case Heuristic::MANHATTAN:
			dx = abs(tile->GetGridPosition().x - m_pTarget->GetGridPosition().x);
			dy = abs(tile->GetGridPosition().y - m_pTarget->GetGridPosition().y);
			distance = dx + dy;
			break;
		case Heuristic::EUCLIDEAN:
			// calculates the euclidean distance ("as the crow flies") for each tile
			distance = Util::Distance(tile->GetGridPosition(), m_pTarget->GetGridPosition());
			break;
		}

		tile->SetTileCost(distance);
	}
}

void PlayScene::m_buildObstacles()
{
	for (int i=0;i<300;++i)
	{
		m_pObstacles.push_back(new Obstacle());
	}
}

void PlayScene::m_removeObstacleAt(const int col,const int row)
{
	for (const auto& obstacle : m_pObstacles)
	{
		if (obstacle!=nullptr)
		{
			if (static_cast<int>(obstacle->GetGridPosition().x)==col&&static_cast<int>(obstacle->GetGridPosition().y)==row)
			{
				RemoveChild(obstacle);
				m_pObstacles[(row * Config::COL_NUM) + col] = new Obstacle();
			}
		}
	}
}

void PlayScene::m_removeObstacleAt(const glm::vec2 grid_position)
{
	m_removeObstacleAt(static_cast<int>(grid_position.x), static_cast<int>(grid_position.y));
}

void PlayScene::m_removeAllObstacles()
{
	for (const auto& obstacle:m_pObstacles)
	{
		RemoveChild(obstacle);
	}
	m_pObstacles.clear();
}


void PlayScene::m_findShortestPath()
{
	
}

void PlayScene::m_displayPathList()
{
}

void PlayScene::m_resetPathFinding()
{
}

void PlayScene::m_resetSimulation()
{
}

Tile* PlayScene::m_getTile(const int col, const int row) const
{
	return m_pGrid[(row * Config::COL_NUM) + col];
}

Tile* PlayScene::m_getTile(const glm::vec2 grid_position) const
{
	const auto col = grid_position.x;
	const auto row = grid_position.y;

	return m_getTile(static_cast<int>(col), static_cast<int>(row));
}

void PlayScene::m_initializeTileMap()
{
	m_tileMap =  "---------I----------";
	m_tileMap += "---------I----------";
	m_tileMap += "---------I----------";
	m_tileMap += "--S------I----------";
	m_tileMap += "---------I----------";
	m_tileMap += "---------I----------";
	m_tileMap += "----I--------I------";
	m_tileMap += "----I--------I------";
	m_tileMap += "----I--------I------";
	m_tileMap += "----I--------I------";
	m_tileMap += "----I--------I------";
	m_tileMap += "----I--------I-G----";
	m_tileMap += "----I--------I------";
	m_tileMap += "----I--------I------";
	m_tileMap += "----I--------I------";
}

void PlayScene::m_buildTileMap()
{
	for (int row =0;row<Config::ROW_NUM;++row)
	{
		for (int col=0;col<Config::COL_NUM;++col)
		{
			if (m_tileMap[row*Config::COL_NUM+col]=='I')
			{
				m_addObjectToGrid(m_pObstacles[row * Config::COL_NUM + col], col, row, TileStatus::IMPASSABLE);
			}
			if (m_tileMap[row * Config::COL_NUM + col] == 'S')
			{
				m_addObjectToGrid(m_pStarShip, col, row, TileStatus::START);
			}
			if (m_tileMap[(row * Config::COL_NUM )+ col] == 'G')
			{
				m_addObjectToGrid(m_pTarget, col, row, TileStatus::GOAL);
			}
			if (m_tileMap[(row * Config::COL_NUM )+ col] == '-')
			{
				m_getTile(col, row)->SetTileStatus(TileStatus::IMPASSABLE);
			}
		}
	}
}

void PlayScene::m_updateTileMap(const int col,const int row,const TileStatus status)
{
	switch (status)
	{
	case TileStatus::UNVISITED:
		m_tileMap[(row * Config::COL_NUM) + col] = '-';
		break;
	case TileStatus::OPEN:
		m_tileMap[(row * Config::COL_NUM) + col] = 'O';
		break;
	case TileStatus::CLOSED:
		m_tileMap[(row * Config::COL_NUM) + col] = 'C';
		break;
	case TileStatus::GOAL:
		m_tileMap[(row * Config::COL_NUM) + col] = 'G';
		break;
	case TileStatus::START:
		m_tileMap[(row * Config::COL_NUM) + col] = 'S';
		break;
	case TileStatus::IMPASSABLE:
		m_tileMap[(row * Config::COL_NUM) + col] = 'I';
		break;
	case TileStatus::PATH:
		m_tileMap[(row * Config::COL_NUM) + col] = 'P';
		break;
	}
}

void PlayScene::m_updateTileMap(const glm::vec2 grid_position, const TileStatus status)
{
	m_updateTileMap(static_cast<int>(grid_position.x), static_cast<int>(grid_position.y), status);
}
