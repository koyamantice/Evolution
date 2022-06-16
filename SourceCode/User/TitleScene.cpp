#include "TitleScene.h"
#include"SceneManager.h"
#include "Audio.h"
#include "input.h"
#include<string>
#include "DebugText.h"
#include"ImageManager.h"
#include"ModelManager.h"
#include <TisGame.h>
void TitleScene::Initialize(DirectXCommon* dxCommon) {
	// �J��������
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height);
	Texture::SetCamera(camera);
	// 3D�I�u�W�F�N�g�ɃJ�������Z�b�g
	Object3d::SetCamera(camera);
	// ���f���ǂݍ���
	Audio::GetInstance()->LoadSound(1, "Resources/Sound/titleBGM.wav");
	Audio::GetInstance()->LoopWave(1, 0.12f);
	//srand(NULL);
	// ���C�g����
	lightGroup = LightGroup::Create();
	// 3D�I�u�G�N�g�Ƀ��C�g���Z�b�g
	Object3d::SetLightGroup(lightGroup);

	//���̏��Ԃŏ����Ȃ��Ɨ�O�X���[�ɂȂ�܂�
	// �f�o�C�X���Z�b�g
	FBXObject3d::SetDevice(dxCommon->GetDev());
	// �J�������Z�b�g
	FBXObject3d::SetCamera(camera);
	// �O���t�B�b�N�X�p�C�v���C������
	FBXObject3d::CreateGraphicsPipeline();
	//�w�i�X�v���C�g����
	sprite[back] = Sprite::Create(ImageManager::TITLE, { 0.0f,0.0f });
	sprite[button] = Sprite::Create(ImageManager::button, buttonPos[0]);
	sprite[button]->SetAnchorPoint({ 0.5f,0.5f });
	//sprite[button]->SetScale(0.4f);
	sprite[select] = Sprite::Create(ImageManager::Tselect, { 76.0f,418.0f });
	//�X�v���C�g����
	Actor* Act_[2]{};
	Act_[0] = new Actor(std::string("player"));
	Act_[0]->Initialize(ModelManager::GetIns()->GetModel(ModelManager::Player));


	actor[Player].reset(Act_[Player]);
}
//�J������
void TitleScene::Finalize() {
	//�R���̃��f���̃f���[�g
	for (int i = 0; i < 4;i++) {
		delete sprite[i];
	}
}
//�X�V
void TitleScene::Update(DirectXCommon* dxCommon) {
	Input* input = Input::GetInstance();
	camera->Update();
	actor[Player]->Update();
}
//�`��
void TitleScene::Draw(DirectXCommon* dxCommon) {
	//ImGui::Begin("test");
	////ImGui::SliderFloat("cameraPos.y", &cameraPos.y, 30, 0);
	//ImGui::Text("P::%d", P);
	//ImGui::Unindent();
	//ImGui::End();
	Sprite::PreDraw();
	//�w�i�p
	sprite[back]->Draw();
	sprite[select]->Draw();
	sprite[select]->Draw();
	sprite[button]->Draw();

	actor[Player]->Draw();

}

