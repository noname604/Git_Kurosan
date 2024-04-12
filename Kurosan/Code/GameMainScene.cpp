#include "GameMainScene.h"
#include"RankingData.h"
#include"DxLib.h"
#include<math.h>

GameMainScene::GameMainScene() : high_score(0), back_ground(NULL), barrier_image(NULL), mileage(0), nowscore(0), coin(0), item_get_se(0),getkey(0),
player(nullptr), obstacle(nullptr), item(nullptr), BGM(0)
{

	for (int i = 0; i < 3; i++)
	{
		obstacle_image[i] = NULL;
		obstacle_count[i] = NULL;
	}

	for (int i = 0; i < 2; i++)
	{
		item_image[i] = NULL;
	}
}

GameMainScene::~GameMainScene()
{

}

//初期化処理
void GameMainScene::Initialize()
{
	//高得点値を読み込む
	ReadHighScore();

	//画像の読み込み
	back_ground = LoadGraph("Resource/images/back.bmp");
	barrier_image = LoadGraph("Resource/images/barrier.png");
	int result = LoadDivGraph("Resource/images/obstacle.png", 3, 3, 1, 104, 72, obstacle_image);
	item_image[0] = LoadGraph("Resource/images/item/coin.png");
	item_image[1] = LoadGraph("Resource/images/item/key.png");

	//サウンドの読み込み
	item_get_se = LoadSoundMem("Resource/Sounds/SE/se_coin_get.mp3");
	BGM = LoadSoundMem("Resource/Sounds/bgm/main/mp3");

	//エラーチェック
	if (back_ground == -1)
	{
		throw("Resource/images/back.bmpがありません\n");
	}
	if (result == -1)
	{
		throw("Resource/images/obstacle.bmpがありません\n");
	}
	if (barrier_image == -1)
	{
		throw("Resource/images/barrier.pngがありません\n");
	}

	//オブジェクトの生成
	player = new Player;
	obstacle = new Obstacle * [10];
	item = new Item * [10];

	//オブジェクト初期化
	player->Initialize();

	for (int i = 0; i < 10; i++)
	{

		obstacle[i] = nullptr;
	}

	for (int i = 0; i < 10; i++)
	{
		item[i] = nullptr;
	}
}

//更新処理
eSceneType GameMainScene::Update()
{
	StopSoundMem(BGM);

	//プレイヤーの更新
	player->Update();

	//移動距離の更新
	mileage += (int)player->GetSpeed() + 5;

	int count = 0;
	int nownumber = 0;

	//障害物生成処理
	if (mileage / 20 % 100 == 0)
	{

		for (int i = 0; i < 10; i++)
		{

			nownumber = i;

			if (obstacle[i] == nullptr)
			{

				int type = GetRand(3) % 3;
				if (type == 0)
				{

					while (count <= 2)
					{

						obstacle[nownumber] = new Obstacle(type, obstacle_image[type]);
						obstacle[nownumber]->Initialize(type, count);
						nownumber++;
						count++;
					}

					count = 0;

					break;
				}
				else
				{

					obstacle[i] = new Obstacle(type, obstacle_image[type]);
					obstacle[i]->Initialize(type, 0);
					break;
				}
			}
		}
	}

	//アイテム生成処理
	if (mileage / 20 % 100 == 0)
	{
		for (int i = 0; i < 10; i++)
		{
			int Prob = GetRand(10);
			if (Prob > 1) {
				int type = 0;
				item[i] = new Item(type, item_image[type]);
				item[i]->Initialize();
				break;
			}
			else{
				int type = 1;
				item[i] = new Item(type, item_image[type]);
				item[i]->Initialize();
				break;
			}
		}
	}

	//敵の更新と当たり判定チェック
	for (int i = 0; i < 10; i++)
	{
		if (obstacle[i] != nullptr)
		{
			obstacle[i]->Update(player->GetSpeed());

			//画面外に行ったら、敵を削除してスコア加算
			if (obstacle[i]->GetLocation().y >= 640.0f)
			{
				obstacle_count[obstacle[i]->GetType()]++;
				//nowscore += (obstacle[i]->GetType() + 1) * 200;
				obstacle[i]->Finalize();
				delete obstacle[i];
				obstacle[i] = nullptr;
			}

			if (obstacle[i] != nullptr)
			{
				//当たり判定の確認
				if (IsHitCheck(player, obstacle[i]))
				{
					if (player->IsCarMode())
					{
						obstacle[i]->Finalize();
						delete obstacle[i];
						obstacle[i] = nullptr;
					}
					else
					{
						player->SetActive(false);
						player->DecreaseHp(-100.0f);
						obstacle[i]->Finalize();
						delete obstacle[i];
						obstacle[i] = nullptr;
					}
				}
				/*else
				{
					nowscore += (obstacle[i]->GetType() + 1) * 200;
				}*/
			}
		}
	}
	//アイテムの更新と当たり判定チェック
	for (int i = 0; i < 10; i++)
	{
		if (item[i] != nullptr)
		{
			item[i]->Update(player->GetSpeed());

			//画面外に行ったら、アイテムを削除
			if (item[i]->GetLocation().y >= 640.0f)
			{
				item[i]->Finalize();
				delete item[i];
				item[i] = nullptr;

			}

			if (item[i] != nullptr)
			{
				//当たり判定の確認
				if (IsItemHitCheck(player, item[i]))
				{
					

					if (item[i]->GetType() == 0)
					{
						coin += 1;
						nowscore += 500;
					}
					else
					{
						player->GetKey();
					}

					item[i]->Finalize();
					delete item[i];
					item[i] = nullptr;
					PlaySoundMem(item_get_se, DX_PLAYTYPE_BACK);
				}
			}
		}
	}

	//現在のスコアの更新
	/*for (int i = 0; i < 3; i++)
	{
		nowscore += (i + 1) * 200 * obstacle_count[i];
	}
	nowscore += coin * 500;*/

	//プレイヤーの燃料か体力が0未満なら、リザルトに遷移する
	if (player->GetHp() < 0.0f)
	{
		return eSceneType::E_RESULT;
	}

	return GetNowScene();
}

