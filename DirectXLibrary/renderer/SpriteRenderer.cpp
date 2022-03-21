#include "SpriteRenderer.h"
#include "../math/MyMath.h"
#include "../math/Matrix4.h"
#include "../app/Application.h"
#include "../pipeline/GraphicsPipeline.h"
#include "SpriteUtil.h"

//スプライトの最大値
static const int MAX_SPRITE = 1000;

gamelib::SpriteRenderer::SpriteRenderer()
{
	spriteCount = 0;
	u_pVertexBuffer = std::make_unique<VertexBuffer<VertexSprite>>();
	u_pVertexBuffer->Create(MAX_SPRITE, MESH_PRIMITIVE::POINT_LIST);

	u_pConstBuffer = std::make_unique<ConstBuffer>();
	u_pConstBuffer->Init(0, sizeof(Matrix4));
	Vector2 screenSize = Application::GetInstance()->GetWindowSize();
	u_pConstBuffer->Map(&Matrix4(MatrixScreen(screenSize.x, screenSize.y)));

	GraphicsPipelineDesc gpsoDesc;
	gpsoDesc.semantics = { "POSITION", "TEXCOORD", "PSIZE", "COLOR", "PIVOT", "RECTANGLE" };
	gpsoDesc.hlsl.vs.file = "PointSpriteVS.hlsl";
	gpsoDesc.hlsl.ps.file = "PointSpritePS.hlsl";
	gpsoDesc.hlsl.gs.file = "PointSpriteGS.hlsl";
	gpsoDesc.cullMode = D3D12_CULL_MODE_NONE;
	gpsoDesc.depthEnable = false;
	gpsoDesc.primitive = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	gpsoDesc.rootParams = { ROOT_PARAMETER::WORLD, ROOT_PARAMETER::TEXTURE };
	u_pPipeline = std::make_unique<GraphicsPipeline>(gpsoDesc);
}

void gamelib::SpriteRenderer::Renderer(const std::weak_ptr<Texture>& w_pTexture, const Vector2& pos, const Vector2& size, float angle,
	const Vector2& pivot, const Vector4& rect, const Vector4& color)
{
	if (spriteCount >= MAX_SPRITE)
	{
		return;
	}
	static VertexSprite sprite;
	sprite = { pos, size, angle, color, pivot, rect };
	auto vertMap = ((VertexSprite*)u_pVertexBuffer->GetMapPointer()) + spriteCount;
	memcpy(vertMap, &sprite, sizeof(VertexSprite));
	w_pTexture.lock()->GraphicsSRVCommand(1);
	u_pVertexBuffer->Draw(1, spriteCount++);
}

void gamelib::SpriteRenderer::Begin()
{
	u_pPipeline->Command();
	u_pConstBuffer->GraphicsCommand();
	u_pVertexBuffer->BufferCommand();
}

void gamelib::SpriteRenderer::SetScreenSize(const Vector2& screenSize)
{
	u_pConstBuffer->Map(&Matrix4(MatrixScreen(screenSize.x, screenSize.y)));
}

void gamelib::SpriteRenderer::Draw(const std::weak_ptr<Texture>& w_pTexture, const Vector2& pos, const Vector4& color)
{
	Renderer(w_pTexture, pos, w_pTexture.lock()->GetSize(), 0, Vector2::Zero(), Vector4(0, 0, 1, 1), color);
}

void gamelib::SpriteRenderer::Draw(const std::weak_ptr<Texture>& w_pTexture, const Vector2& pos, const Vector2& pivot, const Vector4& color)
{
	Renderer(w_pTexture, pos, w_pTexture.lock()->GetSize(), 0, pivot, Vector4(0, 0, 1, 1), color);
}

void gamelib::SpriteRenderer::DrawTurnX(const std::weak_ptr<Texture>& w_pTexture, const Vector2& pos, const Vector4& color)
{
	Renderer(w_pTexture.lock(), pos, w_pTexture.lock()->GetSize() * Vector2(-1, 1), 0, Vector2::Zero(), Vector4(0, 0, 1, 1), color);
}

