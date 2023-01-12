#include "PlayScene.h"
#include "SceneManager.h"
#include "AudioManager.h"
#include "input.h"
#include <string>
#include "ModelManager.h"
#include <TisGame.h>
#include "Player.h"
#include "PlayerUI.h"
#include"Enemy.h"
#include <SourceCode/Common/Easing.h>
#include"ActorManager.h"

void PlayScene::Initialize(DirectXCommon* dxCommon) {
	InitCommon(dxCommon);
	LoadData();
	UpdateCommand();
	for (std::unique_ptr<Touch>& touch : touchs) {
		touch->SetColor(Touch::FireColor::f_green);
	}

	//スプライト生成
	ActorManager::GetInstance()->AttachActor("Player");
	player_shadow = ActorManager::GetInstance()->SearchActor("Player");
	ActorManager::GetInstance()->AttachActor("Enemy");
	enemy_shadow = ActorManager::GetInstance()->SearchActor("Enemy");
	ActorManager::GetInstance()->AttachActor("ClearCrystal");
	goal_shadow = ActorManager::GetInstance()->SearchActor("ClearCrystal");
	goal_shadow->SetPosition(enemy_shadow->GetPosition());
	goal_shadow->SetIsActive(false);
	for (int i = 0; i < 10; i++) {
		ActorManager::GetInstance()->AttachBullet("Red");
	}
	for (int i = 0; i < 10; i++) {
		ActorManager::GetInstance()->AttachBullet("Green");
	}


	Object3d* Sky{};
	Sky = new Object3d();
	Sky->SetModel(ModelManager::GetIns()->GetModel(ModelManager::kSkydome));
	Sky->SetScale(XMFLOAT3(1.0f, 1.0f, 1.0f));
	Sky->SetColor({0.3f,0.3f,0.3f,1.0f});
	Sky->Initialize();
	kSkydome.reset(Sky);

	TouchableObject* kGround{};
	kGround = new TouchableObject();
	kGround->Initialize(ModelManager::GetIns()->GetModel(ModelManager::kGround));
	kGround->SetPosition(XMFLOAT3(-50, -0.5f, 50));
	kGround->SetScale(XMFLOAT3(5, 5, 5));
	ground.reset(kGround);

	const int w = 256;
	const int h = 256;
	const int l = 6;
	for (int i = 0; i < 6; i++) {
		Sprite* CameCon_[6]{};
		CameCon_[i] = Sprite::Create(ImageManager::CameCon, { 0,0 });
		Camecon[i].reset(CameCon_[i]);
		int number_index_y = i / l;
		int number_index_x = i % l;
		Camecon[i]->SetTextureRect(
			{ static_cast<float>(number_index_x) * w, static_cast<float>(number_index_y) * h },
			{ static_cast<float>(w), static_cast<float>(h) });
		Camecon[i]->SetSize({ 128,128 });
		Camecon[i]->SetScale(1.0f);
		Camecon[i]->SetAnchorPoint({ 0.5f,0.5f });
	}
	Camecon[0]->SetPosition({ 440.0f,530.0f });
	Camecon[1]->SetPosition({ 440.0f,530.0f });
	Camecon[2]->SetPosition({ 440.0f,530.0f });
	Camecon[3]->SetPosition({ 840.0f,530.0f });
	Camecon[4]->SetPosition({ 840.0f,530.0f });
	Camecon[5]->SetPosition({ 640.0f,530.0f });
	const int p = 2;
	for (int i = 0; i < 2; i++) {
		Sprite* Rockon_[2]{};
		Rockon_[i] = Sprite::Create(ImageManager::Rockon, { 0,0 });
		Rockon[i].reset(Rockon_[i]);
		int number_index_y = i / p;
		int number_index_x = i % p;
		Rockon[i]->SetTextureRect(
			{ static_cast<float>(number_index_x) * w, static_cast<float>(number_index_y) * h },
			{ static_cast<float>(w), static_cast<float>(h) });
		Rockon[i]->SetSize({ 84,84 });
		Rockon[i]->SetScale(1.0f);
		Rockon[i]->SetAnchorPoint({ 0.5f,0.5f });
	}

	Sprite* _clear = nullptr;
	_clear = Sprite::Create(ImageManager::Clear, { 0,0 });
	Clear.reset(_clear);

	Sprite* _Over = nullptr;
	_Over = Sprite::Create(ImageManager::Over, { 0,0 });
	Over.reset(_Over);

	//スプライト生成
	Sprite* Effect_ = Sprite::Create(ImageManager::Black, { 0.0f,0.0f });
	FeedBlack.reset(Effect_);
	FeedBlack->SetColor({ 1,1,1,feedAlpha });

	Sprite* IntroWord_1 = Sprite::Create(ImageManager::Intro01, { 1230.0f,600.0f }, { 1,1,1,1 }, { 1.0f, 0 });
	IntroWord[0].reset(IntroWord_1);
	Sprite* IntroWord_2 = Sprite::Create(ImageManager::Intro02, { 1230.0f,600.0f }, { 1,1,1,1 }, { 1.0f, 0 });
	IntroWord[1].reset(IntroWord_2);
	Sprite* IntroWord_3 = Sprite::Create(ImageManager::Intro03, { 1230.0f,600.0f }, { 1,1,1,1 }, { 1.0f, 0 });
	IntroWord[2].reset(IntroWord_3);
	Sprite* IntroWord_4 = Sprite::Create(ImageManager::Intro04, { 1230.0f,600.0f }, { 1,1,1,1 }, { 1.0f, 0 });
	IntroWord[3].reset(IntroWord_4);
	Sprite* IntroWord_5 = Sprite::Create(ImageManager::Intro05, { 1230.0f,600.0f }, { 1,1,1,1 }, { 1.0f, 0 });
	IntroWord[4].reset(IntroWord_5);
	Sprite* IntroWord_6 = Sprite::Create(ImageManager::Intro06, { 1230.0f,600.0f }, { 1,1,1,1 }, { 1.0f, 0 });
	IntroWord[5].reset(IntroWord_6);

	camera->SetTarget(player_shadow->GetPosition());

	postEffect = new PostEffect();
	postEffect->Initialize();

	distance.x = sinf(angle * (XM_PI / 180)) * 13.0f;
	distance.y = cosf(angle * (XM_PI / 180)) * 13.0f;

	dis.x = distance.x;
	dis.y = distance.y;

	partMan = new ParticleManager();
	partMan->Initialize(ImageManager::charge);


}
//開放処理
void PlayScene::Finalize() {
	//３ｄのモデルのデリート
	ActorManager::GetInstance()->Finalize();
}
//更新
void PlayScene::Update(DirectXCommon* dxCommon) {
	Input* input = Input::GetInstance();
	for (std::unique_ptr<Touch>& touch : touchs) {
		touch->Update();
	}
	if (clear) {
		ResultCamera(count);
		count++;
		ActorManager::GetInstance()->ResultUpdate(count);
		const float rnd_vel = 0.5f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		partMan->Add(100, goal_shadow->GetPosition(), vel, XMFLOAT3(), 1.2f, 0.0f, { 1,1,0.5f,1 }, { 1,1,1,0.3f });
		partMan->Update();
		if (input->TriggerButton(Input::A) || input->TriggerButton(Input::B)) {
			Change = true;
		}
		Feed("MAP");
		SkydomeUpdate();
		ground->Update();

		return;
	}
	if (Intro) {
		IntroCamera(count);
		if (Change) {
			if (introFrame < 1.0f) {
				introFrame += 0.01f;
			} else {
				Change = false;
			}
			feedAlpha = Ease(In, Cubic, introFrame, 1, 0);
			FeedBlack->SetColor({ 1,1,1,feedAlpha });
		}
		ActorManager::GetInstance()->IntroUpdate(count);
		kSkydome->Update();
		ground->Update();
		if (count > 1200) {
			FeedBlack->SetColor({ 1,1,1,0 });
			count = 0;
			Intro = false;
		}
		count += speed;
		if (input->PushButton(input->START)) {
			if (count %2 ==1) {
				count--;
			}
			speed = 2;
		}
		if (count % 200 == 0) {
			if (nowWord != 5) {
				nowWord++;
			}
		}
		return;
	}
	if (enemy_shadow->GetPause()) {
		const float rnd_vel = 0.4f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		partMan->Add(120, enemy_shadow->GetPosition(), vel, XMFLOAT3(), 1.2f, 0.0f,{1,1,1,1},{1,1,1,0});
		partMan->Update();

		finishTime++;

		SkydomeSunny(finishTime);

		if (finishTime > 200) {
			enemy_shadow->SetPause(false);
			enemy_shadow->SetCommand(Actor::DEAD);
		}
		return;
	}
	CameraUpda();
	if (pause) {
		pauseUi->Update();
		if (pauseUi->GetEndFlag()) {
			pause = false;
		}
		return;
	}
	if (input->TriggerButton(input->START)) {
		pause = true;
		if (pauseUi->GetEndFlag()) {
			pauseUi->SetEndFlag(false);
		}
	}
	animafrate++;
	if (animafrate == 30) {
		if (animation < 2&&animation > 0) {
			animation += vec;
		} else if(animation == 2) {
			animation = 1;
			vec *= -1;
		} else if(animation == 0) {
			animation = 1;
			vec *= -1;
		}
		if (tapanima==3) {
			tapanima = 4;
		} else {
			tapanima = 3;
		}
		animafrate = 0;
	}
	if (cameraExplanation) {
		CameraAlpha *= 0.8f;
		for (int i = 0; i < 6;i++) {
			Camecon[i]->SetColor({ 1,1,1,CameraAlpha });
		}
	}
	ActorManager::GetInstance()->Update();
	SkydomeUpdate();
	ground->Update();
	partMan->Update();
#pragma region "Clear"
	if (!enemy_shadow->GetIsActive()) {
		goal_shadow->SetIsActive(true);
		if (goal_shadow->GetPause()||player_shadow->GetPause()) {
			Result = true;
			clear = true;
		}
	}
#pragma endregion
#pragma region "GameOver"
	if (ActorManager::GetInstance()->SearchNum("Bullet") <= 0) {
		GameOver = true;
		if (input->TriggerButton(Input::A)) {
			SceneManager::GetInstance()->ChangeScene("TITLE");
		}
	}
#pragma endregion
}

