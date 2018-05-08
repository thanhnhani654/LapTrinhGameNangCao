#include "Server.h"
#include "GameLog\GameLog.h"

Server* Server::inst;

Server::Server()
{
	setUpCompleted = false;
	bSyncTime = false;
}

Server* Server::getInstance()
{
	if (inst == nullptr)
	{
		inst = new Server();
	}
	return inst;
}

void Server::StartUp()
{
	updatePackCount = 0;
	dataPack = nullptr;
	if ((Ret = WSAStartup(0x0202, &wsaData)) != 0)
	{
		printf("WSAStartup() failed with error %d\n", Ret);
		WSACleanup();
		MessageBox(NULL, L"StartUp failed!", L"ERROR", NULL);
		return ;
	}
	else
		printf("WSAStartup() is fine!\n");
}

void Server::prepareListenSocket()
{
	// Prepare a socket to listen for connections
	if ((ListenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
	{
		printf("WSASocket() failed with error %d\n", WSAGetLastError());
		MessageBox(NULL, L"prepareListenSocket failed!", L"ERROR", NULL);
		return ;
	}
	else
		printf("WSASocket() is OK!\n");
}

void Server::Bind()
{
	InternetAddr.sin_family = AF_INET;
	InetPton(AF_INET, L"192.168.1.234", &severbuff);
	//InetPton(AF_INET, L"127.0.0.1", &severbuff);
	InternetAddr.sin_addr = severbuff;
	//inet_addr("192.168.0.145");// htonl(INADDR_ANY);
	//InternetAddr.sin_addr.s_addr = inet_addr(szServer);
	InternetAddr.sin_port = htons(PORT);

	if (::bind(ListenSocket, (PSOCKADDR)&InternetAddr, sizeof(InternetAddr)) == SOCKET_ERROR)
	{
		printf("bind() failed with error %d\n", WSAGetLastError());
		MessageBox(NULL, L"Bind failed!", L"ERROR", NULL);
		return;
	}
	else
		printf("bind() is OK!\n");
}

void Server::listenSocket()
{
	if (listen(ListenSocket, 5))
	{
		printf("listen() failed with error %d\n", WSAGetLastError());
		MessageBox(NULL, L"Listen failed!", L"ERROR", NULL);
		return ;
	}
	else
		printf("listen() is OK!\n");
}

void Server::nonBlock()
{
	NonBlock = 1;
	if (ioctlsocket(ListenSocket, FIONBIO, &NonBlock) == SOCKET_ERROR)
	{
		GAMELOG("ioctlsocket() failed with error %d\n", WSAGetLastError());
		MessageBox(NULL, L"NonBlock failed!", L"ERROR", NULL);
		return;
	}
	else
		GAMELOG("ioctlsocket() is OK!\n");
}

BOOL Server::selectSocket()
{
	// Prepare the Read and Write socket sets for network I/O notification
	FD_ZERO(&ReadSet);
	FD_ZERO(&WriteSet);

	// Always look for connection attempts
	FD_SET(ListenSocket, &ReadSet);

	// Set Read and Write notification for each socket based on the
	// current state the buffer.  If there is data remaining in the
	// buffer then set the Write set otherwise the Read set
	for (int i = 0; i < TotalSockets; i++)
		if (!SocketArray[i]->bRecv)
			FD_SET(SocketArray[i]->Socket, &WriteSet);
		else
			FD_SET(SocketArray[i]->Socket, &ReadSet);
	
	timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0.5;
	
	if ((Total = select(0, &ReadSet, &WriteSet, NULL, &timeout)) == SOCKET_ERROR)
	{
		GAMELOG("select() returned with error %d\n", WSAGetLastError());
		return 1;
	}
	Total++;
	//Total = 1;
	//else
		//GAMELOG("select() is OK!\n");
	//GAMELOG("select() is OK!\n");

	// Check for arriving connections on the listening socket.
	if (FD_ISSET(ListenSocket, &ReadSet))
	{
		Total--;
		static int dmmm = 0;
		if (dmmm == 1)
			int a = 0;
		dmmm++;
		if ((AcceptSocket = accept(ListenSocket, NULL, NULL)) != INVALID_SOCKET)
		{
			// Set the accepted socket to non-blocking mode so the server will
			// not get caught in a blocked condition on WSASends
			NonBlock = 1;

			if (ioctlsocket(AcceptSocket, FIONBIO, &NonBlock) == SOCKET_ERROR)
			{
				printf("ioctlsocket(FIONBIO) failed with error %d\n", WSAGetLastError());
				return 1;
			}
			else
				printf("ioctlsocket(FIONBIO) is OK!\n");

			if (CreateSocketInformation(AcceptSocket) == FALSE)
			{
				printf("CreateSocketInformation(AcceptSocket) failed!\n");
				return 1;
			}
			else
				printf("CreateSocketInformation() is OK!\n");
		}
		else
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				printf("accept() failed with error %d\n", WSAGetLastError());
				return 1;
			}
			else
				printf("accept() is fine!\n");
		}
	}
}

