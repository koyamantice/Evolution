#include"MSecondStage.h"

void MSecondStage::Initialize(DirectXCommon* dxCommon) {
	InitCommon(dxCommon);
	BattleInit();
	TorchSetup(Touch::FireColor::f_blue);

	//�����̓{�X�X�e�[�W����Ȃ��̂ŕs�v
	battle_intro = false;

	//�Q�[���A�N�^�[�̐��������܂��B
	ActorManager::GetInstance()->AttachActor("Player");
	ActorManager::GetInstance()->AttachActor("ClearCrystal");
	for (int i = 0; i < kGnormNum; i++) {
		ActorManager::GetInstance()->AttachBullet("Red");
	}
	for (int i = 0; i < kGnormNum; i++) {
		ActorManager::GetInstance()->AttachBullet("Green");
	}

	//�V�[�����ŕK�v�ȃA�N�^�[���Q�Ƃ��܂��B
	player_shadow = ActorManager::GetInstance()->SearchActor("Player");
	goal_shadow = ActorManager::GetInstance()->SearchActor("ClearCrystal");


	//�X�v���C�g����
	Sprite* _clear = nullptr;
	_clear = Sprite::Create(ImageManager::Clear, { 0,0 });
	Clear.reset(_clear);

	Sprite* _Over = nullptr;
	_Over = Sprite::Create(ImageManager::Over, { 0,0 });
	Over.reset(_Over);


	//�J�����̏�����
	camera->SetTarget(player_shadow->GetPosition());
	camera_distance.x = sinf(camera_angle * (XM_PI / DEGREE_HALF)) * camera_radius;
	camera_distance.z = cosf(camera_angle * (XM_PI / DEGREE_HALF)) * camera_radius;
	e_camera_distance.x = camera_distance.x;
	e_camera_distance.z = camera_distance.z;
	camera->SetEye({
		player_shadow->GetPosition().x + camera_distance.x,
		player_shadow->GetPosition().y + camera_hight,
		player_shadow->GetPosition().z + camera_distance.z
		});

	//�|�X�g�G�t�F�N�g�̏�����
	postEffect = new PostEffect();
	postEffect->Initialize();

	//�p�[�e�B�N���̏�����
	particleEmitter = std::make_unique <ParticleEmitter>(ImageManager::charge);
}
//�J������
void MSecondStage::Finalize() {
	//�R���̃��f���̃f���[�g
	ActorManager::GetInstance()->Finalize();
}
//�X�V
void MSecondStage::Update(DirectXCommon* dxCommon) {
	if (ClearUpdate()) { return; }
	if (PauseUpdate()) { return; }
	//��������̍X�V����
	DescriptionUpdate();
	//�A�N�^�[���ׂĂ̍X�V����
	ActorManager::GetInstance()->Update();
	//�J�����̍X�V����
	CameraUpda();
	//�X�e�[�W�̍X�V����
	FieldUpdate();
	//�p�[�e�B�N���̍X�V����
	particleEmitter->Update();
}



void MSecondStage::ResultCamera(int Timer) {
	camera->SetTarget(goal_shadow->GetPosition());
	camera->SetEye(XMFLOAT3{ player_shadow->GetPosition().x + camera_distance.x,player_shadow->GetPosition().y + camera_hight,player_shadow->GetPosition().z + camera_distance.z });
	camera->Update();
}

bool MSecondStage::ClearUpdate() {
	if (stage_clear) {
		const float rnd_vel = 0.5f;
		particleEmitter->AddCommon(100, goal_shadow->GetPosition(), rnd_vel, 0, 1.2f, 0.0f, { 1,1,0.5f,1 }, { 1,1,1,0.3f });
		particleEmitter->Update();

		if (input->TriggerButton(Input::A) || input->TriggerButton(Input::B)) {
			scene_changer->ChangeStart();
		}
		scene_changer->ChangeScene("SECONDSTAGE");
		ResultCamera(0);
		FieldUpdate();
		return true;
	}
	if (goal_shadow->GetPause()) {
		battle_result = true;
		stage_clear = true;
	}
	return false;
}


//�`��
void MSecondStage::Draw(DirectXCommon* dxCommon) {
	dxCommon->PreDraw();
	Object3d::PreDraw();
	skydome->Draw();
	ground->Draw();
	for (std::unique_ptr<Touch>& torch : torchs) {
		torch->Draw();
	}
	ActorManager::GetInstance()->Draw(dxCommon);
	particleEmitter->Draw(alphaBle);
	Sprite::PreDraw();
	if (scene_first_change) {
		filter_first->Draw();
	}
	if (stage_clear) {
		Clear->Draw();
	}
	if (battle_intro) {
		screens[0]->Draw();
		screens[1]->Draw();
	}
	if (battle_result) {
		screens[0]->Draw();
		screens[1]->Draw();
		Clear->Draw();
	}
	if (gameover) {
		Over->Draw();
	}
	if (pause) {
		pauseUi->Draw();
	}
	scene_changer->Draw();
	dxCommon->PostDraw();
}