void PlayScene::CameraUpda() {
	Input* input = Input::GetInstance();
	if (Reset) {
		ResetCamera();
		return;
	}
	if (input->TiltPushStick(Input::R_RIGHT) || input->TiltPushStick(Input::R_LEFT)) {
		if (!cameraExplanation) {
			cameraExplanation = true;
		}
		if (!pauseUi->GetReverseCamera()) {
			if (input->TiltPushStick(Input::R_RIGHT)) {
				angle -= 3;
			}
			if (input->TiltPushStick(Input::R_LEFT)) {
				angle += 3;
			}
		} else {
			if (input->TiltPushStick(Input::R_RIGHT)) {
				angle += 3;
			}
			if (input->TiltPushStick(Input::R_LEFT)) {
				angle -= 3;
			}
		}
		if (angle > 360||angle < 0) {
			angle += 360;
			angle = (float)((int)angle%360);
		}
		dis.x = sinf(angle * (XM_PI / 180)) * 15.0f;
		dis.y = cosf(angle * (XM_PI / 180)) * 15.0f;
		distance.x = Ease(In, Quad, 0.6f, distance.x, dis.x);
		distance.y = Ease(In, Quad, 0.6f, distance.y, dis.y);

	}
	if (input->TriggerButton(Input::LT)) {
		if (!Reset) {
			if (!cameraExplanation) {
				cameraExplanation = true;
			}
			angleframe = 0.0f;
			firstangle = angle;
			endangle = player_shadow->GetRotation().y;
			firstdis = distance;
			Reset = true;
		}
	}
	player_shadow->SetAngle(angle);
	camera->SetTarget(player_shadow->GetCameraPos(angle,7));
	camera->SetEye(XMFLOAT3{ player_shadow->GetPosition().x + distance.x,player_shadow->GetPosition().y + hight,player_shadow->GetPosition().z + distance.y });
	camera->Update();
}

