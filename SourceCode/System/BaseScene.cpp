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
	ParticleManager::GetInstance()->SetCamera(camera);



	//�����g
	Sprite* _Screen = nullptr;
	_Screen = Sprite::Create(ImageManager::SceneCover, { 0,0 });
	Screen[0].reset(_Screen);
	Sprite* _Screen2 = nullptr;
	_Screen2 = Sprite::Create(ImageManager::SceneCover, { 0,600 });
	Screen[1].reset(_Screen2);
	//�X�v���C�g����
	Sprite* Effect_ = Sprite::Create(ImageManager::Black, { 0.0f,0.0f });
	Effect.reset(Effect_);
	Effect->SetColor({ 1,1,1,alpha });

	//
	PauseUI* pause_ui = new PauseUI();
	pauseUi.reset(pause_ui);


}

void BaseScene::Feed(const std::string& sceneName) {
	static float frame = 0.0f;
	if (Change) {
		if (frame < 1.6f) {
			frame += 0.02f;
		} else {
			SceneManager::GetInstance()->ChangeScene(sceneName);
		}
		alpha = Ease(In, Cubic, frame, 0, 1);
		Effect->SetColor({ 1,1,1,alpha });
	}
}
