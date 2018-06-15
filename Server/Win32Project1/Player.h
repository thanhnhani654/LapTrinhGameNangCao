#pragma once
#include "Server.h"
#include "GameObject\Tank.h"
#include "Controller.h"
#include "GameLog\GameLog.h"
#include <math.h>




class Player : protected Controller
{
private:
	Tank tank;
	static Player* inst;
	
public:
	bool disable;
	edirection moveEvent;
	void Initialize();
	void UpdateInput(float deltatime);
	void Update(float deltatime);
	void Draw();
	void OnKeyDown(int Keycode);
	void Move(float deltatime);
	void Move(edirection moveEvent);


	static Player* getInstance();

	Tank getTank();
	Tank* getTankPtr();
	edirection getDirection();
};