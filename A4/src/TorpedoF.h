#pragma once
#ifndef __TORPEDOF_H__
#define __TORPEDOF_H__

#include "Torpedo.h"

class TorpedoF final:public Torpedo
{
public:
	TorpedoF(float speed =0.0f, glm::vec2 direction= { 0, 0 });
	~TorpedoF();
	virtual void Draw() override;
	virtual void Update() override;
	virtual void Clean() override;
private:
	void BuildAnimations();

	glm::vec2 m_direction;
};





#endif/* defined (__TORPEDOF_H__) */
