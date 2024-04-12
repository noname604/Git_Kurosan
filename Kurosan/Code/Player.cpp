#include "Player.h"
#include"../Utility/InputControl.h"
#include"Dxlib.h"

#define RUN				(0)
#define RUN1			(1)
#define RUN2			(2)
#define HIT				(3)
#define HIT1			(4)
#define HIT2			(5)
#define JUMP			(6)
#define JUMP1			(7)
#define JUMP2			(8)
#define JUMP3			(9)
#define SLIDING			(10)
#define CAR				(11)

#define RUN_SPAN		(105)
#define HIT_SPAN		(60)
#define JUMP_SPAN		(60)
#define SLIDING_SPAN	(60)
#define INVINCIBLETIME	(60)

Player::Player() :is_active(false), image(NULL), location(0.0f), box_size(0.0f), angle(0.0f),
speed(0.0f), hp(0.0f), fuel(0.0f), barrier_count(0), barrier(nullptr),
images{}, situation(0), hit(FALSE), red(FALSE), run_span(0), hit_span(0), jump_span(0), sliding_span(0), invincibletime(0)
{

}

Player::~Player()
{

}

//初期化処理
void Player::Initialize()
{
	is_active = true;
	location = Vector2D(250.0f, 380.0f);
	box_size = Vector2D(19.0f, 30.0f);
	angle = 0.0f;
	speed = 3.0f;
	hp = 1000;
	fuel = 20000;
	barrier_count = 3;

	//画像の読み込み
	images[RUN1] = LoadGraph("Resource/images/player/run1.png");			//走る画像1
	images[RUN2] = LoadGraph("Resource/images/player/run2.png");			//走る画像2
	images[HIT1] = LoadGraph("Resource/images/player/hit1.png");			//ヒット画像1
	images[HIT2] = LoadGraph("Resource/images/player/hit2.png");			//ヒット画像2
	images[JUMP1] = LoadGraph("Resource/images/player/jump1.png");			//ジャンプ画像1
	images[JUMP2] = LoadGraph("Resource/images/player/jump2.png");			//ジャンプ画像2
	images[JUMP3] = LoadGraph("Resource/images/player/jump3.png");			//ジャンプ画像3
	images[SLIDING] = LoadGraph("Resource/images/player/sliding1.png");		//スライディング画像
	images[CAR] = LoadGraph("Resource/images/player/car.png");				//車画像

	image = images[RUN1];
	situation = RUN;
	hit = FALSE;

	run_span = RUN_SPAN;
	hit_span = 0;
	jump_span = 0;
	sliding_span = 0;
	invincibletime = 0;

	//エラーチェック
	if (image == -1)
	{
		throw("Resource/images/car1pol.bmpがありません\n");
	}
}

//更新処理
void Player::Update()
{
	//操作不可状態であれば、自信を回転させる
	if (!is_active)
	{
		if (situation == RUN)
		{
			hit = TRUE;
			hit_span = HIT_SPAN;
		}

		
		invincibletime = INVINCIBLETIME;
		is_active = true;
	}

	//燃料の消費
	fuel -= speed;

	//移動処理
	Movement();

	//加減速処理
	Acceleration();

	//フレームの計算
	if (run_span >= 0)
	{
		run_span -= speed;
	}
	if (hit_span >= 0)
	{
		hit_span -= 1;
	}
	if (jump_span >= 0)
	{
		jump_span -= 1;
	}
	if (sliding_span >= 0)
	{
		sliding_span -= 1;
	}
	//無敵時間
	if (invincibletime >= 0)
	{
		invincibletime -= 1;
	}

	//プレイヤーの状態の更新
	if (jump_span <= 0 && sliding_span <= 0)
	{
		situation = RUN;
	}
	if (jump_span == 1)
	{
		image = images[RUN1];
	}
	if (hit_span == 0)
	{
		hit = FALSE;
	}
	if(invincibletime==0)
	{
		image = images[RUN1];
	}
	
	//当たり判定の変化
	if (situation == RUN)
	{
		box_size = Vector2D(19.0f, 30.0f);
	}
	if (IsCarMode())
	{
		box_size= Vector2D(30.0f, 70.0f);
	}


	//ジャンプ処理
	if (InputControl::GetButtonDown(XINPUT_BUTTON_A))
	{
		Jump();
	}

	//スライディング処理
	if (InputControl::GetButtonDown(XINPUT_BUTTON_B))
	{
		Sliding();
	}

	//テスト用処理
	if (InputControl::GetButtonDown(XINPUT_BUTTON_Y))
	{
		//situation = CAR;
	}

	//アニメーション
	HitAnimation();
	RunAnimation();
	JumpAnimation();

	if (situation == CAR)
	{
		image = images[CAR];
	}
}

