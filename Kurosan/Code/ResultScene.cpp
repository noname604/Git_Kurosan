#include"ResultScene.h"
#include"RankingData.h"
#include"../Utility/InputControl.h"
#include"DxLib.h"

ResultScene::ResultScene() : back_ground(NULL),cursor_image(NULL),score(0), menu_cursor(0)
{
	for (int i = 0; i < 3; i++)
	{
		enemy_image[i] = NULL;
		enemy_count[i] = NULL;
	}
}

ResultScene::~ResultScene()
{

}

//初期化処理
void ResultScene::Initialize()
{

	//画像の読み込み
	back_ground = LoadGraph("Resource/images/back.bmp");
	cursor_image = LoadGraph("Resource/images/cone.bmp");
	int result = LoadDivGraph("Resource/images/obstacle.png", 3, 3, 1, 104, 72, enemy_image);

	//エラーチェック
	if (back_ground == -1)
	{
		throw("Resource/images/back.bmpがありません\n");
	}
	if (cursor_image == -1)
	{
		throw("Resource/images/cone.bmpがありません\n");
	}
	if (result == -1)
	{
		throw("Resource/images/obstacle.pngがありません\n");
	}
	//ゲーム結果の読み込み
	ReadResultData();
}

//更新処理
eSceneType ResultScene::Update()
{
	//Bボタンでランキングに遷移する
	if (InputControl::GetButtonDown(XINPUT_BUTTON_B))
	{
		switch (menu_cursor)
		{
		case 0:
			return eSceneType::E_MAIN;
		case 1:
			return eSceneType::E_RANKING_INPUT;
		case 2:
			return eSceneType::E_TITLE;
		}
	}

	//カーソル下移動
	if (InputControl::GetButtonDown(XINPUT_BUTTON_DPAD_DOWN))
	{
		menu_cursor++;
		//1番下に到達したら、一番上にする
		if (menu_cursor > 2)
		{
			menu_cursor = 0;
		}
	}

	//カーソル上移動
	if (InputControl::GetButtonDown(XINPUT_BUTTON_DPAD_UP))
	{
		menu_cursor--;
		//1番上に到達したら、一番下にする
		if (menu_cursor < 0)
		{
			menu_cursor = 2;
		}
	}
	
	return GetNowScene();
}

//描画処理
void ResultScene::Draw()const
{
	//背景画像を描画
	DrawGraph(0, 0, back_ground, TRUE);

	//スコア等表示領域
	DrawBox(150, 150, 490, 420, GetColor(0, 153, 0), TRUE);
	DrawBox(150, 150, 490, 420, GetColor(0, 0, 0), FALSE);

	DrawBox(500, 0, 640, 480, GetColor(0, 153, 0), TRUE);

	SetFontSize(20);
	DrawString(220, 170, "ゲームオーバー", GetColor(204, 0, 0));

	SetFontSize(16);
	DrawString(185, 200, "走行距離      ", GetColor(0, 0, 0));

	for (int i = 0; i < 3; i++)
	{
		DrawRotaGraph(230, 230 + (i * 20), 0.3f,0, enemy_image[i], TRUE);
		DrawFormatString(260, 222 + (i * 21), GetColor(255, 255, 255), "%6d x %4d=%6d", enemy_count[i], (i + 1) * 50, (i + 1) * 50 * enemy_count[i]);
	}

	//カーソル画像の描画
	DrawRotaGraph(170, 327 + menu_cursor * 40, 0.4, DX_PI / 2.0, cursor_image, TRUE);

	DrawString(185, 290, "スコア", GetColor(0, 0, 0));
	DrawFormatString(185, 290, 0xFFFFFF, "      =%6d", score);

	DrawString(185, 320, "リスタート",GetColor(0, 0, 0));
	DrawString(185, 360, "ランキングに移動", GetColor(0, 0, 0));
	DrawString(185, 400, "タイトルに戻る", GetColor(0, 0, 0));
}

//終了時処理
void ResultScene::Finalize()
{
	//読み込んだ画像を削除
	DeleteGraph(back_ground);
	for (int i = 0; i < 3; i++)
	{
		DeleteGraph(enemy_image[i]);
	}
}

//現在のシーン情報を取得
eSceneType ResultScene::GetNowScene()const
{
	return eSceneType::E_RESULT;
}

//リザルトデータの読み込み
void ResultScene::ReadResultData()
{
	//ファイルオープン
	FILE* fp = nullptr;
	errno_t result = fopen_s(&fp, "Resource/dat/result_data.csv", "r");

	//エラーチェック
	if (result != 0)
	{
		throw("Resource/dat/result_data.csvが読み込めません\n");
	}

	//結果を読み込む
	fscanf_s(fp, "%d,\n", &score);

	//避けた数と得点を取得
	for (int i = 0; i < 3; i++)
	{
		fscanf_s(fp, "%d,\n", &enemy_count[i]);
	}

	//ファイルクロース
	fclose(fp);
}
