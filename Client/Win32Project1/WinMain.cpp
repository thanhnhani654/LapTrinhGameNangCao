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

#define FRAME_LIMIT 60

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmnLine, int nCmdShow)
{
	WNDCLASS wc;
	HWND hwnd;
	MSG msg;
	NetWorkManage client;
	client.Initialize();

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

	if (!client.cClientStartUp())
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
	}

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
	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				done = 1;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		current_tick = GetTickCount();
		deltatime = current_tick - last_tick;
		last_tick = current_tick;

		static float tt = 0;
		static int co = 0;
		co++;
		tt += deltatime;
		if (tt > 1000)
		{
			GAMELOG("fps: %d", co);
			tt = 0;
			co = 0;
		}

		//Limit FPS
		if (current_tick - last_tick < 1000 / FRAME_LIMIT)
		{
			Sleep(1000 / FRAME_LIMIT - (current_tick - last_tick));
		}

		NetWorkManage::getStartUpdatetime();
		game.GameRun(deltatime);
		int delay = 60;
		static int t = delay;
		if (t < 0)
		{
			//if (client.clientMode == eClientMode::Sending && client.sendMode == eSendMode::SendDataSizePack)
			client.WrapToSend();
			client.SendData();
			//client.cRecv();
			t = delay;
		}
		else
			t--;
	}

	game.GameRelease();
	//client.cClose();

	return msg.wParam;
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