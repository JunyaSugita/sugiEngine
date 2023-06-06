#include "ClearScene.h"

void ClearScene::Initialize()
{
	// レベルデータの読み込み
	levelData_ = JsonLoader::LoadJson("level");

	sphereModel_ = Model::LoadFromObj("sphere", true);
	playerModel_ = Model::LoadFromObj("player");
	models.insert(std::make_pair("sphere", sphereModel_));
	models.insert(std::make_pair("player", playerModel_));

	int objNum = 0;

	for (auto& objectData : levelData_->obj) {
		//ファイルから登録済みモデルを探索
		Model* model = nullptr;
		decltype(models)::iterator it = models.find(objectData.filename);
		if (it != models.end()) {
			model = it->second;
		}
		//モデルを指定して3Dオブジェクトを生成
		Object3d* newObject = Object3d::Create();
		newObject->SetModel(model);

		if (objectData.filename == "player") {
			playerNum_ = objNum;
		}
		else if (objectData.filename == "spawn") {
			spawnNum_ = objNum;
		}

		//obj情報
		worldTransform_.pos = Vector3(objectData.pos);
		worldTransform_.rot = Vector3(objectData.rot);
		worldTransform_.scale = Vector3(objectData.scale);

		newObject->SetWorldTransform(worldTransform_);

		objects.push_back(newObject);
		objNum++;
	}

	spawnPoint_ = objects[spawnNum_]->GetPos();

	//ライト
	lightGroup_ = LightGroup::Create();
	Object3d::SetLight(lightGroup_);
	lightGroup_->SetCircleShadowActive(0, true);
}

void ClearScene::Update()
{
	Input* input = Input::GetInstance();

	if (input->PushKey(DIK_LEFT)) {
		worldTransform_.pos.x -= 0.3f;
	}
	if (input->PushKey(DIK_RIGHT)) {
		worldTransform_.pos.x += 0.3f;
	}
	if (input->TriggerKey(DIK_R)) {
		worldTransform_.pos = spawnPoint_;
	}

	objects[playerNum_]->SetWorldTransform(worldTransform_);
	

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
