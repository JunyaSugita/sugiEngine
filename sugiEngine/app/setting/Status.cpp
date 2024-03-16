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
}