//描画処理
void GameMainScene::Draw()const
{
	//背景画像の描画
	DrawGraph(0, mileage % 480 - 480, back_ground, TRUE);
	DrawGraph(0, mileage % 480, back_ground, TRUE);

	//障害物の描画
	for (int i = 0; i < 10; i++)
	{
		if (obstacle[i] != nullptr)
		{
			obstacle[i]->Draw();
		}
	}

	//アイテムの描画
	for (int i = 0; i < 10; i++)
	{
		if (item[i] != nullptr)
		{
			item[i]->Draw();
		}
	}

	//プレイヤーの描画
	player->Draw();

	//UIの描画
	DrawBox(500, 0, 640, 480, GetColor(0, 153, 0), TRUE);
	SetFontSize(16);
	DrawFormatString(510, 20, GetColor(0, 0, 0), "ハイスコア");
	DrawFormatString(560, 40, GetColor(255, 255, 255), "%08d", high_score);
	DrawFormatString(510, 80, GetColor(0, 0, 0), "避けた数");
	for (int i = 0; i < 3; i++)
	{
		DrawRotaGraph(523 + (i * 50), 120, 0.3, 0, obstacle_image[i], TRUE, FALSE);
		DrawFormatString(510 + (i * 50), 140, GetColor(255, 255, 255), "%03d", obstacle_count[i]);
	}

	DrawFormatString(510, 180, GetColor(0, 0, 0), "獲得したコイン");
	DrawRotaGraph(533, 220, 1, 0, item_image[0], TRUE, FALSE);
	DrawFormatString(555, 212, GetColor(0, 0, 0), "×");
	DrawFormatString(580, 212, GetColor(255, 255, 255), "%03d", coin);

	DrawFormatString(510, 250, GetColor(0, 0, 0), "コインのスコア");
	DrawFormatString(535, 270, GetColor(0, 0, 0), "%10d", nowscore);

	DrawFormatString(510, 300, GetColor(0, 0, 0), "走行距離");
	DrawFormatString(555, 320, GetColor(0, 0, 0), "%08d", mileage / 10);
	DrawFormatString(510, 340, GetColor(0, 0, 0), "スピード");
	DrawFormatString(555, 360, GetColor(0, 0, 0), "%08.1f", player->GetSpeed());

	float fx = 510.0f;
	float fy = 430.0f;
	DrawFormatStringF(fx, fy, GetColor(0, 0, 0), "PLAYER HP");
	DrawBoxAA(fx, fy + 20.0f, fx + (player->GetHp() * 100 / 1000), fy + 40.0f, GetColor(255, 0, 0), TRUE);
	DrawBoxAA(fx, fy + 20.0f, fx + 100.0f, fy + 40.0f, GetColor(0, 0, 0), FALSE);
}

