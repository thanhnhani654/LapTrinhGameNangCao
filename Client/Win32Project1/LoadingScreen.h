#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "Sprite.h"
#include <fstream>


class LoadingScreen
{
private:
	static LoadingScreen* inst;
	RECT rect1;
	RECT rect2;
	RECT rect3;
	RECT rect4;

	bool enable1;
	bool enable2;
	bool enable3;
	bool enable4;
	bool bendGame;
	bool bWin;
	
public:
	static LoadingScreen* getInstance();

	void EnableRect1();
	void EnableRect2();
	void EnableRect3();
	void EnableRect4();
	void EnableEndGameScreen(bool win);
	void DisableAll();
	
	bool isEndGame();


	void DrawRect(LPDIRECT3DDEVICE9 id3ddv, LPDIRECT3DSURFACE9 backBuffer, LPDIRECT3DSURFACE9 sufface);

	void WinScreen(LPDIRECT3DDEVICE9 id3ddv, LPDIRECT3DSURFACE9 backBuffer, LPDIRECT3DSURFACE9 sufface);
	void LoseScreen(LPDIRECT3DDEVICE9 id3ddv, LPDIRECT3DSURFACE9 backBuffer, LPDIRECT3DSURFACE9 sufface);
	void DrawEndGameScreen(LPDIRECT3DDEVICE9 id3ddv, LPDIRECT3DSURFACE9 backBuffer, LPDIRECT3DSURFACE9 sufface);

	void createRect(RECT &rect, int x, int y, int size);
};

