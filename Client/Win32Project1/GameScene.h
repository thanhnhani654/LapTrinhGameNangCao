#pragma once
#include "Client\Client.h"
#include "GameObject.h"
#include "GameObject\Tank.h"
#include "GameObject\Brick.h"
#include "GameObject\Steel.h"
#include "Player.h"
#include "Sprite.h"
#include <fstream>
#include <vector>

using namespace std;

class GameScene
{
protected:
	Player* player;

	vector <Object*> bricks;
	
public:
	void Init();
	void Update(float deltatime);
	void UpdateInput();
	void Draw();

	void LoadMapFromFile();

	Player* getPlayer();
};