#include "ResultScene.h"
#include "../Utility/InputControl.h"
#include "DxLib.h"

ResultScene::ResultScene()
{

}

ResultScene::~ResultScene()
{

}

//初期化処理
void ResultScene::Initialize()
{
}

//更新処理
eSceneType ResultScene::Update()
{
	//現在のシーンタイプを返す
	return GetNowScene();
}

//描画処理
void ResultScene::Draw()const
{
}

//終了時処理
void ResultScene::Finalize()
{
}

//現在のシーン情報を取得
eSceneType ResultScene::GetNowScene()const
{
	return eSceneType::E_RESULT;
}
