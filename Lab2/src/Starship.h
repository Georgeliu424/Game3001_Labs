#pragma once
#ifndef __STARSHIP__
#define __STARSHIP__

#include "DisplayObject.h"
class Starship : public DisplayObject
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

