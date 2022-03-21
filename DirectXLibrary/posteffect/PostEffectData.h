#pragma once
#include "IPostProcess.h"
#include "../pipeline/IPipelineState.h"
#include "../dx12/RenderTarget.h"
#include "GaussianBlur.h"
#include <memory>

namespace gamelib
{
class GrayScale : public IPostProcess
{
private:
	std::unique_ptr<RenderTarget> u_pRenderTarget;
public:
	GrayScale();
	void Befoer() override;
	void After() override;
};

class Sepia : public IPostProcess
{
private:
	std::unique_ptr<RenderTarget> u_pRenderTarget;
public:
	Sepia();
	void Befoer() override;
	void After() override;
};

class Vignetting : public IPostProcess
{
public:
	void Befoer() override;
	void After() override;
};

class Mosaic : public IPostProcess
{
private:
	std::unique_ptr<RenderTarget> u_pRenderTarget;
public:
	Mosaic();
	void Befoer() override;
	void After() override;
};

class Grain : public IPostProcess
{
private:
	std::unique_ptr<RenderTarget> u_pRenderTarget;
public:
	Grain();
	void Befoer() override;
	void After() override;
};

class RetroFilter : public IPostProcess
{
private:
	std::unique_ptr<RenderTarget> u_pRenderTarget;
public:
	RetroFilter();
	void Befoer() override;
	void After() override;
};

class GaussFilter : public IPostProcess
{
private:
	std::weak_ptr<IPipelineState> w_pRenderPipeline;
	std::weak_ptr<IPipelineState> w_pBlurWPipeline;
	std::weak_ptr<IPipelineState> w_pBlurHPipeline;

	std::unique_ptr<GaussianBlur> u_pGaussianBlur;
	//ècÅAâ°Ç…ÉuÉâÅ[Çä|ÇØÇÈÇΩÇﬂ2ñá
	std::unique_ptr<RenderTarget> u_pRenderTargets[2];
public:
	GaussFilter();
	void Befoer() override;
	void After() override;
};

class Bloom : public IPostProcess
{
private:
	std::weak_ptr<IPipelineState> w_pHighlimPipeline;
	std::weak_ptr<IPipelineState> w_pBlurWPipeline;
	std::weak_ptr<IPipelineState> w_pBlurHPipeline;
	std::weak_ptr<IPipelineState> w_pBloomPipeline;
	std::unique_ptr<RenderTarget> u_pHighlimRenderTarget;

	std::unique_ptr<GaussianBlur> u_pGaussianBlur;
	//ècÅAâ°Ç…ÉuÉâÅ[Çä|ÇØÇÈÇΩÇﬂ2ñá
	std::unique_ptr<RenderTarget> u_pRenderTargets[2];
public:
	Bloom();
	void Befoer() override;
	void After() override;
};

class DepthOfField : public IPostProcess
{
private:

public:
	DepthOfField();
	void Befoer() override;
	void After() override;
};
} // namespace gamelib
