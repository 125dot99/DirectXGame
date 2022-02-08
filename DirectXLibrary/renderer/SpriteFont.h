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
class SpriteFontAscii
{
private:
	//1�x�ɕ`�悷��e�L�X�g��
	static const int MAX_BUFF_COUNT = 256;
	//���ׂ�ꂽ������
	static const char ASCII_FONT_LINE = 16;

	Vector2 fontSize;
	std::weak_ptr<Texture> fontTexture;
	SpriteRenderer* spriteRenderer;
public:
	SpriteFontAscii(SpriteRenderer* pSpriteRenderer, const 	std::weak_ptr<Texture>& w_p_ascii_texture);
	//������o�b�t�@
	void Draw(const std::string& text, const Vector2& pos, const Vector2& scale = { 1,1 }, const Vector4& color = {1,1,1,1});
	//int�^����
	void Draw(int value, const Vector2& pos, const Vector2& scale = { 1,1 }, const Vector4& color = { 1,1,1,1 });
	//float�^����
	void Draw(float value, const Vector2& pos, const Vector2& scale = { 1,1 }, const Vector4& color = { 1,1,1,1 });
};

class SpriteFont
{
private:
	Vector2 charSize;

	FontAnimation animation;
	std::vector<unsigned short> charIndices;
	std::vector<Vector2> textPosArray;
	std::weak_ptr<Texture> fontTexture;	
	SpriteRenderer* spriteRenderer;
public:
	SpriteFont(SpriteRenderer* pSpriteRenderer, const std::weak_ptr<Texture>& w_p_fontTexture);

	void LoadGctFile(const std::string& _gctFile);

	void NextReadText(int lineCount = 0);

	void Draw(const Vector2& pos, const Vector2& scale);

	bool IsEnd() { return charIndices.size() == 0; }
};
} // namespace gamelib