//描画処理
void Player::Draw()
{
	//プレイヤー画像の描画
	DrawRotaGraphF(location.x, location.y-20, 1.0, angle, image, TRUE);

	//当たり判定描画
	//DrawBox(location.x + box_size.x, location.y + box_size.y, location.x - box_size.x, location.y - box_size.y, GetColor(0, 0, 255), FALSE);

	//バリアが生成されていたら、描画を行う
	if (barrier != nullptr)
	{
		barrier->Draw(this->location);
	}
}

//終了時処理
void Player::Finalize()
{
	//読み込んだ画像を削除
	DeleteGraph(image);

	//バリアが生成されていたら、削除する
	if (barrier != nullptr)
	{
		delete barrier;
	}
}

//状態設定処理
void Player::SetActive(bool flg)
{
	this->is_active = flg;
}

//体力減少処理
void Player::DecreaseHp(float value)
{
	this->hp += value;
}

//鍵取得
void Player::GetKey()
{
	situation = CAR;
	invincibletime = INVINCIBLETIME * 5;
	image = images[CAR];

}

//位置情報取得処理
Vector2D Player::GetLocation()const
{
	return this->location;
}

//当たり判定の大きさ取得処理
Vector2D Player::GetBoxSize()const
{
	return this->box_size;
}

//速さ取得処理
float Player::GetSpeed()const
{
	return this->speed;
}

//燃料取得処理
float Player::GetFuel()const
{
	return this->fuel;
}

//体力取得処理
float Player::GetHp()const
{
	return this->hp;
}

//バリア枚数取得処理
int Player::GetBarriarCount()const
{
	return this->barrier_count;
}

//バリア有効か?を取得
bool Player::IsBarrier()const
{
	return(barrier != nullptr);
}

//ジャンプの真偽を取得
bool Player::IsJump()const
{
	if (situation == JUMP)
	{
		return TRUE;
	}
	return FALSE;
}

//スライディングの真偽を取得
bool Player::IsSliding()const
{
	if (situation == SLIDING)
	{
		return TRUE;
	}
	return FALSE;
}

//車モードの真偽を取得
bool Player::IsCarMode()
{
	if (image == images[CAR])
	{
		return TRUE;
	}
	return FALSE;
}

//無敵状態の真偽を取得
bool Player::IsInvincible()
{
	if (invincibletime > 0)
	{
		return TRUE;
	}
	return FALSE;
}

//移動処理
void Player::Movement()
{
	Vector2D move = Vector2D(0.0f);
	angle = 0.0f;

	//十字移動処理
	if (InputControl::GetButton(XINPUT_BUTTON_DPAD_LEFT))
	{
		move += Vector2D(-3.0f, 0.0f);
		angle = -DX_PI_F / 18;
	}
	if (InputControl::GetButton(XINPUT_BUTTON_DPAD_RIGHT))
	{
		move += Vector2D(3.0f, 0.0f);
		angle = DX_PI_F / 18;
	}
	if (InputControl::GetButton(XINPUT_BUTTON_DPAD_UP))
	{
		//move += Vector2D(0.0f, -1.0f);
	}
	if (InputControl::GetButton(XINPUT_BUTTON_DPAD_DOWN))
	{
		//move += Vector2D(0.0f, 1.0f);
	}

	//スティック移動処理
	if (InputControl::GetLeftStick().x < -0.2)
	{
		move += Vector2D(-3.0f, 0.0f);
		angle = -DX_PI_F / 18;
	}
	if (InputControl::GetLeftStick().x > 0.2)
	{
		move += Vector2D(3.0f, 0.0f);
		angle = DX_PI_F / 18;
	}
	if (InputControl::GetLeftStick().y < -0.2)
	{
		//move += Vector2D(0.0f, -1.0f);
	}
	if (InputControl::GetLeftStick().y > 0.2)
	{
		//move += Vector2D(0.0f, 1.0f);
	}

	location += move;

	//画面外に行かないように制限する
	if ((location.x < box_size.x) || (location.x >= 640.0f - 180.0f) || (location.y < box_size.y) || (location.y >= 480.0f - box_size.y))
	{
		location -= move;
	}
}

