#pragma once
#include <vector>
#include <string>
#include <memory>
#include "SpriteRenderer.h"
#include "SpriteUtil.h"
#include "../dx12/Texture.h"
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

class SpriteFontAscii
{
private:
	//1度に描画するテキスト数
	static const int MAX_BUFF_COUNT = 256;
	//並べられた文字数
	static const char ASCII_FONT_LINE = 16;

	Vector2 fontSize;
	std::weak_ptr<Texture> fontTexture;
	SpriteRenderer* spriteRenderer;
public:
	SpriteFontAscii(SpriteRenderer* pSpriteRenderer, const 	std::weak_ptr<Texture>& w_p_ascii_texture);
	//文字列バッファ
	void Draw(const std::string& text, const Vector2& pos, const Vector2& scale = { 1,1 }, const Vector4& color = {1,1,1,1});
	//int型入力
	void Draw(int value, const Vector2& pos, const Vector2& scale = { 1,1 }, const Vector4& color = { 1,1,1,1 });
	//float型入力
	void Draw(float value, const Vector2& pos, const Vector2& scale = { 1,1 }, const Vector4& color = { 1,1,1,1 });
};

class SpriteFont
{
private:
	Vector2 charSize;
	SpriteRenderer* spriteRenderer;
	std::vector<unsigned short> charIndices;
	std::vector<Vector2> textPosArray;
	std::vector<Vector4> textRect;
	FontAnimation& fontAnimation;
	std::weak_ptr<Texture> fontTexture;	
public:
	SpriteFont(SpriteRenderer* pSpriteRenderer, const std::weak_ptr<Texture>& w_pFontTexture, FontAnimation& _fontAnimation);

	void LoadGctFile(const std::string& gctFile);

	void NextReadText(int lineCount = 0);

	void Draw(const Vector2& pos, const Vector2& scale);

	inline bool IsEnd() { return charIndices.size() == 0; }
};
} // namespace gamelib
