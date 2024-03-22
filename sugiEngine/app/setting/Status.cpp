#include "Status.h"
#include "JsonLoader.h"

using namespace std;

Status* Status::GetInstance()
{
	static Status instance;

	return &instance;
}

void Status::LoadJson()
{
	spellData_.push_back(JsonLoader::LoadSpellData("fireBall"));
	spellData_.push_back(JsonLoader::LoadSpellData("magicMissile"));
	spellData_.push_back(JsonLoader::LoadSpellData("iceBolt"));
	spellData_.push_back(JsonLoader::LoadSpellData("chainLightning"));
	spellData_.push_back(JsonLoader::LoadSpellData("enchantFire"));
	spellData_.push_back(JsonLoader::LoadSpellData("flame"));
	spellData_.push_back(JsonLoader::LoadSpellData("wind"));
}
