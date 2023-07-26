#include "SugiFramework.h"
#include "MyGame.h"

//windowsのエントリーポイント
int32_t WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int32_t) {
	std::unique_ptr<SugiFramework> game = std::make_unique<MyGame>();

	game->Run();

	return 0;
}