void PlayScene::IntroCamera(int Timer) {
	if (Timer <= 720) {
		if (speed == 1) {
			angle += 0.5f;
			if (IntroHight > hight) {
				IntroHight -= 0.075f;
			} else {
				IntroHight = hight;
			}
		} else {
			angle += 1.0f;
			if (IntroHight > hight) {
				IntroHight -= 0.150f;
			} else {
				IntroHight = hight;
			}
		}
	}

	dis.x = sinf(angle * (XM_PI / 180)) * 13.0f;
	dis.y = cosf(angle * (XM_PI / 180)) * 13.0f;
	distance.x = Ease(In, Quad, 0.6f, distance.x, dis.x);
	distance.y = Ease(In, Quad, 0.6f, distance.y, dis.y);
	player_shadow->SetAngle(angle);
	camera->SetTarget(player_shadow->GetCameraPos(angle,7));
	camera->SetEye(XMFLOAT3{ player_shadow->GetPosition().x + distance.x,IntroHight,player_shadow->GetPosition().z + distance.y });
	camera->Update();
}

void PlayScene::ResultCamera(int Timer) {
	if (Timer <= 720) {
		angle += 0.5f;
	}

	//player_shadow->SetAngle(angle);
	camera->SetTarget(goal_shadow->GetPosition());
	camera->SetEye(XMFLOAT3{ player_shadow->GetPosition().x + distance.x,player_shadow->GetPosition().y + hight,player_shadow->GetPosition().z + distance.y });
	camera->Update();
}

