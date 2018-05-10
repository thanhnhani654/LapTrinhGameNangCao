#include "Client.h"
#include "../GameLog/GameLog.h"

SOCKET NetWorkManage::sClient;
Timer NetWorkManage::localTime;
NetWorkManage* NetWorkManage::inst;

NetWorkManage::NetWorkManage()
{
	setUpCompleted = false;
	bsyncTimed = false;
	deltaclock = 0;
}

NetWorkManage* NetWorkManage::getInstance()
{
	if (inst == nullptr)
		inst = new NetWorkManage();
	return inst;
}

void NetWorkManage::Initialize()
{
	sendMode = SendDataSizePack;
	clientMode = eClientMode::Sending;
	DataPack = new DataInfomation(1);
	localTime.getCurrentTime();
}

#pragma region First establish

BOOL NetWorkManage::cClientStartUp()
{
	if ((WSAStartup(MAKEWORD(2, 2), &wsd)) != 0)
	{
		printf("Failed to load Winsock library! Error %d\n", WSAGetLastError());
		return FALSE;
	}
	else
	{
		printf("Winsock library loaded successfully!\n");
		return TRUE;
	}
}

BOOL NetWorkManage::cCreateSocket()
{
	sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (sClient == INVALID_SOCKET)
	{
		printf("socket() failed with error code %d\n", WSAGetLastError());
		return FALSE;
	}
	else
	{
		printf("socket() looks fine!\n");
		//return TRUE;
	}
	
	server.sin_addr.s_addr = inet_addr(szServer);
	server.sin_family = AF_INET;
	server.sin_port = htons(iPort);
	
	if (server.sin_addr.s_addr == INADDR_NONE)
	{
		host = gethostbyname(szServer);
		if (host == NULL)
		{
			printf("Unable to resolve server %s\n", szServer);
			return FALSE;
		}
		else
		{
			printf("The hostname resolved successfully!\n");
		}
		CopyMemory(&server.sin_addr, host->h_addr_list[0], host->h_length);
		return TRUE;
	}
}

void NetWorkManage::nonBlock()
{
	nonBlocking = 1;
	if (ioctlsocket(sClient, FIONBIO, &nonBlocking) == SOCKET_ERROR)
	{
		printf("ioctlsocket() failed with error %d\n", WSAGetLastError());
		MessageBox(NULL, L"NonBlock failed!", L"ERROR", NULL);
		return;
	}
	else
		printf("ioctlsocket() is OK!\n");
}

