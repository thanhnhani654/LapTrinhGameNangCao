#pragma once
//#include "Client\Client.h"
#include "Server.h"
#include <d3d9.h>
#include <d3dx9.h>
#include "GameScene.h"
#include "Sprite.h"
#include "RollbackSys\CareTaker.h"
#include "Client\Timer.h"


class  Game
{
private:
	LPDIRECT3D9 d3d = nullptr;
	LPDIRECT3DDEVICE9 d3ddv;
	LPDIRECT3DSURFACE9 surface = NULL;
	LPDIRECT3DSURFACE9 back_buffer = NULL;

	Originator orig;
	CareTaker caretaker;
	Timer time;

public:
	GameScene gameScene;

	bool GameInit(HWND hwnd);
	bool GameRun(float deltatime);
	bool GameRelease();

	static Sprite sprites;

};

#pragma once
