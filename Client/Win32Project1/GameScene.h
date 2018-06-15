#pragma once
#include "Client\Client.h"
#include "GameObject.h"
#include "GameObject\Tank.h"
#include "GameObject\Brick.h"
#include "GameObject\Steel.h"
#include "GameObject\MapBounding.h"
#include "Player.h"
#include "Sprite.h"
#include <fstream>
#include <vector>
#include "Behavior\TankAI.h"

using namespace std;

class GameScene
{
protected:
	Player* player1;
	Player* player2;
	Player* player3;
	Player* player4;

	vector <Object*> bricks;
	MapBounding bound1;
	MapBounding bound2;
	MapBounding bound3;
	MapBounding bound4;

	Sprite loadingP1;
	Sprite loadingP2;
	Sprite loadingP3;
	Sprite loadingP4;
	bool bStartGame;
	
	TankAI AI1;
	TankAI AI2;

public:
	void Init();
	void Update(float deltatime);
	void UpdateInput(float deltatime);
	void Draw();

	void LoadMapFromFile();

	bool isStartGame();

	Player* getPlayer(int id);
};