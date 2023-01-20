#include "BaseScene.h"
#include <SourceCode/Common/Easing.h>
#include"SceneManager.h"

void BaseScene::InitCommon(DirectXCommon* dxCommon) {
	// �J��������
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height);
	// ���C�g����
	lightGroup = LightGroup::Create();
	//Object2d�̏�����
	Object2d::SetCamera(camera);
	//Object3d�̏�����
	Object3d::StaticInitializeCommon(camera, lightGroup);
	//FBX�̏�����
	FBXObject3d::StaticInitializeCommon(dxCommon->GetDev(), camera);
	//���̑}��
	
	//�p�[�e�B�N���ɃJ�����Z�b�g
	ParticleManager::CreateCommon(dxCommon->GetDev(), camera, dxCommon->GetCmdList());
	//
	ImageManager::GetIns()->LoadParticle();

	//�V�[���؂�ւ��̏�����
	SceneChanger* scene_changer_ = new SceneChanger();
	scene_changer_->Init();
	scene_changer.reset(scene_changer_);
}

