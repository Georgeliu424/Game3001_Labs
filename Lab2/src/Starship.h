#pragma once
#ifndef __STARSHIP__
#define __STARSHIP__

#include "Agent.h"
class Starship : public Agent
{
public:

	Starship();

	~Starship() ;


	//LifeCycle Functions
	void Draw() override;
	void Update() override;
	void Clean() override;


private:





};

#endif

