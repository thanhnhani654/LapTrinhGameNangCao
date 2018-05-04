#include "Game.h"
#include <sstream>
#include <iostream>

Sprite Game::sprites;

bool Game::GameInit(HWND hwnd)
{

	//Initialize Game Device
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3ddv);

	d3ddv->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &back_buffer);

	if (d3ddv == NULL)
	{
		MessageBox(NULL, TEXT("Game Init False!"), TEXT("ERROR!"), MB_OK);
		return false;
	}

	d3ddv->CreateOffscreenPlainSurface(100, 100, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &surface, NULL);

	//////////////////////////////////////////////////////////////////////////
	////////////////Initialize		Client		Sever/////////////////////////
	//////////////////////////////////////////////////////////////////////////

	//client.ClientStartUp();
	//client.

	//////////////////////////////////////////////////////////////////////////
	////////////////Initialize		Game		Elementals////////////////////
	//////////////////////////////////////////////////////////////////////////
	sprites.GetDevice(d3ddv);
	gameScene.Init();
	updater = new Updater(&gameScene);

	return true;
}

bool Game::GameRun(float deltatime)
{
	d3ddv->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	
	//Update Keyboard and Mouse First
	gameScene.UpdateInput();

	//Update all other stat 
	gameScene.Update(deltatime);

	if (d3ddv->BeginScene())
	{

		d3ddv->ColorFill(surface, NULL, D3DCOLOR_XRGB(255, 0, 0));
		
		//Draw game
		gameScene.Draw();

		d3ddv->EndScene();
	}

	d3ddv->Present(NULL, NULL, NULL, NULL);

	return true;
}

bool Game::GameRelease()
{
	if (d3ddv != NULL)
		d3ddv->Release();
	if (d3d != NULL)
		d3d->Release();
	return true;
}
