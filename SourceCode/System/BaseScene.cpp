#include "BaseScene.h"
#include <SourceCode/Common/Easing.h>
#include"SceneManager.h"

void BaseScene::InitCommon(DirectXCommon* dxCommon) {
	// �J��������
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height);
	Object2d::SetCamera(camera);
	// 3D�I�u�W�F�N�g�ɃJ�������Z�b�g
	Object3d::SetCamera(camera);
	//���̑}��
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
	//�p�[�e�B�N���ɃJ�����Z�b�g
	ParticleManager::CreateCommon(dxCommon->GetDev(), camera, dxCommon->GetCmdList());
	ImageManager::GetIns()->LoadParticle();


	SceneChanger* scene_changer_ = new SceneChanger();
	scene_changer_->Init();
	scene_changer.reset(scene_changer_);
}

