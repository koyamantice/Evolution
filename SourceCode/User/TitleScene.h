#pragma once
#include "SceneManager.h"
#include "Actor.h"

/// <summary>
/// �^�C�g���V�[��
/// </summary>

class TitleScene : public BaseScene {
public:

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(DirectXCommon* dxCommon) override;
	/// <summary>
	/// �I��
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// ���t���[���X�V
	/// </summary>
	void Update(DirectXCommon* dxCommon) override;
	/// <summary>
	/// �`��
	/// </summary>
	void Draw(DirectXCommon* dxCommon) override;

private:

	void Feed(const std::string& sceneName);

	//�����o�ϐ�
	enum {
		Button,
		SpriteMax,
	};
	std::unique_ptr<Sprite> UI[SpriteMax];
	std::unique_ptr<Sprite> Effect;
	bool Change = false;
	float alpha = 0.0f;
	float frame = 0;
	std::string SceneName = "null";
};

