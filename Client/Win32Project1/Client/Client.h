#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <WinSock2.h>
#include "Timer.h"
#include <stdio.h>
#include <string>
#include <time.h>
#include "../Logger/Logger.h"

using namespace CPlusPlusLogging;

#define DEFAULT_PORT 5150
#define DEFAULT_BUFFER 2048

#define DEFAULT_MESSAGE "\'A test message from client\'"
#define FLAG_DATASIZE "DataSize:"
#define SIZE_FLAG_DATASIZE 9
//#define szServer "192.168.1.224"
//#define szServer "192.168.1.227"
#define szServer "127.0.0.1"

enum ePlayerFlags : uint8_t
{
	player1,
	player2,
	player3,
	player4
};

enum eObjectId : uint8_t
{
	PlayerID,
	BrickID,
	BulletID,
	BotID
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

	void reset()
	{
		len = 0;
		count = 0;
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

enum funcId : uint8_t
{
	Pl_Move_Event,
	Pl_Fire,
	Pl_Position,
	Pl_Disable,
	Pl_StartGame,
	Pl_EndGame
};

struct CompareUpdate
{
	uint32_t id;
	eObjectId objectid;
	uint8_t inGameID;
	funcId funcid;
	char* data;
};

class NetWorkManage
{
protected:
	WSADATA wsd;
	
	
	int result;
	struct sockaddr_in server;
	struct hostent *host = NULL;

	//char szServer[128];
	int iPort = DEFAULT_PORT;
	BOOL bSendOnly = TRUE;
	
	
	
public:
	//char* szServer;
	char szBuffer[DEFAULT_BUFFER];
	int ret;
	int playerID;
	float ping;
	bool disconected;
	bool bStartGame;

	NetWorkManage();
	void Initialize();
	eSendMode sendMode;
	eClientMode clientMode;
	static SOCKET sClient;
	ULONG nonBlocking;
	bool setUpCompleted;
	
	float deltaclock;

	static Timer localTime;

	static NetWorkManage* inst;
	static NetWorkManage* getInstance();
	DataInfomation* DataPack;			//main

	DataInfomation* tempDataPack;		//main
	float kkkk;
	
	BOOL cClientStartUp();
	BOOL cCreateSocket();
	void nonBlock();
	BOOL cConnect();
	BOOL cClose();
	

	BOOL CreateEventData(void* data,eObjectId objectid, uint8_t ingameID, funcId FuncID, uint32_t datasize);
	BOOL getStartUpdatetime();

	//Goi data lai va gan cac flag nhan dien
	
	BOOL SendData();
	bool bSend;

	////////////////////////update
	BOOL cRecv();
	//void analysis();

	//SyncTime
	float TimeA;		//time A;
	bool brequestSyncTime;
	void requestSyncTime();
	bool bsyncTimed;
	void SyncTime(int i);

	//Client Prediction
	float RTT;
	void getRtt();
};