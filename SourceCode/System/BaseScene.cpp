#include "BaseScene.h"
#include <SourceCode/Common/Easing.h>
#include"SceneManager.h"

void BaseScene::InitCommon(DirectXCommon* dxCommon) {
	// �J��������
	camera = std::make_unique<DebugCamera>(WinApp::window_width, WinApp::window_height);
	// ���C�g����
	lightGroup = std::make_unique<LightGroup>();
	lightGroup->Initialize();
	
	//Object2d�̏�����
	Object2d::SetCamera(camera.get());
	
	//Object3d�̏�����
	Object3d::StaticInitializeCommon(camera.get(), lightGroup.get());
	
	//FBX�̏�����
	FBXObject3d::StaticInitializeCommon(dxCommon->GetDev(), camera.get());

	//�p�[�e�B�N���ɃJ�����Z�b�g
	ParticleManager::CreateCommon(dxCommon->GetDev(), camera.get(), dxCommon->GetCmdList());
	//�p�[�e�B�N���ŕK�v�ȃe�N�X�`����ǂݍ���
	ImageManager::GetIns()->LoadParticle();

	//�V�[���؂�ւ��̏�����
	scene_changer = std::make_unique <SceneChanger>();
	scene_changer->Init();
	scene_changer->InitOver();

}

