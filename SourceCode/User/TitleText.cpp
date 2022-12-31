#include"TitleText.h"
#include <SourceCode/User/ModelManager.h>
#include"Easing.h"




TitleText::TitleText() {
}

TitleText::~TitleText() {
}

void TitleText::Init() {

	for (int i = 0; i < 6; i++) {
		Object3d* newText_ = new Object3d();
		if (i == 0) {
			newText_->SetModel(ModelManager::GetIns()->GetModel(ModelManager::kTextG));
		} else if (i == 1) {
			newText_->SetModel(ModelManager::GetIns()->GetModel(ModelManager::kTextN));
		} else if (i == 2) {
			newText_->SetModel(ModelManager::GetIns()->GetModel(ModelManager::kTextO));
		} else if (i == 3) {
			newText_->SetModel(ModelManager::GetIns()->GetModel(ModelManager::kTextM));
		} else if (i == 4) {
			newText_->SetModel(ModelManager::GetIns()->GetModel(ModelManager::kTextE));
		} else {
			newText_->SetModel(ModelManager::GetIns()->GetModel(ModelManager::kTextN));
		}
		newText_->Initialize();
		newText_->SetPosition({ 0,-10,-500 });
		newText_->SetRotation({ 90,0,0 });
		newText_->SetScale({ 5.0f,5.0f,5.0f });
		texts[i].reset(newText_);
	}
	for (int i = 0; i < 6; i++) {
		texts[i]->Initialize();
	}

	// モデル読み込み
	modelSkydome = Model::CreateFromOBJ("skydome");
	modelGround = Model::CreateFromOBJ("ground");
	modelFighter = Model::CreateFromOBJ("House");
	modelSphere = Model::CreateFromOBJ("snag");
	modelPine = Model::CreateFromOBJ("Pine1");

	models.insert(std::make_pair("skydome", modelSkydome));
	models.insert(std::make_pair("ground", modelGround));
	models.insert(std::make_pair("house", modelFighter));
	models.insert(std::make_pair("snag", modelSphere));
	models.insert(std::make_pair("Pine1",modelPine));
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

	frame += 0.01f;

	if (frame < 1.0f) {
		pos[0]=Ease(In,Back,frame,0,-16.0f);
		pos[1]=Ease(In,Back,frame,0,-10.0f);
		pos[2]=Ease(In,Back,frame,0,-4.0f);
		pos[3]=Ease(In,Back,frame,0,2.0f);
		pos[4]=Ease(In,Back,frame,0,8.0f);
		pos[5]=Ease(In,Back,frame,0,14.0f);
	} else {
		frame = 1.0f;
	}

	for (int i = 0; i < 6; i++) {
		texts[i]->SetPosition({ pos[i],8.0f,10.0f });
	}
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
}
