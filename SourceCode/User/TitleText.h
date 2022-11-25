#pragma once
#include"Object3d.h"
#include"FBXObject3d.h"
#include <list>
#include <memory>
#include<cassert>
#include <SourceCode/System/DirectXCommon.h>


class TitleText {
public:
	TitleText();
	~TitleText();

	void Init();
	void Upda();
	void Draw(DirectXCommon* dxCommon);


private:
	using XMFLOAT3 = DirectX::XMFLOAT3;
private:
	std::unique_ptr<Object3d> texts[6];
	std::unique_ptr<FBXObject3d> Player;
	std::unique_ptr<Object3d> house;

	float pos[6]{};

	float frame=0;
};
