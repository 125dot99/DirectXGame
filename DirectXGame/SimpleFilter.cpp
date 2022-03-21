#include "SimpleFilter.h"

#include "math/MyMath.h"
#include "posteffect/PostEffectData.h"

void SimpleFilter::CreateFilter()
{
	switch (filterState)
	{
	case FILTER_STATE_ENUM::NONE:
		filter.reset();
		break;
	case FILTER_STATE_ENUM::GRAY_COLOR:
		filter = std::make_unique<GrayScale>();
		break;
	case FILTER_STATE_ENUM::SEPIA_COLOR: 
		filter = std::make_unique<Sepia>(); 
		break;
	case FILTER_STATE_ENUM::MOSAIC:
		filter = std::make_unique<Mosaic>();
		break;
	case FILTER_STATE_ENUM::GRAIN: 
		filter = std::make_unique<Grain>();
		break;
	case FILTER_STATE_ENUM::GAUSS_BLUR:
		filter = std::make_unique<GaussFilter>();
		break;
	case FILTER_STATE_ENUM::BLOOM:
		filter = std::make_unique<Bloom>();
		break;
	default:
		break;
	}
}

SimpleFilter::SimpleFilter(FILTER_STATE_ENUM filterEnum)
{
	filterState = filterEnum;
	CreateFilter();
}

void SimpleFilter::Move(int value)
{
	if (value == 0)
	{
		return;
	}
	int nv = Wrap((int)filterState + value, (int)FILTER_STATE_ENUM::NONE, (int)FILTER_STATE_ENUM::BLOOM);
	filterState = (FILTER_STATE_ENUM)nv;
	CreateFilter();
}

void SimpleFilter::ChangeFilter(FILTER_STATE_ENUM filterEnum)
{
	filterState = filterEnum;
	CreateFilter();
}

void SimpleFilter::Draw()
{
	if (filter == nullptr)
	{
		return;
	}
	filter->Befoer();
	filter->After();
}

int SimpleFilter::GetFilterIndex() const
{
	return (int)filterState;
}
