#include "Server.h"
#include "GameLog\GameLog.h"

Server* Server::inst;

Server::Server()
{
	setUpCompleted = false;
	bSyncTime = false;
	autoSendTime = 0;
	countPack = 0;
	tempASDataPack = nullptr;
	autoSendDataPack = nullptr;
}

Server* Server::getInstance()
{
	if (inst == nullptr)
	{
		inst = new Server();
	}
	return inst;
}

#pragma region First establish

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
	//InetPton(AF_INET, L"192.168.43.7", &severbuff);
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

void Server::SyncTime()
{
	//Khong biet comment the nao nen ghi do cong thuc SyncTime
	// TimeA(Client): thoi gian luc Client gui goi tin yeu cau dong bo hoa thoi gian
	// TimeB(Server): thoi gian luc Server nhan goi tin yeu cau dong bo hoa thoi gian
	// TimeC(Client): thoi gian luc Client nhan goi tin phan hoi chua thoi gian cua Server luc Server nhan goi tin yeu cau dong bo hoa cua Clien =))
	// 1/2 Thoi gian tren duong truyen: (TimeC - TimeA)/2
	// Thoi gian hien tai o Server: ((TimeC - TimeA)/2 + TimeB)
	// ClockDelta = ((TimeC - TimeA)/2 + TimeB) - TimeC
	// ClientTime = time.getCurrentTime + ClockDelta
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
			//SocketInfo->bRecv = true;
			FD_SET(SocketInfo->Socket, &ReadSet);
			static int countt = 0;
			countt++;
			GAMELOG("Send Time Sync Successful!!!!			times: %d		Call From: SyncTime", countt);
		}
	}

	delete[] timesync;
	bSyncTime = true;
	//delete[] miltime;
}

#pragma endregion

