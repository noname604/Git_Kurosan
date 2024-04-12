#include "Title.h"
#include"../Utility/InputControl.h"
#include"DxLib.h"

Title::Title() : background_image(NULL), menu_image(NULL), cursor_image(NULL), menu_cursor(0),credit_time(0),credit_image(NULL),end(NULL),title_image(0),title_car(0),
				 cursol_se(0), kettei_se(0)
{

}
Title::~Title()
{

}

//初期化処理
void Title::Initialize()
{
	credit_time = -1;
	end = FALSE;
	//画像の読み込み
	title_car = LoadGraph("Resource/Title/T_Car.png");
	background_image = LoadGraph("Resource/Title/T_Fire.bmp");
	title_image = LoadGraph("Resource/Font/title.png");
	menu_image = LoadGraph("Resource/Font/F_Menu.png");
	cursor_image = LoadGraph("Resource/images/cone.bmp");
	credit_image = LoadGraph("Resource/Title/credit.png");

	// サウンドの読み込み
	cursol_se = LoadSoundMem("Resource/sounds/se/cursor.mp3");
	kettei_se = LoadSoundMem("Resource/sounds/se/kettei.mp3");

	//エラーチェック
	if (background_image == -1)
	{
		throw("Resource/Title/T_Car.pngがありません\n");
	}
	if (background_image == -1)
	{
		throw("Resource/Title/T_Fire.bmpがありません\n");
	}
	if (title_image == -1)
	{
		throw("Resource/image/title.pngがありません\n");
	}
	if (menu_image == -1)
	{
		throw("Resource/Font/F_Menu.pngがありません\n");
	}
	if (cursor_image == -1)
	{
		throw("Resorce/images/cone.bmpがありません\n");
	}
	if (credit_image == -1)
	{
		throw("Resource/Title/credit.pngがありません\n");
	}
}
//更新処理
eSceneType Title::Update()
{
	if (credit_time > 0)
	{
		credit_time -= 1;
	}
	
	//カーソル下移動
	if (InputControl::GetButtonDown(XINPUT_BUTTON_DPAD_DOWN))
	{
		menu_cursor++;
		PlaySoundMem(cursol_se, DX_PLAYTYPE_BACK);
		//1番下に到達したら、一番上にする
		if (menu_cursor > 3)
		{
			menu_cursor = 0;
		}
	}

	//カーソル上移動
	if (InputControl::GetButtonDown(XINPUT_BUTTON_DPAD_UP))
	{
		menu_cursor--;
		PlaySoundMem(cursol_se, DX_PLAYTYPE_BACK);
		//1番上に到達したら、一番下にする
		if (menu_cursor < 0)
		{
			menu_cursor = 3;
		}
	}

	//カーソル決定（決定した画面に遷移する）
	if (InputControl::GetButtonDown(XINPUT_BUTTON_B))
	{
		PlaySoundMem(kettei_se, DX_PLAYTYPE_BACK);
		switch (menu_cursor)
		{
		case 0:
			return eSceneType::E_MAIN;
		case 1:
			return eSceneType::E_RULE;
		case 2:
			return eSceneType::E_RANKING_DISP;;
		default:
			credit_time = 180;    //3秒間セット
			end = TRUE;
			//return eSceneType::E_END;
		}
	}
	//3秒後ゲームを落とす
	if (credit_time == 0)           
	{
		return eSceneType::E_END;
	}

	//現在のシーンタイプを返す
	return GetNowScene();
}

//描画処理
void Title::Draw()const
{
	//タイトル画面の描画
	DrawGraph(0, 0, background_image, FALSE);

	//題名の描画
	DrawGraph(5, 30, title_image, TRUE);

	//車の描画
	DrawGraph(200, 90, title_car, TRUE);

	//メニュー画像の描画
	DrawGraph(55, 200, menu_image, TRUE);

	//カーソル画像の描画
	DrawRotaGraph(35, 220 + menu_cursor * 40, 0.7, DX_PI / 2.0, cursor_image, TRUE);

	//クレジット画像の描画
	if (end == TRUE)
	{
		DrawGraph(0, 0, credit_image, FALSE);

		SetFontSize(20);
		DrawString(150, 50, "使用させていただいたサイト", 0xffffff, 0);

		DrawString(20, 160, "silhouetteAC様", 0xffffff, 0);
		DrawString(400, 160, "illustimage様", 0xffffff, 0);
		DrawString(20, 200, "illustAC様", 0xffffff, 0);
		DrawString(400, 200, "ロゴ作成様", 0xffffff, 0);
		DrawString(20, 250, "魔王魂様", 0xffffff, 0);
			DrawString(400, 250, "ニコニ・コモンズ様", 0xffffff, 0);

		DrawString(150, 450, "---- thank you for playing BurnOut ----", 0xffffff, 0);
	}
}

//終了時処理
void Title::Finalize()
{
	//読み込んだ画像を削除
	DeleteGraph(background_image);
	DeleteGraph(menu_image);
	DeleteGraph(cursor_image);
	DeleteGraph(credit_image);
}

//現在のシーン情報を取得
eSceneType Title::GetNowScene()const
{
	return eSceneType::E_TITLE;
}
