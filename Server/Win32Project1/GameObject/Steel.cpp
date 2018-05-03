#include "Steel.h"

void Steel::Initialize()
{
	Sprite* steel = new Sprite();
	steel->Initialize();
	steel->SetAnimation(Tile_Steel);

	AddSteel(0, 0);

	SetPosition(100, 100);
}

void Steel::AddSteel(int x, int y)
{
	//Top - Left
	BrickSprite* steelSprite1 = new BrickSprite();
	steelSprite1->sprite.Initialize();
	steelSprite1->sprite.SetAnimation(Tile_Steel);
	steelSprite1->position.x = x * 2;
	steelSprite1->position.y = y * 2;

	//Top - Right
	BrickSprite* steelSprite2 = new BrickSprite();
	steelSprite2->sprite.Initialize();
	steelSprite2->sprite.SetAnimation(Tile_Steel);
	steelSprite2->position.x = x * 2 + 1;
	steelSprite2->position.y = y * 2;

	//Bottom - Left
	BrickSprite* steelSprite3 = new BrickSprite();
	steelSprite3->sprite.Initialize();
	steelSprite3->sprite.SetAnimation(Tile_Steel);
	steelSprite3->position.x = x * 2;
	steelSprite3->position.y = y * 2 + 1;

	//Bottom - Right
	BrickSprite* steelSprite4 = new BrickSprite();
	steelSprite4->sprite.Initialize();
	steelSprite4->sprite.SetAnimation(Tile_Steel);
	steelSprite4->position.x = x * 2 + 1;
	steelSprite4->position.y = y * 2 + 1;

	sprites.push_back(*steelSprite1);
	sprites.push_back(*steelSprite2);
	sprites.push_back(*steelSprite3);
	sprites.push_back(*steelSprite4);
}

void Steel::Draw()
{
	for (vector<BrickSprite>::iterator it = sprites.begin(); it != sprites.end(); ++it)
	{
		(*it).sprite.Render((*it).GetPosition().x * 8 + GetPosition().x + 4, (*it).GetPosition().y * 8 + GetPosition().y + 4);
	}
}