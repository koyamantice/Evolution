#pragma once
#include"Object2d.h"
#include <ParticleEmitter.h>
#include "Actor.h"
class MashGhost;

class EnemyAttack {
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	EnemyAttack(Actor* _enemy);
	~EnemyAttack();

	void Initialize();
	void Updata();
	void Draw();

	void Stamp(const XMFLOAT3& pos);

	void SetPredict(const bool& _predict, const float& _timer);
private:
	bool burning = false;
	float scale = 0.0f;
	float effectRate = 0.0f;
	std::unique_ptr<Object2d> Explo = nullptr;
	enum PredictKind {
		PREDICTING = 0,
		PREDICTED,
		PREDICTMAX
	};
	std::unique_ptr<Object2d> Predicted[PREDICTMAX] = {};
	bool predict = false;
	ParticleManager* partMan = nullptr;
};