#include "FbxLoader.h"

#include <unordered_map>
#include <fbxsdk.h>
#include <fstream>
#include <list>
#include <assert.h>

#include "ResourceManager.h"
#include "../math/Matrix4.h"
#include "UtilityString.h"

using namespace fbxsdk;
//FBXのファイルパス
const std::string FBX_PATH = "Resources/Model/Fbx/";
static FbxManager* fbxManager;
static FbxImporter* fbxImporter;
static std::unordered_map<std::string, FbxScene*> fbxScenes{};

//ボーン番号とスキンウェイトのペア
struct WeightSet
{
    UINT index;
    float weight;
};

gamelib::SkinMesh* gamelib::FbxLoader::LoadModelFromFile(const std::string& modelName)
{
    //シーン生成
    FbxScene* fbxScene = FbxScene::Create(fbxManager, "fbxScene");
    //ファイルからロードしたFBXの情報をシーンにインポート
    fbxImporter->Import(fbxScene);
    //メッシュを三角面化
    FbxGeometryConverter geometryConverter(fbxManager);
    geometryConverter.SplitMeshesPerMaterial(fbxScene, true);
    geometryConverter.Triangulate(fbxScene, true);
    //モデル生成
    SkinMesh* model = new SkinMesh;
    model->name = modelName;
    //FBXノードの数を取得
    int nodeCount = fbxScene->GetNodeCount();
    //あらかじめ必要数分のメモリを確保してモデルに流し込む
    model->nodes.reserve(nodeCount);
    //ルートノードから順に解析してモデルに流し込む
    ParseNodeRecursive(model, fbxScene->GetRootNode());
    //FBXシーン解放
    model->fbxScene = fbxScene;
    fbxScenes[modelName] = model->fbxScene;
    return model;
}

