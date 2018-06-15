#include "TankAI.h"

void TankAI::Initialize(float x, float y)
{
	tank.Initialize(eTankType::Tank_White_1);
	tank.SetPosition(x, y);
	srand(time(NULL));
}

void TankAI::UpdateInput(float deltatime)
{
	tank.UpdateInput(deltatime);
	//tank.GetMovement()->Move(moveEvent);
}

void TankAI::Update(float deltatime)
{
	/*AiMove(deltatime);
	AiFire(deltatime);*/
	tank.Update(deltatime);
}

void TankAI::Draw()
{
	if (tank.alreadyDie)
		return;
	tank.Draw();
}

void TankAI::AiMove(float deltatime)
{
	if (runTime > 0)
	{
		runTime -= deltatime;
		return;
	}

	randomRunTime();
	randomEvent();
}

void TankAI::AiFire(float deltatime)
{
	if (fireTime > 0)
	{
		fireTime -= deltatime;
		return;
	}
	if (!tank.fired)
	{
		randomFireTime();
		tank.Fire();
	}
}

void TankAI::randomFireTime()
{
	fireTime = rand() % (1000 - 200 + 1) + 200;
}

void TankAI::randomRunTime()
{
	runTime = rand() % (1000 - 200 + 1) + 200;
}

void TankAI::randomEvent()
{
	int temp = rand() % 5 + 1;

	switch (temp)
	{
	case 1:
		moveEvent = edirection::UP;
		break;
	case 2:
		moveEvent = edirection::DOWN;
		break;
	case 3:
		moveEvent = edirection::LEFT;
		break;
	case 4:
		moveEvent = edirection::RIGHT;
		break;
	case 5:
		moveEvent = edirection::NIL;
		break;
	default:
		break;
	}
}