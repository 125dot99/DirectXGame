#include "FbxAnimation.h"

#include <fbxsdk.h>
#include "../pipeline/IPipelineState.h"
#include "../loader/FbxLoader.h"
#include "../math/MyMath.h"

struct gamelib::FbxAnimaParameter //Fbxsdkでのアニメーション
{
	bool isPlay = false;
	bool isLoop = false;
	fbxsdk::FbxTime frameTime;
	fbxsdk::FbxTime startTime;
	fbxsdk::FbxTime endTime;
	fbxsdk::FbxTime currentTime;
};

gamelib::Matrix4 ConvertMatrixFromFbxMatrix(const fbxsdk::FbxMatrix& src)
{
	gamelib::Matrix4 result;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result.m[i][j] = (float)src.Get(i, j);
		}
	}
	return result;
}

gamelib::FbxAnimation::FbxAnimation(IMesh* p_mash)
{
	SkinMesh* skinMesh = dynamic_cast<SkinMesh*>(p_mash);
	fbxScene = skinMesh->fbxScene;
	//fbxScene = (fbxsdk::FbxScene*)FbxLoader::ReadFbxScene(); skinMesh->fbxScene;
	index = 0;
	boneSize = (int)skinMesh->bones.size();
	pMeshBone = skinMesh->bones.data();

	cbuffer = std::make_unique<ConstBuffer>();
	cbuffer->Init((int)ROOT_PARAMETER::BONE + 1, sizeof(CBMatrixBones));
	animaParam = std::make_unique<FbxAnimaParameter>();
	CBMatrixBones constMapSkin;
	constMapSkin.Copy();
	cbuffer->Map(&constMapSkin);
}

gamelib::FbxAnimation::~FbxAnimation()
{
	pMeshBone = nullptr;
	fbxScene = nullptr;
}

void gamelib::FbxAnimation::Play(int index)
{
	this->index = index;
	FbxAnimStack* animstack = fbxScene->GetSrcObject<FbxAnimStack>(index);
	if (animstack == nullptr)
	{
		return;
	}
	fbxScene->SetCurrentAnimationStack(animstack);
	//アニメーション時間取得
	FbxTakeInfo* takeinfo = fbxScene->GetTakeInfo(animstack->GetName());
	//開始時間取得
	animaParam->startTime = takeinfo->mLocalTimeSpan.GetStart();
	double start = takeinfo->mLocalTimeSpan.GetStart().GetSecondDouble();
	//終了時間取得
	animaParam->endTime = takeinfo->mLocalTimeSpan.GetStop();
	double end = takeinfo->mLocalTimeSpan.GetStop().GetSecondDouble();
	//開始時間に合わせる
	double currentTime = start;
	animaParam->currentTime = animaParam->startTime;
	animaParam->isPlay = true;
	animaParam->frameTime.SetTime(0, 0, 0, 1, 0, FbxTime::EMode::eFrames60);
}

void gamelib::FbxAnimation::Update()
{
	if (!animaParam->isPlay)
	{
		return;
	}
	CBMatrixBones constMapSkin;
	static FbxMatrix fbxCurrentPose;
	for (int i = 0; i < boneSize; i++)
	{
		//今の姿勢行列を取得
		fbxCurrentPose = pMeshBone[i].fbxCluster->GetLink()->EvaluateGlobalTransform(animaParam->currentTime);
		//合成してスキニング行列
		constMapSkin.bones[i] = pMeshBone[i].invInitialPose * ConvertMatrixFromFbxMatrix(fbxCurrentPose);
	}
	animaParam->currentTime = Wrap(animaParam->currentTime + animaParam->frameTime, animaParam->startTime, animaParam->endTime);
	cbuffer->Map(&constMapSkin);
}

void gamelib::FbxAnimation::GraphicsCommand()
{
	cbuffer->GraphicsCommand();
}
