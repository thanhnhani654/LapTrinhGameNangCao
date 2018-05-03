#pragma once
#include "Client\Client.h"
#include "GameObject\Tank.h"
#include "Controller.h"
#include "GameLog\GameLog.h"


class Player : protected Controller
{
private:
	Tank tank;
public:
	void Initialize();
	void UpdateInput();
	void Update(float deltatime);
	void Draw();
	void OnKeyDown(int Keycode);
	void Move(float deltatime);
};