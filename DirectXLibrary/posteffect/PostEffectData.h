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
	std::unique_ptr<RenderTarget> render;
public:
	GrayScale();
	void Befoer() override;
	void After() override;
};

class Sepia : public IPostProcess
{
private:
	std::unique_ptr<RenderTarget> render;
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
	std::unique_ptr<RenderTarget> render;
public:
	Mosaic();
	void Befoer() override;
	void After() override;
};

class Grain : public IPostProcess
{
private:
	std::unique_ptr<RenderTarget> render;
public:
	Grain();
	void Befoer() override;
	void After() override;
};

class RetroFilter : public IPostProcess
{
private:
	std::unique_ptr<RenderTarget> render;
public:
	RetroFilter();
	void Befoer() override;
	void After() override;
};

class GaussFilter : public IPostProcess
{
private:
	IPipelineState* renderPipeline;
	IPipelineState* blurWPipeline;
	IPipelineState* blurHPipeline;

	std::unique_ptr<GaussianBlur> gauss;
	std::unique_ptr<RenderTarget> textures[2];
public:
	GaussFilter();
	void Befoer() override;
	void After() override;
};

class Bloom : public IPostProcess
{
private:
	IPipelineState* highlimPipeline;
	IPipelineState* hdBlurWPipeline;
	IPipelineState* hdBlurHPipeline;
	IPipelineState* bloomPipeline;

	std::unique_ptr<GaussianBlur> gauss;
	std::unique_ptr<RenderTarget> highTex;
	std::unique_ptr<RenderTarget> textures[2];
public:
	Bloom();
	void Befoer() override;
	void After() override;
};
} // namespace gamelib
