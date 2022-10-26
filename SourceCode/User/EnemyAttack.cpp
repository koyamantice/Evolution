#include"EnemyAttack.h"
#include"Collision.h"
#include"ImageManager.h"
#include<SourceCode/Common/Easing.h>
#include"ActorManager.h"
EnemyAttack::EnemyAttack() {
}

EnemyAttack::~EnemyAttack() {
}

void EnemyAttack::Init() {
	Texture* Explo_ = Texture::Create(ImageManager::Fire, {0,0,0},
	{1,1,1}, { 1,1,1,1 });
	Explo_->SetIsBillboard(true);
	Explo_->TextureCreate();
	Explo_->SetRotation({ 0,0,0 });
	Explo.reset(Explo_);


}

void EnemyAttack::Upda() {
	Explo->Update();
	if (burning) {
		XMFLOAT3 pos = Explo->GetPosition();
		ActorManager::GetInstance()->DamageBullet(pos, 1.5f);
		if (effectRate < 1.0f) {
			effectRate += 0.08f;
		} else {
			effectRate = 0.0f;
			Explo->SetScale({ 0,0,0 });
			Explo->SetPosition({ 0,-100,0 });
			burning = false;
		}
	}
	scale = Ease(In, Quad, effectRate, 0.5f, 1.0f);
	Explo->SetScale({ scale,scale,scale });




}

void EnemyAttack::Draw() {
	if (burning) {
		Explo->Draw();
	}
}

bool EnemyAttack::Run() {


	return true;

}

void EnemyAttack::Stamp(XMFLOAT3 pos) {
	if (!burning) {
		burning = true;
		Explo->SetPosition({ pos.x,0,pos.z });
	}
}

