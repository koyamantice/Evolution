#include"TitleText.h"
#include <SourceCode/User/ModelManager.h>
#include"Easing.h"
#include"ImageManager.h"

#define DEGREE_MAX 360.0f
#define DEGREE_HALF 180.0f
#define DEGREE_QUARTER 90.0f

TitleText::TitleText() {
	Init();
}

TitleText::~TitleText() {
	delete modelSkydome;
	delete modelGround;
	delete modelFighter;
	delete modelSphere;
	delete modelPine;
	grounds.clear();
	models.clear();
}

void TitleText::Init() {

	for (int i = 0; i < 6; i++) {
		Object3d* newText_ = new Object3d();
		if (i == 0) {
			newText_->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::kTextG));
		} else if (i == 1) {
			newText_->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::kTextN));
		} else if (i == 2) {
			newText_->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::kTextO));
		} else if (i == 3) {
			newText_->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::kTextM));
		} else if (i == 4) {
			newText_->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::kTextE));
		} else {
			newText_->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::kTextN));
		}
		newText_->Initialize();
		newText_->SetPosition(pos[i]);
		newText_->SetRotation({ DEGREE_QUARTER,DEGREE_HALF,0 });
		newText_->SetScale({ 8.0f,8.0f,8.0f });
		texts[i].reset(newText_);
	}
	for (int i = 0; i < 6; i++) {
		texts[i]->Initialize();
		texts[i]->SetPosition({ pos[i] });

	}
	
	Object3d* door_ = new Object3d();
	door_->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::kDoor));
	door_->Initialize();
	door_->SetPosition({2.0f,0,10.0f});
	door_->SetRotation({ 0,DEGREE_QUARTER,0 });
	door_->SetScale({ 3.0f,3.0f,3.0f });
	door.reset(door_);


	partMan = new ParticleManager();
	partMan->Initialize(ImageManager::fire);


	// モデル読み込み
	modelSkydome = Model::CreateFromOBJ("skydome");
	modelGround = Model::CreateFromOBJ("ground");
	modelFighter = Model::CreateFromOBJ("House");
	modelPine = Model::CreateFromOBJ("Pine");

	models.insert(std::make_pair("skydome", modelSkydome));
	models.insert(std::make_pair("Ground", modelGround));
	models.insert(std::make_pair("House", modelFighter));
	models.insert(std::make_pair("Pine",modelPine));
	levelData = LevelLoader::LoadFile("level_editor");
	// レベルデータからオブジェクトを生成、配置
	for (auto& objectData : levelData->objects) {
		// ファイル名から登録済みモデルを検索
		Model* model = nullptr;
		decltype(models)::iterator it = models.find(objectData.fileName);
		if (it != models.end()) {
			model = it->second;
		}

		// モデルを指定して3Dオブジェクトを生成
		Object3d* newObj = nullptr;
		newObj = new Object3d();
		newObj->Initialize();
		newObj->SetModel(model);

		// 座標
		DirectX::XMFLOAT3 pos;
		DirectX::XMStoreFloat3(&pos, objectData.translation);
		newObj->SetPosition(pos);

		// 回転角
		DirectX::XMFLOAT3 rot;
		DirectX::XMStoreFloat3(&rot, objectData.rotation);
		newObj->SetRotation(rot);

		// 座標
		DirectX::XMFLOAT3 scale;
		DirectX::XMStoreFloat3(&scale, objectData.scaling);
		newObj->SetScale(scale);

		std::unique_ptr<Object3d> newObj_;
		newObj_.reset(newObj);
		// 配列に登録
		grounds.push_back(std::move(newObj_));
	}
}

void TitleText::Upda() {
	for (int i = 0; i < 6; i++) {
		texts[i]->Update();
	}

	DoorUpdate();

	for (std::unique_ptr<Object3d>& obj : grounds) {
		obj->Update();
	}
}

void TitleText::Draw(DirectXCommon* dxCommon) {
	for (int i = 0; i < 6; i++) {
		texts[i]->Draw();
	}
	for (std::unique_ptr<Object3d>& obj : grounds) {
		obj->Draw();
	}
	door->Draw();
	partMan->Draw(addBle);
}

void TitleText::DoorUpdate() {
	if (door_status == kOpening) {
		const float rnd_pos = 1.0f;
		XMFLOAT3 mag{};
		mag.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		mag.y = (float)rand() / RAND_MAX * rnd_pos;
		mag.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		const float rnd_vel = 0.2f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel;
		vel.z = (float)rand() / RAND_MAX * rnd_vel;
		vel.y += 0.1f;
		const float rnd_acc = 0.01f;
		XMFLOAT3 acc{};
		acc.x = (float)rand() / RAND_MAX * rnd_acc - rnd_acc / 2.0f;
		acc.y = (float)rand() / RAND_MAX * rnd_acc - rnd_acc / 2.0f;
		partMan->Add(60, { -2.0f + mag.x,2 + mag.y,10.0f + mag.z }, vel, acc, 3.0f, 0.0f, { 1.0f,1.0f,0,1.0f }, { 1.0f,1.0f,0,1.0f });
	}
	partMan->Update();

	float rot = 0;
	frame += door_vel * door_status;
	if (frame <= 1.0f && frame >= 0.0f) {
		rot = Ease(In, Linear, frame, DEGREE_QUARTER, DEGREE_HALF);
	} else {
		if (door_status == kOpening) {
			door_status = kClosing;
			rot = DEGREE_HALF;
		} else {
			door_status = kOpening;
			rot = DEGREE_QUARTER;
		}
	}
	door->SetRotation({ 0,rot,0 });
	door->Update();
}
