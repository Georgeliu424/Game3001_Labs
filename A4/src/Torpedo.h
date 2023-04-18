#pragma once
#ifndef __TORPEDO__
#define __TORPEDO__

#include "TorpedoAnimationState.h"
#include "Sprite.h"

class Torpedo : public Sprite
{
public:
    Torpedo(float speed = 0.0f);
    virtual ~Torpedo();

    // Life Cycle Methods
    virtual void Draw() override;
    virtual void Update() override;
    virtual void Clean() override;

    // setters
    void SetAnimationState(TorpedoAnimationState new_state);

protected:
    void BuildAnimations();

    TorpedoAnimationState m_currentAnimationState;
    float m_speed;
};

#endif /* defined (__TORPEDO__) */