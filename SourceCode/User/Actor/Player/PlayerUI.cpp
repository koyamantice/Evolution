#include"PlayerUI.h"
#include"ActorManager.h"
#include <SourceCode/User/ImageManager.h>
#include <SourceCode/Common/Easing.h>
PlayerUI::PlayerUI() {
}

PlayerUI::~PlayerUI() {
}

void PlayerUI::OnInitialize() {
	Damage = Sprite::Create(ImageManager::Damage, { 480,50 });
	Damage->SetColor(INVISIBLE_ALPHA);
	Dmapos = Damage->GetPosition();

	Bullet = Sprite::Create(ImageManager::CharaCover, { 700,612 });
	Bullet->SetScale(0.8f);
	

	Slash = Sprite::Create(ImageManager::slash, { 1030,620 });
	Slash->SetScale(0.8f);

	Sprite* _numBack[2]{};
	const DirectX::XMFLOAT2 numPos[2] = { {1070,610},{830,610} };
	for (int i = 0; i < 2;i++) {
		numBack[i] = Sprite::Create(ImageManager::numBack, numPos[i]);
	}

	const int w = 54;
	const int h = 60;
	const int l = 10;
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 10; j++) {
			num[i][j] = Sprite::Create(ImageManager::Num, { 0,0 });
			int number_index_y = j / l;
			int number_index_x = j % l;
			num[i][j]->SetTextureRect(
				{ static_cast<float>(number_index_x) * w, static_cast<float>(number_index_y) * h },
				{ static_cast<float>(w), static_cast<float>(h) });
			num[i][j]->SetSize({ 84,84 });
			num[i][j]->SetScale(1.0f);
			num[i][j]->SetAnchorPoint({ 0.5f,0.5f });
		}
	}
	for (int j = 0; j < 10; j++) {
		num[0][j]->SetPosition({ 1150 + 62 ,600+62 });
		num[1][j]->SetPosition({ 1060 + 62, 600+62 });
	}
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 10; j++) {
			numBullet[i][j] = Sprite::Create(ImageManager::Num, { 0,0 });
			int number_index_y = j / l;
			int number_index_x = j % l;
			numBullet[i][j]->SetTextureRect(
				{ static_cast<float>(number_index_x) * w, static_cast<float>(number_index_y) * h },
				{ static_cast<float>(w), static_cast<float>(h) });
			numBullet[i][j]->SetSize({ 84,84 });
			numBullet[i][j]->SetScale(1.0f);
			numBullet[i][j]->SetAnchorPoint({ 0.5f,0.5f });
		}
	}
	for (int j = 0; j < 10; j++) {
		numBullet[0][j]->SetPosition({ 974 ,662 });
		numBullet[1][j]->SetPosition({ 882, 662 });
	}
	OnLive = 30;
	OldLive = OnLive;
}

void PlayerUI::OnUpdate() {
	Actor* AttachActor = ActorManager::GetInstance()->SearchActor("Player");

	if (isDamage) {
			Alpha = 1.0f;
			Dmapos.y = Ease(In,Quad, DamageFrame,30,80);
			DamageFrame += 0.025f;
			if (DamageFrame>1.0f) {
				isDamage = false;
				DamageFrame = 0.0f;
			}
			Damage->SetPosition(Dmapos);
	} else {

			Alpha *= 0.85f;
	}
	Damage->SetColor({1,1,1,Alpha});

	if (!scaleChange) {
		OnLive = ActorManager::GetInstance()->SerchWaitBul();
		onLive.clear();
		for (int tmp = OnLive; tmp > 0;) {
			onLive.push_back(tmp % 10);
			tmp /= 10;
		}
		if (OnLive != OldLive) {
			if (OnLive<OldLive) {
				decrement = true;
			}else{
				increment = true;
			}
			scaleChange = true;
		}
		OldLive = OnLive;
	} else {
		if (frame < 1.0f) {
			frame += 0.1f;
		} else {
			frame = 0;
			scaleChange = false;
			decrement = false;
			increment = false;
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 10; j++) {
					numBullet[i][j]->SetColor({ 1,1,1,1 });
					numBullet[i][j]->SetSize({ 84,84 });
				}
			}
		}
		scapos.x = Ease(In, Quad, frame, 84, 128);
		scapos.y = Ease(In, Quad, frame, 84, 128);
		for (int j = 0; j < 10; j++) {
			for (int i = 0; i < 2; i++) {
				if (decrement) {
					numBullet[i][j]->SetColor({ 1.0f,0.5f,0.5f,1 });
				}
				if (increment) {
					numBullet[i][j]->SetColor({ 0.5f,1.0f,0.5f,1 });
				}
			}
			numBullet[0][j]->SetSize(scapos);
		}
	}


	if (!stockChange) {
		Stock = ActorManager::GetInstance()->SearchNum("Bullet");
		if (Stock > 100) {
			Stock = 99;
		}
		stock.clear();
		for (int tmp = Stock; tmp > 0;) {
			stock.push_back(tmp % 10);
			tmp /= 10;
		}
		if (Stock != OldStock) {
			if (Stock < OldStock) {
				stock_decre = true;
			} else {
				stock_incre = true;
			}
			stockChange = true;
		}
		OldStock = Stock;
	} else {
		if (stockFrame < 1.0f) {
			stockFrame += 0.1f;
		} else {
			stockFrame = 0;
			stockChange = false;
			for (int j = 0; j < 10; j++) {
				num[0][j]->SetSize({ 84,84 });
			}
			stock_decre = false;
			stock_incre = false;
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 10; j++) {
					num[i][j]->SetColor({ 1,1,1,1 });
					num[i][j]->SetSize({ 84,84 });
				}
			}
		}
		scapos2.x = Ease(In, Quad, stockFrame, 84, 128);
		scapos2.y = Ease(In, Quad, stockFrame, 84, 128);
			for (int j = 0; j < 10; j++) {
				for (int i = 0; i < 2; i++) {
					if (stock_decre) {
						num[i][j]->SetColor({ 1.0f,0.5f,0.5f,1 });
					}
					if (stock_incre) {
						num[i][j]->SetColor({ 0.5f,1.0f,0.5f,1 });
					}
				}
				num[0][j]->SetSize(scapos2);
			}

	}
}

void PlayerUI::OnFinalize() {
}

void PlayerUI::OnDraw() {
	Sprite::PreDraw();
	//1Chara->Draw();
	Bullet->Draw();
	Damage->Draw();
	Slash->Draw();
	//for (int i = 0; i < 2; i++) {
	//	numBack[i]->Draw();
	//}
	for (int i = 0; i < stock.size() && i < 2; i++) {
		num[i][stock[i]]->Draw();
	}
	if (stock.size()==0) {
		num[0][0]->Draw();
	}
	for (int i = 0; i < onLive.size() && i < 2; i++) {
		numBullet[i][onLive[i]]->Draw();
	}
	if (onLive.size() == 0) {
		numBullet[0][0]->Draw();
	}

}

void PlayerUI::OnAttached() {
}
