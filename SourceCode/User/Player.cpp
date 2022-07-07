#include"Player.h"
#include"ModelManager.h"
#include"ImageManager.h"
#include<fstream>
void Player::LoadData() {
	std::ifstream file;
	file.open("Resources/csv/status.csv");
	assert(file.is_open());

	parameterCommands << file.rdbuf();
	
	file.close();
}

void Player::UpdateCommand() {
	std::string line;

	while (getline(parameterCommands,line)) {
		//解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		//半角スペース区切りで行の先頭文字列を取得
		getline(line_stream,word,',');
		//"//"から始まる行はコメント
		if (word.find("//")==0) {
			//飛ばす
			continue;
		}
		//各コマンド
		if (word.find("HP")==0) {
			getline(line_stream, word, ',');
			hp = (float)std::atof(word.c_str());
		}else if(word.find("VEL")==0) {
			getline(line_stream, word, ',');
			vel = (float)std::atof(word.c_str());
			
			break;
		}
	}
}

void Player::DebugUpdate() {
}

void Player::OnInit() {
	obj->SetRotation(XMFLOAT3{0,0,0});
	LoadData();
	UpdateCommand();
	Texture* Lock_ = Texture::Create(ImageManager::Lock,obj->GetPosition(), {0.5f,0.5f,0.5f}, {1,1,1,1});
	Lock_->TextureCreate();
	Lock_->SetRotation({0,45,0});
	LockOn.reset(Lock_);
}

void Player::OnUpda() {
	Move();
	Shot();
	LockOn->Update();
	LockOn->SetPosition({ obj->GetPosition().x, obj->GetPosition().y, obj->GetPosition().z});
	for (std::unique_ptr<Bullet>& bullet:bullets) {
		bullet->Update();
	}
}

void Player::OnDraw() {
	Texture::PreDraw();
	LockOn->Draw();
	for (std::unique_ptr<Bullet>& bullet : bullets) {
		bullet->Draw();
	}
}

void Player::OnFinal() {
}

void Player::Move() {
	XMFLOAT3 pos = obj->GetPosition();
	XMFLOAT3 rot= obj->GetRotation();
	if (input->PushKey(DIK_W)) {
		pos.z = obj->GetPosition().z - vel;
	}
	if (input->PushKey(DIK_S)) {
		pos.z = obj->GetPosition().z + vel;
	}
	if (input->PushKey(DIK_D)) {
		pos.x = obj->GetPosition().x - vel;
	}
	if (input->PushKey(DIK_A)) {
		pos.x = obj->GetPosition().x + vel;
	}

	float StickX= input->GetPosX();
	float StickY = input->GetPosY();
	const float PI = 3.14159f;

	if (!(StickX<500 && StickX>-500)) {
		pos.x -= sin(atan2(StickX, StickY)) * vel;
		rot.y = ((-atan2(StickX, StickY) * (180.0f / PI)))+180;
	}
	if (!(StickY<500 && StickY>-500)) {
		pos.z += cos(atan2(StickX, StickY)) * vel;
		rot.y = ((-atan2(StickX, StickY) * (180.0f / PI)))+180;
	}

	obj->SetPosition(pos);
	obj->SetRotation(rot);
}

void Player::Shot() {
	if (input->TriggerKey(DIK_SPACE)||input->TriggerButton(input->Button_A)) {
		std::unique_ptr<Bullet> newBullet = std::make_unique<Bullet>();
		newBullet->Initialize(ModelManager::GetIns()->GetModel(ModelManager::hole));
		newBullet->SetPosition(obj->GetPosition());
		bullets.push_back(std::move(newBullet));
	}
}
