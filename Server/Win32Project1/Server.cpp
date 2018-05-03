#include "Server.h"
#include "GameLog\GameLog.h"
char* Server::dataPack;
UpdatePack Server::updatePack[2400];
int Server::updatePackCount;

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
	InetPton(AF_INET, L"192.168.1.132", &severbuff);
	InternetAddr.sin_addr = severbuff;
	//inet_addr("192.168.0.145");// htonl(INADDR_ANY);
	//InternetAddr.sin_addr.s_addr = inet_addr(szServer);
	InternetAddr.sin_port = htons(PORT);

	if (bind(ListenSocket, (PSOCKADDR)&InternetAddr, sizeof(InternetAddr)) == SOCKET_ERROR)
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
		printf("ioctlsocket() failed with error %d\n", WSAGetLastError());
		MessageBox(NULL, L"NonBlock failed!", L"ERROR", NULL);
		return;
	}
	else
		printf("ioctlsocket() is OK!\n");
}

void Server::processSend()
{
	FD_ZERO(&ReadSet);
	FD_ZERO(&WriteSet);
	FD_SET(ListenSocket, &ReadSet);
	_LPSOCKET_INFORMATION SocketInfo = NULL;

	if (FD_ISSET(ListenSocket, &ReadSet))
	{
		Total--;
		if ((AcceptSocket = accept(ListenSocket, NULL, NULL)) != INVALID_SOCKET)
		{
			// Set the accepted socket to non-blocking mode so the server will
			// not get caught in a blocked condition on WSASends
			NonBlock = 1;
			if (ioctlsocket(AcceptSocket, FIONBIO, &NonBlock) == SOCKET_ERROR)
			{
				printf("ioctlsocket(FIONBIO) failed with error %d\n", WSAGetLastError());
				return ;
			}
			else
				printf("ioctlsocket(FIONBIO) is OK!\n");
			if (CreateSocketInformation(AcceptSocket) == FALSE)
			{
				printf("CreateSocketInformation(AcceptSocket) failed!\n");
				return;
			}
			else
			{
				printf("CreateSocketInformation() is OK!\n");
			}
		}
		else
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				printf("accept() failed with error %d\n", WSAGetLastError());
				return ;
			}
			else
			{
				SocketInfo = SocketArray[0];
				Total++;
				printf("accept() is fine!\n");
			}
		}

	}

	for (i = 0; i < TotalSockets; i++)
	{
		if (SocketInfo != NULL)
		{
			SocketInfo->Buffer[0] = 'a';
			//SocketInfo->DataBuf.buf = SocketInfo->Buffer;
			SocketInfo->DataBuf.buf = dataPack;
			SocketInfo->DataBuf.len = 8;
			if (WSASend(SocketInfo->Socket, &(SocketInfo->DataBuf), 1, &SendBytes, 0, NULL, NULL) == SOCKET_ERROR)
			{
				MessageBox(NULL, L"Send failed!", L"ERROR", NULL);
				if (WSAGetLastError() != WSAEWOULDBLOCK)
				{
					printf("WSASend() failed with error %d\n", WSAGetLastError());
					FreeSocketInformation(i);
				}
				else
					printf("WSASend() is OK!\n");
				continue;
			}
			else
			{
				//MessageBox(NULL, L"Send OK!", L"ERROR", NULL);
				printf("SERVER TIME da duoc gui!\n");
				SocketInfo->BytesSEND += SendBytes;

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

void Server::Recv()
{
	FD_ZERO(&ReadSet);
	FD_ZERO(&WriteSet);
	FD_SET(ListenSocket, &ReadSet);
	_LPSOCKET_INFORMATION SocketInfo = NULL;

	if (FD_ISSET(ListenSocket, &ReadSet))
	{
		Total--;
		if ((AcceptSocket = accept(ListenSocket, NULL, NULL)) != INVALID_SOCKET)
		{
			// Set the accepted socket to non-blocking mode so the server will
			// not get caught in a blocked condition on WSASends
			NonBlock = 1;
			if (ioctlsocket(AcceptSocket, FIONBIO, &NonBlock) == SOCKET_ERROR)
			{
				printf("ioctlsocket(FIONBIO) failed with error %d\n", WSAGetLastError());
				return;
			}
			else
				printf("ioctlsocket(FIONBIO) is OK!\n");
			if (CreateSocketInformation(AcceptSocket) == FALSE)
			{
				printf("CreateSocketInformation(AcceptSocket) failed!\n");
				return;
			}
			else
			{
				printf("CreateSocketInformation() is OK!\n");
			}
		}
		else
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				printf("accept() failed with error %d\n", WSAGetLastError());
				return;
			}
			else
			{
				SocketInfo = SocketArray[0];
				Total++;
				printf("accept() is fine!\n");
			}
		}

	}

	for (i = 0; i < TotalSockets; i++)
	{
		if (SocketInfo != NULL)
		{
			Total--;
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
				SocketInfo->BytesRECV = RecvBytes;
				
				analysis(SocketInfo);
				// If zero bytes are received, this indicates the peer closed the connection.

				if (RecvBytes == 0)
				{
					FreeSocketInformation(i);
					continue;
				}

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
	for (i = Index; i < TotalSockets; i++)
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

	while (loop)
	{
		if (tempData != nullptr)
			delete[] tempData; 
		tempData = new char[2];
		memcpy(tempData, socketInfo->DataBuf.buf + readlen, 2);
		

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
		memcpy(tempData, socketInfo->DataBuf.buf + readlen, 4);		
		objectID = static_cast<eObjectId>((*tempData));
		///Debug
		if (objectID == eObjectId::GameObject)
		{
			static int cou = 0;
			GAMELOG("True Object ID. %d", cou);
			cou++;
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
		memcpy(tempData, socketInfo->DataBuf.buf + readlen, 4);	     readlen += 4;
		funcID = static_cast<funcId>((*tempData));

		///Debug
		if (funcID == funcId::Pl_Move_Event)
			GAMELOG("True FuncID");
		else
			GAMELOG("WRONG FUNCID");

		//Get Data
		delete[] tempData;
		tempData = new char[1];
		memcpy(tempData, socketInfo->DataBuf.buf + readlen, 1);	
		readlen += 1;
		//edirection* moveEvent = reinterpret_cast<edirection*>(tempData);

		//Player::getInstance()->Move(*moveEvent);
		//delete moveEvent;

		//Add Event Update
		uPack->_updateEvent[eventCount] = createEvent(objectID, funcID, tempData);	
		uPack->eventCount++;
		


		delete[] tempData;
		tempData = nullptr;
		eventCount++;
	}
}

