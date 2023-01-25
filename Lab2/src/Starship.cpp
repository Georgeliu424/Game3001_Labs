#include "Starship.h"

#include "Game.h"
#include "TextureManager.h"
#include "Util.h"

Starship::Starship()
{
	TextureManager::Instance().Load("../Assets/textures/ncl.png", "starship");

	const auto size = TextureManager::Instance().GetTextureSize("starship");
	SetWidth(static_cast<int>(size.x));
	SetHeight(static_cast<int>(size.y));
	GetTransform()->position = glm::vec2(0.0f, 0.0f);
	GetRigidBody()->bounds = glm::vec2(GetWidth(),GetHeight());
	GetRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	GetRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	GetRigidBody()->isColliding = false;
	SetType(GameObjectType::AGENT);

	m_maxSpeed = 50.0f;//a max number of pixels moved per frames
	m_turnRate = 5.0f; //a max degree of pixels to turn each time-step
	m_accelerationRate = 4.0f;//a max number of pixels to add to the velocity each frame

	SetCurrentDirection((glm::vec2(1.0f, 0.0f))); //facing right 

}

Starship::~Starship() = default;

void Starship::Draw()
{
	TextureManager::Instance().Draw("starship",
		GetTransform()->position, static_cast<double>(GetCurrentHeading()), 255, true);
}

void Starship::Update()
{
	m_move();
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

void Starship::SetMaxSpeed(float speed)
{
	m_maxSpeed = speed;
}

void Starship::SetTurnRate(float angle) 
{
	m_turnRate = angle;
}

void Starship::SetAccelerationRate(float rate)
{
	m_accelerationRate = rate;
}

void Starship::SetDesiredVelocity(glm::vec2 target_postion)
{
	SetTargetPosition(target_postion);
	m_desiredVelocity = Util::Normalize(target_postion - GetTransform()->position) ;
	GetRigidBody()->velocity = m_desiredVelocity - GetRigidBody()->velocity;

}

void Starship::seek()
{
	SetDesiredVelocity(GetTargetPosition());
	const glm::vec2 steering_direction = GetDesiredVelocity() - GetCurrentDirection();

	LookwhereYouregoing(steering_direction);

	GetRigidBody()->acceleration = GetCurrentDirection() * GetAccelerationRate();
} 

void Starship::LookwhereYouregoing(glm::vec2 target_direction)
{
	const float target_rotation = Util::SignedAngle(GetCurrentDirection(), target_direction);
	const float turn_sensitivity = 5.0f;
	if (abs(target_rotation)>turn_sensitivity)
	{
		if (target_rotation>0.0f)
		{
			// turn right 
			SetCurrentHeading(GetCurrentHeading()+ GetTurnRate()); 
		}
		else if (target_rotation < 0.0f)
		{
			// turn left 
			SetCurrentHeading(GetCurrentHeading() - GetTurnRate());
		}
	}
}


void Starship::m_move()
{
	seek();
	//                    final pos  position term    velocity         acceleration term                       
	//kinematic equation-> pf      = pi +             vi * (time) + (0.5)* ai * (time *time)
	

	const float dt = Game::Instance().GetDeltaTime();

	const glm::vec2 initial_position = GetTransform()->position;

	const glm::vec2 velocity_term = GetRigidBody()->velocity * dt;

	const glm::vec2 acceleration_term = GetRigidBody()->acceleration * 0.5f;


	glm::vec2 final_position = initial_position + velocity_term + acceleration_term;

	GetTransform()->position = final_position;

	GetRigidBody()->velocity *= GetMaxSpeed();

	GetRigidBody()->velocity += GetRigidBody()->acceleration;


	GetRigidBody()->velocity = Util::Clamp(GetRigidBody()->velocity, GetMaxSpeed());

}
