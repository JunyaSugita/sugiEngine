#include "SugiFramework.h"
#include "MyGame.h"

//windowsのエントリーポイント
int32_t WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int32_t) {
	SugiFramework* game = new MyGame();

	game->Run();

	delete game;

	return 0;
}