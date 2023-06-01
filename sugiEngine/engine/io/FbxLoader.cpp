#include "FbxLoader.h"
#include <cassert>

using namespace std;
using namespace  DirectX;

const string FbxLoader::baseDirectory = "Resources/";
const string FbxLoader::defaultTextureFileName = "white1x1.png";

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
	FbxIOSettings* ios = FbxIOSettings::Create(fbxManager, IOSROOT);
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

void FbxLoader::LoadModelFromFile(const string& modelName)
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

	//モデル生成
	FbxModel* model = new FbxModel();
	model->name = modelName;

	int nodeCount = fbxScene->GetNodeCount();
	model->nodes.reserve(nodeCount);

	//解析してモデルに流す
	ParseNodeRecursive(model, fbxScene->GetRootNode());
	//解放
	fbxScene->Destroy();

	model->CreateBuffers(device);
}

void FbxLoader::ParseNodeRecursive(FbxModel* model, FbxNode* fbxNode, Node* parent)
{
	//ノード追加
	model->nodes.emplace_back();
	Node& node = model->nodes.back();
	//ノード名
	node.name = fbxNode->GetName();

	FbxDouble3 rotation = fbxNode->LclRotation.Get();
	FbxDouble3 scaling = fbxNode->LclScaling.Get();
	FbxDouble3 translation = fbxNode->LclTranslation.Get();

	//形式変換して代入
	node.rotation = { (float)rotation[0],(float)rotation[1],(float)rotation[2],0.0f };
	node.scaling = { (float)scaling[0],(float)scaling[1],(float)scaling[2],0.0f };
	node.translation = { (float)translation[0],(float)translation[1],(float)translation[2],1.0f };

	//回転角をラジアンに変換
	node.rotation.m128_f32[0] = XMConvertToRadians(node.rotation.m128_f32[0]);
	node.rotation.m128_f32[1] = XMConvertToRadians(node.rotation.m128_f32[1]);
	node.rotation.m128_f32[2] = XMConvertToRadians(node.rotation.m128_f32[2]);

	//スケール、回転、平行移動の計算
	XMMATRIX matScaling, matRotation, matTranslation;
	matScaling = XMMatrixScalingFromVector(node.scaling);
	matRotation = XMMatrixRotationRollPitchYawFromVector(node.rotation);
	matTranslation = XMMatrixTranslationFromVector(node.translation);

	//ローカル変換座標
	node.transform = XMMatrixIdentity();
	node.transform *= matScaling;
	node.transform *= matRotation;
	node.transform *= matTranslation;

	//グローバル変形行列計算
	node.globalTransform = node.transform;
	if (parent) {
		node.parent = parent;
		node.globalTransform *= parent->globalTransform;
	}

	FbxNodeAttribute* fbxNodeAttribute = fbxNode->GetNodeAttribute();

	if (fbxNodeAttribute) {
		if (fbxNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh) {
			model->meshNode = &node;
			ParseMesh(model, fbxNode);
		}
	}

	for (int i = 0; i < fbxNode->GetChildCount(); i++) {
		ParseNodeRecursive(model, fbxNode->GetChild(i), &node);
	}
}

void FbxLoader::ParseMesh(FbxModel* model, FbxNode* fbxNode)
{
	FbxMesh* fbxMesh = fbxNode->GetMesh();

	ParseMeshVertices(model, fbxMesh);
	ParseMeshFaces(model, fbxMesh);
	ParseMaterial(model, fbxNode);
}

void FbxLoader::ParseMeshVertices(FbxModel* model, FbxMesh* fbxMesh)
{
	auto& vertices = model->vertices;

	//頂点座標データ数
	const int controlPointsCount = fbxMesh->GetControlPointsCount();
	//必要数だけ頂点データ配列を確保
	FbxModel::VertexPosNormalUv vert{};
	model->vertices.resize(controlPointsCount, vert);
	//FBXメッシュの頂点座標配列を取得
	FbxVector4* pCoord = fbxMesh->GetControlPoints();
	//FBXメッシュの全頂点座標をモデル内の配列にコピー
	for (int i = 0; i < controlPointsCount; i++) {
		FbxModel::VertexPosNormalUv& vertex = vertices[i];

		vertex.pos.x = (float)pCoord[i][0];
		vertex.pos.y = (float)pCoord[i][1];
		vertex.pos.z = (float)pCoord[i][2];
	}

}

