/**
 * @file FieldManager.h
 * @brief フィールドを生成する
 */
#pragma once
#include "Model.h"
#include "BaseObj.h"
#include "JsonLoader.h"
#include "Wall.h"
#include "Torch.h"

class FieldManager final {
private:
	FieldManager() = default;
	~FieldManager() = default;

public:
	//コピーコンストラクタ無効
	FieldManager(const FieldManager& obj) = delete;
	//代入演算子を無効
	FieldManager& operator=(const FieldManager& obj) = delete;

	static FieldManager* GetInstance();

public:
	void Initialize(int num);
	void Update();
	void Draw();

	//当たり判定の大きさ取得
	//ナビポイントの番号取得
	int32_t GetNaviPointNum() {
		return navePointNum_;
	}
	//ステージを読み込む
	void LoadStage(int num);

	//ライトをセット
	static void SetLight(LightGroup* lightGroup);

private:
	/// <summary>
	/// 壁を設置
	/// </summary>
	/// <param name="pos">壁の位置</param>
	/// <param name="scale">壁の大きさ</param>
	void SetWall(Vector3 pos,Vector3 scale);

	/// <summary>
	/// 床を設置
	/// </summary>
	/// <param name="pos">床の位置</param>
	/// <param name="scale">床の大きさ</param>
	void SetFloor(Vector3 pos, Vector3 scale);

	/// <summary>
	/// ゴールを設置
	/// </summary>
	/// <param name="pos">ゴールの場所</param>
	void SetGoal(Vector3 pos);

	/// <summary>
	/// たいまつの設置
	/// </summary>
	/// <param name="pos">たいまつの場所</param>
	/// <param name="rot">たいまつの角度</param>
	/// <param name="scale"></param>
	void SetTorch(Vector3 pos,Vector3 rot,Vector3 scale);

private:
	//ライトの明るさ
	const float ATTEN_LOW = 0.01f;
	const float ATTEN_HIGT = 0.001f;
	//ゴール関係
	const Vector3 COLOR_GOAL = {0,0,1};
	const float ATTEN_GOAL = 0.002f;
	//ゴールの高さ
	const float GOAL_Y = 2;

	LevelData* levelData_;

	std::unique_ptr<Model> boxModel_;
	std::vector <std::unique_ptr<BaseObj>> objs_;

	std::vector<std::unique_ptr<Wall>> walls_; 
	std::vector<std::unique_ptr<Torch>> torchs_;

	int32_t navePointNum_;

	//ライト関係
	static LightGroup* lightGroup_;
	int32_t useLightNum_;
	float stageAtten_;
};
