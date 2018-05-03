#pragma once
//#include "Client\Client.h"
#include "GameObject\Tank.h"
#include "Controller.h"
#include "GameLog\GameLog.h"


class Player : protected Controller
{
private:
	Tank tank;
	static Player* inst;
	edirection moveEvent;
public:
	
	void Initialize();
	void UpdateInput();
	void Update(float deltatime);
	void Draw();
	void OnKeyDown(int Keycode);
	void Move();
	void Move(edirection moveEvent);


	static Player* getInstance();

	Tank getTank();

};