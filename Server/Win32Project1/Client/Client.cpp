#include "Client.h"

SOCKET NetWorkManage::sClient;
DataInfomation* NetWorkManage::DataSizePack;
DataInfomation* NetWorkManage::DataPack;
DataInfomation* NetWorkManage::StoreOPS;
DataInfomation* NetWorkManage::tempDataPack;
int NetWorkManage::count;

void NetWorkManage::Initialize()
{
	sendMode = SendDataSizePack;
	clientMode = eClientMode::Sending;
	DataSizePack = new DataInfomation();
	DataPack = new DataInfomation();

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

	if (tempDataPack == NULL)
	{
		tempDataPack = new DataInfomation();
		tempDataPack->len = 5;
		//tempDataSizePack = new DataInfomation();
	}

	//	[playerID][count][ObjectId][funcid][Data][objectid][funcid][data]...[objectid][funcid][position][end]
	//		1				4			4	4		4			4	4
	(*dataconvert) = static_cast<char>(objectid);
	memcpy(tempDataPack->Buffer + tempDataPack->len, dataconvert, sizeof(uint32_t));
	tempDataPack->len += sizeof(uint32_t);
	
	(*dataconvert) = static_cast<char>(FuncID);
	memcpy(tempDataPack->Buffer + tempDataPack->len, dataconvert, sizeof(uint32_t));
	tempDataPack->len += sizeof(uint32_t);

	dataconvert = new char(datasize);
	dataconvert = reinterpret_cast<char*>(data);
	/*if (tempDataPack->len > 450)
	{
		MessageBox(NULL, TEXT("Tran du lieu"), TEXT("ERROR"), NULL);
	}*/
	
	count++;
	if (count > 90)
	{
		MessageBox(NULL, TEXT("Count > 90"), TEXT("ERROR"), NULL);
	}
	memcpy(tempDataPack->Buffer + tempDataPack->len, dataconvert, datasize);

	tempDataPack->len += datasize;
	tempDataPack->count++;

	return TRUE;
}

BOOL NetWorkManage::WrapToSend()
{
	count = 0;
	char* flagData = new char(1);

	//GetPlayID
	flagData[0] = '1';

	memcpy(tempDataPack->Buffer, flagData, 1);

	//Count Data
	flagData = new char(4);
	(*flagData) = static_cast<char>(tempDataPack->count);
	memcpy(tempDataPack->Buffer + 1, flagData, 4);

	//Event Data
	memcpy(DataPack, tempDataPack, sizeof(tempDataPack));
	memcpy(DataPack->Buffer, tempDataPack->Buffer, DEFAULT_BUFFER);
	

	//Once Per Send Data
	if (StoreOPS != NULL)
	{
		memcpy(DataPack->Buffer + DataPack->len, StoreOPS->Buffer, StoreOPS->len);
		DataPack->count++;
		DataPack->len += StoreOPS->len;
	}

	int count = tempDataPack->count;

	delete[] flagData;
	delete[] tempDataPack;
	tempDataPack = NULL;
	return TRUE;
}

BOOL NetWorkManage::SendData()
{
	switch (sendMode)
	{
	case eSendMode::SendDataSizePack:
		send(sClient, DataPack->Buffer, DEFAULT_BUFFER, 0);

		break;
	case eSendMode::SendDataPack:
		send(sClient, DataPack->Buffer, DataSizePack->len, 0);
		break;
	}
	return TRUE;
}

BOOL NetWorkManage::StoreOPSData(void* data, uint32_t FuncID, uint32_t datasize)
{
	if (StoreOPS == NULL)
	{
		StoreOPS = new DataInfomation();
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



//BOOL NetWorkManage::CreateEventData(void* data, uint32_t FuncID, uint32_t datasize)
//{
//	char* dataconvert = new char(sizeof(uint32_t));
//
//	if (tempDataPack == NULL || tempDataSizePack == NULL)
//	{
//		tempDataPack = new DataInfomation();
//		tempDataSizePack = new DataInfomation();
//	}
//
//	dataconvert = reinterpret_cast<char*>(FuncID);
//	memcpy(tempDataSizePack->Buffer + tempDataSizePack->len, dataconvert, sizeof(uint32_t));
//	tempDataSizePack->len += sizeof(uint32_t);
//	tempDataSizePack->count++;
//
//	dataconvert = reinterpret_cast<char*>(datasize);
//	memcpy(tempDataSizePack->Buffer + tempDataSizePack->len, dataconvert, sizeof(uint32_t));
//	tempDataSizePack->len += sizeof(uint32_t);
//
//	dataconvert = new char(datasize);
//	dataconvert = reinterpret_cast<char*>(data);
//	memcpy(tempDataPack->Buffer + tempDataPack->len, dataconvert, datasize);
//	tempDataPack->len += datasize;
//
//	return TRUE;
//}