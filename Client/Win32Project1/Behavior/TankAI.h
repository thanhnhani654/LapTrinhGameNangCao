#pragma once
#include "../GameObject/Tank.h"
#include <ctime>

class TankAI
{
private:
	Tank tank;
	float runTime;
	float fireTime;

public:
	edirection moveEvent;
	void Initialize(float x, float y);

	void AiMove(float deltatime);
	void AiFire(float deltatime);
	void randomRunTime();
	void randomEvent();
	void randomFireTime();

	void UpdateInput(float deltatime);
	void Update(float deltatime);
	void Draw();
};