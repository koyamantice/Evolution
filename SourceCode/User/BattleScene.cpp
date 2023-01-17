#include "BattleScene.h"
#include <ModelManager.h>

void BattleScene::BattleInit() {
	//導入枠
	Sprite* _Screen = nullptr;
	_Screen = Sprite::Create(ImageManager::SceneCover, { 0,0 });
	screens[0].reset(_Screen);
	Sprite* _Screen2 = nullptr;
	_Screen2 = Sprite::Create(ImageManager::SceneCover, { 0,600 });
	screens[1].reset(_Screen2);

	//ポーズUIを生成
	PauseUI* pause_ui = new PauseUI();
	pauseUi.reset(pause_ui);

	//スプライト生成
	Sprite* first_ = Sprite::Create(ImageManager::Black, { 0.0f,0.0f });
	filter_first.reset(first_);
	filter_first->SetColor({ 1,1,1,filter_alpha });


	//ステージの生成します
	Object3d* skydome_{};
	skydome_ = new Object3d();
	skydome_->SetModel(ModelManager::GetIns()->GetModel(ModelManager::kSkydome));
	skydome_->SetScale(XMFLOAT3(1.0f, 1.0f, 1.0f));
	skydome_->SetColor({ 0.3f,0.3f,0.3f,1.0f });
	skydome_->Initialize();
	skydome.reset(skydome_);

	Object3d* ground_{};
	ground_ = new Object3d();
	ground_->SetModel(ModelManager::GetIns()->GetModel(ModelManager::kGround));
	ground_->SetPosition(XMFLOAT3(-50, -0.5f, 50));
	ground_->SetScale(XMFLOAT3(5, 5, 5));
	ground_->Initialize();
	ground.reset(ground_);

}

void BattleScene::FieldUpdate() {
	float rot = skydome->GetRotation().y;
	rot += 0.1f;
	skydome->SetRotation({ 0,rot,0 });
	skydome->Update();
	for (std::unique_ptr<Touch>& torch : torchs) {
		torch->Update();
	}
	ground->Update();
}

void BattleScene::TorchSetup(int color) {
	std::ifstream file{};
	file.open("Resources/csv/torchpop.csv");
	assert(file.is_open());
	torch_pop << file.rdbuf();
	file.close();

	std::string line;
	while (getline(torch_pop, line)) {
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
			pos.x = (float)std::atof(word.c_str());
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
				} else if (word.find("BACK") == 0) {
					rot = 180;
				} else if (word.find("RIGHT") == 0) {
					rot = 90;
				} else if (word.find("LEFT") == 0) {
					rot = -90;
				}
				getline(line_stream, word, ',');
				std::unique_ptr<Touch> new_torch;
				new_torch.reset(new Touch(pos, { 0,rot,0 }));
				new_torch->SetColor(color);
				torchs.push_back(std::move(new_torch));
			}
		}
		if (word.find("END") == 0) {
			break;
		}
	}



}


