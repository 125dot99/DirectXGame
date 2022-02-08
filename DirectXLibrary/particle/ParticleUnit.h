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
	
	Emitter* emitter;
	//�p�[�e�B�N���̔z��
	std::forward_list<Particle> particles;
	//���_�o�b�t�@
	std::unique_ptr<VertexBuffer<VertexPosColorScaleAngle>> vertexBuffer;
	//�e�N�X�`��
	std::weak_ptr<Texture> texture;

	void AddParticle();
public:
	ParticleUnit(const std::weak_ptr<Texture>& w_p_Texture, Emitter* p_Emitter);
	~ParticleUnit();

	void Update();

	virtual void Draw();
};

class ParticleToonUnit : public ParticleUnit
{
private:
	std::weak_ptr<Texture> normalTex;
public:
	ParticleToonUnit(const std::weak_ptr<Texture>& pTexture, 
		const std::weak_ptr<Texture>& pNormalTex, Emitter* p_Emitter);

	void Draw() override;
};
} // namespace gamelib
