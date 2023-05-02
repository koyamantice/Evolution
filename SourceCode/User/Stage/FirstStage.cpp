#include "FirstStage.h"

void FirstStage::Initialize(DirectXCommon* dxCommon) {
	InitCommon(dxCommon);
	BattleInit();
	TorchSetup(Touch::FireColor::f_green);
	//�Q�[���A�N�^�[�̐��������܂��B
	ActorManager::GetInstance()->AttachActor("Player");
	ActorManager::GetInstance()->AttachActor("MashGhost");
	lightGroup->SetPointLightActive(0, true);
	lightGroup->SetPointLightActive(1, true);
	lightGroup->SetPointLightActive(2, true);
	lightGroup->DefaultLightSetting();

	for (int i = 0; i < 3; i++) {
		lightGroup->SetPointLightPos(i, pointLightPos[i]);
	}


	for (int i = 0; i < kGnormNum; i++) {
		ActorManager::GetInstance()->AttachBullet("Red");
	}
	for (int i = 0; i < kGnormNum; i++) {
		ActorManager::GetInstance()->AttachBullet("Green");
	}
	//�V�[�����ŕK�v�ȃA�N�^�[���Q�Ƃ��܂��B
	player_shadow = ActorManager::GetInstance()->SearchActor("Player");
	enemy_shadow = ActorManager::GetInstance()->SearchActor("Enemy");

	//���������̌��t
	for (int i = 0; i < intro_word_max; i++) {
		IntroWord[i] = Sprite::Create(ImageManager::Intro01 + i, { 1230.0f,600.0f }, { 1,1,1,1 }, { 1.0f, 0 });
	}

	//�J�����̏�����
	camera_distance.x = sinf(camera_angle * (XM_PI / DEGREE_HALF)) * camera_radius;
	camera_distance.z = cosf(camera_angle * (XM_PI / DEGREE_HALF)) * camera_radius;
	player_shadow->SetAngle(camera_angle);
	camera->SetTarget(player_shadow->GetCameraPos(camera_angle, camera_target));
	camera->SetEye(XMFLOAT3{
		player_shadow->GetPosition().x + camera_distance.x,
		first_hight,
		player_shadow->GetPosition().z + camera_distance.z
		});
	camera->Update();

	//�|�X�g�G�t�F�N�g�̏�����
	postEffect = new PostEffect();
	postEffect->Initialize();
	//�I�[�f�B�I
	audioManager = std::make_unique<AudioManager>();

	audioManager->LoadWave("BGM/battle.wav");
	audioManager->PlayWave("BGM/battle.wav", 0.5f);
	audioManager->LoadWave("SE/lastHit.wav");
	audioManager->LoadWave("BGM/Smash.wav");

	//�p�[�e�B�N���̏�����
	particleEmitter = std::make_unique <ParticleEmitter>(ImageManager::charge);
}
//�X�V
void FirstStage::Update(DirectXCommon* dxCommon) {
	for (int i = 0; i < 3; i++) {
		lightGroup->SetPointLightPos(i, pointLightPos[i]);
	}
	//���������̏���
	if (IntroUpdate()) { return; }
	//�N���A��̏���
	if (ClearUpdate()) { return; }
	//�|�[�Y�����̍X�V
	if (PauseUpdate()) { return; }
	//�Q�[���I�[�o�[�̏���
	GameOverUpdate();
	//�A�N�^�[���ׂĂ̍X�V����
	ActorManager::GetInstance()->Update();
	//�J�����̍X�V����
	CameraUpda();
	//�X�e�[�W�̍X�V����
	FieldUpdate();
	//��������̍X�V����
	hud->Update();
	//�p�[�e�B�N���̍X�V����
	particleEmitter->Update();
}

//�`��
void FirstStage::Draw(DirectXCommon* dxCommon) {
	dxCommon->PreDraw();
	DebugDraw();
	BattleBackDraw();
	//�w�i�p
	ActorManager::GetInstance()->Draw(dxCommon);
	BattleFrontDraw(alphaBle, IntroWord[nowWord].get());
	dxCommon->PostDraw();
}

