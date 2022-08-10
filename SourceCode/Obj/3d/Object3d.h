#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string>
#include "Model.h"
#include"CollisionInfo.h"
#include "Camera.h"
#include "LightGroup.h"
class BaseCollider;

/// <summary>
/// 3Dオブジェクト
/// </summary>
class Object3d
{
protected: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public: // サブクラス	

	// パイプラインセット
	struct PipelineSet
	{
		// ルートシグネチャ
		ComPtr<ID3D12RootSignature> rootsignature;
		// パイプラインステートオブジェクト
		ComPtr<ID3D12PipelineState> pipelinestate;
	};

	// 定数バッファ用データ構造体B0
	struct ConstBufferDataB0
	{
		XMMATRIX viewproj;    // ビュープロジェクション行列
		XMMATRIX world; // ワールド行列
		XMFLOAT3 cameraPos; // カメラ座標（ワールド座標）
		XMFLOAT4 color;	// 色 (RGBA)
	};

private: // 定数


public: // 静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="window_width">画面幅</param>
	/// <param name="window_height">画面高さ</param>
	/// <returns>成否</returns>
	static bool StaticInitialize(ID3D12Device* device, ID3D12GraphicsCommandList*cmdList, int window_width, int window_height, Camera* camera = nullptr);

	/// <summary>
	/// グラフィックパイプラインの生成
	/// </summary>
	static void CreateGraphicsPipeline();

	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="cmdList">描画コマンドリスト</param>
	static void PreDraw();

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <returns></returns>
	static Object3d* Create();

	/// <summary>
	/// カメラセット
	/// </summary>
	/// <returns></returns>
	static void SetCamera(Camera* camera) {
		Object3d::camera = camera;
	}

	/// <summary>
	///ライトセット
	/// </summary>
	/// <returns></returns>
	static void SetLightGroup(LightGroup* lightGroup) {
		Object3d::lightGroup = lightGroup;
	}

	/// <summary>
	/// 視点座標の取得
	/// </summary>
	/// <returns>座標</returns>
	static const XMFLOAT3& GetEye() { return eye; }

	/// <summary>
	/// 視点座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	static void SetEye(XMFLOAT3 eye);

	/// <summary>
	/// 注視点座標の取得
	/// </summary>
	/// <returns>座標</returns>
	static const XMFLOAT3& GetTarget() { return target; }

	/// <summary>
	/// 注視点座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	static void SetTarget(XMFLOAT3 target);

	/// <summary>
	/// ベクトルによる移動
	/// </summary>
	/// <param name="move">移動量</param>
	static void CameraMoveVector(XMFLOAT3 move);

	/// <summary>
	/// ベクトルによる視点移動
	/// </summary>
	/// <param name="move">移動量</param>
	static void CameraMoveEyeVector(XMFLOAT3 move);

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	// テクスチャあり用パイプライン
	static PipelineSet pipelineSet;
	// ビュー行列
	static XMMATRIX matView;
	// 射影行列
	static XMMATRIX matProjection;
	// 視点座標
	static XMFLOAT3 eye;
	// 注視点座標
	static XMFLOAT3 target;
	// 上方向ベクトル
	static XMFLOAT3 up;
	// ビルボード行列
	static XMMATRIX matBillboard;
	// Y軸回りビルボード行列
	static XMMATRIX matBillboardY;
	// カメラ
	static Camera* camera;
	// ライト
	static LightGroup* lightGroup;


private:// 静的メンバ関数

	/// <summary>
	/// カメラ初期化
	/// </summary>
	/// <param name="window_width">画面横幅</param>
	/// <param name="window_height">画面縦幅</param>
	static void InitializeCamera(int window_width, int window_height);

	/// <summary>
	/// ビュー行列を更新
	/// </summary>
	static void UpdateViewMatrix();

public: // メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Object3d() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Object3d();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	//bool Initialize();
	virtual bool Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	//void Update();
	virtual void Update();
	/// <summary>
	/// 描画
	/// </summary>
	//void Draw();
	virtual void Draw();
	//行列の更新
	void UpdateWorldMatrix();

	/// <summary>
	/// モデルを取得
	/// </summary>
	inline Model* GetModel() { return model; }

	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns>座標</returns>
	const XMFLOAT3& GetPosition() { return position; }
	/// <returns>座標</returns>
	const XMFLOAT3& GetOldPosition() { return oldPosition; }

	const XMFLOAT3& GetRotation() { return rotation; }

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(XMFLOAT3 position) { this->position = position; }
	void SetoldPosition(XMFLOAT3 oldPosition) { this->oldPosition = oldPosition; }

	void SetRotation(XMFLOAT3 rotation) { this->rotation = rotation; }

	void SetColor(XMFLOAT4 color) { this->color = color; }
	/// <summary>
	/// スケールの設定
	/// </summary>
	/// <param name="position">スケール</param>
	void SetScale(XMFLOAT3 scale) { this->scale = scale; }

	/// <summary>
	/// モデルのセット
	/// </summary>
	/// <param name="model">モデル</param>
	void SetModel(Model* model) { this->model = model; }

	void SetBillboard(bool isBillboard) { this->isBillboard = isBillboard; }
	///<summary>
	///ワールド行列の取得
	///</summary>
	/// <returns>ワールド行列</return>
	const XMMATRIX& GetMatWorld() { return matWorld; }

	///<summary>
	///コライダーのセット
	///</summary>
	/// <returns>コライダー</return>
	void SetCollider(BaseCollider* collider);

	///<summary>
	///衝突時コールバック関数
	///</summary>
	/// <param name="info">衝突情報</param>
	virtual void OnCollision(const CollisionInfo& info){}

	BaseCollider* collider = nullptr;

//private: // メンバ変数
protected: // メンバ変数
	const char* name = nullptr;

	ComPtr<ID3D12Resource> constBuffB0; // 定数バッファ
	// 色
	XMFLOAT4 color = { 1,1,1,1 };
	// ローカルスケール
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z軸回りのローカル回転角
	XMFLOAT3 rotation = { 0,0,0 };
	// ローカル座標
	XMFLOAT3 position = { 0,0,0 };
	//fps
	XMFLOAT3 oldPosition = position;
	// ローカルワールド変換行列
	XMMATRIX matWorld;
	// 親オブジェクト
	Object3d* parent = nullptr;
	// モデル
	Model* model = nullptr;
	// ビルボード
	bool isBillboard = false;
	//コライダー
	//BaseCollider* collider = nullptr;
};

