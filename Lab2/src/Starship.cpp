#include "Starship.h"
#include "TextureManager.h"
Starship::Starship()
{
	TextureManager::Instance().Load("../Assets/textures/ncl.png", "starship");

	const auto size = TextureManager::Instance().GetTextureSize("starship");
	SetWidth(static_cast<int>(size.x));
	SetHeight(static_cast<int>(size.y));
	GetTransform()->position = glm::vec2(0.0f, 0.0f);
	GetRigidBody()->bounds = glm::vec2(GetWidth(),GetHeight());
	GetRigidBody()->velocity = glm::vec2(0, 0);
	GetRigidBody()->isColliding = false;

	SetType(GameObjectType::AGENT);

	m_maxSpeed = 50.0f;//a max number of pixels moved per frames
	m_turnRate = 5.0f; //a max degree of pixels to turn 
	m_accelerationRate = 4.0f;//a max number of pixels to add to the velocity each frame

	SetCurrentDirection((glm::vec2(1.0f, 0.0f))); //facing right 

}

Starship::~Starship() = default;

void Starship::Draw()
{
	TextureManager::Instance().Draw("starship", GetTransform()->position, 0, 255, true);
	GetTransform()->position, static_cast<double>(GetCurrentHeading(), 255, true);
}

void Starship::Update()
{
}

void Starship::Clean()
{
}

float Starship::GetMaxSpeed() const
{
	return m_maxSpeed;
}

float Starship::GetTurnRate() const
{
	return m_turnRate;
}

float Starship::GetAccelerationRate() const
{
	return m_accelerationRate;
}

glm::vec2 Starship::GetDesiredVelocity() const
{
	return m_desiredVelocity;
}

void Starship::SetMaxSoeed(float Speed)
{
	m_maxSpeed = Speed;
}

void Starship::SetTurnRate(float Angle)
{
	m_turnRate = Angle;
}

void Starship::SetAccelerationRate(float rate)
{
	m_accelerationRate = rate;
}

void Starship::SetDesiredVelocity(glm::vec2 target_postion)
{
}

void Starship::seek()
{
}

void Starship::Lookwhereyouregoing(glm::vec2 target_direction)
{
}

void Starship::m_move()
{
}