//�J������
void FirstStage::Finalize() {
	//�R���̃��f���̃f���[�g
	ActorManager::GetInstance()->Finalize();
}

bool FirstStage::DebugDraw() {
	if (!isDebug) { return false; }
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
	ImGui::SetNextWindowPos(ImVec2(1100, 0));
	//ImGui::SetNextWindowSize(ImVec2(180, 240));

	ImGui::Begin("scene");
	ImGui::Text("fps:%f", FPSManager::GetInstance()->GetFps());
	ImGui::Text("distance.x:%f", camera_distance.x);
	ImGui::Text("distance.z:%f", camera_distance.z);
	ImGui::SliderFloat("pointLightPos[0].x", &pointLightPos[0].x, -50.0f,50.0f);
	ImGui::SliderFloat("pointLightPos[0].y", &pointLightPos[0].y, -50.0f,50.0f);
	ImGui::SliderFloat("pointLightPos[0].z", &pointLightPos[0].z, -50.0f,50.0f);
	ImGui::SliderFloat("pointLightPos[1].x", &pointLightPos[1].x, -50.0f,50.0f);
	ImGui::SliderFloat("pointLightPos[1].y", &pointLightPos[1].y, -50.0f,50.0f);
	ImGui::SliderFloat("pointLightPos[1].z", &pointLightPos[1].z, -50.0f,50.0f);
	ImGui::SliderFloat("pointLightPos[2].x", &pointLightPos[2].x, -50.0f,50.0f);
	ImGui::SliderFloat("pointLightPos[2].y", &pointLightPos[2].y, -50.0f,50.0f);
	ImGui::SliderFloat("pointLightPos[2].z", &pointLightPos[2].z, -50.0f,50.0f);

	
	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	return true;
}

bool FirstStage::IntroUpdate() {
	if (!battle_intro) { return false; }
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

	//�X�L�b�v
	if (intro_skip) {
		if (feedin_frame > 0.0f) {
			feedin_frame -= 1.0f / feedin_frame_max;
		} else {
			intro_skip = false;
			intro_count = intro_count_max;
			IntroCamera(intro_count_max);
			ActorManager::GetInstance()->IntroUpdate(intro_count / intro_count_max, "", kSecondScene);
			FieldUpdate();
		}
		filter_alpha = Ease(Out, Cubic, feedin_frame, 1.0f, 0.0f);
		filter_first->SetColor({ 1,1,1,filter_alpha });
		return true;
	}

	if (input->TriggerButton(input->START) ||
		input->TriggerKey(DIK_SPACE)) {
		if (!intro_skip) {
			intro_skip = true;
		}
	}

	intro_count += intro_speed;

	//���������I������
	if (intro_count >= intro_count_max) {
		intro_count = 0;
		battle_intro = false;
		return true;
	}

	//�����̌��𕪊����Ă��܂�.
	if ((int)intro_count % ((int)(intro_count_max / intro_speed) / intro_word_max) == 0) {
		if (nowWord != intro_word_max - 1) {
			nowWord++;
		}
	}

	IntroCamera(intro_count);
	ActorManager::GetInstance()->IntroUpdate(intro_count / intro_count_max);
	FieldUpdate();
	return true;
}

void FirstStage::IntroCamera(const float timer) {
	//���n���Ă���̗P�\
	const float delay = 0.9f;
	const float reaching_time = intro_count_max * delay;
	float hight = camera_hight;
	if (timer / reaching_time <= 1.0f) {
		camera_angle = Ease(In, Linear, timer / reaching_time, DEGREE_MAX, 0);
		hight = Ease(In, Linear, timer / reaching_time, first_hight, camera_hight);
	}
	if (timer / intro_count_max >= 1.0f) {
		camera_angle = min(0, camera_angle);
		hight = camera_hight;
	}
	camera_distance.x = sinf(camera_angle * (XM_PI / DEGREE_HALF)) * camera_radius;
	camera_distance.z = cosf(camera_angle * (XM_PI / DEGREE_HALF)) * camera_radius;

	player_shadow->SetAngle(camera_angle);
	camera->SetTarget(player_shadow->GetCameraPos(camera_angle, camera_target));
	camera->SetEye(XMFLOAT3{ player_shadow->GetPosition().x + camera_distance.x,hight,player_shadow->GetPosition().z + camera_distance.z });
	camera->Update();
}

