#pragma once
#include "../GameScene.h"

struct SnapShoot
{
	GameScene scene;
	float militime;
};

class Memento
{
	SnapShoot* snapShoot;
public:
	//Chua viet phuong thuc xoa
	
	Memento(SnapShoot snapShoot);
	SnapShoot* getSnapShoot();
	void deleteSnapShoot();
};