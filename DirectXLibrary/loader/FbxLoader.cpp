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
//FBX�̃t�@�C���p�X
const std::string FBX_PATH = "Resources/Model/Fbx/";
static FbxManager* fbxManager;
static FbxImporter* fbxImporter;
static std::unordered_map<std::string, FbxScene*> fbxScenes{};

//�{�[���ԍ��ƃX�L���E�F�C�g�̃y�A
struct WeightSet
{
    UINT index;
    float weight;
};

gamelib::SkinMesh* gamelib::FbxLoader::LoadModelFromFile(const std::string& modelName)
{
    //�V�[������
    FbxScene* fbxScene = FbxScene::Create(fbxManager, "fbxScene");
    //�t�@�C�����烍�[�h����FBX�̏����V�[���ɃC���|�[�g
    fbxImporter->Import(fbxScene);
    //���b�V�����O�p�ʉ�
    FbxGeometryConverter geometryConverter(fbxManager);
    geometryConverter.SplitMeshesPerMaterial(fbxScene, true);
    geometryConverter.Triangulate(fbxScene, true);
    //���f������
    SkinMesh* model = new SkinMesh;
    model->name = modelName;
    //FBX�m�[�h�̐����擾
    int nodeCount = fbxScene->GetNodeCount();
    //���炩���ߕK�v�����̃��������m�ۂ��ă��f���ɗ�������
    model->nodes.reserve(nodeCount);
    //���[�g�m�[�h���珇�ɉ�͂��ă��f���ɗ�������
    ParseNodeRecursive(model, fbxScene->GetRootNode());
    //FBX�V�[�����
    model->fbxScene = fbxScene;
    fbxScenes[modelName] = model->fbxScene;
    return model;
}

