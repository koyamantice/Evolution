#include"ActorComponent.h"

void ActorComponent::Initialize() {
	if (isActive) {
		OnInitialize();
	}
}

void ActorComponent::Update() {
	if (isActive) {
		OnUpdate();
	}
}

void ActorComponent::Draw() {
	if (isActive) {
		OnDraw();
	}
}

void ActorComponent::Finalize() {
	if (isActive) {
		OnFinalize();
	}
}
void ActorComponent::SetAttachActor(std::weak_ptr<Actor> Actor) {
	attachActor = Actor;
	OnAttached();
}