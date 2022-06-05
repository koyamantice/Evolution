#pragma once
#include "BaseScene.h"
#include "AbstractSceneFactory.h"
#include <memory> 

/// <summary>
/// �V�[���}�l�[�W���[
/// </summary>
class SceneManager {
public:
	static SceneManager* GetInstance();
/// <summary>
/// �X�V
/// </summary>
	void Update(DirectXCommon* dxCommon);
	/// <summary>
	/// �`��
	/// </summary>
	void Draw(DirectXCommon* dxCommon);
	void Finalize();

	bool IsEndRequst() { return endResquest_; }

	void SetEnd(bool endResquest_) { this->endResquest_ = endResquest_; }

public:
	//���V�[���\��
	void ChangeScene(const std::string& sceneName);

	void SetSceneFactory(AbstractSceneFactory* sceneFactory) { sceneFactory_ = sceneFactory; }
private:
	bool endResquest_ = false;
	//���̃V�[��
	BaseScene* scene_ = nullptr;
	//���̃V�[��
	BaseScene* nextScene_ = nullptr;

	AbstractSceneFactory* sceneFactory_ = nullptr;

	~SceneManager()=default;
	SceneManager()=default;
	SceneManager(const SceneManager&)=delete;
	void operator =(const SceneManager&)=delete;
};

