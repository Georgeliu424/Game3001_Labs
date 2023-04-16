#include"TorpedoF.h"
#include "TextureManager.h"

TorpedoF::TorpedoF(const float speed, glm::vec2 direction)
	: Torpedo(speed), m_direction(direction)
{
	TextureManager::Instance().LoadSpriteSheet(
		"../Assets/sprites/torpedo.txt",
		"../Assets/sprites/torpedo_k.png",
		"Torpedo");

	SetSpriteSheet(TextureManager::Instance().GetSpriteSheet("Torpedo"));

	// set frame width
	SetWidth(64);

	// set frame height
	SetHeight(64);

	GetTransform()->position = glm::vec2(400.0f, 300.0f);
	GetRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	GetRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	GetRigidBody()->isColliding = false;
	SetType(GameObjectType::PROJECTILE);

	m_direction = { direction.x * speed, direction.y * speed };

	BuildAnimations();
}

TorpedoF::~TorpedoF()
= default;

void TorpedoF::Draw()
{
	// draw the TorpedoF according to animation state
	switch (m_currentAnimationState)
	{
	case TorpedoAnimationState::FIRED:
		TextureManager::Instance().PlayAnimation("Torpedo", GetAnimation("fired"),
			GetTransform()->position, 5.0f, 0, 255, true);
		break;
	default:
		break;
	}
}

void TorpedoF::Update()
{
	GetTransform()->position += m_direction;
}

void TorpedoF::Clean()
{
}


void TorpedoF::BuildAnimations()
{
	auto fired_animation = Animation();

	fired_animation.name = "fired";
	fired_animation.frames.push_back(GetSpriteSheet()->GetFrame("fired1"));
	fired_animation.frames.push_back(GetSpriteSheet()->GetFrame("fired2"));
	fired_animation.frames.push_back(GetSpriteSheet()->GetFrame("fired3"));
	fired_animation.frames.push_back(GetSpriteSheet()->GetFrame("fired4"));
	fired_animation.frames.push_back(GetSpriteSheet()->GetFrame("fired5"));
	fired_animation.frames.push_back(GetSpriteSheet()->GetFrame("fired6"));
	fired_animation.frames.push_back(GetSpriteSheet()->GetFrame("fired7"));
	fired_animation.frames.push_back(GetSpriteSheet()->GetFrame("fired8"));

	SetAnimation(fired_animation);
}