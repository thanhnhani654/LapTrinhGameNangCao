#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <WinSock2.h>
#include "Timer.h"
#include <stdio.h>
#include <string>
#include "../Logger/Logger.h"

using namespace CPlusPlusLogging;

#define DEFAULT_PORT 5150
#define DEFAULT_BUFFER 8192

#define DEFAULT_MESSAGE "\'A test message from client\'"
#define FLAG_DATASIZE "DataSize:"
#define SIZE_FLAG_DATASIZE 9
#define szServer "192.168.1.132"

enum ePlayerFlags : uint8_t
{
	player1,
	player2,
	player3,
	player4
};

enum eObjectId : uint32_t
{
	SizePack,
	GameObject
};

struct DataInfomation
{
	char Buffer[DEFAULT_BUFFER];
	int len;
	int count;
	DataInfomation(int type)
	{
		switch (type)
		{
		case 1:
			count = 0;
			len = 5;
		case 2:
			count = 0;
			len = 0;
		}
		
	}
};

enum eSendMode
{
	SendDataSizePack,
	SendDataPack,
};

enum eClientMode
{
	Waiting,
	Sending,
};

enum funcId : uint32_t
{
	Pl_Move_Event,
	Pl_Fire,
	Pl_Position,
};

class NetWorkManage
{
protected:
	WSADATA wsd;
	
	char szBuffer[DEFAULT_BUFFER];
	int result;
	struct sockaddr_in server;
	struct hostent *host = NULL;

	//char szServer[128];
	int iPort = DEFAULT_PORT;
	BOOL bSendOnly = TRUE;
	
	int ret;
	
public:
	void Initialize();
	eSendMode sendMode;
	eClientMode clientMode;
	static SOCKET sClient;

	static Timer localTime;

	//static DataInfomation* tempDataSizePack;
	static DataInfomation* tempDataPack;		//main
	static DataInfomation* DataSizePack;		//xoa
	static DataInfomation* DataPack;			//main
	static DataInfomation* StoreOPS;			//xoa
	static int count;
	
	BOOL cClientStartUp();
	BOOL cCreateSocket();
	BOOL cConnect();
	BOOL cSend();
	BOOL cClose();
	
	int GetPlayerID();

	static BOOL CreateEventData(void* data,eObjectId objectid, funcId FuncID, uint32_t datasize);
	static BOOL getStartUpdatetime();

	//OPS:: Once Per Send
	static BOOL StoreOPSData(void* data, uint32_t FuncID, uint32_t datasize);
	//Goi data lai va gan cac flag nhan dien
	BOOL WrapToSend();
	BOOL SendData();

	////////////////////////TEST
	BOOL cRecv();
	static char* testdata;
};