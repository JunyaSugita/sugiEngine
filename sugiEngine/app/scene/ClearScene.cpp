#include "ClearScene.h"

using namespace std;

void ClearScene::Initialize()
{
	// レベルデータの読み込み
	levelData_ = JsonLoader::LoadJson("level");

	models_.insert(std::make_pair("sphere", sphereModel_.get()));
	models_.insert(std::make_pair("player", playerModel_.get()));

	int32_t objNum = 0;

	for (auto& objectData : levelData_->obj) {
		//ファイルから登録済みモデルを探索
		Model* model = nullptr;
		decltype(models_)::iterator it = models_.find(objectData.filename);
		if (it != models_.end()) {
			model = it->second;
		}
		//モデルを指定して3Dオブジェクトを生成
		std::unique_ptr<Object3d> newObject = move(Object3d::Create());
		newObject->SetModel(model);

		if (objectData.filename == "player") {
			playerNum_ = objNum;
		}
		else if (objectData.filename == "spawn") {
			spawnNum_ = objNum;
		}

		//obj情報
		worldTransform_.SetPos(Vector3(objectData.pos));
		worldTransform_.SetRot(Vector3(objectData.rot));
		worldTransform_.SetScale(Vector3(objectData.scale));

		newObject->SetWorldTransform(worldTransform_);

		objects_.push_back(newObject.get());
		objNum++;
	}

	spawnPoint_ = objects_[spawnNum_]->GetPos();

	//ライト
	lightGroup_ = LightGroup::Create();
	Object3d::SetLight(lightGroup_.get());
	lightGroup_->SetCircleShadowActive(0, true);
}

void ClearScene::Update()
{
	objects_[playerNum_]->SetWorldTransform(worldTransform_);


	for (auto& object : objects_) {
		object->Update();
	}

	//ライト
	lightGroup_->Update();

}

void ClearScene::BackSpriteDraw()
{

}

void ClearScene::Draw()
{
	for (auto& object : objects_) {
		object->Draw();
	}
}

void ClearScene::ObjDraw()
{
}

void ClearScene::ObjDraw2()
{
}

void ClearScene::ParticleDraw()
{
}

void ClearScene::SpriteDraw()
{

}

void ClearScene::Finalize()
{
}
