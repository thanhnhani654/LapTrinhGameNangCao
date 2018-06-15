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
#include <time.h>
#include <mutex>

#define FRAME_LIMIT 60

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
int mainFunc(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmnLine, int nCmdShow);
int clientFunc();
//void lpstrToCChar(char* string, char* getchar);
static int stop = 0;

static std::mutex locker;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmnLine, int nCmdShow)
{
	/*locker.lock();
	NetWorkManage::getInstance()->szServer = lpCmnLine;
	locker.unlock();*/

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
	locker.lock();
	NetWorkManage* client = NetWorkManage::getInstance();
	client->Initialize();
	locker.unlock();

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
	wc.lpszClassName = TEXT("BattleCity");
	//wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClass(&wc))
	{
		MessageBox(NULL, TEXT("CANT RUN"), TEXT("BattleCity"), MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	hwnd = CreateWindow(//WS_EX_CLIENTEDGE,
		wc.lpszClassName,
		TEXT("Client"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		224,
		264,
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
	GAMELOG("StartUp Success");
	if (!client->cCreateSocket())
	{
		MessageBox(NULL, TEXT("Failed at CreateSocket!"), TEXT("Error"), NULL);
	}
	GAMELOG("CreteSocket Success");
	if (!client->cConnect())
	{
		MessageBox(NULL, TEXT("Failed at Connect!"), TEXT("Error"), NULL);
		exit(0);
	}
	GAMELOG("Connect Success");

	
	//client->nonBlock();

	//////////////////////////////////////////////////////////////

	float last_tick = 0;
	float current_tick = clock();
	float deltatime = 0;
	//int count_per_frame = 1000 / FRAME_LIMIT;

	Controller InitKeyBoard;
	InitKeyBoard._InitKeyboard(hwnd, hInstance);

	Game game;
	if (!game.GameInit(hwnd))
		return false;
	
	
	locker.lock();
	client->setUpCompleted = true;
	client->requestSyncTime();

	
	locker.unlock();
	int done = 0;
	float timeDelay = 0;
	float syncTimeInterval = 0;;
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
		syncTimeInterval += deltatime;
		if (syncTimeInterval > 1000)
		{
			syncTimeInterval = 0;
			client->requestSyncTime();
		}

		//locker.lock();
		static float tt = 0;

		static int co = 0;
		co++;
		tt += deltatime;


		if (tt > 1000)
		{
			//GAMELOG("ping %f", client->ping);
			GAMELOG("fps: %d", co);
			tt = 0;
			co = 0;
		}

		//Limit FPS
		current_tick = clock();
		deltatime = current_tick - last_tick;
		if (deltatime < 1000.0f / FRAME_LIMIT)
		{
			//Sleep((1000.0f / FRAME_LIMIT) - (deltatime));
		}
		last_tick = current_tick;
		
		game.GameRun(deltatime);
		
		
		// New Send
		timeDelay += deltatime;
		locker.lock();
		if (client->disconected)
			break;
		{
			client->SendData();
			timeDelay = 0;
		}
		locker.unlock();

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
			//locker.unlock();
			continue;
		}

		current_tick = GetTickCount();
		deltatime = current_tick - last_tick;
		/*if (deltatime < 1000.0f / FRAME_LIMIT)
		{
			Sleep((1000.0f / FRAME_LIMIT) - (deltatime));
		}*/
		last_tick = current_tick;

		timeDelay2 += deltatime;
		//GAMELOG("delta2 %f", deltatime);
		if (client->cRecv())
		{
			locker.lock();
			if (Updater::getInstance() != nullptr)
			{		
				Updater::getInstance()->analysis();
				
				//Updater::getInstance()->ChecknUpdate();
			}
			locker.unlock();
		}
		
		if (stop == 1)
			break;
		
	}

	return 1;
}


//void lpstrToCChar(char* string, char* getchar)
//{
//	int size = WideCharToMultiByte(CP_UTF8, 0, string, -1, NULL, 0, NULL, NULL);
//	getchar = new char[size];
//	WideCharToMultiByte(CP_UTF8, 0, string, -1, getchar, size, NULL, NULL);
//}