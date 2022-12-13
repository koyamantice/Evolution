#include "LevelLoader.h"
#include <string>
#include <json.hpp>
#include <fstream>
#include <cassert>

const std::string LevelLoader::kDefaultBaseDirectory = "Resources/levels/";
const std::string LevelLoader::kExtension = ".json";

LevelData* LevelLoader::LoadFile(const std::string& fileName) {
	// 連結してフルパスを得る
	const std::string fullpath = kDefaultBaseDirectory + fileName + kExtension;

	// ファイルストリーム
	std::ifstream file;

	// ファイルを開く
	file.open(fullpath);
	// ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	// JSON文字列から解凍したデータ
	nlohmann::json deserialized;

	// 解凍
	file >> deserialized;

	// 正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// "name"を文字列として取得
	std::string name = deserialized["name"].get<std::string>();
	// 正しいレベルデータファイルかチェック
	assert(name.compare("scene") == 0);

	// レベルデータ格納用インスタンスを生成
	LevelData* levelData = new LevelData();

	// "objects"の全オブジェクトを走査
	for (nlohmann::json& object: deserialized["objects"]) {
		assert(object.contains("type"));

		//種別を取得
		std::string type = object["type"].get<std::string>();
		//TODO:
		if (object.contains("childen")) {

		}
		// MESH
		if (type.compare("MESH") == 0) {
			//
			levelData->objects.emplace_back(LevelData::ObjectData{});
			// 今追加した要素の参照を得る
			LevelData::ObjectData& objectData = levelData->objects.back();

			if (object.contains("file_name")) {
				//ファイル名
				objectData.fileName = object["file_name"];
				//トランスフォームの読み込み
				nlohmann::json& transform = object["transform"];
				//並行移動
				objectData.translation.m128_f32[0] = (float)transform["translation"][1];
				objectData.translation.m128_f32[1] = (float)transform["translation"][2];
				objectData.translation.m128_f32[2] = (float)transform["translation"][0];
				objectData.translation.m128_f32[3] = 1.0f;

				//回転角
				objectData.rotation.m128_f32[0] = (float)transform["rotation"][1];
				objectData.rotation.m128_f32[1] = (float)transform["rotation"][2];
				objectData.rotation.m128_f32[2] = (float)transform["rotation"][0];
				objectData.rotation.m128_f32[3] = 0.0f;

				//スケーリング
				objectData.scaling.m128_f32[0] = (float)transform["scaling"][1];
				objectData.scaling.m128_f32[1] = (float)transform["scaling"][2];
				objectData.scaling.m128_f32[2] = (float)transform["scaling"][0];
				objectData.scaling.m128_f32[3] = 0.0f;
			}
		}
	}

	return levelData;
}

LevelData* LevelLoader::LoadCamera(const std::string& fileName) {
	// 連結してフルパスを得る
	const std::string fullpath = kDefaultBaseDirectory + fileName + kExtension;

	// ファイルストリーム
	std::ifstream file;

	// ファイルを開く
	file.open(fullpath);
	// ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	// JSON文字列から解凍したデータ
	nlohmann::json deserialized;

	// 解凍
	file >> deserialized;

	// 正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// "name"を文字列として取得
	std::string name = deserialized["name"].get<std::string>();
	// 正しいレベルデータファイルかチェック
	assert(name.compare("scene") == 0);

	// レベルデータ格納用インスタンスを生成
	LevelData* levelData = new LevelData();

	// "objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"]) {
		assert(object.contains("type"));

		//種別を取得
		std::string type = object["type"].get<std::string>();
		//TODO:
		if (object.contains("childen")) {

		}
		// MESH
		if (type.compare("CAMERA") == 0) {
			//
			levelData->objects.emplace_back(LevelData::ObjectData{});
			// 今追加した要素の参照を得る
			LevelData::CameraData& objectData = levelData->camera;

			//トランスフォームの読み込み
			nlohmann::json& transform = object["transform"];
			//並行移動
			objectData.eye.x = (float)transform["translation"][1];
			objectData.eye.y = (float)transform["translation"][2];
			objectData.eye.z = -(float)transform["translation"][0];
			//
			objectData.target.x = (float)transform["rotation"][1];
			objectData.target.y = (float)transform["rotation"][2];
			objectData.target.z = (float)transform["rotation"][0];
		}
	}

	return levelData;
}