void gamelib::FbxLoader::ParseNodeRecursive(SkinMesh* model, fbxsdk::FbxNode* fbxNode, FbxMeshNode* parent)
{
    //���f���Ƀm�[�h�ǉ�
    model->nodes.emplace_back();
    FbxMeshNode& node = model->nodes.back();
    //�m�[�h�����擾
    node.name = fbxNode->GetName();
    //�ό`�ϊ����đ��
    FbxDouble3 rotation = fbxNode->LclRotation.Get();
    FbxDouble3 scaling = fbxNode->LclScaling.Get();
    FbxDouble3 translation = fbxNode->LclTranslation.Get();
    //�X�P�[���A��]�A���s�ړ��s��̌v�Z
    Matrix4 matRotation = MatrixIdentity();
    matRotation *= MatrixRotateZ((float)rotation[2]);//Z
    matRotation *= MatrixRotateX((float)rotation[0]);//X
    matRotation *= MatrixRotateY((float)rotation[1]);//Y
    //���[�J���ό`�s��̌v�Z
    node.transform = MatrixIdentity();
    node.transform *= MatrixScale(Vector3((float)scaling[0], (float)scaling[1], (float)scaling[2]));
    node.transform *= matRotation;
    node.transform *= MatrixTranslate(Vector3((float)translation[0], (float)translation[1], (float)translation[2]));
    //�O���[�o���ό`�s��̌v�Z
    node.globalTransform = node.transform;
    if (parent)
    {
        node.parent = parent;
        //�e�̕ό`����Z
        node.globalTransform *= parent->globalTransform;
    }
    //FBX�m�[�h�̃��b�V���������
    FbxNodeAttribute* fbxNodeAttribute = fbxNode->GetNodeAttribute();
    if (fbxNodeAttribute &&
        fbxNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
    {
        ParseMesh(model, fbxNode);
    }
    //�q�m�[�h�ɑ΂��čċA�Ăяo��
    int childCount = fbxNode->GetChildCount();
    for (int i = 0; i < childCount; i++)
    {
        ParseNodeRecursive(model, fbxNode->GetChild(i), &node);
    }
}

void gamelib::FbxLoader::ParseMesh(SkinMesh* model, fbxsdk::FbxNode* fbxNode)
{
    //�m�[�h�̃��b�V�����擾
    FbxMesh* fbxMesh = fbxNode->GetMesh();
    //���_���W�ǂݎ��
    ParseMeshPoints(model, fbxMesh);
    //�C���f�b�N�X�̓ǂݎ��
    ParseMeshIndices(model, fbxMesh);
    //�}�e���A���̓ǂݎ��
    ParseMaterial(model, fbxNode);
    //�X�L�j���O���̓ǂݎ��
    ParseSkin(model, fbxMesh);
    //���_�f�[�^����蒼��
    RemakeMeshVertices(model, fbxMesh);
}

void gamelib::FbxLoader::ParseMeshPoints(SkinMesh* model, fbxsdk::FbxMesh* fbxMesh)
{
    std::vector<VertexNormalUvBones>& vertices = model->vertexBuffer->vertices;
    //���_���W�f�[�^�̐�
    const int controlPointsCount = fbxMesh->GetControlPointsCount();
    //�K�v���������_�f�[�^�z����m��
    VertexNormalUvBones vert;
    vertices.resize(controlPointsCount, vert);
    //FBX���b�V���̒��_���W�z����擾
    FbxVector4* pCoord = fbxMesh->GetControlPoints();
    //FBX���b�V���̑S���_���W�����f�����̔z��ɃR�s�[
    for (int i = 0; i < controlPointsCount; i++)
    {
        vertices[i].pos = Vector3((float)pCoord[i][0], (float)pCoord[i][1], (float)pCoord[i][2]);
    }
}

void gamelib::FbxLoader::ParseMeshIndices(SkinMesh* model, fbxsdk::FbxMesh* fbxMesh)
{
    std::vector<unsigned short>& indices = model->indexBuffer->indices;
    //1�t�@�C���ɕ������b�V���̃��f���͔�Ή�
    assert(indices.size() == 0);
    //�ʂ̐�
    const int polygonCount = fbxMesh->GetPolygonCount();
    //�O�p�ʃ|���S��
    const int polygonSize = 3;
    for (int i = 0; i < polygonCount; i++)
    {
        for (int j = 0; j < polygonSize; j++)
        {
            //FBX���_�z��̃C���f�b�N�X
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
    //�擪�̃}�e���A�����擾
    FbxLayerElementMaterial* layerMaterial = fbxNode->GetMesh()->GetElementMaterial(0);
    int index = layerMaterial->GetIndexArray().GetAt(0);
    FbxSurfaceMaterial* fbxMaterial = fbxNode->GetSrcObject<FbxSurfaceMaterial>(index);
    if (!fbxMaterial)
    {
        return;
    }
    //�}�e���A�������擾
    material->name = fbxMaterial->GetName();
    auto resourceManager = ResourceManager::GetInstance();
    if (fbxMaterial->GetClassId().Is(FbxSurfaceLambert::ClassId))//�����o�[�g
    {
        FbxSurfaceLambert* lambert = static_cast<FbxSurfaceLambert*>(fbxMaterial);
        //�����W��
        FbxDouble3 ambient = lambert->Ambient;
        material->ambient = Vector3((float)ambient[0], (float)ambient[1], (float)ambient[2]);
        //�g�U���ˌ��W��
        FbxDouble3 diffuse = lambert->Diffuse;
        material->diffuse = Vector3((float)diffuse[0], (float)diffuse[1], (float)diffuse[2]);
        //�f�B�t���[�Y�e�N�X�`�������o��
        const FbxProperty diffuseProperty = fbxMaterial->FindProperty(FbxSurfaceLambert::sDiffuse);
        if (diffuseProperty.IsValid())
        {
            const FbxFileTexture* texture = diffuseProperty.GetSrcObject<FbxFileTexture>();
            if (texture)
            {
                const char* filepath = texture->GetFileName();
                //�t�@�C���p�X����t�@�C�������o
                std::string path_str(filepath);
                //�f�B���N�g�����܂񂾃t�@�C���p�X����t�@�C�����𒊏o����
                std::string name = utility_string::ExtractFileName(path_str);
                resourceManager->LoadTextureFromFile(FBX_PATH + model->name + "/" + name);
                material->vec_w_p_textures.emplace_back(resourceManager->GetTexture(utility_string::ExtractFileName(name)));
            }
        }
    } else
    if (fbxMaterial->GetClassId().Is(FbxSurfacePhong::ClassId))//�t�H��
    {
        FbxSurfacePhong* phong = static_cast<FbxSurfacePhong*>(fbxMaterial);
        //�����W��
        FbxDouble3 ambient = phong->Ambient;
        material->ambient = Vector3((float)ambient[0], (float)ambient[1], (float)ambient[2]);
        //�g�U���ˌ��W��
        FbxDouble3 diffuse = phong->Diffuse;
        material->diffuse = Vector3((float)diffuse[0], (float)diffuse[1], (float)diffuse[2]);
        //���ʔ��ˌ��W��
        FbxDouble3 specular = phong->Specular;
        material->specular = Vector3((float)specular[0], (float)specular[1], (float)specular[2]);
        //�f�B�t���[�Y�e�N�X�`�������o��
        const FbxProperty diffuseProperty = fbxMaterial->FindProperty(FbxSurfacePhong::sDiffuse);
        if (diffuseProperty.IsValid())
        {
            const FbxFileTexture* texture = diffuseProperty.GetSrcObject<FbxFileTexture>();
            if (texture)
            {
                const char* filepath = texture->GetFileName();
                //�t�@�C���p�X����t�@�C�������o
                std::string path_str(filepath);
                //�f�B���N�g�����܂񂾃t�@�C���p�X����t�@�C�����𒊏o����
                std::string name = utility_string::ExtractFileName(path_str);
                resourceManager->LoadTextureFromFile(FBX_PATH + model->name + "/" + name);
                material->vec_w_p_textures.emplace_back(resourceManager->GetTexture(utility_string::ExtractFileName(name)));
            }
        }
    }
    if (material->vec_w_p_textures.size() == 0)
    {           
        //1x1�̔����e�N�X�`����K��
        material->vec_w_p_textures.emplace_back(resourceManager->GetDefalutTexture());
    }
}

void gamelib::FbxLoader::ParseSkin(SkinMesh* model, fbxsdk::FbxMesh* fbxMesh)
{
    //�X�L�j���O���
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
    //�{�[���z��̎Q��
    std::vector<FbxBone>& bones = model->bones;
    //�{�[���̐�
    int clusterCount = fbxSkin->GetClusterCount();
    bones.reserve(clusterCount);
    //�S�Ẵ{�[���ɂ���
    for (int i = 0; i < clusterCount; i++)
    {
        //FBX�{�[�����
        FbxCluster* fbxCluster = fbxSkin->GetCluster(i);
        //�{�[�����̂̃m�[�h�̖��O���擾
        const char* boneName = fbxCluster->GetLink()->GetName();
        //�V�����{�[����ǉ�
        bones.emplace_back(FbxBone(boneName));
        FbxBone& bone = bones.back();
        //����{�[����FBX�{�[����R�Â���
        bone.fbxCluster = fbxCluster;
        //FBX���珉���p���s����擾����
        FbxAMatrix fbxMat;
        fbxCluster->GetTransformLinkMatrix(fbxMat);
        //Matrix4�ɕύX����
        Matrix4 initialPose;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                initialPose.m[i][j] = (float)fbxMat.Get(i, j);
            }
        }
        //�����p���s��̋t�s��𓾂�
        bone.invInitialPose = MatrixInverse(initialPose);
    }
    std::vector<std::list<WeightSet>> weightLists(verticesSize);
    for (int i = 0; i < clusterCount; i++)
    {
        //FBX�{�[�����
        FbxCluster* fbxCluster = fbxSkin->GetCluster(i);
        //�{�[���ɉe�����󂯂钸�_�̐�
        int controlPointIndicesCount = fbxCluster->GetControlPointIndicesCount();
        //�{�[���ɉe�����󂯂钸�_�̔z��
        int* countrolPointIndices = fbxCluster->GetControlPointIndices();
        double* controlPointWeights = fbxCluster->GetControlPointWeights();
        //�e�����󂯂�S���_
        for (int j = 0; j < controlPointIndicesCount; j++)
        {
            //���_�ԍ�
            int vertIndex = countrolPointIndices[j];
            //�X�L���E�F�C�g
            float weight = (float)controlPointWeights[j];
            //���̉e�����󂯂�{�[�����X�g�ɁA�{�[���ƃE�F�C�g�̃y�A��ǉ�
            weightLists[vertIndex].emplace_back(WeightSet{ (UINT)i, weight });
        }
    }
    //���_�z�񏑂������p
    auto& vertices = model->vertexBuffer->vertices;
    //�e���_�ɂ���
    for (int i = 0; i < verticesSize; i++)
    {
        //���_�̃E�F�C�g����ł��傫��4��I��
        auto& weightList = weightLists[i];
        //�召��r�p�̃����_�����w�肵�č~���Ƀ\�[�g
        weightList.sort([](auto const& lhs, auto const& rhs)
        {
            //���̗v�f���傫�����true�A����łȂ����false��Ԃ�
            return lhs.weight > rhs.weight;
        });
        int weightArrayIndex = 0;
        //�~���\�[�g�ς݂̃E�F�C�g���X�g����
        for (auto& WeightSet : weightList)
        {
            //���_�f�[�^�ɏ�������
            vertices[i].boneIndex[weightArrayIndex] = WeightSet.index;
            vertices[i].boneWeight[weightArrayIndex] = WeightSet.weight;
            //4�ڂɒB������I��
            if (++weightArrayIndex >= VertexNormalUvBones::MAX_BONE_INDICES)
            {
                //2�Ԗڈȍ~�̃E�F�C�g�����v
                float weight = 0.0f;
                for (int j = 1; j < VertexNormalUvBones::MAX_BONE_INDICES; j++)
                {
                    weight += vertices[i].boneWeight[j];
                }
                //���v��1.0f�ɂȂ�悤�ɒ���
                vertices[i].boneWeight[0] = 1.0f - weight;
                break;
            }
        }
    }
}

void gamelib::FbxLoader::Initialize()
{
    assert(fbxManager == nullptr);
    //FBX�}�l�[�W���[�̐���
    fbxManager = FbxManager::Create();
    //FBX�}�l�[�W���[�̓��o�͐ݒ�
    FbxIOSettings* ios = FbxIOSettings::Create(fbxManager, IOSROOT);
    fbxManager->SetIOSettings(ios);
    //FBX�C���|�[�^�[�̐ݒ�
    fbxImporter = FbxImporter::Create(fbxManager, "");
}

void gamelib::FbxLoader::Finalize()
{
    fbxImporter->Destroy();
    fbxManager->Destroy();
}

gamelib::SkinMesh* gamelib::FbxLoader::ReadFbxModel(const std::string& modelName)
{
    //���f���Ɠ������O�̃t�H���_����ǂݍ���
    const std::string directoryPath = FBX_PATH + modelName + "/";
    const std::string fileName = modelName + ".fbx";
    const std::string fullpath = directoryPath + fileName;
    //�t�@�C�������w�肵��FBX�t�@�C����ǂݍ���
    if (!fbxImporter->Initialize(fullpath.c_str(), -1, fbxManager->GetIOSettings()))
    {
        assert(!"FBX�t�@�C���̓ǂݍ��݂Ɏ��s���܂���");
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
