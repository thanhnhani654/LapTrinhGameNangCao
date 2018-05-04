#include "Updater.h"

Updater* Updater::inst;

Updater* Updater::getInstance()
{
	if (inst == nullptr)
		return nullptr;
	return inst;
}

Updater::Updater(GameScene* scene)
{
	this->scene = scene;
	client = NetWorkManage::getInstance();
	inst = this;
}

//Khi client chay va thay doi su kien lien tuc tao ra su khac biet hoi bi ro giua client va server.
// Chuan doan ban dau cho thay nguyen nhan co the do FPS
void Updater::analysis()
{
	char* tempData = new char(DEFAULT_BUFFER);
	char* temp = nullptr;
	memcpy(tempData, client->szBuffer, client->ret);
	bool startread = false;
	int readlen = 0;

	CompareUpdate comUp;

	char tempflag[2];
	char tempObjectId[4];
	char tempfuncID[4];

	while (true)
	{
		/*if (temp != nullptr)
			delete[] temp;
		temp = new char[2];*/
		
		//memcpy(temp, tempData + readlen, 2);
		tempflag[0] = client->szBuffer[readlen];
		tempflag[1] = client->szBuffer[readlen+1];
		
		if (!startread)
			if (tempflag[0] == 's' && tempflag[1] == 'p')
			{
				startread = true;
				readlen += 2;
			}
			else
			{
				break;
			}

		//check end Update Pack
		if (startread)
			if (tempflag[0] == 'e' && tempflag[1] == 'p')
			{
				startread = false;
				readlen += 2;
				continue;
			}

		if (readlen > 100)
		{
			break;
		}

		if (readlen == 18)
			int WTF = 0;
		//Get Object ID
		/*if (temp != nullptr)
		delete[] temp;		temp = nullptr;
		temp = new char[4];*/
		memcpy(tempObjectId, tempData + readlen, 4);	readlen += 4;
		comUp.objectid = static_cast<eObjectId>((*tempObjectId));

		//Get FuncId
		/*if (temp != nullptr)
		delete[] temp;		temp = nullptr;
		temp = new char[4];*/
		memcpy(tempfuncID, tempData + readlen, 4);	     readlen += 4;
		comUp.funcid = static_cast<funcId>((*tempfuncID));

		//Get Data
		switch (comUp.objectid)
		{
		case eObjectId::GameObject:
			switch (comUp.funcid)
			{
			case funcId::Pl_Position:
				comUp.data = new char(sizeof(D3DXVECTOR2));
				memcpy(comUp.data, client->szBuffer + readlen, sizeof(D3DXVECTOR2));
				readlen += sizeof(D3DXVECTOR2);
				D3DXVECTOR2* vec;
				vec = reinterpret_cast<D3DXVECTOR2*>(comUp.data);
			}
			break;
		default:
			break;
		}

		updateFSList.push_back(comUp);
	}
}

//template <typename Type>
//Type Updater::getData(void* data, eObjectId objectId, funcId funcid)
//{
//	switch (objectId)
//	{
//	case eObjectId:
//		switch (funcid)
//		{
//		case funcId::Pl_Position:
//
//		}
//	}
//}

void Updater::ChecknUpdate()
{
	if (updateFSList.empty())
		return;

	for (std::vector<CompareUpdate>::iterator it = updateFSList.begin(); it != updateFSList.end(); it++)
	{
		switch ((*it).objectid)
		{
		case eObjectId::GameObject:
			switch ((*it).funcid)
			{
			case funcId::Pl_Position:
				D3DXVECTOR2* position = reinterpret_cast<D3DXVECTOR2*>((*it).data);
				scene->getPlayer()->getTank()->SetPosition(position->x, position->y);
				break;
			}
			break;
		}
	}
	updateFSList.clear();
}