#include "Sprite.h"


LPDIRECT3DDEVICE9 Sprite::d3ddv;

vector <SpritePos> Sprite::spritepos;

void Sprite::GetDevice(LPDIRECT3DDEVICE9 id3ddv)
{
	d3ddv = id3ddv;
}

void Sprite::LoadSprite(LPCWSTR Path, int Width, int Height)
{
	D3DXIMAGE_INFO info;
	HRESULT result;
	_SpriteHandler = NULL;
	_Image = NULL;

	result = D3DXCreateSprite(d3ddv, &_SpriteHandler);

	if (result != DI_OK)
		return;

	result = D3DXGetImageInfoFromFile(Path, &info);

	result = D3DXCreateTextureFromFileEx(
		d3ddv,
		Path,
		info.Width,
		info.Height,
		1,
		D3DUSAGE_DYNAMIC,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DCOLOR_XRGB(0, 0, 0),
		&info,
		NULL,
		&_Image);
}

void Sprite::Initialize()
{
	LoadSprite(TEXT("Resources/Sprites.png"), 400, 256);
}

void Sprite::Render(float X, float Y)
{

	HRESULT hr;

	RECT rect;
	rect.left =  spritepos[index].x;
	rect.top =  spritepos[index].y;
	rect.bottom = spritepos[index].y + spritepos[index].h;
	rect.right = spritepos[index].x + spritepos[index].w;

	_SpriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

	D3DXVECTOR3 position(X, Y, 0);

	D3DXMATRIX mt;
	D3DXMatrixIdentity(&mt);
	mt._11 = 1.0f;
	mt._22 = -1.0f;
	mt._41 = 8;
	mt._42 = 225;
	D3DXVECTOR4 vp_pos;

	D3DXVec3Transform(&vp_pos, &position, &mt);

	D3DXVECTOR3 p(vp_pos.x, vp_pos.y, 0);

	D3DXVECTOR3 center(spritepos[index].w / 2, spritepos[index].h / 2, 0);

	hr = _SpriteHandler->Draw(
		_Image,
		&rect,
		&center,
		&p,
		D3DCOLOR_XRGB(255, 255, 255)
	);

	_SpriteHandler->End();
}

void Sprite::Next()
{
	index += 1;
	if (index < begin || index > end)
	{
		index = begin;
	}
}


//Used for Tile and other
void Sprite::SetAnimation(AnimationName eanim)
{
	currentAnim = eanim;
	index = eanim;
	begin = eanim;
	end = eanim;
}

//Only Used for Tank
void Sprite::SetAnimation(int i)
{
	if (i > 255)
	{
		MessageBox(NULL, TEXT("SetAnimation only use for Tank"), TEXT("Error"), NULL);
		return;
	}
	begin = i * 2;
	end = i * 2 + 1;

	//For Check Load Sprite true or false
	/*begin = i ;
	end = i ;*/
}


void Sprite::LoadSpritePos()
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("Resources/sprite-data.xml");

	if (!result)
	{
		MessageBox(NULL, TEXT("Sprite not found!"), TEXT("LoadSpritePos Error"), NULL);
	}

	pugi::xml_node tools = doc.child("sprite-data");

	int i = 0;

	SpritePos tempspritepos;

	for (pugi::xml_node tool = doc.first_child(); tool; tool = tool.next_sibling())
	{
		for (pugi::xml_node tool2 = tool.first_child(); tool2; tool2 = tool2.next_sibling())
		{
			// Get X
			pugi::xml_node tool3 = tool2.first_child();
			tempspritepos.x = atoi(tool3.child_value());
			tool3 = tool3.next_sibling();
			
			//Get Y
			tempspritepos.y = atoi(tool3.child_value());
			tool3 = tool3.next_sibling();

			//Get W
			tempspritepos.w = atoi(tool3.child_value());
			tool3 = tool3.next_sibling();

			//Get H
			tempspritepos.h = atoi(tool3.child_value());
			tool3 = tool3.next_sibling();

			spritepos.push_back(tempspritepos);

			i++;
		}
	}
}
