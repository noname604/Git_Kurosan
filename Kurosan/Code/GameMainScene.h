#pragma once

#include "SceneBase.h"
#include"Player.h"
#include "Obstacle.h"
#include "Item.h"

class GameMainScene : public SceneBase
{
private:
	int high_score;				//ハイスコア
	int back_ground;			//背景画像
	int barrier_image;			//バリア画像
	int mileage;				//走行距離
	int nowscore;				//現在のスコア
	int coin;					//獲得コイン枚数
	int obstacle_image[3];		//障害物の画像
	int obstacle_count[3];		//回避した障害物のカウント
	int item_image[2];			//アイテムの画像
	int item_get_se;			//アイテム入手時のSE
	int BGM;					//BGM
	bool getkey;				//鍵獲得
	Player* player;				//プレイヤー
	Obstacle** obstacle;		//障害物
	Item** item;				//アイテム


public:
	GameMainScene();
	virtual ~GameMainScene();

	virtual void Initialize() override;
	virtual eSceneType Update() override;
	virtual void Draw() const override;
	virtual void Finalize() override;

	virtual eSceneType GetNowScene() const override;

	//鍵獲得処理
	bool GetKey();

private:
	//ハイスコア読み込み処理
	void ReadHighScore();
	//当たり判定
	bool IsHitCheck(Player* p, Obstacle* e);
	//アイテムの当たり判定
	bool IsItemHitCheck(Player* p, Item* i);
	
};

