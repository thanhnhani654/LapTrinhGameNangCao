#include "Client.h"

SOCKET NetWorkManage::sClient;
DataInfomation* NetWorkManage::DataSizePack;
DataInfomation* NetWorkManage::DataPack;
DataInfomation* NetWorkManage::StoreOPS;
DataInfomation* NetWorkManage::tempDataPack;
char* NetWorkManage::testdata;
int NetWorkManage::count;
Timer NetWorkManage::localTime;

void NetWorkManage::Initialize()
{
	testdata = nullptr;
	sendMode = SendDataSizePack;
	clientMode = eClientMode::Sending;
	DataSizePack = new DataInfomation(1);
	DataPack = new DataInfomation(1);
	localTime.getCurrentTime();
}

///////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////

BOOL NetWorkManage::cSend()
{
	/*strcpy_s(szzMessage, sizeof(szzMessage), DEFAULT_MESSAGE);

	result = send(sClient, szzMessage, strlen(szzMessage), 0);

	if (result == 0)
		return TRUE;
	else if (result == SOCKET_ERROR)
	{
		printf("send() failed with error code %d\n", WSAGetLastError());
		return FALSE;
	}
	printf("send() should be fine. Send %d bytes\n", result);
*/
	return TRUE;
}

BOOL NetWorkManage::CreateEventData(void* data,eObjectId objectid, funcId FuncID, uint32_t datasize)
{
	char* dataconvert = new char(sizeof(uint32_t));

	//[start flag][UpdateTime][ObjectID][FuncID][data]...[end flag][start flag]...[end flag]....
	//      2b        4b          1b(4b) 1b(4b)   nb         2b         2b            2b

	(*dataconvert) = static_cast<char>(objectid);
	memcpy(tempDataPack->Buffer + tempDataPack->len, dataconvert, sizeof(uint32_t));
	tempDataPack->len += sizeof(uint32_t);
	
	(*dataconvert) = static_cast<char>(FuncID);
	memcpy(tempDataPack->Buffer + tempDataPack->len, dataconvert, sizeof(uint32_t));
	tempDataPack->len += sizeof(uint32_t);

	dataconvert = new char(datasize);
	dataconvert = reinterpret_cast<char*>(data);
	memcpy(tempDataPack->Buffer + tempDataPack->len, dataconvert, datasize);
	tempDataPack->len += datasize;
	/*count++;
	if (count > 90)
	{
		MessageBox(NULL, TEXT("Count > 90"), TEXT("ERROR"), NULL);
	}*/
	
	//tempDataPack->count++;
	return TRUE;
}

BOOL NetWorkManage::WrapToSend()
{
	delete[] tempDataPack;
	tempDataPack = NULL;
	return TRUE;
}

BOOL NetWorkManage::SendData()
{
	if (DataPack->Buffer == NULL)
		return FALSE;
	send(sClient, DataPack->Buffer, DEFAULT_BUFFER, 0);
	delete DataPack;
	DataPack = new DataInfomation(2);
	/*switch (sendMode)
	{
	case eSendMode::SendDataSizePack:
		send(sClient, DataPack->Buffer, DEFAULT_BUFFER, 0);

		break;
	case eSendMode::SendDataPack:
		send(sClient, DataPack->Buffer, DataSizePack->len, 0);
		break;
	}*/
	return TRUE;
}

//Ham nay du. Khong co tac dung. Khong dung nua. Da Xoa
BOOL NetWorkManage::StoreOPSData(void* data, uint32_t FuncID, uint32_t datasize)
{
	if (StoreOPS == NULL)
	{
		StoreOPS = new DataInfomation(2);
	}
	char* dataconvert = new char(sizeof(uint32_t));

	dataconvert = reinterpret_cast<char*>(FuncID);
	memcpy(StoreOPS->Buffer, dataconvert, sizeof(uint32_t));
	StoreOPS->len += sizeof(uint32_t);

	dataconvert = new char(datasize);
	dataconvert = reinterpret_cast<char*>(data);
	memcpy(StoreOPS->Buffer + StoreOPS->len, dataconvert, datasize);
	StoreOPS->len += datasize;
	return TRUE;
}

BOOL NetWorkManage::cRecv()
{
	ret = recv(sClient, szBuffer, 8192, 0);
	
	if (ret == 0)        // Graceful close
	{
		printf("It is a graceful close!\n");
		MessageBox(NULL, L"recv() 0", L"ERROR", NULL);
		return FALSE;
	}
	else if (ret == SOCKET_ERROR)
	{
		printf("recv() failed with error code %d\n", WSAGetLastError());
		MessageBox(NULL, L"recv() ERRROR", L"ERROR", NULL);
		return FALSE;
	}

	if (testdata == nullptr)
	{
		testdata = new char(sizeof(FLOAT));
	}

	memcpy(testdata, szBuffer, 8);
	float tempfloat;
	char* tempdata = new char(sizeof(FLOAT));

	memcpy(tempdata, NetWorkManage::testdata, 4);
	tempfloat = (FLOAT)(*tempdata);

	

	return TRUE;
}

BOOL NetWorkManage::getStartUpdatetime()
{
	localTime.getCurrentTime();

	if (tempDataPack == NULL)
	{
		tempDataPack = new DataInfomation(2);
	}

	char* flag = new char(2);

	//EndPack Flag
	if (tempDataPack->len != 0)
	{
		flag = "ep";		//End Pack
		int a = tempDataPack->len;
		memcpy(tempDataPack->Buffer + tempDataPack->len, flag, 2);
		tempDataPack->len += 2;
		
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
	float militime = localTime.getmilitime();
	dataConverter = reinterpret_cast<char*>(&militime);

	memcpy(tempDataPack->Buffer + tempDataPack->len, dataConverter, sizeof(float));
	tempDataPack->len += sizeof(float);
	
	static int coun = 0;
	if (coun == 60)
		int h = 0;
	coun++;

	//delete[] dataConverter;
	return TRUE;
}