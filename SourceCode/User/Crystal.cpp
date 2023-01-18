#include"Crystal.h"
#include <SourceCode/FrameWork/ActorManager.h>
#include <SourceCode/FrameWork/collision/Collision.h>
#include "ImageManager.h"

void Crystal::OnInit() {
	obj->SetPosition(XMFLOAT3(0, 0, 35.0f));
	//obj->SetScale(XMFLOAT3(2, 2, 2));
	//player = ActorManager::GetInstance()->SearchActor("Player");
	Object2d* InOut_ = Object2d::Create(ImageManager::InOut, { 0,3,0 }, { 0.5f,0.3f,0.5f }, { 1,1,1,1 });
	InOut_->Object2dCreate();
	InOut_->SetRotation({ 0,0,0 });
	InOut_->SetColor({ 1.0f,1.0f,1.0f ,1.0f });
	in_out.reset(InOut_);


	Sprite* Test_ = Sprite::Create(ImageManager::Test, { 640,360 }, { 1,1,1,1 }, {0.5, 0.5});
	Test.reset(Test_);

}

void Crystal::OnUpda() {
	obj->SetRotation(XMFLOAT3{ 0, obj->GetRotation().y - 1,0 });
	obj->SetPosition(XMFLOAT3{ 0, sinf(rad * 0.05f) * 0.3f ,35.0f });
	in_out->SetPosition(obj->GetPosition().x, 7.0f,obj->GetPosition().z);
	in_out->SetRotation(XMFLOAT3{ 0,180+angle,0 });

	rad++;

	if(isCollide){
		if (Input::GetInstance()->TriggerButton(Input::A)) {
			pause = true;
		}
	}
	if (pause) {

		if (Input::GetInstance()->TriggerButton(Input::START)) {
			pause = false;
			isCollide = false;
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
void Crystal::ResultOnUpdate(const float& Timer) {
	isActive = false;




}
void Crystal::OnDraw(DirectXCommon* dxCommon) {
	if (isCollide) {
		Object2d::PreDraw();
		in_out->Draw();
	}
}

void Crystal::OnFinal() {
}
