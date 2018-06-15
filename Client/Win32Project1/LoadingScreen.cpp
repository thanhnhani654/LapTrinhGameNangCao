#include "LoadingScreen.h"

LoadingScreen* LoadingScreen::inst;

LoadingScreen* LoadingScreen::getInstance()
{
	if (inst == nullptr)
	{
		inst = new LoadingScreen();
		inst->bendGame = false;
	}
	return inst;
}

void LoadingScreen::EnableRect1()
{
	enable1 = true;
}
void LoadingScreen::EnableRect2()
{
	enable2 = true;
}
void LoadingScreen::EnableRect3()
{
	enable3 = true;
}
void LoadingScreen::EnableRect4()
{
	enable4 = true;
}

void LoadingScreen::DisableAll()
{
	enable1 = false;
	enable2 = false;
	enable3 = false;
	enable4 = false;
}

void LoadingScreen::DrawRect(LPDIRECT3DDEVICE9 id3ddv, LPDIRECT3DSURFACE9 backBuffer, LPDIRECT3DSURFACE9 surface)
{
	id3ddv->ColorFill(surface, NULL, D3DCOLOR_XRGB(0, 255, 0));

	//RECT rect;
	if (!enable1)
		return;
	rect1.left = 20;
	rect1.top = 160;
	rect1.right = 60;
	rect1.bottom = 200;
	id3ddv->StretchRect(surface, NULL, backBuffer, &rect1, D3DTEXF_NONE);
	
	if (!enable2)
		return;
	rect2.left = 150;
	rect2.top = 160;
	rect2.right = 190;
	rect2.bottom = 200;
	id3ddv->StretchRect(surface, NULL, backBuffer, &rect2, D3DTEXF_NONE);

	if (!enable3)
		return;
	rect3.left = 20;
	rect3.top = 20;
	rect3.right = 60;
	rect3.bottom = 60;
	id3ddv->StretchRect(surface, NULL, backBuffer, &rect3, D3DTEXF_NONE);

	if (!enable4)
		return;
	rect4.left = 150;
	rect4.top = 20;
	rect4.right = 190;
	rect4.bottom = 60;
	id3ddv->StretchRect(surface, NULL, backBuffer, &rect4, D3DTEXF_NONE);

}


void LoadingScreen::WinScreen(LPDIRECT3DDEVICE9 id3ddv, LPDIRECT3DSURFACE9 backBuffer, LPDIRECT3DSURFACE9 surface)
{
	//Thong so ban dau
	int size = 10;
	id3ddv->ColorFill(surface, NULL, D3DCOLOR_XRGB(0, 255, 0));
	static bool initialize = false;
	static RECT rect[1000];
	static int len = 0;


	if (initialize)
	{
		for (int i = 0; i < len; i++)
			id3ddv->StretchRect(surface, NULL, backBuffer, &rect[i], D3DTEXF_NONE);
		return;
	}

	#pragma region LoadMap
	fstream map;
	map.open("Resources/winnermap.txt", ios::in);

	if (map.fail())
	{
		MessageBox(NULL, TEXT("Map not found!"), TEXT("ERROR"), NULL);
		return;
	}

	int width = 18;
	int height = 24;

	int maps[24][18];

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			map >> maps[y][x];
		}
	}

	map.close();
#pragma endregion
	
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			if (maps[y][x] == 1)
			{
				createRect(rect[len], x * size, y * size, size);
				len++;
			}
			if (len > 90)
				int a = 0;
		}
	}
	initialize = true;
}

void LoadingScreen::LoseScreen(LPDIRECT3DDEVICE9 id3ddv, LPDIRECT3DSURFACE9 backBuffer, LPDIRECT3DSURFACE9 surface)
{
	//Thong so ban dau
	int size = 10;
	id3ddv->ColorFill(surface, NULL, D3DCOLOR_XRGB(0, 255, 0));
	static bool initialize = false;
	static RECT rect[1000];
	static int len = 0;


	if (initialize)
	{
		for (int i = 0; i < len; i++)
			id3ddv->StretchRect(surface, NULL, backBuffer, &rect[i], D3DTEXF_NONE);
		return;
	}

#pragma region LoadMap
	fstream map;
	map.open("Resources/losermap.txt", ios::in);

	if (map.fail())
	{
		MessageBox(NULL, TEXT("Map not found!"), TEXT("ERROR"), NULL);
		return;
	}

	int width = 22;
	int height = 24;

	int maps[24][22];

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			map >> maps[y][x];
		}
	}

	map.close();
#pragma endregion

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			if (maps[y][x] == 1)
			{
				createRect(rect[len], x * size, y * size, size);
				len++;
			}
			if (len > 90)
				int a = 0;
		}
	}
	initialize = true;
}

void LoadingScreen::DrawEndGameScreen(LPDIRECT3DDEVICE9 id3ddv, LPDIRECT3DSURFACE9 backBuffer, LPDIRECT3DSURFACE9 surface)
{
	if (bendGame)
	{
		if (bWin)
		{
			WinScreen(id3ddv, backBuffer, surface);
		}
		else
		{
			LoseScreen(id3ddv, backBuffer, surface);
		}
	}
}

void LoadingScreen::createRect(RECT &rect, int x, int y,int size)
{
	rect.left = x;
	rect.top = y;
	rect.right = x + size;
	rect.bottom = y + size;
}

bool LoadingScreen::isEndGame()
{
	return bendGame;
}

void LoadingScreen::EnableEndGameScreen(bool win)
{
	bendGame = true;
	if (win)
		bWin = true;
	else
		bWin = false;
}