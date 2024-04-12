#pragma once

#include "SceneBase.h"

class Title :public SceneBase
{
private:

private:
	int background_image;      //背景画像
	int menu_image;            //メニュー画像
	int cursor_image;          //カーソル画像
	int menu_cursor;           //メニューカーソル番号

	int credit_time;           //クレジット時間
	int credit_image;          //クレジット画像
	bool end;                  //クレジット画面反映
	int title_image;           //題名画像
	int title_car;             //車画像
	int BGM;				   //BGM

	int cursol_se;			   // カーソルSE
	int kettei_se;			   // 決定SE

public:
	Title();
	virtual ~Title();

	virtual void Initialize() override;
	virtual eSceneType Update() override;
	virtual void Draw() const override;
	virtual void Finalize() override;

	virtual eSceneType GetNowScene() const override;
};