//加減速処理
void Player::Acceleration()
{
	//LBボタンが押されたら、減速する
	if (InputControl::GetButtonDown(XINPUT_BUTTON_LEFT_SHOULDER) && speed > 1.0f)
	{
		speed -= 1.0f;
	}

	//RBボタンが押されたら、加速する
	if (IsCarMode())
	{
		if (InputControl::GetButtonDown(XINPUT_BUTTON_RIGHT_SHOULDER) && speed < 20.0f)
		{
			speed += 1.0f;
		}
	}
	else
	{
		if (InputControl::GetButtonDown(XINPUT_BUTTON_RIGHT_SHOULDER) && speed < 10.0f)
		{
			speed += 1.0f;
		}
	}

	if (IsCarMode())
	{
		speed = 20.0f;
	}

	if (!IsCarMode() && speed >= 20.0f)
	{
		speed = 10.0f;
	}
}

//ジャンプ処理
void Player::Jump()
{
	//プレイヤーが走っているならジャンプする
	if (situation == RUN)
	{
		situation = JUMP;

		//ジャンプ有効時間を設定する
		jump_span = JUMP_SPAN;
	}	
}

//スライディング処理
void Player::Sliding()
{
	//プレイヤーが走っているならスライディングする
	if (situation == RUN)
	{
		situation = SLIDING;

		//画像をスライディングにする
		image = images[SLIDING];

		//スライディング有効時間を設定する
		sliding_span = SLIDING_SPAN;
	}	
}

//走るアニメーション
void Player::RunAnimation()
{
	if (situation == RUN)
	{
		if (run_span <= 0)
		{
			if (image == images[RUN1] || image == images[HIT1])
			{
				image = images[RUN2];

			}
			else if (image == images[RUN2] || image == images[HIT2] || image == images[JUMP3] || image == images[SLIDING])
			{
				image = images[RUN1];

			}	
			run_span = RUN_SPAN;
		}

		if (red == TRUE)
		{
			if (image == images[RUN1])
			{
				image = images[HIT1];
			}
			else if (image == images[RUN2])
			{
				image = images[HIT2];
			}
		}
		else
		{
			if (image == images[RUN1])
			{
				image = images[RUN1];
			}
			else if (image == images[RUN2])
			{
				image = images[RUN2];
			}
		}
	}	
}

//ヒットアニメーション
void Player::HitAnimation()
{
	if (hit == TRUE)
	{
		if (hit_span > 0)
		{
			if (hit_span <= HIT_SPAN && hit_span > HIT_SPAN - (HIT_SPAN / 8))
			{
				red = TRUE;
			}
			else if (hit_span <= HIT_SPAN - (HIT_SPAN / 8) && hit_span > HIT_SPAN - ((HIT_SPAN / 8) * 2))
			{
				red = FALSE;
			}
			else if (hit_span <= HIT_SPAN - ((HIT_SPAN / 8) * 2) && hit_span > HIT_SPAN - ((HIT_SPAN / 8) * 3))
			{
				red = TRUE;
			}
			else if (hit_span <= HIT_SPAN - ((HIT_SPAN / 8) * 4) && hit_span > HIT_SPAN - ((HIT_SPAN / 8) * 5))
			{
				red = FALSE;
			}
			else if (hit_span <= HIT_SPAN - ((HIT_SPAN / 8) * 5) && hit_span > HIT_SPAN - ((HIT_SPAN / 8) * 6))
			{
				red = TRUE;
			}
			else if (hit_span <= HIT_SPAN - ((HIT_SPAN / 8) * 6) && hit_span > HIT_SPAN - ((HIT_SPAN / 8) * 7))
			{
				red = FALSE;
			}
			else if ( hit_span > 0)
			{
				red = TRUE;
			}
		}
	}
	if (hit_span < 0)
	{
		red = FALSE;
		//hit = FALSE;
	}
}

//ジャンプアニメーション
void Player::JumpAnimation()
{
	if (situation == JUMP)
	{
		if (jump_span <= JUMP_SPAN && jump_span > JUMP_SPAN - (JUMP_SPAN / 3))
		{
			image = images[JUMP1];
		}
		if (jump_span <= JUMP_SPAN - (JUMP_SPAN / 3) && jump_span > (JUMP_SPAN / 3))
		{
			image = images[JUMP2];
		}
		if (jump_span <= (JUMP_SPAN / 3) && jump_span > 0)
		{
			image = images[JUMP3];
		}
		if (jump_span == 0)
		{
			image = images[RUN2];
		}
	}
}