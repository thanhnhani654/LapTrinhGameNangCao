#include "Game.h"
#include <sstream>
#include <iostream>



Sprite Game::sprites;

bool Game::GameInit(HWND hwnd)
{

	//Initialize Game Device
	debugDraw = false;
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
	

	return true;
}

bool Game::GameRun(float deltatime)
{
	//debug define
	debugDraw = false;
	//debugDraw = true;

	if (Server::getInstance()->playerCount == 1)
		gameScene.getPlayer(1)->disable = false;
	if (Server::getInstance()->playerCount == 2)
		gameScene.getPlayer(2)->disable = false;
	if (Server::getInstance()->playerCount == 3)
		gameScene.getPlayer(3)->disable = false;
	if (Server::getInstance()->playerCount == 4)
		gameScene.getPlayer(4)->disable = false;

	d3ddv->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	
	

	//Update all other stat 
	//Roll Back System//
	//Store Snapshoot
	locker->lock();
	SnapShoot snapShoot;
	snapShoot.scene = gameScene;
	time.getCurrentTime();
	snapShoot.militime = time.getmilitime();
	orig.setState(snapShoot);
	caretaker.addMemento(orig.createMemento());	
	gameScene = caretaker.reUpdate();
	//if (gameScene.collidSave.size() != 0)
	//	Box2D::listBox = gameScene.collidSave;
	
	locker->unlock();
	//Update Keyboard and Mouse First
	gameScene.UpdateInput(deltatime);

	CollisionManager::getInstance()->doCollision(deltatime);

	gameScene.Update(deltatime);

	if (!Server::getInstance()->wrapDataAutoSend())
	{
		GAMELOG("Wrap Failed!!!");
	}
	


	if (d3ddv->BeginScene())
	{

		d3ddv->ColorFill(surface, NULL, D3DCOLOR_XRGB(255, 0, 0));
		
		//Draw game
		gameScene.Draw();

		#pragma region debugDraw
		if (debugDraw)
		{
			Box2D::debugDraw(d3ddv);
		}
		#pragma endregion


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

void Game::getLocker(std::mutex* locker)
{
	this->locker = locker;
}