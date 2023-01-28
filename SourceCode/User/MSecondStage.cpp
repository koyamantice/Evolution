#include"MSecondStage.h"

void MSecondStage::Initialize(DirectXCommon* dxCommon) {
	InitCommon(dxCommon);
	BattleInit();
	TorchSetup(Touch::FireColor::f_blue);

	//�����̓{�X�X�e�[�W����Ȃ��̂ŕs�v
	battle_intro = true;

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
	goal_shadow->SetIsActive(false);
	for (int i = 0; i < kMaxNestNum; i++) {
		ActorManager::GetInstance()->AttachActor("Honey");
		honey_[i] = ActorManager::GetInstance()->SearchActor("Honey");
		honey_[i]->SetIsRefer(true);
	}
	honey_[0]->SetPosition({ -20.0f,0,20.0f});
	honey_[1]->SetPosition({ 0.0f,0,-20.0f });
	honey_[2]->SetPosition({ 20.0f,0,20.0f});

	const XMFLOAT2 kBasicAnchor = { 0.5f,0.5f };

	Sprite* _mission{};
	_mission = Sprite::Create(ImageManager::kMissionMsecond, { 640,100 });
	_mission->SetAnchorPoint(kBasicAnchor);
	mission_.reset(_mission);
	
	const XMFLOAT2 kBasicSize = { 64,64 };
	for (int i = 0; i < kHoneyNumMax; i++) {
		Sprite* _honey_get[kHoneyNumMax]{};
		_honey_get[i] = Sprite::Create(ImageManager::kMsecondNum, { 640-64,135 });
		honey_get_[i].reset(_honey_get[i]);
		int number_index_y = i / kHoneyNumMax;
		int number_index_x = i % kHoneyNumMax;
		honey_get_[i]->SetTextureRect(
			{ static_cast<float>(number_index_x) * kBasicSize.x, static_cast<float>(number_index_y) * kBasicSize.y },
			{ static_cast<float>(kBasicSize.x), static_cast<float>(kBasicSize.y) });
		
		honey_get_[i]->SetSize(kBasicSize);
		//���S���W�ɂ��܂��B
		honey_get_[i]->SetAnchorPoint(kBasicAnchor);
	}


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
	particleEmitter = std::make_unique <ParticleEmitter>(ImageManager::nul);
}
//�J������
void MSecondStage::Finalize() {
	//�R���̃��f���̃f���[�g
	ActorManager::GetInstance()->Finalize();
}
//�X�V
void MSecondStage::Update(DirectXCommon* dxCommon) {
	//���������̏���
	if (IntroUpdate()) { return; }
	//�N���A��̏���
	if (ClearUpdate()) { return; }
	//�|�[�Y�����̍X�V
	if (PauseUpdate()) { return; }
	//��������̍X�V����
	DescriptionUpdate();
	//�A�N�^�[���ׂĂ̍X�V����
	ActorManager::GetInstance()->Update();
	//�J�����̍X�V����
	CameraUpda();
	//�X�e�[�W�̍X�V����
	FieldUpdate();
	//�I���̍X�V����
	HoneyUpdate();
	//��������̍X�V����
	hud->Update();
	//�p�[�e�B�N���̍X�V����
	particleEmitter->Update();
}

bool MSecondStage::IntroUpdate() {
	//�t�F�[�h�C���@�\
	if (scene_first_change) {
		if (feedin_frame <= 1.0f) {
			feedin_frame += 1.0f / feedin_frame_max;
		} else {
			battle_intro = false;
			scene_first_change = false;
		}
		filter_alpha = Ease(Out, Cubic, feedin_frame, 1, 0);
		filter_first->SetColor({ 1,1,1,filter_alpha });
		ActorManager::GetInstance()->IntroUpdate(0,"", kSecondScene);
		CameraUpda();
		FieldUpdate();
		return true;
	} else {
		return false;
	}
}

void MSecondStage::ResultCamera(int Timer) {
	camera->SetTarget(player_shadow->GetPosition());
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
	if (!goal_shadow->GetIsActive()) {
		if (honey_[kLeftNest]->GetCommand() == Actor::APPROCH &&
			honey_[kMiddleNest]->GetCommand() == Actor::APPROCH &&
			honey_[kRightNest]->GetCommand() == Actor::APPROCH) {
			goal_shadow->SetIsActive(true);
			return false;
		}
	}
	if (goal_shadow->GetPause()) {
		battle_result = true;
		stage_clear = true;
	}

	return false;
}

void MSecondStage::HoneyUpdate() {
	nowOpenHoney = 0;
	for (int i = 0; i < kMaxNestNum; i++) {
		if (honey_[i]->GetCommand() == Actor::APPROCH) {
			nowOpenHoney++;
			const float rnd_vel = 0.2f;
			const float rnd_height = 0.3f;
			particleEmitter->AddParabo(50, honey_[i]->GetPosition(), rnd_height, rnd_vel, 1.5f, 0.0f, { 1.0f,1.0f,0.0f,0.8f }, { 1,1,0,0 });
			particleEmitter->Update();
		}
	}
}

void MSecondStage::DrawLocal() {

	mission_->Draw();
	honey_get_[nowOpenHoney]->Draw();

}


//�`��
void MSecondStage::Draw(DirectXCommon* dxCommon) {
	dxCommon->PreDraw();
	
	BattleBackDraw();

	ActorManager::GetInstance()->Draw(dxCommon);

	BattleFrontDraw(addBle);

	dxCommon->PostDraw();
}
