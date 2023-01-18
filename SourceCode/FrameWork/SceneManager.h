#pragma once

#include <memory> 

#include "BaseScene.h"
#include "BattleScene.h"

#include "AbstractSceneFactory.h"

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
	/// <summary>
	/// �I��
	/// </summary>
	void Finalize();

	//�V�[���I�����N�G�X�g
	const bool& IsEndRequst() { return endResquest_; }
	void SetEnd(const bool& endResquest_) { this->endResquest_ = endResquest_; }

public:
	//���V�[���ɕύX���܂��B
	void ChangeScene(const std::string& sceneName);
	//�V�[���t�@�N�g���[���Z�b�g����
	void SetSceneFactory(AbstractSceneFactory* sceneFactory) { sceneFactory_ = sceneFactory; }

private:

	//�V�[���I��
	bool endResquest_ = false;
	//���̃V�[��
	BaseScene* scene_ = nullptr;
	//���̃V�[��
	BaseScene* nextScene_ = nullptr;
	//�V�[���t�@�N�g���[
	AbstractSceneFactory* sceneFactory_ = nullptr;

private:
	~SceneManager() = default;
	SceneManager() = default;
	SceneManager(const SceneManager&) = delete;
	void operator =(const SceneManager&) = delete;
};

