#include "FbxLoader.h"
#include <cassert>

using namespace std;

const string FbxLoader::baseDirectory = "Resources/";

FbxLoader* FbxLoader::GetInstance()
{
    static FbxLoader instance;
    return &instance;
}

void FbxLoader::Initialize(ID3D12Device* device)
{
    //再初期化チェック
    assert(fbxManager == nullptr);
    //引数からメンバ変数に代入
    this->device = device;
    //FBXマネージャの生成
    fbxManager = FbxManager::Create();
    //FBXマネージャの入出力設定
    FbxIOSettings* ios = FbxIOSettings::Create(fbxManager,IOSROOT);
    fbxManager->SetIOSettings(ios);
    //FBXインポータ
    fbxImporter = FbxImporter::Create(fbxManager, "");
}

void FbxLoader::Finalize()
{
    //インスタンス破棄
    fbxImporter->Destroy();
    fbxManager->Destroy();
}

void FbxLoader::LoadMadelFromFile(const string& modelName)
{
    //モデルと同じ名前のフォルダから読み込み
    const string directoryPath = baseDirectory + modelName + "/";
    //拡張子を付加
    const string fileName = modelName + ".fbx";
    //合体
    const string fullpath = directoryPath + fileName;

    //ファイル名を指定して読み込み
    if (!fbxImporter->Initialize(fullpath.c_str(), -1, fbxManager->GetIOSettings())) {
        assert(0);
    }

    //シーン生成
    FbxScene* fbxScene = FbxScene::Create(fbxManager, "fbxScane");

    //ファイルからロードしたFBXの情報をシーンにインポート
    fbxImporter->Import(fbxScene);
}
