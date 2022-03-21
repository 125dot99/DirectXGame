#include "ParticleUnit.h"

#include "../math/MyMath.h"
#include "../math/MyRandom.h"
#include "../pipeline/PipelineManager.h"

gamelib::ParticleUnit::ParticleUnit(const std::weak_ptr<Texture>& w_pTexture, Emitter* pEmitter) : w_pTexture(w_pTexture), pEmitter(pEmitter)
{
	u_pVertexBuffer = std::make_unique<VertexBuffer<VertexPosColorScaleAngle>>();
	u_pVertexBuffer->Create(VP_MAX_SIZE, MESH_PRIMITIVE::POINT_LIST);
}


void gamelib::ParticleUnit::AddParticle()
{
	if (pEmitter->frame++ < pEmitter->spwanFrame || 
		std::distance(f_listParticles.begin(), f_listParticles.end()) >= VP_MAX_SIZE)
	{
		return;
	}
	static Particle particle;
	pEmitter->frame = 0;
	for (int i = 0; i < pEmitter->spawnCount; i++)
	{
		particle.position = pEmitter->position + Vector3(
			Random(pEmitter->positionRandFactor.x), 
			Random(pEmitter->positionRandFactor.y), 
			Random(pEmitter->positionRandFactor.z));
		particle.velocity = pEmitter->velocity + Vector3(
			Random(pEmitter->velocityRandFactor.x),
			Random(pEmitter->velocityRandFactor.y),
			Random(pEmitter->velocityRandFactor.z));
		particle.accel = pEmitter->accel;
		particle.startScale = pEmitter->startScale;
		particle.endScale = pEmitter->endScale;
		particle.startColor = pEmitter->startColor;
		particle.endColor = pEmitter->endColor;
		particle.lifeFrame = pEmitter->life;
		f_listParticles.emplace_front(particle);
	}
}

void gamelib::ParticleUnit::Update()
{
	AddParticle();
	for (auto&& x : f_listParticles)
	{
		float f = (float)x.frame / (float)x.lifeFrame;
		x.velocity += x.accel;
		x.position += x.velocity;
		x.color = Lerp(x.startColor, x.endColor, f);
		x.scale = Lerp(x.startScale, x.endScale, f);
		x.rotation = Lerp(x.startRotation, x.endRotation, f);
		x.frame++;//経過フレーム
	}
	//パーティクル削除
	f_listParticles.remove_if([](Particle& x) { return x.frame >= x.lifeFrame; });
}

void gamelib::ParticleUnit::Draw()
{
	UINT count = (UINT)std::distance(f_listParticles.cbegin(), f_listParticles.cend());
	if (count == 0)
	{
		return;
	}
	auto vertexMap = (VertexPosColorScaleAngle*)u_pVertexBuffer->GetMapPointer();
	for (auto&& x : f_listParticles)
	{
		vertexMap->pos = x.position;
		vertexMap->color = x.color;
		vertexMap->scale = x.scale;
		vertexMap->angle = x.rotation;
		vertexMap++;
	}
	u_pVertexBuffer->UnMap();
	w_pTexture.lock()->GraphicsSRVCommand(3);
	u_pVertexBuffer->BufferCommand();
	u_pVertexBuffer->Draw(count, 0);
}

