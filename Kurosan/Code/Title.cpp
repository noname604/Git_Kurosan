#include "Title.h"
#include "../Utility/InputControl.h"
#include "DxLib.h"

Title::Title()
{
	image = LoadGraph("image/tamesi50.png");
}

Title::~Title()
{

}

//����������
void Title::Initialize()
{

}
//�X�V����
eSceneType Title::Update()
{
	//���݂̃V�[���^�C�v��Ԃ�
	return GetNowScene();
}

//�`�揈��
void Title::Draw()const
{
	DrawGraph(100, 100, image, TRUE);
}

//�I��������
void Title::Finalize()
{
	DeleteGraph(image);
}

//���݂̃V�[�������擾
eSceneType Title::GetNowScene()const
{
	return eSceneType::E_TITLE;
}
