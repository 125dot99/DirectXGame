#pragma once
#include <forward_list>
#include <memory>
#include "Particle.h"
#include "Emitter.h"
#include "../dx12/Texture.h"
#include "../dx12/VertexBuffer.h"

namespace gamelib
{
//�p�[�e�B�N���̔����@
class ParticleUnit
{
private:
	//�p�[�e�B�N������������ő吔
	static const int VP_MAX_SIZE = 1024;
	//�G�~�b�^�[
	Emitter* pEmitter;
	//�p�[�e�B�N���̔z��
	std::forward_list<Particle> f_listParticles;
	//���_�o�b�t�@
	std::unique_ptr<VertexBuffer<VertexPosColorScaleAngle>> u_pVertexBuffer;
	//�e�N�X�`��
	std::weak_ptr<Texture> w_pTexture;

	void AddParticle();
public:
	ParticleUnit(const std::weak_ptr<Texture>& w_pTexture, Emitter* pEmitter);

	void Update();

	void Draw();
};
} // namespace gamelib
