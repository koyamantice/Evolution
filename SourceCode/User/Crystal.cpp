#include"Crystal.h"
#include <SourceCode/FrameWork/ActorManager.h>
#include <SourceCode/FrameWork/collision/Collision.h>
#include "ImageManager.h"

void Crystal::OnInit() {
	obj->SetPosition(XMFLOAT3(0,3,-5));
	//player = ActorManager::GetInstance()->SearchActor("Player");
	Texture* InOut_ = Texture::Create(ImageManager::InOut, { 0,3,0 }, { 0.5f,0.3f,0.5f }, { 1,1,1,1 });
	InOut_->TextureCreate();
	InOut_->SetRotation({ 0,0,0 });
	InOut_->SetColor({ 1.0f,1.0f,1.0f ,1.0f });
	in_out.reset(InOut_);

}

void Crystal::OnUpda() {
	obj->SetRotation(XMFLOAT3{ 0,obj->GetRotation().y - 1,0 });
	obj->SetPosition(XMFLOAT3{ 0,sinf(rad * 0.05f) * 0.3f+3.5f,35.0f });
	in_out->SetPosition(obj->GetPosition().x, 7.0f,obj->GetPosition().z);
	in_out->SetRotation(XMFLOAT3{ 0,180+angle,0 });

	rad++;

	if(isCollide){
		if (Input::GetInstance()->TriggerButton(Input::A)) {
			pause = true;
		}
	}


	in_out->Update();

}
void Crystal::OnCollision(const std::string& Tag) {
	if (Tag == "Player") {
		isCollide = true;
	} else {
		isCollide = false;
	}

}
void Crystal::OnDraw(DirectXCommon* dxCommon) {
	if (isCollide) {
		Texture::PreDraw();
		in_out->Draw();
	}
}

void Crystal::OnFinal() {
}