//描画
void PlayScene::Draw(DirectXCommon* dxCommon) {
	dxCommon->PreDraw();
	//postEffect->PreDrawScene(dxCommon->GetCmdList());
	//ImGui::Begin("playscene");
	//ImGui::SliderFloat("bulletX", &distance.x, 0, 360);
	//ImGui::SliderFloat("bulletY", &distance.y, 0, 360);
	//ImGui::SliderFloat("Anglet", &angle, 0, 360);
	//ImGui::End();
	Object3d::PreDraw();
	kSkydome->Draw();
	ground->Draw();
	for (std::unique_ptr<Touch>& touch : touchs) {
		touch->Draw();
	}
	//背景用
	ActorManager::GetInstance()->Draw(dxCommon);
	partMan->Draw(alphaBle);
	Sprite::PreDraw();
	if (Change) {
		FeedBlack->Draw();
	}
	if (clear) {
		Clear->Draw();
	}
	if (Intro) {
		Screen[0]->Draw();
		Screen[1]->Draw();
		IntroWord[nowWord]->Draw();
	} else{
		if (!clear) {
			Camecon[animation]->Draw();
			Camecon[tapanima]->Draw();
			Camecon[5]->Draw();
		}
	}
	if (Result) {
		Screen[0]->Draw();
		Screen[1]->Draw();
		Clear->Draw();
	}
	if (GameOver) {
		Over->Draw();
	}
	if (pause) {
		pauseUi->Draw();
	}
	dxCommon->PostDraw();
}

void PlayScene::ResetCamera() {
	player_shadow->SetCanMove(false);

	if (angleframe < 1.0f) {
		angleframe += 0.05f;
	} else {
		Reset = false;
		player_shadow->SetCanMove(true);
		angleframe = 1.0f;
		distance.x = dis.x;
		distance.x = dis.y;
	}

	angle = Ease(In, Quad, angleframe, firstangle, endangle);

	dis.x = sinf(angle * (XM_PI / 180)) * 13.0f;
	dis.y = cosf(angle * (XM_PI / 180)) * 13.0f;

	distance.x = Ease(In, Quad, angleframe, firstdis.x, dis.x);
	distance.y = Ease(In, Quad, angleframe, firstdis.y, dis.y);
	camera->SetTarget(player_shadow->GetCameraPos(angle,7));
	camera->SetEye(XMFLOAT3{ player_shadow->GetPosition().x + distance.x,player_shadow->GetPosition().y + hight,player_shadow->GetPosition().z + distance.y });
	camera->Update();
}

void PlayScene::SkydomeUpdate() {
	float rot = kSkydome->GetRotation().y;
	rot += 0.1f;
	
	kSkydome->SetRotation({ 0,rot,0 });
	
	kSkydome->Update();
}

void PlayScene::SkydomeSunny(int time) {



}

void PlayScene::LoadData() {
	std::ifstream file;
	file.open("Resources/csv/touchpop.csv");
	assert(file.is_open());
	touch_pop << file.rdbuf();
	file.close();
}

void PlayScene::UpdateCommand() {
	std::string line;
	while (getline(touch_pop, line)) {
		//解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		//半角スペース区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');
		//"//"から始まる行はコメント
		if (word.find("//") == 0) {
			//飛ばす
			continue;
		}
		//各コマンド
		if (word.find("POP") == 0) {
			getline(line_stream, word, ',');
			XMFLOAT3 pos{};
			pos.x= (float)std::atof(word.c_str());
			getline(line_stream, word, ',');
			pos.y = (float)std::atof(word.c_str());
			getline(line_stream, word, ',');
			pos.z = (float)std::atof(word.c_str());
			getline(line_stream, word, ',');
			if (word.find("DIR") == 0) {
				getline(line_stream, word, ',');
				float rot = 0;
				if (word.find("FRONT") == 0) {
					rot = 0;
				}else if (word.find("BACK") == 0) {
					rot = 180;
				}else if (word.find("RIGHT") == 0) {
					rot = 90;
				}else if (word.find("LEFT") == 0) {
					rot = -90;
				}
				getline(line_stream, word, ',');
				std::unique_ptr<Touch> new_touch;
				new_touch.reset(new Touch(pos,{0,rot,0}));
				touchs.push_back(std::move(new_touch));
			}
		}
		if (word.find("END") == 0) {
			break;
		}
	}


}
