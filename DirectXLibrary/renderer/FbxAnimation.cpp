#include "FbxAnimation.h"

#include <fbxsdk.h>
#include "../loader/ResourceManager.h"
#include "../pipeline/IPipelineState.h"
#include "../math/MyMath.h"

struct gamelib::FbxAnimaParameter //Fbxsdkでのアニメーション
{
	bool isPlay;
	bool isLoop;
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

gamelib::FbxAnimation::FbxAnimation(const std::string& skinmeshName)
{
	auto skinMesh = std::dynamic_pointer_cast<SkinMesh>(ResourceManager::GetInstance()->GetMesh(skinmeshName).lock());
	pFbxScene = skinMesh->pFbxScene;
	index = 0;
	boneSize = (int)skinMesh->vecFbxBones.size();
	pMeshBone = skinMesh->vecFbxBones.data();

	u_pConstBuffer = std::make_unique<ConstBuffer>();
	u_pConstBuffer->Init((int)ROOT_PARAMETER::BONE + 1, sizeof(CBMatrixBones));
	u_pAnimaParam = std::make_unique<FbxAnimaParameter>();
	CBMatrixBones constMapSkin;
	constMapSkin.Copy();
	u_pConstBuffer->Map(&constMapSkin);
}

gamelib::FbxAnimation::~FbxAnimation()
{
	pMeshBone = nullptr;
	pFbxScene = nullptr;
}

void gamelib::FbxAnimation::Play(int index)
{
	this->index = index;
	FbxAnimStack* animstack = pFbxScene->GetSrcObject<FbxAnimStack>(index);
	if (animstack == nullptr)
	{
		return;
	}
	pFbxScene->SetCurrentAnimationStack(animstack);
	//アニメーション時間取得
	FbxTakeInfo* takeinfo = pFbxScene->GetTakeInfo(animstack->GetName());
	//開始時間取得
	u_pAnimaParam->startTime = takeinfo->mLocalTimeSpan.GetStart();
	double start = takeinfo->mLocalTimeSpan.GetStart().GetSecondDouble();
	//終了時間取得
	u_pAnimaParam->endTime = takeinfo->mLocalTimeSpan.GetStop();
	double end = takeinfo->mLocalTimeSpan.GetStop().GetSecondDouble();
	//開始時間に合わせる
	double currentTime = start;
	u_pAnimaParam->currentTime = u_pAnimaParam->startTime;
	u_pAnimaParam->isPlay = true;
	u_pAnimaParam->frameTime.SetTime(0, 0, 0, 1, 0, FbxTime::EMode::eFrames60);
}

void gamelib::FbxAnimation::Update()
{
	if (!u_pAnimaParam->isPlay)
	{
		return;
	}
	CBMatrixBones constMapSkin;
	FbxMatrix fbxCurrentPose;
	for (int i = 0; i < boneSize; i++)
	{
		//今の姿勢行列を取得
		fbxCurrentPose = pMeshBone[i].fbxCluster->GetLink()->EvaluateGlobalTransform(u_pAnimaParam->currentTime);
		//合成してスキニング行列
		constMapSkin.vecFbxBones[i] = pMeshBone[i].invInitialPose * ConvertMatrixFromFbxMatrix(fbxCurrentPose);
	}
	u_pAnimaParam->currentTime = Wrap(u_pAnimaParam->currentTime + u_pAnimaParam->frameTime, u_pAnimaParam->startTime, u_pAnimaParam->endTime);
	u_pConstBuffer->Map(&constMapSkin);
}

void gamelib::FbxAnimation::GraphicsCommand()
{
	u_pConstBuffer->GraphicsCommand();
}