void Server::Recv()
{
	/*FD_ZERO(&ReadSet);
	FD_ZERO(&WriteSet);
	FD_SET(ListenSocket, &ReadSet);*/
	_LPSOCKET_INFORMATION SocketInfo = NULL;

	
	
	for (int i = 0; Total > 0 && i < TotalSockets; i++)
	{		
		if (!SocketArray[i]->bRecv) {
			continue;
		}
		/*if (FD_ISSET(SocketArray[i]->Socket, &WriteSet))
			continue;*/
		SocketInfo = SocketArray[i];
		if (!SocketInfo->bRecv)
			continue;
		
		if (SocketInfo != NULL)
		{
			//Total--;
			SocketInfo->DataBuf.buf = SocketInfo->Buffer;
			SocketInfo->DataBuf.len = DATA_BUFSIZE;
			Flags = 0;

			if (WSARecv(SocketInfo->Socket, &(SocketInfo->DataBuf), 1, &RecvBytes, &Flags, NULL, NULL) == SOCKET_ERROR)
			{
				if (WSAGetLastError() != WSAEWOULDBLOCK)
				{
					printf("WSARecv() failed with error %d\n", WSAGetLastError());
					FreeSocketInformation(i);
				}
				else
					printf("WSARecv() is OK!\n");
				continue;
			}
			else
			{
				static int countt = 0;
				countt++;
				GAMELOG("RECV									times: %d", countt);

				SocketInfo->BytesRECV = RecvBytes;
				if (RecvBytes == 0)
					int a = 0;
				if (SocketInfo->Buffer[0] != 's')
				{
					GAMELOG("RECV NULL DATA");
					continue;
				}
				static int count = 0;
				count++;
				GAMELOG("RECV SUCCESSFUL!!!						times: %d", count);
				//Delta Recv
				static float tick = 0;
				tick = GetTickCount();
				static float lastTick = 0;
				float DELTATIME = tick - lastTick;
				lastTick = tick;
				//GAMELOG("Delta RECV: %f", DELTATIME);

				SocketInfo->bRecv = false;
				analysis(SocketInfo);
				// If zero bytes are received, this indicates the peer closed the connection.

				/*if (RecvBytes == 0)
				{
					FreeSocketInformation(i);
					continue;
				}*/

			}

		}
	}
}

BOOL Server::CreateSocketInformation(SOCKET s)
{
	_LPSOCKET_INFORMATION SI;
	printf("Accepted socket number %d\n", s);
	if ((SI = (_LPSOCKET_INFORMATION)GlobalAlloc(GPTR, sizeof(_SOCKET_INFORMATION))) == NULL)
	{
		printf("GlobalAlloc() failed with error %d\n", GetLastError());
		return FALSE;
	}
	else
		printf("GlobalAlloc() for SOCKET_INFORMATION is OK!\n");
	// Prepare SocketInfo structure for use
	SI->Socket = s;
	SI->BytesSEND = 0;
	SI->BytesRECV = 0;
	SI->bRecv = true;

	SocketArray[TotalSockets] = SI;
	TotalSockets++;
	return(TRUE);
}

