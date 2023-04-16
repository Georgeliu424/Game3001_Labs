#pragma once
#ifndef __TORPEDO_K__
#define __TORPEDO_K__

#include "TorpedoAnimationState.h"
#include "Torpedo.h"

class TorpedoK final : public Torpedo
{
public:
    TorpedoK(float speed = 0.0f, glm::vec2 direction = { 0, 0 });
    virtual ~TorpedoK();

    // Life Cycle Methods
    virtual void Draw() override;
    virtual void Update() override;
    virtual void Clean() override;

private:
    void BuildAnimations();

    glm::vec2 m_direction;


};

#endif /* defined (__TORPEDO_K__) */