﻿#pragma once

#include "fbxsdk.h"
#include "FBXModel.h"

#include <d3d12.h>
#include <d3dx12.h>
#include <string>

class FbxLoader
{
private:	//エイリアス
//std::を省略
	using string = std::string;
public:
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static FbxLoader* GetInstance();
	/// <summary>
	///初期化
	/// </summary>
	/// <param name="device">D3D12デバイス</param>
	void Initialize(ID3D12Device* device);
	/// <summary>
	/// 後始末
	/// </summary>
	void Finalize();
	/// <summary>
	/// ファイルからFBXモデル読み込み
	/// </summary>
	/// <param name="FBXModelName"></param>
	FBXModel* LoadModelFromFile(const string& ModelName);

	/// <summary>
	/// 再帰的にノード構成を解析
	/// </summary>
	/// <param name="modle"></param>
	/// <param name="fbxNode">解析対象のノード</param>
	void ParseNodeRecursive(FBXModel* FBXModel, FbxNode* fbxNode, Node* parent = nullptr);

	/// <summary>
	/// メッシュ読み取り
	/// </summary>
	/// <param name="FBXModel">読み込み先モデルオブジェクト</param>
	/// <param name="fbxNode">解析対象のノード</param>
	void ParseMesh(FBXModel* FBXModel, FbxNode* fbxNode);

	/// <summary>
	/// FBXの行列をXMMatrixに変換
	/// </summary>
	/// <param name="dst">書き込み先</param>
	/// <param name="src">元となるFBX行列</param>
	static void ConvertMatrixFromFbx(DirectX::XMMATRIX* dst, const FbxAMatrix& src);

	//頂点座標読み取り
	void ParseMeshVertices(FBXModel* FBXModel, FbxMesh* fbxMesh);
	//面積情報読み取り
	void ParseMeshFaces(FBXModel* FBXModel, FbxMesh* fbxMesh);
	//マテリアル読み取り
	void ParseMaterial(FBXModel* FBXModel, FbxNode* fbxNode);
	//テクスチャ読み込み
	void LoadTexture(FBXModel* FBXModel, const std::string& fullpath);
	//スキニング情報の読み取り
	void ParseSkin(FBXModel* FBXModel, FbxMesh* fbxMesh);

	//ディレクトリを含んだファイルパスからファイル名を抽出する
	std::string ExtractFileName(const std::string& path);

private:
	// privateなコンストラクタ（シングルトンパターン）
	FbxLoader() = default;
	// privateなデストラクタ（シングルトンパターン）
	~FbxLoader() = default;
	// コピーコンストラクタを禁止（シングルトンパターン）
	FbxLoader(const FbxLoader& obj) = delete;
	// コピー代入演算子を禁止（シングルトンパターン）
	void operator=(const FbxLoader& obj) = delete;

	//D3D12デバイス
	ID3D12Device* device = nullptr;
	//FBXマネージャ
	FbxManager* fbxManager = nullptr;
	//FBXインポータ
	FbxImporter* fbxImporter = nullptr;


public:	//定数
	//モデル格納ルートパス
	static const string baseDirectory;
	static const string nextDirectory;
	//テクスチャがない場合の標準テクスチャファイル名
	static const string defaultTextureFileName;
};