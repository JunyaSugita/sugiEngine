#include "SugiFramework.h"
#include "MyGame.h"
#include <fbxsdk.h>

//windowsのエントリーポイント
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	FbxManager* fbxManager = FbxManager::Create();
	SugiFramework* game = new MyGame();

	game->Run();

	delete game;

	return 0;
}