void gamelib::FbxLoader::ParseNodeRecursive(SkinMesh* model, fbxsdk::FbxNode* fbxNode, FbxMeshNode* parent)
{
    //モデルにノード追加
    model->nodes.emplace_back();
    FbxMeshNode& node = model->nodes.back();
    //ノード名を取得
    node.name = fbxNode->GetName();
    //変形変換して代入
    FbxDouble3 rotation = fbxNode->LclRotation.Get();
    FbxDouble3 scaling = fbxNode->LclScaling.Get();
    FbxDouble3 translation = fbxNode->LclTranslation.Get();
    //スケール、回転、平行移動行列の計算
    Matrix4 matRotation = MatrixIdentity();
    matRotation *= MatrixRotateZ((float)rotation[2]);//Z
    matRotation *= MatrixRotateX((float)rotation[0]);//X
    matRotation *= MatrixRotateY((float)rotation[1]);//Y
    //ローカル変形行列の計算
    node.transform = MatrixIdentity();
    node.transform *= MatrixScale(Vector3((float)scaling[0], (float)scaling[1], (float)scaling[2]));
    node.transform *= matRotation;
    node.transform *= MatrixTranslate(Vector3((float)translation[0], (float)translation[1], (float)translation[2]));
    //グローバル変形行列の計算
    node.globalTransform = node.transform;
    if (parent)
    {
        node.parent = parent;
        //親の変形を乗算
        node.globalTransform *= parent->globalTransform;
    }
    //FBXノードのメッシュ情報を解析
    FbxNodeAttribute* fbxNodeAttribute = fbxNode->GetNodeAttribute();
    if (fbxNodeAttribute &&
        fbxNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
    {
        ParseMesh(model, fbxNode);
    }
    //子ノードに対して再帰呼び出し
    int childCount = fbxNode->GetChildCount();
    for (int i = 0; i < childCount; i++)
    {
        ParseNodeRecursive(model, fbxNode->GetChild(i), &node);
    }
}

void gamelib::FbxLoader::ParseMesh(SkinMesh* model, fbxsdk::FbxNode* fbxNode)
{
    //ノードのメッシュを取得
    FbxMesh* fbxMesh = fbxNode->GetMesh();
    //頂点座標読み取り
    ParseMeshPoints(model, fbxMesh);
    //インデックスの読み取り
    ParseMeshIndices(model, fbxMesh);
    //マテリアルの読み取り
    ParseMaterial(model, fbxNode);
    //スキニング情報の読み取り
    ParseSkin(model, fbxMesh);
    //頂点データを作り直す
    RemakeMeshVertices(model, fbxMesh);
}

void gamelib::FbxLoader::ParseMeshPoints(SkinMesh* model, fbxsdk::FbxMesh* fbxMesh)
{
    std::vector<VertexNormalUvBones>& vertices = model->vertexBuffer->vertices;
    //頂点座標データの数
    const int controlPointsCount = fbxMesh->GetControlPointsCount();
    //必要数だけ頂点データ配列を確保
    VertexNormalUvBones vert;
    vertices.resize(controlPointsCount, vert);
    //FBXメッシュの頂点座標配列を取得
    FbxVector4* pCoord = fbxMesh->GetControlPoints();
    //FBXメッシュの全頂点座標をモデル内の配列にコピー
    for (int i = 0; i < controlPointsCount; i++)
    {
        vertices[i].pos = Vector3((float)pCoord[i][0], (float)pCoord[i][1], (float)pCoord[i][2]);
    }
}

void gamelib::FbxLoader::ParseMeshIndices(SkinMesh* model, fbxsdk::FbxMesh* fbxMesh)
{
    std::vector<unsigned short>& indices = model->indexBuffer->indices;
    //1ファイルに複数メッシュのモデルは非対応
    assert(indices.size() == 0);
    //面の数
    const int polygonCount = fbxMesh->GetPolygonCount();
    //三角面ポリゴン
    const int polygonSize = 3;
    for (int i = 0; i < polygonCount; i++)
    {
        for (int j = 0; j < polygonSize; j++)
        {
            //FBX頂点配列のインデックス
            int index = fbxMesh->GetPolygonVertex(i, j);
            assert(index >= 0 && "Index : -1");
            indices.emplace_back(index);
        }
    }
}

void gamelib::FbxLoader::ParseMeshUVs(std::vector<Vector2>& uvs, fbxsdk::FbxMesh* fbxMesh)
{
    FbxStringList uvNames;
    fbxMesh->GetUVSetNames(uvNames);
    FbxArray<FbxVector2> fbxUvs;
    fbxMesh->GetPolygonVertexUVs(uvNames.GetStringAt(0), fbxUvs);
    const int uvSize = fbxUvs.Size();
    uvs.resize(uvSize);
    for (int i = 0; i < uvSize; i++)
    {
        uvs[i] = Vector2((float)fbxUvs[i][0], 1.0f - (float)fbxUvs[i][1]);
    }
}

void gamelib::FbxLoader::ParseMeshNormals(std::vector<Vector3>& normals, fbxsdk::FbxMesh* fbxMesh)
{
    FbxArray<FbxVector4> fbxNormals;
    fbxMesh->GetPolygonVertexNormals(fbxNormals);
    const int normalSize = fbxNormals.Size();
    normals.resize(normalSize);
    for (int i = 0; i < normalSize; i++)
    {
        normals[i] = Vector3((float)fbxNormals[i][0], (float)fbxNormals[i][1], (float)fbxNormals[i][2]);
    }
}

void gamelib::FbxLoader::RemakeMeshVertices(SkinMesh* model, fbxsdk::FbxMesh* fbxMesh)
{
    std::vector<Vector3> normals;
    ParseMeshNormals(normals, fbxMesh);

    std::vector<Vector2> uvs;
    ParseMeshUVs(uvs, fbxMesh);
   
    auto& vertices = model->vertexBuffer->vertices;
    auto& indices = model->indexBuffer->indices;
    const int indexSize = (int)indices.size();
    std::vector<VertexNormalUvBones> remake(indexSize);
    for (int i = 0; i < indexSize; i++)
    {
        remake[i] = vertices[indices[i]];
        remake[i].normal = normals[i];
        remake[i].uv = uvs[i];
        indices[i] = i;
    }
    vertices.swap(remake);
}

void gamelib::FbxLoader::ParseMaterial(SkinMesh* model, fbxsdk::FbxNode* fbxNode)
{
    auto& material = model->material;
    const int materialCount = fbxNode->GetMaterialCount();
    material = std::make_unique<Material>();
    if (materialCount == 0)
    {
        return;
    }
    //先頭のマテリアルを取得
    FbxLayerElementMaterial* layerMaterial = fbxNode->GetMesh()->GetElementMaterial(0);
    int index = layerMaterial->GetIndexArray().GetAt(0);
    FbxSurfaceMaterial* fbxMaterial = fbxNode->GetSrcObject<FbxSurfaceMaterial>(index);
    if (!fbxMaterial)
    {
        return;
    }
    //マテリアル名を取得
    material->name = fbxMaterial->GetName();
    auto resourceManager = ResourceManager::GetInstance();
    if (fbxMaterial->GetClassId().Is(FbxSurfaceLambert::ClassId))//ランバート
    {
        FbxSurfaceLambert* lambert = static_cast<FbxSurfaceLambert*>(fbxMaterial);
        //環境光係数
        FbxDouble3 ambient = lambert->Ambient;
        material->ambient = Vector3((float)ambient[0], (float)ambient[1], (float)ambient[2]);
        //拡散反射光係数
        FbxDouble3 diffuse = lambert->Diffuse;
        material->diffuse = Vector3((float)diffuse[0], (float)diffuse[1], (float)diffuse[2]);
        //ディフューズテクスチャを取り出す
        const FbxProperty diffuseProperty = fbxMaterial->FindProperty(FbxSurfaceLambert::sDiffuse);
        if (diffuseProperty.IsValid())
        {
            const FbxFileTexture* texture = diffuseProperty.GetSrcObject<FbxFileTexture>();
            if (texture)
            {
                const char* filepath = texture->GetFileName();
                //ファイルパスからファイル名抽出
                std::string path_str(filepath);
                //ディレクトリを含んだファイルパスからファイル名を抽出する
                std::string name = utility_string::ExtractFileName(path_str);
                resourceManager->LoadTextureFromFile(FBX_PATH + model->name + "/" + name);
                material->vec_w_p_textures.emplace_back(resourceManager->GetTexture(utility_string::ExtractFileName(name)));
            }
        }
    } else
    if (fbxMaterial->GetClassId().Is(FbxSurfacePhong::ClassId))//フォン
    {
        FbxSurfacePhong* phong = static_cast<FbxSurfacePhong*>(fbxMaterial);
        //環境光係数
        FbxDouble3 ambient = phong->Ambient;
        material->ambient = Vector3((float)ambient[0], (float)ambient[1], (float)ambient[2]);
        //拡散反射光係数
        FbxDouble3 diffuse = phong->Diffuse;
        material->diffuse = Vector3((float)diffuse[0], (float)diffuse[1], (float)diffuse[2]);
        //鏡面反射光係数
        FbxDouble3 specular = phong->Specular;
        material->specular = Vector3((float)specular[0], (float)specular[1], (float)specular[2]);
        //ディフューズテクスチャを取り出す
        const FbxProperty diffuseProperty = fbxMaterial->FindProperty(FbxSurfacePhong::sDiffuse);
        if (diffuseProperty.IsValid())
        {
            const FbxFileTexture* texture = diffuseProperty.GetSrcObject<FbxFileTexture>();
            if (texture)
            {
                const char* filepath = texture->GetFileName();
                //ファイルパスからファイル名抽出
                std::string path_str(filepath);
                //ディレクトリを含んだファイルパスからファイル名を抽出する
                std::string name = utility_string::ExtractFileName(path_str);
                resourceManager->LoadTextureFromFile(FBX_PATH + model->name + "/" + name);
                material->vec_w_p_textures.emplace_back(resourceManager->GetTexture(utility_string::ExtractFileName(name)));
            }
        }
    }
    if (material->vec_w_p_textures.size() == 0)
    {           
        //1x1の白いテクスチャを適応
        material->vec_w_p_textures.emplace_back(resourceManager->GetDefalutTexture());
    }
}

void gamelib::FbxLoader::ParseSkin(SkinMesh* model, fbxsdk::FbxMesh* fbxMesh)
{
    //スキニング情報
    FbxSkin* fbxSkin = static_cast<FbxSkin*>(fbxMesh->GetDeformer(0, FbxDeformer::eSkin));
    const int verticesSize = (int)model->vertexBuffer->vertices.size();
    if (fbxSkin == nullptr)
    {
        for (int i = 0; i < verticesSize; i++)
        {
            model->vertexBuffer->vertices[i].boneIndex[0] = 0;
            model->vertexBuffer->vertices[i].boneWeight[0] = 1.0f;
        }
        return;
    }
    //ボーン配列の参照
    std::vector<FbxBone>& bones = model->bones;
    //ボーンの数
    int clusterCount = fbxSkin->GetClusterCount();
    bones.reserve(clusterCount);
    //全てのボーンについて
    for (int i = 0; i < clusterCount; i++)
    {
        //FBXボーン情報
        FbxCluster* fbxCluster = fbxSkin->GetCluster(i);
        //ボーン自体のノードの名前を取得
        const char* boneName = fbxCluster->GetLink()->GetName();
        //新しくボーンを追加
        bones.emplace_back(FbxBone(boneName));
        FbxBone& bone = bones.back();
        //自作ボーンとFBXボーンを紐づける
        bone.fbxCluster = fbxCluster;
        //FBXから初期姿勢行列を取得する
        FbxAMatrix fbxMat;
        fbxCluster->GetTransformLinkMatrix(fbxMat);
        //Matrix4に変更する
        Matrix4 initialPose;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                initialPose.m[i][j] = (float)fbxMat.Get(i, j);
            }
        }
        //初期姿勢行列の逆行列を得る
        bone.invInitialPose = MatrixInverse(initialPose);
    }
    std::vector<std::list<WeightSet>> weightLists(verticesSize);
    for (int i = 0; i < clusterCount; i++)
    {
        //FBXボーン情報
        FbxCluster* fbxCluster = fbxSkin->GetCluster(i);
        //ボーンに影響を受ける頂点の数
        int controlPointIndicesCount = fbxCluster->GetControlPointIndicesCount();
        //ボーンに影響を受ける頂点の配列
        int* countrolPointIndices = fbxCluster->GetControlPointIndices();
        double* controlPointWeights = fbxCluster->GetControlPointWeights();
        //影響を受ける全頂点
        for (int j = 0; j < controlPointIndicesCount; j++)
        {
            //頂点番号
            int vertIndex = countrolPointIndices[j];
            //スキンウェイト
            float weight = (float)controlPointWeights[j];
            //その影響を受けるボーンリストに、ボーンとウェイトのペアを追加
            weightLists[vertIndex].emplace_back(WeightSet{ (UINT)i, weight });
        }
    }
    //頂点配列書き換え用
    auto& vertices = model->vertexBuffer->vertices;
    //各頂点について
    for (int i = 0; i < verticesSize; i++)
    {
        //頂点のウェイトから最も大きい4つを選択
        auto& weightList = weightLists[i];
        //大小比較用のラムダ式を指定して降順にソート
        weightList.sort([](auto const& lhs, auto const& rhs)
        {
            //左の要素が大きければtrue、それでなければfalseを返す
            return lhs.weight > rhs.weight;
        });
        int weightArrayIndex = 0;
        //降順ソート済みのウェイトリストから
        for (auto& WeightSet : weightList)
        {
            //頂点データに書き込み
            vertices[i].boneIndex[weightArrayIndex] = WeightSet.index;
            vertices[i].boneWeight[weightArrayIndex] = WeightSet.weight;
            //4つ目に達したら終了
            if (++weightArrayIndex >= VertexNormalUvBones::MAX_BONE_INDICES)
            {
                //2番目以降のウェイトを合計
                float weight = 0.0f;
                for (int j = 1; j < VertexNormalUvBones::MAX_BONE_INDICES; j++)
                {
                    weight += vertices[i].boneWeight[j];
                }
                //合計で1.0fになるように調整
                vertices[i].boneWeight[0] = 1.0f - weight;
                break;
            }
        }
    }
}