void FirstStage::DescriptionUpdate() {
}

void FirstStage::ResultCamera(const float timer) {
	camera->SetTarget(player_shadow->GetCameraPos(camera_angle, camera_target));
	camera->SetEye(XMFLOAT3{ player_shadow->GetPosition().x + camera_distance.x,player_shadow->GetPosition().y + camera_hight,player_shadow->GetPosition().z + camera_distance.z });
	camera->Update();
}

void FirstStage::SmashCamera(const float timer) {
	XMFLOAT3 s_eye = { player_shadow->GetPosition().x + camera_distance.x,player_shadow->GetPosition().y + camera_hight,player_shadow->GetPosition().z + camera_distance.z };
	XMFLOAT3 e_eye = { enemy_shadow->GetPosition().x + camera_distance.x,enemy_shadow->GetPosition().y + 25,enemy_shadow->GetPosition().z + camera_distance.z };
	XMFLOAT3 ease_target = enemy_shadow->GetPosition();
	XMFLOAT3 ease_eye = e_eye;
	float ease_time = timer / (float)60.0f;

	if (ease_time <= 1.0f) {

		ease_target = {
		Ease(In,Linear,ease_time,player_shadow->GetCameraPos(camera_angle, camera_target).x,enemy_shadow->GetPosition().x),
		Ease(In,Linear,ease_time,player_shadow->GetCameraPos(camera_angle, camera_target).y,enemy_shadow->GetPosition().y),
		Ease(In,Linear,ease_time,player_shadow->GetCameraPos(camera_angle, camera_target).z,enemy_shadow->GetPosition().z)
		};

		ease_eye = {
			Ease(In,Linear,ease_time,s_eye.x,e_eye.x),
			Ease(In,Linear,ease_time,s_eye.y,e_eye.y),
			Ease(In,Linear,ease_time,s_eye.z,e_eye.z)
		};
	}

	camera->SetTarget(ease_target);
	camera->SetEye(ease_eye);
	camera->Update();

}


bool FirstStage::ClearUpdate() {
	if (stage_clear) {
		if (input->TriggerButton(Input::A) || input->TriggerButton(Input::B)) {
			sceneChanger_->ChangeStart();
		}
		ResultCamera(0);
		sceneChanger_->ChangeScene("MSECOND");
		ActorManager::GetInstance()->ResultUpdate(0);
		//�p�[�e�B�N���̍X�V����
		particleEmitter->Update();
		FieldUpdate();
		return true;
	}
	if (enemy_shadow->GetPause()) {
		battle_result = true;
		if (player_shadow->GetCanMove()) {
			player_shadow->SetCanMove(false);
			audioManager->PlayWave("SE/lastHit.wav", 0.5f);
			audioManager->StopWave("BGM/battle.wav");
		}
		const float rnd_vel = 0.4f;
		particleEmitter->AddCommon(120, enemy_shadow->GetPosition(), rnd_vel, 0, 1.2f, 0.0f, { 1,1,1,1 }, { 1,1,1,0 });
		particleEmitter->Update();
		finish_time++;

		SmashCamera((float)(finish_time));
		//enemy_shadow->Update();
		ActorManager::GetInstance()->Update();
		FieldUpdate();

		if (finish_time >= 100.0f) {
			enemy_shadow->SetCanMove(false);
		}

		if (finish_time >= 200.0f) {
			stage_clear = true;
			audioManager->PlayWave("BGM/Smash.wav", 0.3f);
		}
		return true;
	}
	return false;
}


bool FirstStage::GameOverUpdate() {
	if (ActorManager::GetInstance()->SearchNum("Bullet") <= 0) {
		gameover = true;
		player_shadow->SetCanMove(false);
		sceneChanger_->ChangeStart();
	}
	if (gameover) {
		sceneChanger_->ChangeGameOver();
	}
	return false;

}
