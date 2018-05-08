//#define  WIN32_LEAN_AND_MEAN
#define _WINSOCKAPI_

#pragma once
#include <Windows.h>
#include "Client\Client.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include "Game.h"
#include "Controller.h"
#include "GameLog\GameLog.h"
#include "Client\Updater.h"
#include <thread>
#include <mutex>

#define FRAME_LIMIT 60

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
int mainFunc(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmnLine, int nCmdShow);
int clientFunc();
static int stop = 0;

static std::mutex locker;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmnLine, int nCmdShow)
{
	std::thread gameThread(mainFunc, hInstance, hPrevInstance, lpCmnLine, nCmdShow);
	std::thread clientThread(clientFunc);
	

	if (gameThread.joinable())
	{
		gameThread.join();
	}
	if (clientThread.joinable())
	{
		clientThread.join();
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

int mainFunc(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmnLine, int nCmdShow)
{
	WNDCLASS wc;
	HWND hwnd;
	MSG msg;
	NetWorkManage* client = NetWorkManage::getInstance();
	client->Initialize();

	//wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = TEXT("MeTroid");
	//wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);




	if (!RegisterClass(&wc))
	{
		MessageBox(NULL, TEXT("CANT RUN"), TEXT("MeTroid"), MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	hwnd = CreateWindow(//WS_EX_CLIENTEDGE,
		TEXT("MeTroid"),
		TEXT("Tittle"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		300,
		300,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	//////////////////////////////////////////////////////////////

	if (!client->cClientStartUp())
	{
		MessageBox(NULL, TEXT("Failed at ClientStartUp!"), TEXT("Error"), NULL);
	}
	if (!client->cCreateSocket())
	{
		MessageBox(NULL, TEXT("Failed at CreateSocket!"), TEXT("Error"), NULL);
	}
	if (!client->cConnect())
	{
		MessageBox(NULL, TEXT("Failed at Connect!"), TEXT("Error"), NULL);
	}
	//client->nonBlock();

	//////////////////////////////////////////////////////////////

	float last_tick = 0;
	float current_tick = GetTickCount();
	float deltatime = 0;
	//int count_per_frame = 1000 / FRAME_LIMIT;

	Controller InitKeyBoard;
	InitKeyBoard._InitKeyboard(hwnd, hInstance);

	Game game;
	if (!game.GameInit(hwnd))
		return false;
	client->setUpCompleted = true;
	int done = 0;
	float timeDelay = 0;
	game.getLocker(&locker);
	while (!done)
	{
		//Lau Lau co xuat hien loi o day nhung ko biet loi o dau
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				done = 1;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (!client->bsyncTimed)
			continue;

		//locker.lock();
		static float tt = 0;
		static int co = 0;
		co++;
		tt += deltatime;

		if (tt > 1000)
		{
			//GAMELOG("fps: %d", co);
			tt = 0;
			co = 0;
		}

		//Limit FPS
		current_tick = GetTickCount();
		deltatime = current_tick - last_tick;
		if (deltatime < 1000.0f / FRAME_LIMIT)
		{
			Sleep((1000.0f / FRAME_LIMIT) - (deltatime));
		}
		last_tick = current_tick;

		locker.lock();
		NetWorkManage::getInstance()->getStartUpdatetime();
		locker.unlock();

		game.GameRun(deltatime);
	}


	stop = 1;
	game.GameRelease();
	client->cClose();

	return msg.wParam;
}

int clientFunc()
{
	float current_tick = GetTickCount();
	float deltatime = 0;
	float last_tick = 0;
	float timeDelay = 0;
	float timeDelay2 = 0;
	NetWorkManage* client = NetWorkManage::getInstance();

	while (TRUE)
	{
		if (!client->setUpCompleted)
		{
			continue;
		}
		if (!client->bsyncTimed)
			client->SyncTime();

		current_tick = GetTickCount();
		deltatime = current_tick - last_tick;
		/*if (deltatime < 1000.0f / FRAME_LIMIT)
		{
			Sleep((1000.0f / FRAME_LIMIT) - (deltatime));
		}*/
		last_tick = current_tick;
		timeDelay += deltatime;
		timeDelay2 += deltatime;
		locker.lock();
		if (timeDelay > 30)
		{
			if (!client->SendData())
			{
				locker.unlock();
				timeDelay = 0;
				continue;
			}
			timeDelay = 0;
		}
		else
		{
			locker.unlock();
			continue;
		}

		locker.unlock();
		
		if (client->cRecv())
		{
			locker.lock();
			if (Updater::getInstance() != nullptr)
			{		
				if (timeDelay2 > 1000)
				{
					Updater::getInstance()->analysis();
					Updater::getInstance()->ChecknUpdate();
					timeDelay2 = 0;
				}
			}
			locker.unlock();
		}
		
		if (stop == 1)
			break;
		
	}

	return 1;
}