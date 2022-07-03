#include "TitleScene.h"
#include"SceneManager.h"
#include "AudioManager.h"
#include "input.h"
#include<string>
#include "DebugText.h"
#include"ImageManager.h"
#include"ModelManager.h"
#include <TisGame.h>
#include"Player.h"
void TitleScene::Initialize(DirectXCommon* dxCommon) {
	// �J��������
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height);
	Texture::SetCamera(camera);
	// 3D�I�u�W�F�N�g�ɃJ�������Z�b�g
	Object3d::SetCamera(camera);
	// ���f���ǂݍ���
	//AudioManager::GetInstance()->LoadSound(1, "Resources/Sound/titleBGM.wav");
	//AudioManager::GetInstance()->LoopWave(1, 0.12f);
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
	//sprite[back] = Sprite::Create(ImageManager::TITLE, { 0.0f,0.0f });
	//�X�v���C�g����
	Actor* Act_[Chr_Max]{};
	Act_[MPlayer] = new Player();
	Act_[MPlayer]->Initialize(ModelManager::GetIns()->GetModel(ModelManager::Player));
	actor[MPlayer].reset(Act_[MPlayer]);
	Object3d* Sky{};
	Sky = new Object3d();
	Sky->SetModel(ModelManager::GetIns()->GetModel(ModelManager::skydome));
	Sky->Initialize();
	skydome.reset(Sky);
	camera->SetTarget(actor[MPlayer]->GetPosition());
}
//�J������
void TitleScene::Finalize() {
	//�R���̃��f���̃f���[�g
}
//�X�V
void TitleScene::Update(DirectXCommon* dxCommon) {
	Input* input = Input::GetInstance();
	XMFLOAT3 plaPos = actor[MPlayer]->GetPosition();
	camera->SetTarget(XMFLOAT3{plaPos.x,plaPos.y,plaPos.z});
	camera->SetEye(XMFLOAT3{ plaPos.x,plaPos.y+10.0f,plaPos.z+10.0f});
	camera->Update();
	actor[MPlayer]->Update();
	skydome->Update();
	if (input->PushKey(DIK_0)) {
		int a = 0;
		a++;
	}

}
//�`��
void TitleScene::Draw(DirectXCommon* dxCommon) {
	ImGui::Begin("test");
	//ImGui::SliderFloat("cameraPos.y", &cameraPos.y, 30, 0);
	ImGui::Unindent();
	ImGui::End();
	//Sprite::PreDraw();
	Object3d::PreDraw();
	skydome->Draw();
	//�w�i�p
	actor[MPlayer]->Draw();
}

