#include "SugiFramework.h"
#include "MyGame.h"

//windowsのエントリーポイント
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	SugiFramework* game = new MyGame();

	game->Run();

	delete game;

	return 0;
}