//終了時処理
void GameMainScene::Finalize()
{
	StopSoundMem(BGM);

	int score = 0;

	//スコアを計算する
	score += (mileage / 10);
	
	for (int i = 0; i < 3; i++)
	{
		score += (i + 1) * 200 * obstacle_count[i];
	}

	score += coin * 500;

	//リザルトデータの書き込み
	FILE* fp = nullptr;
	//ファイルオープン
	errno_t result = fopen_s(&fp, "Resource/dat/result_data.csv", "w");

	//エラーチェック
	if (result != 0)
	{
		throw("Resource/dat/result_data.csvが開けません\n");
	}

	//スコアを保存
	fprintf(fp, "%d,\n", score);

	//避けた数と得点を保存
	for (int i = 0; i < 3; i++)
	{
		fprintf(fp, "%d,\n", obstacle_count[i]);
	}

	//獲得したコインの枚数を保存
	fprintf(fp, "%d,\n", coin);

	//ファイルクローズ
	fclose(fp);

	//動的確保したオブジェクトを削除
	player->Finalize();
	delete player;

	for (int i = 0; i < 10; i++)
	{
		if (obstacle[i] != nullptr)
		{
			obstacle[i]->Finalize();
			delete obstacle[i];
			obstacle[i] = nullptr;
		}
	}

	delete[]obstacle;

	for (int i = 0; i < 10; i++)
	{
		if (item[i] != nullptr)
		{
			item[i]->Finalize();
			delete item[i];
			item[i] = nullptr;
		}
	}

	delete[]item;
}

//現在のシーン情報を取得
eSceneType GameMainScene::GetNowScene()const
{
	return eSceneType::E_MAIN;
}

//ハイスコアの読み込み
void GameMainScene::ReadHighScore()
{
	RankingData data;
	data.Initialize();

	high_score = data.GetScore(0);

	data.Finalize();
}

//当たり判定処理(プレイヤーと敵)
bool GameMainScene::IsHitCheck(Player* p, Obstacle* e)
{
	switch (e->GetType())
	{
	case 0:
		break;
	case 1:		//ジャンプ回避チェック
		if (p->IsJump())
		{
			return false;
		}
		else
		{
			break;
		}
	case 2:		//スライディング回避チェック
		if (p->IsSliding())
		{
			return false;
		}
		else
		{
			break;
		}
	default:
		break;
	}

	//プレイヤーが無敵状態なら、当たり判定を無視する
	if (p->IsInvincible())
	{
		return false;
	}

	//敵情報が無ければ、当たり判定を無視する
	if (e == nullptr)
	{
		return false;
	}

	//位置情報の差分を取得
	Vector2D diff_location = p->GetLocation() - e->GetLocation();

	//当たり判定サイズの大きさを取得
	Vector2D box_ex = p->GetBoxSize() + e->GetBoxSize();

	//コリジョンデータより位置情報の差分が小さいなら、ヒット判定とする
	return((fabsf(diff_location.x) < box_ex.x) && (fabsf(diff_location.y) < box_ex.y));
}

//当たり判定処理(プレイヤーとアイテム)
bool GameMainScene::IsItemHitCheck(Player* p, Item* i)
{
	//敵情報が無ければ、当たり判定を無視する
	if (i == nullptr)
	{
		return false;
	}

	//位置情報の差分を取得
	Vector2D diff_location = p->GetLocation() - i->GetLocation();

	//当たり判定サイズの大きさを取得
	Vector2D box_ex = p->GetBoxSize() + i->GetBoxSize();

	//コリジョンデータより位置情報の差分が小さいなら、ヒット判定とする
	return((fabsf(diff_location.x) < box_ex.x) && (fabsf(diff_location.y) < box_ex.y));
}

