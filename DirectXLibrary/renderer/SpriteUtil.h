#pragma once
#include "../math/Vector.h"

namespace gamelib
{
struct SpriteAnimation
{
	int count;
	int startIndex;
	int animaIndex;
	int endIndex;

	void SetIndex(int stratIndex, int endIndex);
	void Update(int fixedFrame);
};
struct FontAnimation
{
	float time;
	int animaIndex;
	int endIndex;
	
	void SetIndex(int stratIndex, int endIndex);
	void Update(float nextAnimaSpeed);
	bool IsEnd() const;
};

namespace sprite_util
{
inline Vector2 TextureTableIndexXSort(const Vector2& indexSize, int index)
{
	int x = index / (int)indexSize.y;
	int prevx = index;
	while (x > indexSize.x)
	{
		prevx = x;
		x /= (int)indexSize.y;
	}
	int y = prevx % (int)indexSize.y;
	return Vector2((float)x, (float)y);
}

inline Vector2 TextureTableIndexYSort(const Vector2& indexSize, int index)
{
	int y = index / (int)indexSize.x;
	int prevy = index;
	while (y > indexSize.y)
	{
		prevy = y;
		y /= (int)indexSize.x;
	}
	int x = prevy % (int)indexSize.x;
	return Vector2((float)x, (float)y);
}

inline Vector4 DivisionTextureUvRect(const Vector2& indexSize, int index)
{
	Vector2 rect = TextureTableIndexYSort(indexSize, index);
	Vector2 offset = 1.0f / indexSize;
	return Vector4(rect.x * offset.x, rect.y * offset.y, (rect.x + 1) * offset.x, (rect.y + 1) * offset.y);
}
} //namespace sprite_util
} // namespace gamelib