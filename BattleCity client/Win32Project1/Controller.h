#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>


#define KEY_DOWN(code) ( IsKeyDown(code) )
#define KEYBOARD_BUFFER_SIZE	1024

class Controller
{
private:

	// Cac gia tri ban dau bat buoc phai co
	LPDIRECTINPUT8 dinput = NULL;
	LPDIRECTINPUTDEVICE8 didev = NULL;
	static LPDIRECTINPUTDEVICE8 Keyboard;
	BYTE KeyStates[256];
	DIDEVICEOBJECTDATA KeyEvents[KEYBOARD_BUFFER_SIZE];
	static LPDIRECTINPUT8 di;

public:

	void _InitKeyboard(HWND hWnd, HINSTANCE hInstance);

	void _ProcessKeyBoard();

	//Phuong thuc tra ve true neu phim duoc kiem tra dang o trang thai nhan
	int IsKeyDown(int KeyCode);

	//Phuong thuc cho phep thuc chuc nang khi phim chuyen tu trang thai tha sang nhan
	virtual void OnKeyDown(int Keycode);

	//Phuong thuc cho phep thuc hien chuc nang khi phim chuyen tu trang thai nhan sang tha
	virtual void OnKeyUp(int Keycode);

	

};
