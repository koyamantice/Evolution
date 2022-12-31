#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <forward_list>
#include "Camera.h"

/// <summary>
/// パーティクルマネージャ
/// </summary>
class ParticleManager {
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス
	// 頂点データ構造体
	struct VertexPos {
		XMFLOAT3 pos; // xyz座標
		float scale; // スケール
	};

	// パイプラインセット
	struct PipelineSet {
		// ルートシグネチャ
		ComPtr<ID3D12RootSignature> rootsignature;
		// パイプラインステートオブジェクト
		ComPtr<ID3D12PipelineState> pipelinestate;
	};

	// 定数バッファ用データ構造体
	struct ConstBufferData {
		XMMATRIX mat;	// ビュープロジェクション行列
		XMMATRIX matBillboard;	// ビルボード行列
	};

	// パーティクル1粒
	class Particle {
		// Microsoft::WRL::を省略
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
		// DirectX::を省略
		using XMFLOAT2 = DirectX::XMFLOAT2;
		using XMFLOAT3 = DirectX::XMFLOAT3;
		using XMFLOAT4 = DirectX::XMFLOAT4;
		using XMMATRIX = DirectX::XMMATRIX;

	public:
		// 座標
		XMFLOAT3 position = {};
		// 速度
		XMFLOAT3 velocity = {};
		// 加速度
		XMFLOAT3 accel = {};
		// 色
		XMFLOAT3 color = {};
		// スケール
		float scale = 1.0f;
		// 回転
		float rotation = 0.0f;
		// 初期値
		XMFLOAT3 s_color = {};
		float s_scale = 1.0f;
		float s_rotation = 0.0f;
		// 最終値
		XMFLOAT3 e_color = {};
		float e_scale = 0.0f;
		float e_rotation = 0.0f;
		// 現在フレーム
		int frame = 0;
		// 終了フレーム
		int num_frame = 0;

	};

private: // 定数
	static const int vertexCount = 65536;		// 頂点数
public: // 静的メンバ関数

	/// <summary>
	/// パーティクル共通部分の初期化
	/// </summary>
	/// <param name="dev">デバイス</param>
	/// <param name="cmdList">コマンドリスト</param>
	static void ParticleManagerCommon(ID3D12Device* dev, ID3D12GraphicsCommandList* cmdList, Camera* camera);

public: // メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <returns>ParticleManager</returns>
	static std::unique_ptr<ParticleManager> Create(std::wstring fName);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns></returns>
	void Initialize(std::wstring fName);
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// カメラのセット
	/// </summary>
	/// <param name="camera">カメラ</param>
	static  void SetCamera(Camera* camera) { ParticleManager::camera = camera; }

	/// <summary>
	/// 減算合成描画前処理
	/// </summary>
	static void DrawPrevSubBlend();
	/// <summary>
	/// 加算合成描画前処理
	/// </summary>
	static void DrawPrevAddBlend();

	/// <summary>
	/// 半透明合成描画前処理
	/// </summary>
	static void DrawPrevAlphaBlend();

	/// <summary>
	/// パーティクルの追加
	/// </summary>
	/// <param name="life">生存時間</param>
	/// <param name="position">初期座標</param>
	/// <param name="velocity">速度</param>
	/// <param name="accel">加速度</param>
	/// <param name="start_scale">開始時スケール</param>
	/// <param name="end_scale">終了時スケール</param>
	void Add(int life, XMFLOAT3 position, XMFLOAT3 velocity, XMFLOAT3 accel, float start_scale, float end_scale);

	/// <summary>
	/// デスクリプタヒープの初期化
	/// </summary>
	/// <returns></returns>
	void InitializeDescriptorHeap();

	/// <summary>
	/// 半透明合成パイプライン生成
	/// </summary>
	static void CreateAlphaBlendPipeline();
	/// <summary>
	/// 加算合成パイプライン生成
	/// </summary>
	static void CreateAddBlendPipeline();

	/// <summary>
	/// 減算合成パイプライン生成
	/// </summary>
	static void CreateSubBlendPipeline();

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	/// <returns>成否</returns>
	void LoadTexture(std::wstring fName);

	/// <summary>
	/// モデル作成
	/// </summary>
	void CreateModel();

private: // メンバ変数
	//デバイス
	static ID3D12Device* device;
	//コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	//半透明合成パイプラインセット
	static PipelineSet alphaBlendPipelineSet;
	//加算合成パイプラインセット
	static PipelineSet addBlendPipelineSet;
	//減算合成パイプラインセット
	static PipelineSet subBlendPipelineSet;
	// カメラ
	static Camera* camera;
	// デスクリプタサイズ
	UINT descriptorHandleIncrementSize = 0u;
	// デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeap;
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	// テクスチャバッファ
	ComPtr<ID3D12Resource> texbuff;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView;
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	// パーティクル配列
	std::forward_list<Particle> particles;

};

