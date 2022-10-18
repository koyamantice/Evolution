#pragma once
#include "SceneManager.h"
#include "Actor.h"
#include"TitleText.h"

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
	std::unique_ptr<TitleText> text;
	bool Change = false;
	float alpha = 0.0f;
	float frame = 0;
	float C;
	float A;

	


	std::string SceneName = "null";
};

