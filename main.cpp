#include "MyGame.h"

//windowsのエントリーポイント
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	MyGame game;

	//ゲームの初期化
	game.Initialize();

	// ゲームループ
	while (true) {
		//ゲームを終了させるコマンドが来たらループを抜ける
		if (game.GetIsEnd()) {
			break;
		}

		//毎フレーム処理
		game.Update();

		//描画
		game.Draw();

	}

	//ゲームの終了
	game.Finalize();

	return 0;
}
