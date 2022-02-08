#include "SpriteFont.h"

#include <sstream>

#include "../math/MyMath.h"
#include "../loader/GctLoader.h"

gamelib::SpriteFontAscii::SpriteFontAscii(SpriteRenderer* pSpriteRenderer, const std::weak_ptr<Texture>& w_p_ascii_texture)
{
	spriteRenderer = pSpriteRenderer;
	fontTexture = w_p_ascii_texture;
	fontSize = fontTexture.lock()->GetSize() / Vector2(ASCII_FONT_LINE, 6.0f);
}

void gamelib::SpriteFontAscii::Draw(const std::string& text, const Vector2& position, const Vector2& scale, const Vector4& color)
{
	for (int i = 0, end = (int)text.size(); i < end; i++)
	{
		const unsigned char& character = text[i];
		//ASCII
		int fontIndex = character - ASCII_FONT_LINE * 2;
		if (character >= 0x7f)
		{
			fontIndex = 0;
		}
		int fontIndexY = fontIndex / (int)ASCII_FONT_LINE;
		int fontIndexX = fontIndex % (int)ASCII_FONT_LINE;
		//1�������̕�
		Vector2 pos = Vector2(position.x + (fontSize.x * scale.x * (float)i), position.y);
		Vector2 offset = Vector2((float)fontIndexX, (float)fontIndexY) * fontSize;
		Vector4 rect(offset.x, offset.y, offset.x + fontSize.x, offset.y + fontSize.y);
		spriteRenderer->DrawExtendRect(fontTexture, pos, fontSize * scale, rect, color);
	}
}

void gamelib::SpriteFontAscii::Draw(int value, const Vector2& pos, const Vector2& scale, const Vector4& color)
{
	Draw(std::to_string(value), pos, scale, color);
}

void gamelib::SpriteFontAscii::Draw(float value, const Vector2& pos, const Vector2& scale, const Vector4& color)
{
	std::stringstream str;
	str << value;
	Draw(str.str(), pos, scale, color);
}

gamelib::SpriteFont::SpriteFont(SpriteRenderer* pSpriteRenderer, const std::weak_ptr<Texture>& w_p_fontTexture)
{
	fontTexture = w_p_fontTexture;
	spriteRenderer = pSpriteRenderer;
	charSize.x = fontTexture.lock()->GetSize().x / GctData::MAX_SIZE_X;
	charSize.y = fontTexture.lock()->GetSize().y / GctData::MAX_SIZE_Y;
}

void gamelib::SpriteFont::LoadGctFile(const std::string& _gctFile)
{
	charIndices.swap(GctLoader::ReadGctData(_gctFile).numArray);
	Vector2 indexSize = Vector2((float)GctData::MAX_SIZE_X, (float)GctData::MAX_SIZE_Y);
	for (auto&& x : charIndices)
	{
		textPosArray.emplace_back(sprite_util::TextureTableIndexXSort(indexSize, x) * charSize);
	}
}

void gamelib::SpriteFont::NextReadText(int lineCount)
{
	int preIndex = animation.endIndex;
	charIndices.erase(charIndices.begin(), charIndices.begin() + preIndex);
	textPosArray.erase(textPosArray.begin(), textPosArray.begin() + preIndex);
	int endIndex = 0;
	int count = 0;
	for (int i = 0, end = (int)charIndices.size(); i < end; i++)
	{
		endIndex++;
		if (charIndices[i] == GctData::INDENTION)//���s
		{
			if (++count == lineCount)
			{
				break;
			}
		}
	}
	animation.SetIndex(0, endIndex);
}

void gamelib::SpriteFont::Draw(const Vector2& position, const Vector2& scale)
{
	Vector2 pos = position;
	Vector4 rect;
	animation.Update(0.2f);
	int index = animation.animaIndex;
	for (int i = 0; i < index; i++)
	{
		if (charIndices[i] == GctData::INDENTION)//���s
		{
			pos.x = position.x;
			pos.y += charSize.y * scale.y;
			if (i + 1 == 0)
			{
				pos.y = position.y;
			}
			continue;
		}
		rect.x = textPosArray[i].x;
		rect.y = textPosArray[i].y;
		rect.z = rect.x + charSize.x;
		rect.w = rect.y + charSize.y;
		spriteRenderer->DrawExtendRect(fontTexture, pos, charSize * scale, rect);
		pos.x += charSize.x * scale.x;
	}
	if (animation.IsEnd())
	{
		return;
	}
	float rectX = Lerp(0.0f, charSize.x, animation.time);//���`���
	rect.x = textPosArray[index].x;
	rect.y = textPosArray[index].y;
	rect.z = rect.x + rectX;
	rect.w = rect.y + charSize.y;
	spriteRenderer->DrawExtendRect(fontTexture, pos, Vector2(rectX, charSize.y) * scale, rect);
}