void FbxLoader::ParseMeshFaces(FbxModel* model, FbxMesh* fbxMesh)
{
	auto& vertices = model->vertices;
	auto& indices = model->indices;

	//1ファイルに複数メッシュのモデルは非対応
	assert(indices.size() == 0);
	//面の数
	const int polygonCount = fbxMesh->GetPolygonCount();
	//UVデータの数
	const int textureUVCount = fbxMesh->GetTextureUVCount();
	//UV名リスト
	FbxStringList uvNames;
	fbxMesh->GetUVSetNames(uvNames);
	//面ごとの情報読み取り
	for (int i = 0; i < polygonCount; i++) {
		//面を構成する頂点の数を取得(3なら三角形ポリゴン)
		const int polygonSize = fbxMesh->GetPolygonSize(i);
		assert(polygonSize <= 4);

		//1頂点ずつ処理
		for (int j = 0; j < polygonSize; j++) {
			//FBX頂点配列のインデックス
			int index = fbxMesh->GetPolygonVertex(i, j);
			assert(index >= 0);
			//頂点法線読み込み
			FbxModel::VertexPosNormalUv& vertex = vertices[index];
			FbxVector4 normal;
			if (fbxMesh->GetPolygonVertexNormal(i, j, normal)) {
				vertex.normal.x = (float)normal[0];
				vertex.normal.y = (float)normal[1];
				vertex.normal.z = (float)normal[2];
			}
			//テクスチャの読み込み
			if (textureUVCount > 0) {
				FbxVector2 uvs;
				bool lUnmappedUV;
				//0番決め打ちで読み込み
				if (fbxMesh->GetPolygonVertexUV(i, j, uvNames[0], uvs, lUnmappedUV)) {
					vertex.uv.x = (float)uvs[0];
					vertex.uv.y = (float)uvs[1];
				}
			}
			//インデックス配列に頂点インデックス追加
			//3頂点目まで
			if (j < 3) {
				indices.push_back(index);
			}
			//4頂点目
			else {
				//3点追加し、四角形の0,1,2,3のうち2,3,0で三角形を構築
				int index2 = indices[indices.size() - 1];
				int index3 = index;
				int index0 = indices[indices.size() - 3];
				indices.push_back(index2);
				indices.push_back(index3);
				indices.push_back(index0);
			}

		}
	}

}

void FbxLoader::ParseMaterial(FbxModel* model, FbxNode* fbxNode)
{
	const int materialCount = fbxNode->GetMaterialCount();
	if (materialCount > 0) {
		FbxSurfaceMaterial* material = fbxNode->GetMaterial(0);

		bool textureLoaded = false;

		if (material) {

		}

		if (!textureLoaded) {
			LoadTexture(model, baseDirectory + defaultTextureFileName);
		}

		if (material->GetClassId().Is(FbxSurfaceLambert::ClassId)) {
			FbxSurfaceLambert* lambert = static_cast<FbxSurfaceLambert*>(material);

			FbxPropertyT<FbxDouble3> ambient = lambert->Ambient;
			model->ambient.x = (float)ambient.Get()[0];
			model->ambient.y = (float)ambient.Get()[1];
			model->ambient.z = (float)ambient.Get()[2];

			FbxPropertyT<FbxDouble3> diffuse = lambert->Diffuse;
			model->diffuse.x = (float)diffuse.Get()[0];
			model->diffuse.y = (float)diffuse.Get()[1];
			model->diffuse.z = (float)diffuse.Get()[2];
		}

		const FbxProperty diffuseProperty = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
		if (diffuseProperty.IsValid()) {
			const FbxFileTexture* texture = diffuseProperty.GetSrcObject<FbxFileTexture>();
			if (texture) {
				const char* filepath = texture->GetFileName();
				string path_str(filepath);
				string name = ExtractFileName(path_str);

				LoadTexture(model, baseDirectory + model->name + "/" + name);
				textureLoaded = true;
			}
		}
	}
}

void FbxLoader::LoadTexture(FbxModel* model, const string& fullpath)
{
	HRESULT result = S_FALSE;

	TexMetadata& metadata = model->metadata;
	ScratchImage& scratchImg = model->scratchImg;

	wchar_t wfilepath[128];
	MultiByteToWideChar(CP_ACP, 0, fullpath.c_str(), -1, wfilepath, _countof(wfilepath));
	result = LoadFromWICFile(
		wfilepath,
		WIC_FLAGS_NONE,
		&metadata,
		scratchImg
	);
	if (FAILED(result)) {
		assert(0);
	}
}

string FbxLoader::ExtractFileName(const string& path)
{
	size_t pos1;

	//区切り文字('\\')が出てくる一番最後の部分を検索
	pos1 = path.rfind('\\');
	if (pos1 != string::npos) {
		return path.substr(pos1 + 1, path.size() - pos1 - 1);
	}
	//区切り文字'/'が出てくる1番最後の部分を検索
	pos1 = path.rfind('/');
	if (pos1 != string::npos) {
		return path.substr(pos1 + 1, path.size() - pos1 - 1);
	}

	return path;
}
