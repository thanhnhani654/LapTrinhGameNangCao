#include "Brick.h"
#include "../GameLog/GameLog.h"

void Brick::Initialize()
{
	id = eID::ID_Brick;
	brick = new Sprite();
	brick->Initialize();
	brick->SetAnimation(Tile_Brick1);
	Object::Initialize();
}

void Brick::AddBrick(int x, int y)
{
	D3DXVECTOR2 temp;
	//Top - Left
	BrickSprite* brickSprite1 = new BrickSprite();
	brickSprite1->sprite = *brick;
	brickSprite1->position.x = x  * 2;
	brickSprite1->position.y = y  * 2;
	temp.x = brickSprite1->position.x * 8 + GetPosition().x + 8;
	temp.y = brickSprite1->position.y * 8 + GetPosition().y + 8;
	//temp.x = 32;
	//temp.y = 0;
	boxTL = new Box2D(temp, 8, 8, this);
	brickSprite1->box = boxTL;

	//Top - Right
	BrickSprite* brickSprite2 = new BrickSprite();
	brickSprite2->sprite = *brick;
	brickSprite2->position.x = x * 2 + 1;
	brickSprite2->position.y = y * 2;
	temp.x = brickSprite2->position.x * 8 + GetPosition().x + 8;
	temp.y = brickSprite2->position.y * 8 + GetPosition().y + 8;
	boxTR = new Box2D(temp, 8, 8, this);
	brickSprite2->box = boxTR;

	//Bottom - Left
	BrickSprite* brickSprite3 = new BrickSprite();
	brickSprite3->sprite = *brick;
	brickSprite3->position.x = x * 2;
	brickSprite3->position.y = y * 2 + 1;
	temp.x = brickSprite3->position.x * 8 + GetPosition().x + 8;
	temp.y = brickSprite3->position.y * 8 + GetPosition().y + 8;
	boxBL = new Box2D(temp, 8, 8, this);
	brickSprite3->box = boxBL;

	//Bottom - Right
	BrickSprite* brickSprite4 = new BrickSprite();
	brickSprite4->sprite = *brick;
	brickSprite4->position.x = x * 2 + 1;
	brickSprite4->position.y = y * 2 + 1;
	temp.x = brickSprite4->position.x * 8 + GetPosition().x + 8;
	temp.y = brickSprite4->position.y * 8 + GetPosition().y + 8;
	boxBR = new Box2D(temp, 8, 8, this);
	brickSprite4->box = boxBR;

	sprites.push_back(*brickSprite1);
	sprites.push_back(*brickSprite2);
	sprites.push_back(*brickSprite3);
	sprites.push_back(*brickSprite4);
}

void Brick::Draw()
{
	for (vector<BrickSprite>::iterator it = sprites.begin(); it != sprites.end(); ++it)
	{
		if ((*it).disable)
			continue;
		(*it).sprite.Render((*it).GetPosition().x * 8 + GetPosition().x + 4, (*it).GetPosition().y * 8 + GetPosition().y + 4);
	}
}

void Brick::onCollision(Object* object)
{
	for (std::vector<BrickSprite>::iterator it = sprites.begin(); it != sprites.end(); it++)
	{
		if ((*it).box->disable)
			continue;
		if ((*it).box->getShot && (*it).truelydisable)
		{
			(*it).box->disable = true;
		}
	}
}

void Brick::checkDisable(D3DXVECTOR2 pos)
{
	for (std::vector<BrickSprite>::iterator it = sprites.begin(); it != sprites.end(); it++)
	{
		if ((*it).GetPosition() == pos)
		{
			(*it).truelydisable = true;
			//(*it).box->disable = true;
			//GAMELOG("Client:	x:%f		y:%f", pos.x, pos.y);
		}

		if ((*it).disable != (*it).truelydisable)
		{
			(*it).disable = (*it).truelydisable;
			(*it).box->disable = (*it).truelydisable;
		}
	}
}

void Brick::destroy()
{
	deleted = true;
}