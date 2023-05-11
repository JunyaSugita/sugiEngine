#include "ClearScene.h"

void ClearScene::Initialize()
{
	// ���x���f�[�^�̓ǂݍ���
	levelData_ = JsonLoader::LoadJson("untitled");

	sphereModel_ = Model::LoadFromObj("sphere", true);
	models.insert(std::make_pair("sphere", sphereModel_));

	for (auto& objectData : levelData_->obj) {
		//�t�@�C������o�^�ς݃��f����T��
		Model* model = nullptr;
		decltype(models)::iterator it = models.find(objectData.filename);
		if (it != models.end()) {
			model = it->second;
		}
		//���f�����w�肵��3D�I�u�W�F�N�g�𐶐�
		Object3d* newObject = Object3d::Create();
		newObject->SetModel(sphereModel_);

		//obj���
		sphereWorldTransform_.trans_ = Vector3(objectData.pos);
		sphereWorldTransform_.rotation = Vector3(objectData.rot);
		sphereWorldTransform_.scale_ = Vector3(objectData.scale);

		newObject->SetWorldTransform(sphereWorldTransform_);

		objects.push_back(newObject);
	}

	//���C�g
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

	//���C�g
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
