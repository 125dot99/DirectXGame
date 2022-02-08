#include "ParticleUnit.h"

#include "../math/MyMath.h"
#include "../math/MyRandom.h"
#include "../pipeline/PipelineManager.h"

gamelib::ParticleUnit::ParticleUnit(const std::weak_ptr<Texture>& w_p_Texture, Emitter* p_Emitter) : emitter(p_Emitter)
{
	texture = w_p_Texture;
	vertexBuffer = std::make_unique<VertexBuffer<VertexPosColorScaleAngle>>();
	vertexBuffer->Create(VP_MAX_SIZE, MESH_PRIMITIVE::POINT_LIST);
}

gamelib::ParticleUnit::~ParticleUnit()
{

}

void gamelib::ParticleUnit::AddParticle()
{
	if (emitter->frame++ < emitter->spwanFrame || 
		std::distance(particles.begin(), particles.end()) >= VP_MAX_SIZE)
	{
		return;
	}
	static Particle particle;
	emitter->frame = 0;
	for (int i = 0; i < emitter->spawnCount; i++)
	{
		particle.position = emitter->position + Vector3(
			Random(emitter->positionRandFactor.x), 
			Random(emitter->positionRandFactor.y), 
			Random(emitter->positionRandFactor.z));
		particle.velocity = emitter->velocity + Vector3(
			Random(emitter->velocityRandFactor.x),
			Random(emitter->velocityRandFactor.y),
			Random(emitter->velocityRandFactor.z));
		particle.accel = emitter->accel;
		particle.startScale = emitter->startScale;
		particle.endScale = emitter->endScale;
		particle.startColor = emitter->startColor;
		particle.endColor = emitter->endColor;
		particle.lifeFrame = emitter->life;
		particles.emplace_front(particle);
	}
}

void gamelib::ParticleUnit::Update()
{
	AddParticle();
	for (auto&& x : particles)
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
	particles.remove_if([](Particle& x) { return x.frame >= x.lifeFrame; });
}

void gamelib::ParticleUnit::Draw()
{
	UINT count = (UINT)std::distance(particles.cbegin(), particles.cend());
	if (count == 0)
	{
		return;
	}
	auto vertexMap = (VertexPosColorScaleAngle*)vertexBuffer->GetMapPointer();
	for (auto&& x : particles)
	{
		vertexMap->pos = x.position;
		vertexMap->color = x.color;
		vertexMap->scale = x.scale;
		vertexMap->angle = x.rotation;
		vertexMap++;
	}
	vertexBuffer->UnMap();
	texture.lock()->GraphicsSRVCommand(3);
	vertexBuffer->BufferCommand();
	vertexBuffer->Draw(count, 0);
}

gamelib::ParticleToonUnit::ParticleToonUnit(const std::weak_ptr<Texture>& pTexture, 
	const std::weak_ptr<Texture>& pNormalTex, Emitter* p_Emitter) : ParticleUnit(pTexture, p_Emitter), normalTex(pNormalTex)
{

}

void gamelib::ParticleToonUnit::Draw()
{
	PipelineManager::GetInstance()->GetPipelineState("ParticleToonShader")->Command();
	//ライト、テクスチャ2枚
	normalTex.lock()->GraphicsSRVCommand(4);
	ParticleUnit::Draw();
}
