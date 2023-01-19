#include"SecondStage.h"

void SecondStage::Initialize(DirectXCommon* dxCommon) {
	InitCommon(dxCommon);
	BattleInit();
	TorchSetup(Touch::FireColor::f_blue);

	//�Q�[���A�N�^�[�̐��������܂��B
	ActorManager::GetInstance()->AttachActor("Player");
	ActorManager::GetInstance()->AttachActor("Enemy_Bee");
	ActorManager::GetInstance()->AttachActor("ClearCrystal");
	for (int i = 0; i < 10; i++) {
		ActorManager::GetInstance()->AttachBullet("Red");
	}
	for (int i = 0; i < 10; i++) {
		ActorManager::GetInstance()->AttachBullet("Green");
	}
	//�V�[�����ŕK�v�ȃA�N�^�[���Q�Ƃ��܂��B
	player_shadow = ActorManager::GetInstance()->SearchActor("Player");
	enemy_shadow = ActorManager::GetInstance()->SearchActor("Enemy");
	goal_shadow = ActorManager::GetInstance()->SearchActor("ClearCrystal");
	goal_shadow->SetPosition(enemy_shadow->GetPosition());
	goal_shadow->SetIsActive(false);

	//�X�v���C�g����
	Sprite* _clear = nullptr;
	_clear = Sprite::Create(ImageManager::Clear, { 0,0 });
	Clear.reset(_clear);

	Sprite* _Over = nullptr;
	_Over = Sprite::Create(ImageManager::Over, { 0,0 });
	Over.reset(_Over);

	//�J�����̏�����

	camera_distance.x = sinf(camera_angle * (XM_PI / 180)) * camera_radius;
	camera_distance.z = cosf(camera_angle * (XM_PI / 180)) * camera_radius;
	player_shadow->SetAngle(camera_angle);
	camera->SetTarget(first_target);
	camera->SetEye(XMFLOAT3{
		player_shadow->GetPosition().x + camera_distance.x,
		first_hight,
		player_shadow->GetPosition().z + camera_distance.z
		});
	camera->Update();

	//�|�X�g�G�t�F�N�g�̏�����
	postEffect = new PostEffect();
	postEffect->Initialize();

	//�p�[�e�B�N���̏�����
	particleEmitter = std::make_unique <ParticleEmitter>(ImageManager::charge);
}

//�X�V
void SecondStage::Update(DirectXCommon* dxCommon) {
	if (IntroUpdate()) { return; }
	//�N���A��̏���
	if (ClearUpdate()) { return; }
	//�|�[�Y�����̍X�V
	if (PauseUpdate()) { return; }
	//�Q�[���I�[�o�[�̏���
	GameOverUpdate();
	//�N���A��̏���
	if (ClearUpdate()) { return; }
	//�Q�[���I�[�o�[�̏���
	GameOverUpdate();
	//�A�N�^�[���ׂĂ̍X�V����
	ActorManager::GetInstance()->Update();
	//�J�����̍X�V����
	CameraUpda();
	//�X�e�[�W�̍X�V����
	FieldUpdate();
	//�p�[�e�B�N���̍X�V����
	particleEmitter->Update();
}

//�`��
void SecondStage::Draw(DirectXCommon* dxCommon) {
	dxCommon->PreDraw();
	//postEffect->PreDrawScene(dxCommon->GetCmdList());
	ImGui::SetNextWindowPos(ImVec2(980, 0), 1);
	ImGui::SetNextWindowSize(ImVec2(280, 300), 1);
	ImGui::Begin("test");
	ImGui::SliderFloat("intro_count", &intro_count, 0, intro_count_max);
	float frame = intro_count / intro_count_max;
	ImGui::SliderFloat("frame", &frame, 0, 1);
	ImGui::Unindent();
	ImGui::End();	
	Object3d::PreDraw();
	skydome->Draw();
	ground->Draw();
	for (std::unique_ptr<Touch>& torch : torchs) {
		torch->Draw();
	}
	//�w�i�p
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
	//postEffect->Draw(dxCommon->GetCmdList());
	dxCommon->PostDraw();

}
//�J������
void SecondStage::Finalize() {
	//�R���̃��f���̃f���[�g
	ActorManager::GetInstance()->Finalize();
}
bool SecondStage::IntroUpdate() {
	if (battle_intro) {
		//�t�F�[�h�C���@�\
		if (scene_first_change) {
			if (feedin_frame <= 1.0f) {
				feedin_frame += 1.0f / feedin_frame_max;
			} else {
				scene_first_change = false;
			}
			filter_alpha = Ease(Out, Cubic, feedin_frame, 1, 0);
			filter_first->SetColor({ 1,1,1,filter_alpha });
			ActorManager::GetInstance()->IntroUpdate(intro_count);
			FieldUpdate();
			return true;
		}
		intro_count += intro_speed;

		//���������I������
		if (intro_count >= intro_count_max) {
			intro_count = 0;
			battle_intro = false;
			return true;
		}

		IntroCamera(intro_count);
		ActorManager::GetInstance()->IntroUpdate(intro_count / intro_count_max,"Bullet");
		FieldUpdate();
		return true;
	}
	return false;
}
void SecondStage::IntroCamera(const float& Timer) {




}
void SecondStage::ResultCamera(const float& Timer) {
}


bool SecondStage::ClearUpdate() {
	return false;
}
void SecondStage::GameOverUpdate() {
}

float SecondStage::RandHeight(const float& base) {
	const float& rnd_vel = 0.95f;
	float Rand= (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	float itr = 0;
	itr = base + Rand;

	return itr;
}