#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include <Windows.h>
#include "define.h"
#include "tinyxml\tinyxml.h"
#include <iostream>
#include "../Win32Project1/pugixml-1.8/src/pugixml.hpp"
#include <stdlib.h>
#include <vector>

using namespace std;

struct SpritePos
{
	int x;
	int y;
	int w;
	int h;
};

class Sprite
{
protected:
	static LPDIRECT3DTEXTURE9 _Image;
	static LPD3DXSPRITE _SpriteHandler;
	static LPDIRECT3DDEVICE9 d3ddv;

	

	int index;
	int begin;
	int end;
	static vector <SpritePos> spritepos;
	
	AnimationName currentAnim;
	
public:
	Sprite() {}
	void LoadSprite(LPCWSTR Path, int Width, int Height);
	void GetDevice(LPDIRECT3DDEVICE9 d3ddv);

	void Initialize();
	void Render(float X, float Y);
	void Next();

	void SetAnimation(AnimationName eanim);
	void SetAnimation(int i);

	static void LoadSpritePos();

};