void gamelib::SpriteRenderer::DrawTurnY(const std::weak_ptr<Texture>& w_pTexture, const Vector2& pos, const Vector4& color)
{
	Renderer(w_pTexture, pos, w_pTexture.lock()->GetSize() * Vector2(1, -1), 0, Vector2::Zero(), Vector4(0, 0, 1, 1), color);
}

void gamelib::SpriteRenderer::DrawExtend(const std::weak_ptr<Texture>& w_pTexture, const Vector2& pos, const Vector2& size, const Vector4& color)
{
	Renderer(w_pTexture, pos, size, 0, Vector2::Zero(), Vector4(0, 0, 1, 1), color);
}

void gamelib::SpriteRenderer::DrawExtend(const std::weak_ptr<Texture>& w_pTexture, const Vector2& pos, const Vector2& size, const Vector2& pivot, const Vector4& color)
{
	Renderer(w_pTexture, pos, size, 0, pivot, Vector4(0, 0, 1, 1), color);
}

void gamelib::SpriteRenderer::DrawRotate(const std::weak_ptr<Texture>& w_pTexture, const Vector2& pos, float angle, const Vector4& color)
{
	Renderer(w_pTexture, pos, w_pTexture.lock()->GetSize(), angle, Vector2::Zero(), Vector4(0, 0, 1, 1), color);
}

void gamelib::SpriteRenderer::DrawRotate(const std::weak_ptr<Texture>& w_pTexture, const Vector2& pos, float angle, const Vector2& pivot, const Vector4& color)
{
	Renderer(w_pTexture, pos, w_pTexture.lock()->GetSize(), angle, pivot, Vector4(0, 0, 1, 1), color);
}

void gamelib::SpriteRenderer::DrawRect(const std::weak_ptr<Texture>& w_pTexture, const Vector2& pos, const Vector4& rect, const Vector4& color)
{
	const Vector2& texSize = w_pTexture.lock()->GetSize();
	Vector4 graphRect(rect.x / texSize.x, rect.y / texSize.y, rect.z / texSize.x, rect.w / texSize.y);
	Renderer(w_pTexture, pos, texSize, 0, Vector2::Zero(), graphRect, color);
}

void gamelib::SpriteRenderer::DrawExtendRect(const std::weak_ptr<Texture>& w_pTexture, const Vector2& pos, const Vector2& size, const Vector4& rect, const Vector4& color)
{
	const Vector2& texSize = w_pTexture.lock()->GetSize();
	Vector4 graphRect(rect.x / texSize.x, rect.y / texSize.y, rect.z / texSize.x, rect.w / texSize.y);
	Renderer(w_pTexture, pos, size, 0, Vector2::Zero(), graphRect, color);
}

void gamelib::SpriteRenderer::DrawExtendRect(const std::weak_ptr<Texture>& w_pTexture, const Vector2& pos, const Vector2& size, const Vector4& rect, const Vector2& pivot, const Vector4& color)
{
	const Vector2& texSize = w_pTexture.lock()->GetSize();
	Vector4 graphRect(rect.x / texSize.x, rect.y / texSize.y, rect.z / texSize.x, rect.w / texSize.y);
	Renderer(w_pTexture, pos, size, 0, pivot, graphRect, color);
}

void gamelib::SpriteRenderer::DrawExtendMakeRect(const std::weak_ptr<Texture>& w_pTexture, const Vector2& pos, const Vector2& size, const Vector2& indexSize, int index, const Vector4& color)
{
	Vector4 graphRect(sprite_util::DivisionTextureUvRect(indexSize, index));
	Renderer(w_pTexture, pos, size, 0, Vector2::Zero(), graphRect, color);
}

void gamelib::SpriteRenderer::End()
{
	if (spriteCount != 0)
	{
		spriteCount = 0;
		u_pVertexBuffer->UnMap();
	}
}

