#include "Title.h"
#include "../Utility/InputControl.h"
#include "DxLib.h"

Title::Title()
{

}

Title::~Title()
{

}

//初期化処理
void Title::Initialize()
{

}
//更新処理
eSceneType Title::Update()
{
	//現在のシーンタイプを返す
	return GetNowScene();
}

//描画処理
void Title::Draw()const
{
}

//終了時処理
void Title::Finalize()
{

}

//現在のシーン情報を取得
eSceneType Title::GetNowScene()const
{
	return eSceneType::E_TITLE;
}
