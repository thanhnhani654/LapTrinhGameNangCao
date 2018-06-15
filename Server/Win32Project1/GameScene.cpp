#include "GameScene.h"

void GameScene::Init()
{
	player1.Initialize();
	player1.getTankPtr()->SetPosition(0, 8);
	player2.Initialize();
	player2.getTankPtr()->SetPosition(12*16,8);
	player3.Initialize();
	player3.getTankPtr()->SetPosition(0, 8 + 13*16);
	player4.Initialize();
	player4.getTankPtr()->SetPosition(12 * 16, 8 + 13 * 16);

	LoadMapFromFile();
	//player = new Player();
	//Map Bounding
	bound1.Initialize(-100, 0, 100, 500);
	bound2.Initialize(0, -92, 300, 100);
	bound3.Initialize(208, 0, 100, 500);
	bound4.Initialize(0, 232, 300, 100);

	Sprite::LoadSpritePos();
	time.getCurrentTime();
}

void GameScene::UpdateInput(float deltatime)
{
	if (!player1.disable)
		player1.UpdateInput(deltatime);
	if (!player2.disable)
		player2.UpdateInput(deltatime);
	if (!player3.disable)
		player3.UpdateInput(deltatime);
	if (!player4.disable)
		player4.UpdateInput(deltatime);
}

void GameScene::Update(float deltatime)
{
	//xoa
	time.getCurrentTime();
	if (!player1.disable)
		player1.Update(deltatime);
	if (!player2.disable)
		player2.Update(deltatime);
	if (!player3.disable)
		player3.Update(deltatime);
	if (!player4.disable)
		player4.Update(deltatime);
	
	Box2D::destroyProcess();
	Object::DestroyProcess();
	
}

void GameScene::Draw()
{
	//if (!player1.disable)
		player1.Draw();
	//if (!player2.disable)
		player2.Draw();
	//if (!player3.disable)
		player3.Draw();
	//if (!player4.disable)
		player4.Draw();
	for (int i = 0; i < bricks.size(); i++)
	{
		bricks[i]->Draw();
	}
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


	//static int d = 0;
	//d++;
	//if (d == 1)
	//	return;


	static int count = 0;
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
				brick->SetPosition(x * 16 - 1 - 8, y * 16 );
				brick->AddBrick(0, 0);
				bricks.push_back(brick);
			
				

				while (1)
				{
					while (1)
					{
						count++;
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
				
			}
				break;
			case 2:
			{
				
				Steel* steel = new Steel();
				steel->Initialize();
				steel->SetPosition(x * 16 -1 -8, y * 16 );
				bricks.push_back(steel);

				while (1)
				{
					while (1)
					{
						count++;
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


Player* GameScene::getPlayer(int id)
{
	switch (id)
	{
	case 1:
		return &this->player1;
	case 2:
		return &this->player2;
	case 3:
		return &this->player3;
	case 4:
		return &this->player4;
	}
	return nullptr;
	
}