#include"EnemyUI.h"
#include"ActorManager.h"
#include <SourceCode/User/ImageManager.h>
#include <SourceCode/Common/Easing.h>
EnemyUI::EnemyUI() {
}

EnemyUI::~EnemyUI() {
}

void EnemyUI::OnInitialize() {
	HpCover = Sprite::Create(ImageManager::HpCover, { 730,20 });

	HpGauge = Sprite::Create(ImageManager::HpRed, { 730,20 });
	HpGauge->SetSize({0,0});

	UnderGauge = Sprite::Create(ImageManager::HpGreen, { 730,20 });
}

void EnemyUI::OnUpdate() {
	Actor* AttachActor = ActorManager::GetInstance()->SearchActor("Enemy");
	
	HpGauge->SetSize({ HpCover->GetSize().x * (AttachActor->GetHp() / AttachActor->GetMaxHp()) ,HpCover->GetSize().y});
	HpGauge->SetAnchorPoint({ 0,0 });

	if (AttachActor->GetHp() < 0) {
		HpGauge->SetSize({ 0,HpCover->GetSize().y });
	}

	XMFLOAT2 after_pos = HpGauge->GetSize();
	XMFLOAT2 pos = UnderGauge->GetSize();
	pos.x = Ease(In, Quad, 0.3f, pos.x, after_pos.x);
	pos.y = Ease(In, Quad, 0.3f, pos.y, after_pos.y);
	UnderGauge->SetSize(pos);
}

void EnemyUI::OnFinalize() {
}

void EnemyUI::OnDraw() {
	Sprite::PreDraw();
	UnderGauge->Draw();
	HpGauge->Draw();
	HpCover->Draw();
}

void EnemyUI::OnAttached() {
}
