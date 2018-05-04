#pragma once
#include "../GameScene.h"
#include "Client.h"
#include "../GameObject.h"
#include <vector>

class Updater
{
	GameScene* scene;
	NetWorkManage* client;
	std::vector<CompareUpdate> updateFSList;
	static Updater* inst;
public:
	static Updater* getInstance();
	Updater(GameScene* scene);
	void analysis();
	
	template <typename Type>
	Type getData(void* data,eObjectId objectId, funcId funcid);
	void ChecknUpdate();
};