void Server::FreeSocketInformation(DWORD Index)
{
	_LPSOCKET_INFORMATION SI = SocketArray[Index];
	DWORD i;
	closesocket(SI->Socket);
	printf("Closing socket number %d\n", SI->Socket);
	GlobalFree(SI);
	// Squash the socket array
	for (int i = Index; i < TotalSockets; i++)
	{
		SocketArray[i] = SocketArray[i + 1];
	}
	TotalSockets--;
}

UpdateEvent Server::createEvent(eObjectId iObjectId, funcId ifuncId, char* eventdata)
{
	UpdateEvent uevent;
	uevent._objectId = iObjectId;
	uevent._funcId = ifuncId;
	uevent._event = new char(sizeof(eventdata));
	memcpy(uevent._event, eventdata, sizeof(eventdata));

	return uevent;
}

UpdatePack Server::createUpdatePack()
{
	UpdatePack upack;
	upack.eventCount = 0;
	//upack._updateEvent = &arr[0];

	return upack;
}

void Server::addUpdatePack(UpdatePack iupdatePack)
{
	if (iupdatePack.miliTime < 0)
		return;
	if (iupdatePack.eventCount > 0)
	{
		int a = 0;
	}

	for (int i = 0; i < updatePackCount + 1; i++)
	{
		if (updatePackCount == 0)
		{
			updatePack[0] = iupdatePack;
			updatePackCount++;
			return;
		}
		if (iupdatePack.miliTime < updatePack[0].miliTime)
		{
			if (updatePackCount == 2400)
				updatePackCount--;
			for (int j = updatePackCount; j > 1; j--)
				updatePack[j] = updatePack[j - 1];
			updatePack[0] = iupdatePack;
			updatePackCount++;
			return;
		}
		if (iupdatePack.miliTime > updatePack[updatePackCount-1].miliTime)
		{
			if (updatePackCount == 2400)
			{
				updatePackCount--;
				updatePack[updatePackCount] = iupdatePack;
			}
			else
				updatePack[updatePackCount+1] = iupdatePack;
			updatePackCount++;
			return;
		}
		if (iupdatePack.miliTime > updatePack[i].miliTime && (iupdatePack.miliTime < updatePack[i+1].miliTime || i+1 == updatePackCount))
		{
			if (updatePackCount == 2400)
				updatePackCount--;
			for (int j = updatePackCount; j > i; j--)
				updatePack[j] = updatePack[j - 1];
			updatePack[i + 1] = iupdatePack;
			updatePackCount++;
			return;
		}
	}

	
}

void Server::clearUpdatePack()
{
	UpdatePack uclear[2400];
	for (int i = 0; i < 2400; i++)
	{
		updatePack[i] = uclear[i];
	}
	updatePackCount = 0;
}

