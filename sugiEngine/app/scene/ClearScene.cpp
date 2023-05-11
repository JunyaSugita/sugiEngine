#include "ClearScene.h"

void ClearScene::Initialize()
{
	// レベルデータの読み込み
	levelData_ = JsonLoader::LoadJson("untitled");

	sphereModel_ = Model::LoadFromObj("sphere", true);
	models.insert(std::make_pair("sphere", sphereModel_));

	for (auto& objectData : levelData_->obj) {
		//ファイルから登録済みモデルを探索
		Model* model = nullptr;
		decltype(models)::iterator it = models.find(objectData.filename);
		if (it != models.end()) {
			model = it->second;
		}
		//モデルを指定して3Dオブジェクトを生成
		Object3d* newObject = Object3d::Create();
		newObject->SetModel(sphereModel_);

		//obj情報
		sphereWorldTransform_.trans_ = Vector3(objectData.pos);
		sphereWorldTransform_.rotation = Vector3(objectData.rot);
		sphereWorldTransform_.scale_ = Vector3(objectData.scale);

		newObject->SetWorldTransform(sphereWorldTransform_);

		objects.push_back(newObject);
	}

	//ライト
	lightGroup_ = LightGroup::Create();
	Object3d::SetLight(lightGroup_);
	lightGroup_->SetCircleShadowActive(0, true);
}

void ClearScene::Update()
{
	Input* input = Input::GetInstance();

	if (input->PushKey(DIK_LEFT)) {
		sphereWorldTransform_.trans_.x -= 0.3f;
	}
	if (input->PushKey(DIK_RIGHT)) {
		sphereWorldTransform_.trans_.x += 0.3f;
	}

	for (auto& object : objects) {
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
	for (auto& object : objects) {
		object->Draw();
	}
}

void ClearScene::SpriteDraw()
{
	
}

void ClearScene::Delete()
{
	delete sphereModel_;
	delete lightGroup_;
}
