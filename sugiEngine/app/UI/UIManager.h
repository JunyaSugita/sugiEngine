#pragma once

class UIManager {
private:
	UIManager();
	~UIManager();
public:
	//コピーコンストラクタ無効
	UIManager(const UIManager& obj) = delete;
	//代入演算子を無効
	UIManager& operator=(const UIManager& obj) = delete;

	static UIManager* GetInstance();

public:
	void Initialize();
	void Update();
	void Draw();
};