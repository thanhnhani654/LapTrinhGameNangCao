#include "GameScene.h"

void GameScene::Init()
{
	//LoadMapFromFile();
	player = new Player();
	player->Initialize();

	Sprite::LoadSpritePos();
}

void GameScene::UpdateInput()
{
	player->UpdateInput();
}

void GameScene::Update(float deltatime)
{
	player->Update(deltatime);
}

void GameScene::Draw()
{
	player->Draw();
	//int t = bricks.max_size();
	//for (vector<Object*>::iterator it = bricks.begin(); it != bricks.end(); ++it)
	//{
	//	(*it)->Draw();
	//}
}

void GameScene::LoadMapFromFile()
{
	fstream map;
	map.open("Resources/map.txt", ios::in);

	if (map.fail())
	{
		MessageBox(NULL, TEXT("Map not found!"), TEXT("ERROR"), NULL);
		return;
	}

	int width = 13;
	int height = 14;

	int maps[14][13];
	int a;

	for (int y = height-1; y >= 0 ; y--)
	{
		for (int x = 0; x < width; x++)
		{
			map >> maps[y][x];
		}
	}

	map.close();

	//Debug Map
	/*ofstream map1;
	map1.open("Resources/maptest.txt");

	for (int y = height - 1; y > 0; y--)
	{
		for (int x = 0; x < width; x++)
		{
			map1 << maps[y][x];
		}
		map1 << '\n';
	}

	map1.close();*/

	for (int y = height - 1; y >= 0; y--)
	{
		for (int x = 0; x < width; x++)
		{
			int tempx = x;
			int tempy = y;
			switch (maps[y][x])
			{
			case 1:
			{
				Brick* brick = new Brick();
				brick->Initialize();
				brick->SetPosition(x * 16, y * 16);
				bricks.push_back(brick);

				while (1)
				{
					while (1)
					{
						if (maps[tempy - 1][tempx] == 1)
							tempy--;
						else
							break;

						brick->AddBrick(tempx - x, tempy - y);
						maps[tempy][x] = 0;
					}

					if (maps[y][tempx + 1] == 1)
					{
						tempx++;
					}
					else
						break;
					brick->AddBrick(tempx - x, 0);
					maps[y][tempx] = 0;
				}
				break;
			}
			case 2:
			{
				Steel* steel = new Steel();
				steel->Initialize();
				steel->SetPosition(x * 16, y * 16);
				bricks.push_back(steel);

				while (1)
				{
					while (1)
					{
						if (maps[tempy - 1][tempx] == 2)
							tempy--;
						else
							break;

						steel->AddSteel(tempx - x, tempy - y);
						maps[tempy][x] = 0;
					}

					if (maps[y][tempx + 1] == 2)
					{
						tempx++;
					}
					else
						break;
					steel->AddSteel(tempx - x, 0);
					maps[y][tempx] = 0;
				}
			}
				break;
			}
		}
	}

}

Player* GameScene::getPlayer()
{
	return player;
}