#include "FieldManager.h"
#include "EnemyManager.h"

using namespace std;

FieldManager::FieldManager()
{
}

FieldManager::~FieldManager()
{
}

FieldManager* FieldManager::GetInstance()
{
	static FieldManager instance;

	return &instance;
}

void FieldManager::Initialize()
{
	// レベルデータの読み込み
	levelData_ = JsonLoader::LoadJson("level");

	boxModel_ = move(Model::LoadFromObj("box"));
	groundModel_ = move(Model::LoadFromObj("ground"));

	objNum_ = 0;
	for (auto& objectData : levelData_->obj) {
		if (objectData.filename == "box") {
			//モデルを指定して3Dオブジェクトを生成
			object_[objNum_] = make_unique<Object3d>();
			object_[objNum_]->Initialize();
			object_[objNum_]->SetModel(groundModel_.get());

			//obj情報
			WorldTransform worldTransform;
			worldTransform.SetPos(objectData.pos);
			worldTransform.SetRot({0,0,0});
			worldTransform.SetScale(objectData.scale);

			object_[objNum_]->SetWorldTransform(worldTransform);
			object_[objNum_]->Update();

			BoxCol temp;
			temp.pos = objectData.pos;
			temp.size = objectData.scale;
			col_.push_back(temp);

			objNum_++;
		}
		if (objectData.filename == "ground") {
			//モデルを指定して3Dオブジェクトを生成
			object_[objNum_] = make_unique<Object3d>();
			object_[objNum_]->Initialize();
			object_[objNum_]->SetModel(groundModel_.get());

			//obj情報
			WorldTransform worldTransform;
			worldTransform.SetPos(objectData.pos);
			worldTransform.SetRot({ 0,0,0 });
			worldTransform.SetScale(objectData.scale);

			object_[objNum_]->SetWorldTransform(worldTransform);
			object_[objNum_]->Update();

			BoxCol temp;
			temp.pos = objectData.pos;
			temp.size = objectData.scale;
			col_.push_back(temp);

			objNum_++;
		}
		if (objectData.filename == "enemy") {
			EnemyManager::GetInstance()->PopEnemy(objectData.pos);
		}

	}
}

void FieldManager::Update()
{
	for (int i = 0; i < objNum_; i++) {
		object_[i]->Update();
	}
}

void FieldManager::Draw()
{
	for (int i = 0; i < objNum_; i++) {
		object_[i]->Draw();
	}
}
