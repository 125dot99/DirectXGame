#pragma once
#include <vector>
#include <memory>
#include "ParticleUnit.h"
#include "../dx12/ConstBuffer.h"
#include "../pipeline/IPipelineState.h"

namespace gamelib
{
//�p�[�e�B�N���̐����Ǘ�
class ParticleManager
{
private:
	//�p�[�e�B�N���̔z��
	std::vector<ParticleUnit*> particleUnits;
	std::unique_ptr<IPipelineState> pipeline;
public:
	ParticleManager();
	~ParticleManager();

	//�󂯎�����f�[�^���Ƀp�[�e�B�N������
	void Add(ParticleUnit* emitter);

	void Remove(ParticleUnit* emitter);

	void Update();

	void Draw();
};
} // class ParticleManager
