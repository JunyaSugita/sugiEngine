#include "JsonLoader.h"
#include <fstream>

using namespace std;

nlohmann::json JsonLoader::sDeserialized;
LevelData* JsonLoader::sLevelData;

LevelData* JsonLoader::LoadJson(const std::string& filename)
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
	file >> sDeserialized;

	//���������x���f�[�^�t�@�C�����`�F�b�N
	assert(sDeserialized.is_object());
	assert(sDeserialized.contains("name"));
	assert(sDeserialized["name"].is_string());

	//"name"�𕶎���Ƃ��Ď擾
	string name = sDeserialized["name"].get<string>();
	//���������x���f�[�^�t�@�C�����`�F�b�N
	assert(name.compare("scene") == 0);

	//���x���f�[�^�i�[�p�C���X�^���X�𐶐�
	sLevelData = new LevelData();

	LoadRecursive();

	return sLevelData;
}

void JsonLoader::LoadRecursive()
{
	//"objects"�̑S�I�u�W�F�N�g�𑖍�
	for (nlohmann::json& object : sDeserialized["objects"]) {
		assert(object.contains("type"));

		//��ʂ��擾
		std::string type = object["type"].get<std::string>();

		//MESH
		if (type.compare("MESH") == 0) {
			//�v�f�ǉ�
			sLevelData->obj.emplace_back(LevelData::OBJData{});
			//���ǉ������v�f�̎Q�Ƃ𓾂�
			LevelData::OBJData& objectData = sLevelData->obj.back();

			if (object.contains("file_name")) {
				//�t�@�C����
				objectData.filename = object["file_name"];
			}

			//�g�����X�t�H�[���̃p�����[�^�ǂݍ���
			nlohmann::json& transform = object["transform"];
			//���s�ړ�
			objectData.pos.x = (float)transform["translation"][1];
			objectData.pos.y = (float)transform["translation"][2];
			objectData.pos.z = -(float)transform["translation"][0];
			//��]�p
			objectData.rot.x = -(float)transform["rotation"][1];
			objectData.rot.y = -(float)transform["rotation"][2];
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
