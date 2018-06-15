//#define  WIN32_LEAN_AND_MEAN
#define _WINSOCKAPI_

#pragma once
#include <Windows.h>
#include "Server.h"
//#include "Client\Client.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include "Game.h"
#include "Controller.h"
#include <thread>
#include <mutex>
#pragma comment(lib,"ws2_32")

#define FRAME_LIMIT 60

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int mainFunc(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmnLine, int nCmdShow);
int serverFunc();

std::mutex locker;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmnLine, int nCmdShow)
{
	//lpstrToCChar(lpCmnLine,s)
	//mainFunc(hInstance, hPrevInstance, lpCmnLine, nCmdShow);
	std::thread gameThread(mainFunc, hInstance, hPrevInstance, lpCmnLine,  nCmdShow);
	//serverFunc();
	std::thread serverThread(serverFunc);
	
	if (gameThread.joinable())
	{
		gameThread.join();
	}
	if (serverThread.joinable())
	{
		serverThread.join();
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
	//NetWorkManage client;
	//client.Initialize();
	locker.lock();
	Server* server = Server::getInstance();
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
		MessageBox(NULL, TEXT("CANT RUN"), TEXT("MeTroid"), MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	hwnd = CreateWindow(//WS_EX_CLIENTEDGE,
		wc.lpszClassName,
		TEXT("Server"),
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

	/*if (!client.cClientStartUp())
	{
	MessageBox(NULL,TEXT("Failed at ClientStartUp!"),TEXT("Error"),NULL);
	}
	if (!client.cCreateSocket())
	{
	MessageBox(NULL, TEXT("Failed at CreateSocket!"), TEXT("Error"), NULL);
	}
	if (!client.cConnect())
	{
	MessageBox(NULL, TEXT("Failed at Connect!"), TEXT("Error"), NULL);
	}*/
	server->StartUp();
	server->prepareListenSocket();
	server->Bind();
	server->listenSocket();
	server->nonBlock();
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
	int done = 0;
	float fpscount = 0;
	int fps = 0;

	locker.lock();
	server->setUpCompleted = true;
	locker.unlock();

	game.getLocker(&locker);

	while (!done)
	{
		//MessageBox(NULL, L"1", L"A", NULL);
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
			//	MessageBox(NULL, L"E", L"A", NULL);
				done = 1;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		current_tick = GetTickCount();
		deltatime = current_tick - last_tick;
		last_tick = current_tick;
		//debug

		if (deltatime < 1000.0f / FRAME_LIMIT)
		{
			//Sleep((1000.0f / FRAME_LIMIT) - (deltatime));
		}
		fps++;
		fpscount += deltatime;
		static int del = 0;
		if (fpscount > 1000.0f)
		{
			//GAMELOG("fps: %d			deltatime: %d", fps, deltatime);
			fps = 0;
			fpscount = 0;
		}

		del = GetTickCount();
		game.GameRun(deltatime);
		locker.lock();
		Server::getInstance()->AutoSend(deltatime);
		locker.unlock();
	}
	//MessageBox(NULL, L"3", L"A", NULL);
	game.GameRelease();

	return msg.wParam;
}

int serverFunc()
{
	locker.lock();
	Server* server = Server::getInstance();
	locker.unlock();
	float current_tick = GetTickCount();
	float deltatime = 0;
	float last_tick = 0;
	float timeDelay = 0;
	int testdebug = 0;
	while (TRUE)
	{

		current_tick = GetTickCount();
		deltatime = current_tick - last_tick;
		last_tick = current_tick;
		//debug

		if (deltatime < 1000.0f / 300)
		{
			Sleep((1000.0f / 300) - (deltatime));
		}

		locker.lock();
		if (!server->setUpCompleted)				//You shall not pass
		{
			locker.unlock();
			continue;
		}
		locker.unlock();

		Server::getInstance()->selectSocket();
		locker.lock();
		current_tick = GetTickCount();
		deltatime = current_tick - last_tick;

		last_tick = current_tick;
		Server::getInstance()->Recv();
		locker.unlock();
	}
	return 1;
}

