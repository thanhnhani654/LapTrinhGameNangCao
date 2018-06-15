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
	
	Memento(SnapShoot snapShoot);
	SnapShoot* getSnapShoot();
	void deleteSnapShoot();
};