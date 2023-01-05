#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include "Camera.h"

class Object2d
{
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
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT3 normal; // 法線ベクトル
		XMFLOAT2 uv;  // uv座標
	};

	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		XMFLOAT4 color;	// 色 (RGBA)
		XMMATRIX mat;	// ３Ｄ変換行列
	};
	enum blendtype{
		AlphaBlend=0,
		AddBlend
	};
private: // 定数
	static const int vertexCount = 4;		// 頂点数
	static const int indexCount = 3 * 2;
public: // 静的メンバ関数
	/// 静的初期化
	static bool StaticInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, int window_width, int window_height, Camera* camera=nullptr);

	/// テクスチャ読み込み
	static bool LoadTexture(UINT texnumber, const wchar_t* filename);

	/// 描画前処理
	static void PreDraw(blendtype type = AlphaBlend);

	/// 描画後処理
	static void PostDraw();
	//テクスチャ生成
	static Object2d* Create(UINT texNumber, XMFLOAT3 position, XMFLOAT3 size={1.0f,1.0f,1.0f}, XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f });

	/// <summary>
	/// カメラセット
	/// </summary>
	/// <returns></returns>
	static void SetCamera(Camera* camera) {
		Object2d::camera = camera;
	}

private: // 静的メンバ変数
	static const int srvCount = 213;
	// デバイス
	static ID3D12Device* device;
	// デスクリプタサイズ
	static UINT descriptorHandleIncrementSize;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature[2];
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate[2];
	// デスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap> descHeap;
	// 頂点バッファ
	static ComPtr<ID3D12Resource> vertBuff;//
	// インデックスバッファ
	static ComPtr<ID3D12Resource> indexBuff;//アンカー作成時static除外
	// テクスチャバッファ
	static ComPtr<ID3D12Resource> texbuff[srvCount];
	// シェーダリソースビューのハンドル(CPU)
	static CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// シェーダリソースビューのハンドル(CPU)
	static CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
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
	// 頂点バッファビュー
	static D3D12_VERTEX_BUFFER_VIEW vbView;
	// インデックスバッファビュー
	static D3D12_INDEX_BUFFER_VIEW ibView;
	// 頂点データ配列
	static VertexPosNormalUv vertices[vertexCount];
	// 頂点インデックス配列
	static unsigned short indices[indexCount];
	// 頂点シェーダオブジェクト
	static ComPtr<ID3DBlob> vsBlob;
	// ピクセルシェーダオブジェクト
	static ComPtr<ID3DBlob> psBlob;
	// エラーオブジェクト
	static ComPtr<ID3DBlob> errorBlob; 
private:
	UINT texNumber = 0;

private:// 静的メンバ関数
	/// デスクリプタヒープの初期化
	static bool InitializeDescriptorHeap();

	/// カメラ初期化
	static void InitializeCamera(int window_width, int window_height);

	/// グラフィックパイプライン生成
	static bool AlphaInitializeGraphicsPipeline();
	static bool AddInitializeGraphicsPipeline();

	/// ビュー行列を更新
	static void UpdateViewMatrix();

public: // メンバ関数
	void Object2dCreate();
	//コンストラクタ
	Object2d(UINT texNumber, XMFLOAT3 position, XMFLOAT3 size = { 1.0f,1.0f,1.0f }, XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f });
	//初期化処理
	bool Initialize();
	/// 毎フレーム処理
	void Update();
	/// 描画
	void Draw();
	static void Finalize();
	/// 座標の取得
	const XMFLOAT3& GetPosition() { return position; }

	/// 座標の設定
	void SetPosition(XMFLOAT3 position) { this->position = position; }
	void SetPosition(float x,float y,float z) 
	{ this->position.x =x; 
	this->position.y = y;
	this->position.z = z;
	}
	void SetIsBillboard(const bool& isBillboard);
	void SetIsBillboardY(const bool& isBillboardY);

	void SetColor(XMFLOAT4 color);
	void SetRotation(const XMFLOAT3& rotation) { this->rotation = rotation; }
	const XMFLOAT3& GetRotation() { return this->rotation; }
	void SetScale(XMFLOAT3 scale) { this->scale = scale; }
	const XMFLOAT3& GetScale() { return scale; }
private: // メンバ変数
	ComPtr<ID3D12Resource> constBuff; // 定数バッファ
	// 色
	XMFLOAT4 color = { 1,1,1,1 };
	// ローカルスケール
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z軸回りのローカル回転角
	XMFLOAT3 rotation = { 0,0,0 };
	// ローカル座標
	XMFLOAT3 position = { 0,0,0 };
	// ローカルワールド変換行列
	XMMATRIX matWorld;
	// ビルボード
	bool isBillboard = false;
	// Y軸ビルボード
	bool isBillboardY = false;
};