void Server::analysis(_LPSOCKET_INFORMATION socketInfo)
{
	char* tempData = nullptr;
	int readlen = 0;

	bool loop = true;
	bool startread = false;
	int eventCount = 0;
	UpdatePack* uPack = nullptr;

	//SyncTime
	if (socketInfo->Buffer[0] == 's' && socketInfo->Buffer[1] == 't')
	{
		SyncTime();
		return;
	}

	while (loop)
	{
		if (tempData != nullptr)
			delete[] tempData; 
		tempData = new char[2];
		//memcpy(tempData, socketInfo->DataBuf.buf + readlen, 2);
		memcpy(tempData, socketInfo->Buffer + readlen, 2);
		

		//check Start Update Pack
		if (!startread)
			if (tempData[0] == 's' && tempData[1] == 'p')
			{
				startread = true;
				readlen += 2;
				uPack = new UpdatePack();
				*uPack = createUpdatePack();
				
				char* temp = new char[4];

				memcpy(temp, socketInfo->DataBuf.buf + readlen, sizeof(float));
				readlen += sizeof(float);

				uPack->miliTime = (*reinterpret_cast<float*>(temp));
				delete[] temp;
				
			}
			else
			{
				break;
			}

		//check end Update Pack
		if (startread)
			if (tempData[0] == 'e' && tempData[1] == 'p')
			{
				startread = false;
				if (uPack != nullptr)
				{
					addUpdatePack(*uPack);
					eventCount = 0;
					//uPack->uDelete();
					delete uPack;
					uPack = nullptr;
				}
				readlen += 2;
				continue;
			}

		if (readlen > 8000)
		{
			delete uPack;
			uPack = nullptr;
			break;
		}

		

		eObjectId objectID;
		funcId funcID;

		//Get Object ID
		delete[] tempData;
		tempData = new char[4];
		memcpy(tempData, socketInfo->Buffer + readlen, 4);		
		objectID = static_cast<eObjectId>((*tempData));
		///Debug
		if (objectID == eObjectId::GameObject)
		{
			//GAMELOG("True Object ID. %d", cou);
			readlen += 4;
		}
		else
		{
			//GAMELOG("Wrong Object ID.");
			continue;
		}


		//Get FuncId
		delete[] tempData;
		tempData = new char[4];
		memcpy(tempData, socketInfo->Buffer + readlen, 4);	     readlen += 4;
		funcID = static_cast<funcId>((*tempData));

		///Debug
		/*if (funcID == funcId::Pl_Move_Event)
			GAMELOG("True FuncID");
		else
			GAMELOG("WRONG FUNCID");*/

		//Get Data
		delete[] tempData;
		tempData = new char[1];
		memcpy(tempData, socketInfo->Buffer + readlen, 1);
		readlen += 1;

		edirection* moveEvent = reinterpret_cast<edirection*>(tempData);
		if (*moveEvent == edirection::UP)
			int a = 0;
		if (*moveEvent == edirection::NIL)
			int a = 0;

		//Add Event Update
		uPack->_updateEvent[eventCount] = createEvent(objectID, funcID, tempData);	
		uPack->eventCount++;

		delete[] tempData;
		tempData = nullptr;
		eventCount++;
	}
}

void Server::processSend()
{
	_LPSOCKET_INFORMATION SocketInfo = NULL;

	for (int i = 0; Total > 0 && i < TotalSockets; i++)
	{
		//if (FD_ISSET(SocketArray[i], &ReadSet))
		//	continue;

		/*if (SocketArray[i]->bRecv) {
			continue;
		}*/

		if (dataPack == nullptr)
		{
			//SocketArray[i]->bRecv = true;
			continue;
		}
		dataPack->Buffer[dataPack->len] = 'e';
		dataPack->Buffer[dataPack->len + 1] = 'p';
		dataPack->len += 2;

		SocketInfo = SocketArray[i];
		SocketInfo->bRecv = true;

		if (SocketInfo != NULL)
		{
			//SocketInfo->DataBuf.buf = SocketInfo->Buffer;
			SocketInfo->DataBuf.buf = dataPack->Buffer;
			SocketInfo->DataBuf.len = dataPack->len;
			if (WSASend(SocketInfo->Socket, &(SocketInfo->DataBuf), 1, &SendBytes, 0, NULL, NULL) == SOCKET_ERROR)
			{
				
				MessageBox(NULL, L"Send failed!", L"ERROR", NULL);
				if (WSAGetLastError() != WSAEWOULDBLOCK)
				{
					GAMELOG("WSASend() failed with error %d\n", WSAGetLastError());
					FreeSocketInformation(i);
				}
				else
					printf("WSASend() is OK!\n");
				continue;
			}
			else if (SendBytes == 0)
			{
				int a = 0;
			}
			else
			{
				SocketInfo->BytesSEND += SendBytes;
				SocketInfo->bRecv = true;
				
				static int countt = 0;
				countt++;
				GAMELOG("SendDataPack() Successful!!!!			times: %d", countt);

				//if (SocketInfo->BytesSEND == SocketInfo->BytesRECV)
				if (SocketInfo->BytesSEND != 0)
				{
					SocketInfo->BytesSEND = 0;
					SocketInfo->BytesRECV = 0;
				}
			}
		}
	}
}

