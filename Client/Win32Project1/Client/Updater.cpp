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

void Updater::analysis()
{
	bool startread = false;
	int readlen = 0;

	CompareUpdate comUp;

	char tempflag[2];
	char tempObjectId[4];
	char tempfuncID[4];
	while (true)
	{
		tempflag[0] = client->szBuffer[readlen];
		tempflag[1] = client->szBuffer[readlen+1];
		
		//Check StartRead
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

		if (readlen > 600)
		{
			GAMELOG("OVERFLOW				Call From:Updater::analysis()");
			break;
		}

		//Get Object ID
		memcpy(tempObjectId, client->szBuffer + readlen, 1);	readlen += 1;
		comUp.objectid = static_cast<eObjectId>((*tempObjectId));

		//Get FuncId
		memcpy(tempfuncID, client->szBuffer + readlen, 1);	     readlen += 1;
		comUp.funcid = static_cast<funcId>((*tempfuncID));

		//Get Data
		//Can viet 1 ham rieng
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

void Updater::ChecknUpdate()
{
	if (updateFSList.empty())
		return;

	for (std::vector<CompareUpdate>::iterator it = updateFSList.begin(); it != updateFSList.end(); it++)
	{
		//Chua viet ham UpdateState rieng
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