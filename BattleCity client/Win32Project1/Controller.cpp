#include "Controller.h"

LPDIRECTINPUTDEVICE8 Controller::Keyboard;

LPDIRECTINPUT8 Controller::di = NULL;

int Controller::IsKeyDown(int KeyCode)
{
	return (KeyStates[KeyCode] & 0x80) > 0;
}

void Controller::OnKeyUp(int KeyCode)
{
}

void Controller::OnKeyDown(int Keycode)
{
}

void Controller::_InitKeyboard(HWND hWnd, HINSTANCE hInstance)
{
	HRESULT
		hr = DirectInput8Create
		(
			hInstance,
			DIRECTINPUT_VERSION,
			IID_IDirectInput8, (VOID**)&di, NULL
		);
	hr = di->CreateDevice(GUID_SysKeyboard, &Keyboard, NULL);
	hr = Keyboard->SetDataFormat(&c_dfDIKeyboard);
	hr = Keyboard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_EXCLUSIVE);

	DIPROPDWORD dipdw;

	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = KEYBOARD_BUFFER_SIZE;

	hr = Keyboard->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
	hr = Keyboard->Acquire();

}

void Controller::_ProcessKeyBoard()
{
	Keyboard->GetDeviceState(sizeof(KeyStates), KeyStates);

	DWORD dwElements = KEYBOARD_BUFFER_SIZE;
	HRESULT hr = Keyboard->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), KeyEvents, &dwElements, 0);

	for (DWORD i = 0; i < dwElements; i++)
	{
		int KeyCode = KeyEvents[i].dwOfs;
		int KeyState = KeyEvents[i].dwData;
		if ((KeyState & 0x80) > 0)
		{
			OnKeyDown(KeyCode);
		}
		else
			OnKeyUp(KeyCode);
	}
}
