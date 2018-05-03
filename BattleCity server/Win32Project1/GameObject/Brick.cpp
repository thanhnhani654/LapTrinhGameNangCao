#include "Brick.h"

void Brick::Initialize()
{
	Sprite* brick = new Sprite();
	brick->Initialize();
	brick->SetAnimation(Tile_Brick1);

	AddBrick(0, 0);

	SetPosition(100, 100);
}

void Brick::AddBrick(int x, int y)
{
	//Top - Left
	BrickSprite* brickSprite1 = new BrickSprite();
	brickSprite1->sprite.Initialize();
	brickSprite1->sprite.SetAnimation(Tile_Brick1);
	brickSprite1->position.x = x  * 2;
	brickSprite1->position.y = y  * 2;

	//Top - Right
	BrickSprite* brickSprite2 = new BrickSprite();
	brickSprite2->sprite.Initialize();
	brickSprite2->sprite.SetAnimation(Tile_Brick1);
	brickSprite2->position.x = x * 2 + 1;
	brickSprite2->position.y = y * 2;

	//Bottom - Left
	BrickSprite* brickSprite3 = new BrickSprite();
	brickSprite3->sprite.Initialize();
	brickSprite3->sprite.SetAnimation(Tile_Brick1);
	brickSprite3->position.x = x * 2;
	brickSprite3->position.y = y * 2 + 1;

	//Bottom - Right
	BrickSprite* brickSprite4 = new BrickSprite();
	brickSprite4->sprite.Initialize();
	brickSprite4->sprite.SetAnimation(Tile_Brick1);
	brickSprite4->position.x = x * 2 + 1;
	brickSprite4->position.y = y * 2 + 1;

	sprites.push_back(*brickSprite1);
	sprites.push_back(*brickSprite2);
	sprites.push_back(*brickSprite3);
	sprites.push_back(*brickSprite4);
}

void Brick::Draw()
{
	for (vector<BrickSprite>::iterator it = sprites.begin(); it != sprites.end(); ++it)
	{
		(*it).sprite.Render((*it).GetPosition().x * 8 + GetPosition().x + 4, (*it).GetPosition().y * 8 + GetPosition().y + 4);
	}
}