BOOL NetWorkManage::cConnect()
{
	if (connect(sClient, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("connect() failed with error code %d\n", WSAGetLastError());
		return FALSE;
	}
	else
	{
		printf("connect() is pretty damn fine!\n");
		return TRUE;
	}
}

BOOL NetWorkManage::cClose()
{
	if (closesocket(sClient) == 0)
		printf("closesocket() is OK!\n");
	else
	{
		printf("closesocket() failed with error code %d\n", WSAGetLastError());
		return FALSE;
	}


	if (WSACleanup() == 0)
		printf("WSACleanup() is fine!\n");
	else
	{
		printf("WSACleanup() failed with error code %d\n", WSAGetLastError());
		return FALSE;
	}

	return TRUE;
}

#pragma endregion


BOOL NetWorkManage::CreateEventData(void* data,eObjectId objectid, funcId FuncID, uint32_t datasize)
{
	char* dataconvert = new char(sizeof(uint8_t));

	//[start flag][UpdateTime][ObjectID][FuncID][data]...[end flag][start flag]...[end flag]....
	//      2b        4b          1b(4b) 1b(4b)   nb         2b         2b            2b

	//GameObject ID
	(*dataconvert) = static_cast<char>(objectid);
	memcpy(tempDataPack->Buffer + tempDataPack->len, dataconvert, sizeof(uint8_t));
	tempDataPack->len += sizeof(uint8_t);
	
	//FuncID
	(*dataconvert) = static_cast<char>(FuncID);
	memcpy(tempDataPack->Buffer + tempDataPack->len, dataconvert, sizeof(uint8_t));
	tempDataPack->len += sizeof(uint8_t);


	//Data
	dataconvert = new char(datasize);
	dataconvert = reinterpret_cast<char*>(data);
	memcpy(tempDataPack->Buffer + tempDataPack->len, dataconvert, datasize);
	tempDataPack->len += datasize;

	tempDataPack->count++;

	return TRUE;
}

BOOL NetWorkManage::SendData()
{
	//DataPack khong co du lieu thi khong can gui
	if (DataPack->Buffer == nullptr || DataPack->len == 0)
		return FALSE;
	//DataPack co du lieu sai format thi khong can gui
	if (DataPack->Buffer[0] != 's' || DataPack->Buffer[1] != 'p')
	{
		GAMELOG("Wrong Format Data!!				Call From:: NetWorkManage::SendData");
		return FALSE;
	}

	//SEND
	int ret = send(sClient, DataPack->Buffer, DEFAULT_BUFFER, 0);

	if (ret == SOCKET_ERROR)
	{
		GAMELOG("Send Error!");
		return FALSE;
	}
	else if (ret == 0)
	{
		GAMELOG("Send NULL!");
		return FALSE;
	}
	//Debug
	static int count = 0;
	count++;
	GAMELOG("SEND SUCCESSFUL!!!   times: %d				Call From: NetWorkManage::SendData()", count);
	//////////////////////////////////////////////////////////
	delete DataPack;
	DataPack = nullptr;
	return TRUE;
}

BOOL NetWorkManage::cRecv()
{
	ret = recv(sClient, szBuffer, 8192, 0);
	if (szBuffer[0] == 't')
		return FALSE;
	if (ret == 0)        // Graceful close
	{
		printf("It is a graceful close!\n");
		//MessageBox(NULL, L"recv() 0", L"ERROR", NULL);
		GAMELOG("recv() 0");
		return FALSE;
	}
	else if (ret == SOCKET_ERROR)
	{
		printf("recv() failed with error code %d\n", WSAGetLastError());
		//MessageBox(NULL, L"recv() ERRROR", L"ERROR", NULL);
		GAMELOG("recv() failed with error code %d\n", WSAGetLastError());
		return FALSE;
	}	
	//Debug
	static int count = 0; 
	count++;
	GAMELOG("RECV SUCCESSFUL!!!			times: %d", count);
	////////////////////////////////////////////////////////
	return TRUE;
}

BOOL NetWorkManage::getStartUpdatetime()
{
	//Tao nhung thong tin ban dau cho mot pack update gom: flag, UpdateTime

	localTime.getCurrentTime();

	if (tempDataPack == nullptr)
	{
		tempDataPack = new DataInfomation(2);
	}

	//Neu khong co su kien can update thi khong can gui
	if (tempDataPack->count == 0)
	{
		delete[] tempDataPack;
		tempDataPack = new DataInfomation(2);
	}

	if (DataPack == nullptr)
	{
		DataPack = new DataInfomation(2);
	}

	char* flag = new char(2);

	//EndPack Flag. Tao EngPack Flag cho pack Update truoc.
	if (tempDataPack->len != 0)
	{
		flag = "ep";		//End Pack
		memcpy(tempDataPack->Buffer + tempDataPack->len, flag, 2);
		tempDataPack->len += 2;

		if (DataPack->len > 600)
			GAMELOG("OVERFLOW!!			CALL FROM: NetWorkManage::getStartUpdateTime");

		memcpy(DataPack->Buffer + DataPack->len, tempDataPack->Buffer, tempDataPack->len);
		DataPack->len += tempDataPack->len;
		delete[] tempDataPack;
		tempDataPack = new DataInfomation(2);
	}

	//StartPack Flag
	flag = "sp";			//Start Pack
	memcpy(tempDataPack + tempDataPack->len, flag, 2);
	tempDataPack->len += 2;

	//set Time Update
	char* dataConverter = new char[sizeof(float)];
	float militime = localTime.getmilitime() + deltaclock;
	dataConverter = reinterpret_cast<char*>(&militime);

	memcpy(tempDataPack->Buffer + tempDataPack->len, dataConverter, sizeof(float));
	tempDataPack->len += sizeof(float);

	return TRUE;
}

void NetWorkManage::SyncTime()
{
	//Khong biet comment the nao nen ghi do cong thuc SyncTime
	// TimeA(Client): thoi gian luc Client gui goi tin yeu cau dong bo hoa thoi gian
	// TimeB(Server): thoi gian luc Server nhan goi tin yeu cau dong bo hoa thoi gian
	// TimeC(Client): thoi gian luc Client nhan goi tin phan hoi chua thoi gian cua Server luc Server nhan goi tin yeu cau dong bo hoa cua Clien =))
	// 1/2 Thoi gian tren duong truyen: (TimeC - TimeA)/2
	// Thoi gian hien tai o Server: ((TimeC - TimeA)/2 + TimeB)
	// ClockDelta = ((TimeC - TimeA)/2 + TimeB) - TimeC
	// ClientTime = time.getCurrentTime + ClockDelta

	localTime.getCurrentTime();
	float TimeA = localTime.getmilitime() + deltaclock;
	char tsync[] = "st";

	send(sClient, tsync, 2, 0);

	recv(sClient, szBuffer, DEFAULT_BUFFER, 0);

	if (szBuffer[0] == 's' && szBuffer[1] == 't')
	{
		localTime.getCurrentTime();
		float TimeC = localTime.getmilitime() + deltaclock;
		
		char* cTimeB = new char[4];
		memcpy(cTimeB, szBuffer + 2, 4);
		float* TimeB = reinterpret_cast<float*>(cTimeB);

		deltaclock = ((TimeC - TimeA) / 2 + *TimeB) - TimeC;
		bsyncTimed = true;
		delete[] cTimeB;
	}
}