#pragma once
#include "Server.h"
#include "GameObject.h"
#include "GameObject\Tank.h"
#include "GameObject\Brick.h"
#include "GameObject\Steel.h"
#include "GameObject\MapBounding.h"
#include "Player.h"
#include "Sprite.h"
#include <fstream>
#include <vector>
#include "Client\Timer.h"
#include "Collision\Box2D.h"

using namespace std;

class GameScene
{
protected:
	Player player1;
	Player player2;
	Player player3;
	Player player4;

	vector <Object*> bricks;
	Timer time;

	MapBounding bound1;
	MapBounding bound2;
	MapBounding bound3;
	MapBounding bound4;
public:
	void Init();
	void Update(float deltatime);
	void UpdateInput(float deltatime);
	void Draw();

	void LoadMapFromFile();

	Player* getPlayer(int id);
	std::vector<Box2D*> collidSave;
};