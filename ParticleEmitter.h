#pragma once

#include <memory>
#include "ParticleManager.h"

class ParticleEmitter {
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // 静的メンバ関数
	//

public: // メンバ関数
	ParticleEmitter(const UINT& texnumber);
	/// <summary>
	/// 追加
	/// </summary>
	/// <param name="num">数</param>
	/// <param name="life">生存時間</param>
	/// <param name="position">座標</param>
	/// <param name="model">モデル</param>
	void Add(const int& num, const int& life,
		const XMFLOAT3& position, const XMFLOAT3& average_vel, const XMFLOAT3& accel,
		const float& start_scale, const float& end_scale,
		const XMFLOAT4& start_color, const XMFLOAT4& end_color);

	/// <summary>
	/// 基本的なパーティクルモーション
	/// </summary>
	/// <param name="life"></param>
	/// <param name="position"></param>
	/// <param name="average_vel"></param>
	/// <param name="accel"></param>
	/// <param name="start_scale"></param>
	/// <param name="end_scale"></param>
	/// <param name="start_color"></param>
	/// <param name="end_color"></param>
	void AddCommon(const int& life,
		const XMFLOAT3& position, const float& average_vel, const float& average_accel,
		const float& start_scale=1.0f, const float& end_scale = 1.0f,
		const XMFLOAT4& start_color={1,1,1,1}, const XMFLOAT4& end_color={1,1,1,1});


	void AddInNest(const int& life,
		const XMFLOAT3& position, const float& average_margin, const float& average_vel,
		const float& start_scale = 1.0f, const float& end_scale = 0.0f,
		const XMFLOAT4& start_color = { 1.0f,1.0f,0.0f,0.8f }, const XMFLOAT4& end_color = { 1.0f,1.0f,0.0f,0.0f });

	void AddContraction(const int& _life,
		const XMFLOAT3& _position,
		const float& _area,
		const float& _average_vel,
		const float& _start_scale = 1.5f, const float& _end_scale = 0.0f,
		const XMFLOAT4& _start_color = { 0.5f,1.0f,0.5f,1.0f }, const XMFLOAT4& _end_color = { 0.5f,1.0f,0.5f,1.0f });

	void AddCrystalMove(const int& _life,
		const XMFLOAT3& _position,
		const float& _area,
		const float& _average_vel,
		const float& _start_scale = 1.5f, const float& _end_scale = 0.0f,
		const XMFLOAT4& _start_color = { 0.5f,1.0f,0.5f,1.0f }, const XMFLOAT4& _end_color = { 0.5f,1.0f,0.5f,1.0f });

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="cmdList">描画コマンドリスト</param>
	void Draw(blendType type = alphaBle);

	

private: // メンバ変数
	//座標
	XMFLOAT3 position_ = {};
	//速度
	XMFLOAT3 velocity_ = {};
	//加速度
	XMFLOAT3 accel_ = {};
	// スケール初期値
	float startScale_ = 1.0f;
	XMFLOAT3 objStartScale_ = { 1.0f, 1.0f, 1.0f };
	// スケール最終値
	float endScale_ = 0.0f;
	XMFLOAT3 objEndScale_ = { 0.0f, 0.0f, 0.0f };
	// 色(RGBA)初期値
	XMFLOAT4 startColor_ = { 1, 1, 1, 1 };
	// 色(RGBA)最終値
	XMFLOAT4 endColor_ = {};
	//X,Y,Z全て[-5.0,+5.0]でランダムに分布
	float mdPos_ = 10.0f;
	XMFLOAT3 objMdPos_ = { 10.0f, 10.0f, 10.0f };
	//X,Y,Z全て[-0.05,+0.05]でランダムに分布
	float mdVel_ = 0.1f;
	//重力に見立ててYのみ[-0.001f,0]でランダムに分布
	float mdAcc_ = 0.001f;
	std::unique_ptr<ParticleManager> particleManager = nullptr;
};