#pragma once

class UIManager {
private:
	UIManager();
	~UIManager();
public:
	//�R�s�[�R���X�g���N�^����
	UIManager(const UIManager& obj) = delete;
	//������Z�q�𖳌�
	UIManager& operator=(const UIManager& obj) = delete;

	static UIManager* GetInstance();

public:
	void Initialize();
	void Update();
	void Draw();
};