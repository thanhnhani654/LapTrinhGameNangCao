#pragma once
#include "Client\Client.h"
#include <d3d9.h>
#include <d3dx9.h>
#include "GameScene.h"
#include "Sprite.h"
#include "Client\Updater.h"
#include <mutex>
#include "Collision\CollisionManager.h"
#include "LoadingScreen.h"

class  Game
{
private:
	LPDIRECT3D9 d3d = nullptr;
	LPDIRECT3DDEVICE9 d3ddv;
	LPDIRECT3DSURFACE9 surface = NULL;
	LPDIRECT3DSURFACE9 back_buffer = NULL;
	Updater* updater;

	//Client client;

public:
	GameScene gameScene;

	bool GameInit(HWND hwnd);
	bool GameRun(float deltatime);
	bool GameRelease();
	bool debugDraw;

	static Sprite sprites;
	std::mutex* locker;

	void getLocker(std::mutex* locker);

};

#pragma once