BOOL Server::CreateData(void* data, eObjectId objectid, funcId FuncID, uint32_t datasize, int a)
{
	if (dataPack != nullptr)
	{
		delete dataPack;
		dataPack = new DataInfomation();

		dataPack->Buffer[0] = 's';
		dataPack->Buffer[1] = 'p';
		dataPack->len = 2;
	}

	char* dataconvert = new char(datasize);	

	DataInfomation* tempDataPack = new DataInfomation();
	tempDataPack->Buffer[0] = 's';
	tempDataPack->Buffer[1] = 'p';
	tempDataPack->len = 2;

	(*dataconvert) = static_cast<char>(objectid);
	memcpy(tempDataPack->Buffer + tempDataPack->len, dataconvert, sizeof(uint32_t));
	tempDataPack->len += sizeof(uint32_t);

	(*dataconvert) = static_cast<char>(FuncID);
	memcpy(tempDataPack->Buffer + tempDataPack->len, dataconvert, sizeof(uint32_t));
	tempDataPack->len += sizeof(uint32_t);

	dataconvert = new char(datasize);
	dataconvert = reinterpret_cast<char*>(data);
	if (dataconvert != NULL)
		memcpy(tempDataPack->Buffer + tempDataPack->len, dataconvert, datasize);
	else
	{
		D3DXVECTOR2 a;
		a.x = 0;
		a.y = 0;
		dataconvert = reinterpret_cast<char*>(&a);
		memcpy(tempDataPack->Buffer + tempDataPack->len, dataconvert, datasize);
	}
	tempDataPack->len += datasize;

	if (dataPack == nullptr)
	{
		dataPack = new DataInfomation();
		
		dataPack->Buffer[0] = 's';
		dataPack->Buffer[1] = 'p';
		dataPack->len = 2;
		if (a == 1)
			dataPack->Buffer[0] = 't';
	}
	memcpy(dataPack->Buffer, tempDataPack->Buffer, tempDataPack->len);
	dataPack->len = tempDataPack->len;
	
	delete tempDataPack;
	return TRUE;
}

BOOL Server::SendDataPack()
{
	processSend();

	delete dataPack;
	dataPack = nullptr;
	return TRUE;
}

void Server::SyncTime()
{
	Timer timeb;
	timeb.getCurrentTime();

	char* timesync = new char[6];
	timesync[0] = 's';
	timesync[1] = 't';
	char* miltime = new char[4];
	float militime = timeb.getmilitime();
	miltime = reinterpret_cast<char*>(&militime);

	memcpy(timesync + 2, miltime, 4);


	_LPSOCKET_INFORMATION SocketInfo = NULL;
	for (int i = 0; Total > 0 && i < TotalSockets; i++)
	{
		SocketInfo = SocketArray[i];

		SocketInfo->DataBuf.buf = timesync;
		SocketInfo->DataBuf.len = 6;
		for (int j = 0; j < 1; j++)
		if (WSASend(SocketInfo->Socket, &(SocketInfo->DataBuf), 1, &SendBytes, 0, NULL, NULL) == SOCKET_ERROR)
		{
			MessageBox(NULL, L"Send failed!", L"ERROR", NULL);
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				GAMELOG("WSASend() failed with error %d\n", WSAGetLastError());
				FreeSocketInformation(i);
			}
			else
				printf("WSASend() is OK!\n");
			continue;
		}
		else
		{
			SocketInfo->BytesSEND = SendBytes;
			SocketInfo->bRecv = true;
			static int countt = 0;
			countt++;
			
			GAMELOG("Send Time Sync Successful!!!!			times: %d", countt);
		}
	}

	delete[] timesync;
	//delete[] miltime;

}