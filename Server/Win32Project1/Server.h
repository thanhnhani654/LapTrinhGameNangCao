#ifndef      __SERVER_H__
#define      __SERVER_H__

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#pragma once
 #include <winsock2.h>
//#include <WinSock2.h>
#include <WS2tcpip.h>
#include "Client\Client.h"
//#include <windows.h>
//#include <stdio.h>

//#include "Player.h"

#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE

#define PORT 5150
#define szServer "192.168.1.117"
#define DATA_BUFSIZE 8192

typedef struct __SOCKET_INFORMATION {

	CHAR Buffer[DATA_BUFSIZE];

	WSABUF DataBuf;

	SOCKET Socket;

	OVERLAPPED Overlapped;

	DWORD BytesSEND;

	DWORD BytesRECV;

} _SOCKET_INFORMATION, *_LPSOCKET_INFORMATION;

struct UpdateEvent
{
	eObjectId _objectId;
	funcId _funcId;
	char* _event;
};

struct UpdatePack
{
	float miliTime;
	UpdateEvent _updateEvent[8];
	int eventCount;

	//void uDelete()
	//{
	//	delete _updateEvent;
	//}
};

class Server
{
public:
	DWORD TotalSockets = 0;
	_LPSOCKET_INFORMATION SocketArray[FD_SETSIZE];
	BOOL CreateSocketInformation(SOCKET s);
	void FreeSocketInformation(DWORD Index);

	SOCKET ListenSocket;
	SOCKET AcceptSocket;
	SOCKADDR_IN InternetAddr;
	WSADATA wsaData;
	INT Ret;
	FD_SET WriteSet;
	FD_SET ReadSet;
	DWORD i;
	DWORD Total;
	ULONG NonBlock;
	DWORD Flags;
	DWORD SendBytes;
	DWORD RecvBytes;
	in_addr severbuff;
	int mode = 0;
	int request;
	bool doRequest = false;
	bool sendTime = false;
	static UpdatePack updatePack[2400];
	static int updatePackCount;

	void StartUp();
	void prepareListenSocket();
	void Bind();
	void listenSocket();
	void nonBlock();
	void processSend();
	void Recv();

	UpdateEvent createEvent(eObjectId iObjectId, funcId ifuncId, char* eventdata);
	UpdatePack createUpdatePack();
	void addUpdatePack(UpdatePack iupdatePack);
	static void clearUpdatePack();

	void analysis(_LPSOCKET_INFORMATION socketInfo);

	static char* dataPack;		//length = sizeof(8);
};

#endif