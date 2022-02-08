#pragma once
#include <memory>
#include "posteffect/IPostProcess.h"

enum class FILTER_STATE_ENUM
{
	NONE,
	GRAY_COLOR,
	SEPIA_COLOR,
	MOSAIC,
	GRAIN,
	GAUSS_BLUR,
	BLOOM,
};

using namespace gamelib;
//ポストエフェクトを扱うクラス
class SimpleFilter
{
private:
	std::unique_ptr<IPostProcess> filter;
	FILTER_STATE_ENUM filterState = FILTER_STATE_ENUM::NONE;
	void CreateFilter();
public:
	SimpleFilter(FILTER_STATE_ENUM filterEnum = FILTER_STATE_ENUM::NONE);
	void Move(int value);
	void ChangeFilter(FILTER_STATE_ENUM filterEnum);
	void Draw();
};

