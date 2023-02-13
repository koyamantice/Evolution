#pragma once

#include "FBXModel.h"
#include "Camera.h"
#include "LightGroup.h"

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>

#include <string>

class FBXObject3d {
protected: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public:	//定数
	//ボーンの最大数
	static const int MAX_BONES = 32;

	//定数バッファ用データ構造体（スキニング）
	struct ConstBufferDataSkin {
		XMMATRIX bones[MAX_BONES];
	};
public: // サブクラス
	// 定数バッファ用データ構造体（座標変換行列用）
	struct ConstBufferDataTransform {
		XMMATRIX viewproj;    // ビュープロジェクション行列
		XMMATRIX world; // ワールド行列
		XMFLOAT3 cameraPos; // カメラ座標（ワールド座標）
		XMFLOAT4 color;	// 色 (RGBA)
	};
	// 定数バッファ用データ構造体B1
	struct ConstBufferDataB1 {
		XMFLOAT3 ambient; // アンビエント係数
		float pad1; // パディング
		XMFLOAT3 diffuse; // ディフューズ係数
		float pad2; // パディング
		XMFLOAT3 specular; // スペキュラー係数
		float alpha;	// アルファ
	};
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuff;

	// アニメーション用データ構造体
	struct AnimationInfo {
		std::string name;
		FbxAnimStack* stack;
		FbxTakeInfo* fbxinfo;
		float start;
		float end;
	};
public: // 静的メンバ関数
	/// <summary>
	/// グラフィックパイプラインの生成
	/// </summary>
	static void CreateGraphicsPipeline();
	/// <summary>
	///デバイスセット
	/// </summary>
	/// <returns>デバイス</returns>
	static void SetDevice(ID3D12Device* device) { FBXObject3d::device = device; }
	/// <summary>
	///カメラセット
	/// </summary>
	/// <returns>カメラ</returns>
	static void SetCamera(Camera* camera) { FBXObject3d::camera = camera; }
	/// <summary>
	///ライトセット
	/// </summary>
	/// <returns>ライトグループ</returns>
	static void SetLightGroup(LightGroup* lightGroup) {
		FBXObject3d::lightGroup = lightGroup;
	}

	/// <summary>
	/// 共通の初期化処理
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="camera">カメラ</param>
	/// <param name="lightGroup">ライトグループ</param>
	static void StaticInitializeCommon(ID3D12Device* device, Camera* camera, LightGroup* lightGroup);

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device;
	// カメラ
	static Camera* camera;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;
	// ライト
	static LightGroup* lightGroup;

public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	void SetModel(FBXModel* model) { this->model = model; }


	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns>座標</returns>
	const XMFLOAT3& GetPosition() { return position; }

	const XMFLOAT3& GetRotation() { return rotation; }

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(XMFLOAT3 position) { this->position = position; }

	void SetRotation(XMFLOAT3 rotation) { this->rotation = rotation; }
	void SetColor(XMFLOAT4 color) { this->color = color; }
	/// <summary>
	/// スケールの設定
	/// </summary>
	/// <param name="position">スケール</param>
	void SetScale(XMFLOAT3 scale) { this->scale = scale; }
	const XMFLOAT3& GetScale() { return scale; }

	/// <summary>
	/// スケールの設定
	/// </summary>
	/// <param name="position">スケール</param>
	const bool& GetIsFinish() { return isFinish; }


	/// <summary>
	/// アニメーション
	/// </summary>
	void PlayAnimation(const int& num = 0);
	void ReverseAnimation(const int& num = 0);
	void StopAnimation();
	void ResetAnimation();
	//複数fbxの読み込み
	void LoadAnimation();
protected: // メンバ変数
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuffTransform;
	// ローカルスケール
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z軸回りのローカル回転角
	XMFLOAT3 rotation = { 0,0,0 };
	// ローカル座標
	XMFLOAT3 position = { 0,0,0 };
	//色
	XMFLOAT4 color = { 1,1,1,1 };

	XMFLOAT3 ambient;	// アンビエント影響度
	XMFLOAT3 diffuse;	// ディフューズ影響度
	XMFLOAT3 specular;	// スペキュラー影響度
	float alpha;		// アルファ

	// ローカルワールド変換行列
	XMMATRIX matWorld;
	// モデル
	FBXModel* model = nullptr;
	//定数バッファ（スキン）
	ComPtr<ID3D12Resource> constBuffSkin;
	//1フレーム
	FbxTime frameTime;
	//アニメーション開始時間
	FbxTime startTime;
	//アニメーション終了時間
	FbxTime endTime;
	//現在時間（アニメーション）
	FbxTime currentTime;
	//アニメーション再生中
	bool isPlay = false;
	bool isReverse = false;
	//アニメーション終了フレーム
	bool isFinish = false;
	//アニメーション格納
	std::vector<AnimationInfo> Animations;
};

