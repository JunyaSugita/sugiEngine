#include "JsonLoader.h"
#include <fstream>

using namespace std;

nlohmann::json JsonLoader::deserialized_;
LevelData* JsonLoader::levelData_;

LevelData* JsonLoader::LoadJson(std::string filename)
{
	//�A�����ăt���p�X�𓾂�
	const string fullpath = "Resources/level/" + filename + ".Json";

	//�t�@�C���X�g���[��
	ifstream file;

	//�t�@�C�����J��
	file.open(fullpath);
	//�t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail()) {
		assert(0);
	}

	//��
	file >> deserialized_;

	//���������x���f�[�^�t�@�C�����`�F�b�N
	assert(deserialized_.is_object());
	assert(deserialized_.contains("name"));
	assert(deserialized_["name"].is_string());

	//"name"�𕶎���Ƃ��Ď擾
	string name = deserialized_["name"].get<string>();
	//���������x���f�[�^�t�@�C�����`�F�b�N
	assert(name.compare("scene") == 0);

	//���x���f�[�^�i�[�p�C���X�^���X�𐶐�
	levelData_ = new LevelData();

	LoadRecursive();

	return levelData_;
}

void JsonLoader::LoadRecursive()
{
	//"objects"�̑S�I�u�W�F�N�g�𑖍�
	for (nlohmann::json& object : deserialized_["objects"]) {
		assert(object.contains("type"));

		//��ʂ��擾
		std::string type = object["type"].get<std::string>();

		//MESH
		if (type.compare("MESH") == 0) {
			//�v�f�ǉ�
			levelData_->obj.emplace_back(LevelData::OBJData{});
			//���ǉ������v�f�̎Q�Ƃ𓾂�
			LevelData::OBJData& objectData = levelData_->obj.back();

			if (object.contains("file_name")) {
				//�t�@�C����
				objectData.filename = object["file_name"];
			}

			//�g�����X�t�H�[���̃p�����[�^�ǂݍ���
			nlohmann::json& transform = object["transform"];
			//���s�ړ�
			objectData.pos.x = (float)transform["translation"][1];
			objectData.pos.y = (float)transform["translation"][2];
			objectData.pos.z = (float)-transform["translation"][0];
			//��]�p
			objectData.rot.x = (float)-transform["rotation"][1];
			objectData.rot.y = (float)-transform["rotation"][2];
			objectData.rot.y += 90;
			objectData.rot.z = (float)transform["rotation"][0];
			//�X�P�[�����O
			objectData.scale.x = (float)transform["scaling"][1];
			objectData.scale.y = (float)transform["scaling"][2];
			objectData.scale.z = (float)transform["scaling"][0];
		}

		//�I�u�W�F�N�g�����̍ċA�֐�
		if (object.contains("children")) {
			LoadRecursive();
		}
	}
}
