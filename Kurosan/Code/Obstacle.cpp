#include "Obstacle.h"
#include "DxLib.h"

Obstacle::Obstacle(int type, int handle):type(type),image(handle),speed(0.0f),location(0.0f),box_size(0.0f)
{

	/*type = type;
	image = handle;
	speed = 0.0f;
	location = 0.0f;
	box_size = 0.0f;*/
}

Obstacle::~Obstacle()
{

}

void Obstacle::Initialize(int type ,int count)
{

	float random_x = 0.0f;

	
	////出現Xパターンを取得
	//random_x = (float)(GetRand(4) * 105 + 40);

	if (type == 0)
	{
		random_x = count * 105 + 40;
	}
	else
	{
		random_x = (float)(GetRand(4) * 105 + 40);
	}

	
	//生成位置の設定
	location = Vector2D(random_x, -50.0f);
	//当たり判定の設定
	box_size = Vector2D(31.0f, 60.0f);
	//速さの設定
	speed = (float)(this->type * 2);
}

void Obstacle::Update(float speed)
{

	//位置情報に移動量を加算
	location += Vector2D(0.0f, this->speed + speed - 6);
}

void Obstacle::Draw() const
{
	if (location.x <= 250)
	{

		DrawRotaGraphF(location.x, location.y, 1.0, 0.0, image, TRUE, TRUE);
	}
}

void Obstacle::Finalize()
{

}

int Obstacle::GetType() const
{

	return type;
}

Vector2D Obstacle::GetLocation() const
{

	return location;
}

Vector2D Obstacle::GetBoxSize() const
{

	return box_size;
}