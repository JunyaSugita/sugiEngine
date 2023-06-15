#include "SugiFramework.h"
#include "MyGame.h"

//windowsのエントリーポイント
int32_t WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int32_t) {
	unique_ptr<SugiFramework> game = make_unique<MyGame>();

	game->Run();

	return 0;
}