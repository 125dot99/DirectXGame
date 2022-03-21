#pragma once
#include <forward_list>
#include <memory>
#include "Particle.h"
#include "Emitter.h"
#include "../dx12/Texture.h"
#include "../dx12/VertexBuffer.h"

namespace gamelib
{
//パーティクルの発生機
class ParticleUnit
{
private:
	//パーティクル発生させる最大数
	static const int VP_MAX_SIZE = 1024;
	//エミッター
	Emitter* pEmitter;
	//パーティクルの配列
	std::forward_list<Particle> f_listParticles;
	//頂点バッファ
	std::unique_ptr<VertexBuffer<VertexPosColorScaleAngle>> u_pVertexBuffer;
	//テクスチャ
	std::weak_ptr<Texture> w_pTexture;

	void AddParticle();
public:
	ParticleUnit(const std::weak_ptr<Texture>& w_pTexture, Emitter* pEmitter);

	void Update();

	void Draw();
};
} // namespace gamelib