void gamelib::FbxLoader::Initialize()
{
    assert(fbxManager == nullptr);
    //FBXマネージャーの生成
    fbxManager = FbxManager::Create();
    //FBXマネージャーの入出力設定
    FbxIOSettings* ios = FbxIOSettings::Create(fbxManager, IOSROOT);
    fbxManager->SetIOSettings(ios);
    //FBXインポーターの設定
    fbxImporter = FbxImporter::Create(fbxManager, "");
}

void gamelib::FbxLoader::Finalize()
{
    fbxImporter->Destroy();
    fbxManager->Destroy();
}

gamelib::SkinMesh* gamelib::FbxLoader::ReadFbxModel(const std::string& modelName)
{
    //モデルと同じ名前のフォルダから読み込む
    const std::string directoryPath = FBX_PATH + modelName + "/";
    const std::string fileName = modelName + ".fbx";
    const std::string fullpath = directoryPath + fileName;
    //ファイル名を指定してFBXファイルを読み込む
    if (!fbxImporter->Initialize(fullpath.c_str(), -1, fbxManager->GetIOSettings()))
    {
        assert(!"FBXファイルの読み込みに失敗しました");
        return nullptr;
    }
    return LoadModelFromFile(modelName);
}

void* gamelib::FbxLoader::ReadFbxScene(const std::string& modelName)
{
    if (fbxScenes.find(modelName) != fbxScenes.end())
    {
        return fbxScenes[modelName];
    }
    return nullptr;
}
