#include "TisGame.h"
#include "SceneFactory.h"

void TisGame::Initialize(DirectXCommon* dxCommon) {
	//基底クラスの初期化
	Framework::Initialize(dxCommon);
	//マネージャーセット
	sceneFactory_ = new SceneFactory();
	SceneManager::GetInstance()->SetSceneFactory(sceneFactory_);
	SceneManager::GetInstance()->ChangeScene("TITLE");
}

void TisGame::Finalize() {

	//基底クラスの初期化
	Framework::Finalize();
}

void TisGame::Update(DirectXCommon* dxCommon) {
	//基底クラスの更新
	Framework::Update(dxCommon);
	//DirectX毎フレーム処理 ここから

}

void TisGame::Draw(DirectXCommon* dxCommon) {
	//基底クラスの更新
	Framework::Draw(dxCommon);

}
