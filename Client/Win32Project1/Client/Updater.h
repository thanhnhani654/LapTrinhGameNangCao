#pragma once
#include "../GameScene.h"
#include "Client.h"
#include "../GameObject.h"
#include <vector>
#include "../Player.h"
#include "../LoadingScreen.h"

struct storepack
{
	char t[1024];
};

class Updater
{
	GameScene* scene;
	NetWorkManage* client;
	std::vector<CompareUpdate> updateFSList;
	std::vector<CompareUpdate> GG;
	std::vector<CompareUpdate> GG2;
	std::vector<storepack> F10EQ;
	static Updater* inst;
	int ping;
	uint32_t newestEvent;

	bool OverFlowed;
	char buffer[20480];
	int len;
	
public:
	
	D3DXVECTOR2 check1;
	D3DXVECTOR2 check2;

	static Updater* getInstance();
	Updater(GameScene* scene);
	void analysis();
	void reAnalysis();
	
	template <typename Type>
	//That bai cua tao hoa
	Type getData(void* data,eObjectId objectId, funcId funcid);
	void ChecknUpdate();

	void updateState(CompareUpdate pack);
	void updateID();
	void updatePlayer(int len);

	//Dung Game Khi Lag
	bool bUpdateGame;
	float  waitingTime;
	void isUpdateGame(float deltatime);
};