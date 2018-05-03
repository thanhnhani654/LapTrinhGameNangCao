#pragma once
#include "../GameScene.h"

struct SnapShoot
{
	GameScene scene;
	float militime;
};

class Memento
{

public:
	//Chua viet phuong thuc xoa
	SnapShoot* snapShoot;
	Memento(SnapShoot snapShoot);
	SnapShoot* getSnapShoot();
};