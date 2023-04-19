#include "Target.h"
#include "TextureManager.h"
#include "Util.h"


Target::Target():m_maxSpeed(10.0f)
{
	TextureManager::Instance().Load("../Assets/textures/player_idle.png","player");

	const auto size = TextureManager::Instance().GetTextureSize("player");
	SetWidth(static_cast<int>(size.x));
	SetHeight(static_cast<int>(size.y));
	GetTransform()->position = glm::vec2(400.0f, 300.0f);
	GetRigidBody()->velocity = glm::vec2(0, 0);
	GetRigidBody()->isColliding = false;


	m_turnRate = 10.0f; // 5 degrees per frame
	m_maxSpeed = 5.0f;

	m_dbgMode = false;


	SetType(GameObjectType::TARGET);
}

Target::~Target()
= default;

void Target::Draw()
{
	// draw the target
	TextureManager::Instance().Draw("player", GetTransform()->position, 0, 255, true);
}

void Target::Update()
{
	Move();
	CheckBounds();
	m_Turn();
}

void Target::Clean()
{
}

void Target::turnRight()
{
	SetCurrentHeading(GetCurrentHeading() + m_turnRate);
	if (GetCurrentHeading()>=360) 
	{
		SetCurrentHeading(GetCurrentHeading() - 360.0f);
	}
}

void Target::turnLeft()
{
	SetCurrentHeading(GetCurrentHeading() - m_turnRate);
	if (GetCurrentHeading() < 0)
	{
		SetCurrentHeading(GetCurrentHeading() + 360.0f);
	}
}

void Target::moveForward()
{
	GetRigidBody()->velocity = GetCurrentDirection() * m_maxSpeed;
}

void Target::moveBack()
{
	GetRigidBody()->velocity = GetCurrentDirection() * -m_maxSpeed;
}

void Target::move()
{
	GetTransform()->position += GetRigidBody()->velocity;
	GetRigidBody()->velocity *= 0.9f;
}

float Target::getMaxSpeed() const
{
	return m_maxSpeed;
}

glm::vec2 Target::getOrientation() const
{
	return m_orientation;
}

float Target::getRotation()
{
	return m_rotationAngle;
}

void Target::setMaxSpeed(float newSpeed)
{
	m_maxSpeed = newSpeed;
}

void Target::flipDbg()
{
	m_dbgMode = !m_dbgMode;
}

void Target::setOrientation(glm::vec2 orientation)
{
	m_orientation = orientation;
}

void Target::setRotation(float angle)
{
	m_rotationAngle = angle;

	auto offset = -90.0f;
	auto angle_in_radians = (angle + offset) * Util::Deg2Rad;

	auto x = cos(angle_in_radians);
	auto y = sin(angle_in_radians);

	// convert the angle to a normalized vector and store it in Orientation
	setOrientation(glm::vec2(x, y));
}

void Target::setDestination(glm::vec2 destination)
{
	m_destination = destination;
}

void Target::setAnimating(bool state)
{
	m_isAnimating = state;
}

void Target::m_Turn()
{
	m_targetDirection = Target::m_destination - GetTransform()->position;

	m_targetDirection = Util::Normalize(m_targetDirection);
	//std::cout << m_targetDirection.x << m_targetDirection.y << std::endl;
	std::cout << m_rotationAngle << std::endl;
	auto targetRotation = (Util::SignedAngle(getOrientation(), m_targetDirection));
	auto turnSensitivity = 5.0f;
	if (abs(targetRotation) > turnSensitivity)
	{
		if (targetRotation > 0.0f)
		{
			setRotation(getRotation() + m_turnRate);
		}
		else if (targetRotation < 0.0f)
		{
			setRotation(getRotation() - m_turnRate);
		}
	}

}

void Target::Move()
{
	GetTransform()->position = GetTransform()->position + GetRigidBody()->velocity * 5.0f;
}

void Target::CheckBounds()
{
}

void Target::Reset()
{
}