BOOL Server::selectSocket()
{
	///PLSSSSSSSSSSSSSSSSSSSSSSSSS DON'T ASK MEEEEEEEEEEEEEEEEEEEE
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
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	
	/*if ((Total = select(0, &ReadSet, &WriteSet, NULL, &timeout)) == SOCKET_ERROR)
	{
		GAMELOG("select() returned with error %d\n", WSAGetLastError());
		return 1;
	}*/

	// Check for arriving connections on the listening socket.
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
			{
				printf("CreateSocketInformation() is OK!\n");
				playerCount += 1;
				GAMELOG("playerJoin %d", playerCount);
				for (int i = 0; i < TotalSockets; i++)
					playerJoin(SocketArray[i]->Socket);
			}
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
	_LPSOCKET_INFORMATION SocketInfo = NULL;
	
	for (int i = 0; Total > 0 && i < TotalSockets; i++)
	{
		//Chi khi nao 1 Socket da gui moi duoc phep nhan thong tin. Khong biet tai sao phai lam nhu the mac du bo cung duoc, thang code dang tim ly do, thong cam.
		//if (FD_ISSET(SocketArray[i]->Socket, &WriteSet))
		//	continue;

		SocketInfo = SocketArray[i];

		SocketInfo->DataBuf.buf = SocketInfo->Buffer;
		SocketInfo->DataBuf.len = DATA_BUFSIZE;
		Flags = 0;

		if (WSARecv(SocketInfo->Socket, &(SocketInfo->DataBuf), 1, &RecvBytes, &Flags, NULL, NULL) == SOCKET_ERROR)
		{
			#pragma region Error
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				printf("WSARecv() failed with error %d\n", WSAGetLastError());
				FreeSocketInformation(i);
			}
			else
				printf("WSARecv() is OK!\n");
			continue;
			#pragma endregion
		}
		else
		{
			//Debug
			static int countt = 0;
			countt++;
			//GAMELOG("RECV									times: %d		Call From: Recv", countt);
			/////////////////////////
			SocketInfo->BytesRECV = RecvBytes;
			if (RecvBytes == 0)
				int a = 0;
			if (SocketInfo->Buffer[0] != 's')
			{
				//Binh Thuong thi se xay ra neu goi du lieu bi cat lam nhieu manh. Bat thuong se xay ra khi Client gui xai format
				GAMELOG("RECV NULL OR WRONG DATA				Call From: Recv");
				continue;
			}
			//Debug
			static int count = 0;
			count++;
			//GAMELOG("RECV SUCCESSFUL!!!						times: %d		Call From: Recv", count);
			/////////////////////////
			//Debug
			//Delta Recv
			static float tick = 0;
			tick = GetTickCount();
			static float lastTick = 0;
			float DELTATIME = tick - lastTick;
			lastTick = tick;
			//GAMELOG("Delta RECV: %f", DELTATIME);
			/////////////////////////////////////////

			//New
			FD_SET(SocketInfo->Socket, &WriteSet);

			analysis(SocketInfo);

			//Chua co FreeSocket
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
	GAMELOG("Closing socket number %d\n", SI->Socket);
	GlobalFree(SI);
	// Squash the socket array
	for (int i = Index; i < TotalSockets; i++)
	{
		SocketArray[i] = SocketArray[i + 1];
	}
	TotalSockets--;
}

UpdateEvent Server::createEvent(eObjectId iObjectId, uint8_t inGameID, funcId ifuncId, char* eventdata)
{
	//Dung de tao the nho luu lai sau moi lan phan tich
	UpdateEvent uevent;
	uevent._objectId = iObjectId;
	uevent.inGameID = inGameID;
	uevent._funcId = ifuncId;
	uevent._event = new char(sizeof(eventdata));
	memcpy(uevent._event, eventdata, sizeof(eventdata));

	return uevent;
}

UpdatePack Server::createUpdatePack()
{
	//No comment;
	UpdatePack upack;
	upack.eventCount = 0;

	return upack;
}

void Server::addUpdatePack(UpdatePack iupdatePack)
{
	//Thoi gian cua ban Update bi sai thi bo qua. ben ReUpdate check lai them lan nua
	if (iupdatePack.miliTime < 0)
	{
		GAMELOG("ClientUpdate time < 0				Call At: Server::addUpdatePack");
		return;
	}

	updatePack[updatePackCount] = iupdatePack;
	updatePackCount++;

	bool a = true;
	if (a)
		return;

	//Luu va sap xep cac ban ClientUpdate theo thoi gian
	for (int i = 0; i < updatePackCount + 1; i++)
	{
		//Neu chua luu ban ClientUpdate nao thi luu thang vao khoi sap xep
		if (updatePackCount == 0)
		{
			updatePack[0] = iupdatePack;
			updatePackCount++;
			return;
		}
		//Ban Update cu nhat co the se bi dua len dau
		if (iupdatePack.miliTime < updatePack[0].miliTime)
		{
			//Bo luu tru ClientUpdate toi da 2400 ban
			if (updatePackCount == 2400)
				updatePackCount--;
			for (int j = updatePackCount; j > 1; j--)
				updatePack[j] = updatePack[j - 1];
			updatePack[0] = iupdatePack;
			updatePackCount++;
			return;
		}
		//Ban Update moi nhat co the
		if (iupdatePack.miliTime > updatePack[updatePackCount-1].miliTime)
		{
			//Bo luu tru ClientUpdate toi da 2400 ban
			if (updatePackCount == 2400)
			{
				updatePackCount--;
				updatePack[updatePackCount] = iupdatePack;
			}
			else
				updatePack[updatePackCount] = iupdatePack;
			updatePackCount++;
			return;
		}
		//Sap xep binh thuong. chen ban ClientUpdate vao cho can chen
		if (iupdatePack.miliTime > updatePack[i].miliTime && (iupdatePack.miliTime < updatePack[i+1].miliTime || i+1 == updatePackCount))
		{
			//Bo luu tru ClientUpdate toi da 2400 ban
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
	//No comment;
	UpdatePack uclear[2400];
	for (int i = 0; i < 2400; i++)
	{
		updatePack[i] = uclear[i];
	}
	updatePackCount = 0;
}

void Server::analysis(_LPSOCKET_INFORMATION socketInfo)
{
	//Co the  chuyen ve mang. Sau nay se chuyen ve mang thay vi con tro
	char* tempData = nullptr;
	int readlen = 0;

	bool loop = true;
	//Khi nhan duoc flag 'sp' thi se bat dau doc du lieu theo thu tu GameObjectID -> FuncID -> Data
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

		if (readlen > 600)
		{
			MessageBox(NULL, L"Buffer Recv > 600 bytes", L"ERROR", NULL);
			delete uPack;
			uPack = nullptr;
			break;
		}

		eObjectId objectID;
		uint8_t inGameID;
		funcId funcID;

		//Get Object ID
		delete[] tempData;
		tempData = new char[1];
		memcpy(tempData, socketInfo->Buffer + readlen, 1);	readlen += 1;
		objectID = static_cast<eObjectId>((*tempData));

		//Get InGameID
		delete[] tempData;
		tempData = new char[1];
		memcpy(tempData, socketInfo->Buffer + readlen, 1);	readlen += 1;
		inGameID = static_cast<uint8_t>((*tempData));

		//Get FuncId
		delete[] tempData;
		tempData = new char[1];
		memcpy(tempData, socketInfo->Buffer + readlen, 1);	     readlen += 1;
		funcID = static_cast<funcId>((*tempData));

		//Get Data
		delete[] tempData;
		tempData = new char[1];
		memcpy(tempData, socketInfo->Buffer + readlen, 1);
		readlen += 1;

		//Debug co nhan dung du lieu hay khong. de day lau lau dung` lai
		edirection* moveEvent = reinterpret_cast<edirection*>(tempData);
		static edirection lastevent = UP;
		if (inGameID == 1)
		{
			if (lastevent == *moveEvent)
				int a = 0;
			else
				lastevent = *moveEvent;
		}

		//Add Event Update
		uPack->_updateEvent[eventCount] = createEvent(objectID, inGameID, funcID, tempData);	

		uPack->eventCount++;

		delete[] tempData;
		tempData = nullptr;
		eventCount++;
	}
}

void Server::processSend()
{
	_LPSOCKET_INFORMATION SocketInfo = NULL;

	//Khong co gi de gui thi khoi gui
	if (dataPack == nullptr)
	{
		return;
	}

	//Hoan thien goi data truoc khi gui. them flag ket thuc.
	dataPack->Buffer[dataPack->len] = 'e';
	dataPack->Buffer[dataPack->len + 1] = 'p';
	dataPack->len += 2;

	for (int i = 0; i < TotalSockets; i++)
	{	
		SocketInfo = SocketArray[i];

		//FD_SET(SocketInfo->Socket, &ReadSet);

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
		else
		{
			SocketInfo->BytesSEND += SendBytes;

			//Debug
			static int countt = 0;
			countt++;
			GAMELOG("SendDataPack() Successful!!!!			times: %d		Call From:: processSend", countt);

			//Khong biet de lam gi nhung cu de day
			//if (SocketInfo->BytesSEND == SocketInfo->BytesRECV)
			/*if (SocketInfo->BytesSEND != 0)
			{
				SocketInfo->BytesSEND = 0;
				SocketInfo->BytesRECV = 0;
			}*/
		}

	}
}

BOOL Server::CreateData(void* data, eObjectId objectid, uint8_t inGameID, funcId FuncID, uint32_t datasize)
{
	//TempData to cast
	char* dataconvert = new char(datasize);

	DataInfomation* tempDataPack = new DataInfomation();
	tempDataPack->len = 0;

	//Object ID
	(*dataconvert) = static_cast<char>(objectid);
	memcpy(tempDataPack->Buffer + tempDataPack->len, dataconvert, sizeof(uint8_t));
	tempDataPack->len += sizeof(uint8_t);

	//InGameID
	(*dataconvert) = static_cast<char>(inGameID);
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

	if (dataPack == nullptr)
	{
		dataPack = new DataInfomation();
		
		dataPack->Buffer[0] = 's';
		dataPack->Buffer[1] = 'p';
		dataPack->len = 2;
	}
	memcpy(dataPack->Buffer + dataPack->len, tempDataPack->Buffer, tempDataPack->len);
	dataPack->len += tempDataPack->len;
	
	delete tempDataPack;
	return TRUE;
}

BOOL Server::SendDataPack()
{
	processSend();

	//Xoa data truoc khi dong goi moi
	delete dataPack;
	dataPack = nullptr;
	return TRUE;
}

//AutoSend

BOOL Server::CreateEventData(void* data, eObjectId objectid, uint8_t inGameID, funcId FuncID, uint32_t datasize)
{
	char* dataconvert = new char(sizeof(uint8_t));
	char* dataconvert4 = new char(sizeof(uint32_t));
	char dataconvert5[4];

	if (tempASDataPack == nullptr)
		tempASDataPack = new DataInfomation();

	//[start flag]|[ObjectID][inGameID][FuncID][data]|...[end flag]|[start flag]|...|[end flag]|....
	//      2b        1b         1b       1b      nb         2b         2b            2b
	countPack++;
	static std::vector<uint32_t> GG;
	GG.push_back(countPack);

	//ID
	dataconvert4 = reinterpret_cast<char*>(&countPack);
	memcpy(tempASDataPack->Buffer + tempASDataPack->len, dataconvert4, sizeof(uint32_t));
	tempASDataPack->len += sizeof(uint32_t);

	//GameObject ID
	(*dataconvert) = static_cast<char>(objectid);
	memcpy(tempASDataPack->Buffer + tempASDataPack->len, dataconvert, sizeof(uint8_t));
	tempASDataPack->len += sizeof(uint8_t);

	//InGameID
	(*dataconvert) = static_cast<char>(inGameID);
	memcpy(tempASDataPack->Buffer + tempASDataPack->len, dataconvert, sizeof(uint8_t));
	tempASDataPack->len += sizeof(uint8_t);

	//FuncID
	(*dataconvert) = static_cast<char>(FuncID);
	memcpy(tempASDataPack->Buffer + tempASDataPack->len, dataconvert, sizeof(uint8_t));
	tempASDataPack->len += sizeof(uint8_t);

	//Data
	dataconvert = new char(datasize);
	dataconvert = reinterpret_cast<char*>(data);
	memcpy(tempASDataPack->Buffer + tempASDataPack->len, dataconvert, datasize);
	tempASDataPack->len += datasize;

	tempASDataPack->count++;

	return TRUE;
}

BOOL Server::wrapDataAutoSend()
{
	if (tempASDataPack == nullptr)
	{
		tempASDataPack = new DataInfomation();
		return TRUE;
	}

	if (tempASDataPack->len == 0)
		return TRUE;

	if (tempASDataPack->len > 600)
	{
		GAMELOG("tempASDataPack->len > 600				CALLFROM: Server::wrapDataAutoSend()");
		MessageBox(NULL, L"TempData to send > 600 byte				Server::wrapDataAutoSend()", L"ERROR", NULL);
	}

	if (autoSendDataPack != nullptr)
	{
		if (autoSendDataPack->len > 600)
		{
			GAMELOG("autoSendDataPack->len > 600				CALLFROM: Server::wrapDataAutoSend()");
		}
	}
	else
		autoSendDataPack = new DataInfomation();
		
	addStartFlag(autoSendDataPack);

	//char* flag = new char(2);

	memcpy(autoSendDataPack->Buffer + autoSendDataPack->len, tempASDataPack->Buffer, tempASDataPack->len);
	autoSendDataPack->len += tempASDataPack->len;
	addEndFlag(autoSendDataPack);

	tempASDataPack->reset();

	return TRUE;
}

BOOL Server::AutoSend(float deltatime)
{
	if (!bSyncTime)
		return FALSE;
	autoSendTime++;
	static int time = 0;
	time += deltatime;

	if (autoSendDataPack == nullptr)
		return FALSE;
	if (autoSendDataPack->len == 0)
		return FALSE;

	if (autoSendTime < 10)
	{
		//delete[] autoSendDataPack;
		//autoSendDataPack = nullptr;
		return FALSE;
	}

	_LPSOCKET_INFORMATION SocketInfo = NULL;
	
	for (int i = 0; i < TotalSockets; i++)
	{
		SocketInfo = SocketArray[i];

		SocketInfo->DataBuf.buf = autoSendDataPack->Buffer;
		storebuffer t;
		memcpy(t.t, autoSendDataPack->Buffer, 1024);
		F10EQ.push_back(t);
		SocketInfo->DataBuf.len = autoSendDataPack->len;

		if (WSASend(SocketInfo->Socket, &(SocketInfo->DataBuf), 1, &SendBytes, 0, NULL, NULL) == SOCKET_ERROR)
		{
			GAMELOG("Send Failed!!!!  %d						CALLFROM:Server::AutoSend()", WSAGetLastError());
			continue;
		}
		if (kobiksendchua)
		{
			GAMELOG("Send roi!");
			kobiksendchua = false;
		}
		
		static int coun = 0;
		if (time > 1000)
		{
			//GAMELOG("Send client per 120 frame:  %d", coun);
			coun = 0;
			time = 0;
		}
		if (i == 0)
		coun++;

	}

	autoSendDataPack->reset();
}

void Server::addStartFlag(DataInfomation* data)
{
	if (data == nullptr)
		data = new DataInfomation();
	data->Buffer[data->len] = 's';
	data->Buffer[data->len + 1] = 'p';
	data->len += 2;
}

void Server::addEndFlag(DataInfomation* data)
{
	if (data == nullptr)
	{
		GAMELOG("Chua co StartFlag ma them EndFlag					CallFrom: Server::addEndFlag(DataInfomation* data)");
		return;
	}
	data->Buffer[data->len] = 'e';
	data->Buffer[data->len + 1] = 'p';
	data->len += 2;
	if (data->len > 1000)
	{
		delete data;
		data = nullptr;
		data = new DataInfomation();
	}
}

//Controller
void Server::OnKeyDown(int keycode)
{
	switch (keycode)
	{
	case DIK_RETURN:
		break;
	}
}

void Server::playerJoin(SOCKET socket)
{
	_LPSOCKET_INFORMATION client = new _SOCKET_INFORMATION();
	client->Socket = socket;
	//Dong goi Pack khi player Join
	client->Buffer[0] = 'u';
	client->Buffer[1] = 'i';
	client->Buffer[2]= static_cast<char>(playerCount);
	client->Buffer[3] = 'e';
	client->Buffer[4] = 'p';
	client->DataBuf.buf = client->Buffer;
	client->DataBuf.len = 5;
	if (WSASend(client->Socket, &(client->DataBuf), 1, &SendBytes, 0, NULL, NULL) == SOCKET_ERROR)
	{
		GAMELOG("Send ERROR: %d						CALLFROM:Server::AutoSend()", WSAGetLastError());
	}
	//delete client;
}