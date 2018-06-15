#pragma once
#include "Client\Client.h"
#include "GameObject\Tank.h"
#include "Controller.h"
#include "GameLog\GameLog.h"
#include <time.h>
#include "LoadingScreen.h"

class Player : protected Controller
{
private:
	Tank tank;
	bool tankFired;
	float tankFiredRunTime;
	
public:
	bool disable;
	uint8_t playerID;
	edirection moveEvent;
	void Initialize();
	void UpdateInput(float deltatime);
	void Update(float deltatime);
	void Draw();
	void OnKeyDown(int Keycode);
	void Move(float deltatime);
	void Move2(float deltatime);
	void Move3(float deltatime);
	void TankFire(float deltatime);
	Tank* getTank();

	bool bStartGame;
	bool StartGame();
};