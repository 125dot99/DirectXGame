#pragma once
#include <vector>
#include <string>

namespace gamelib
{ 
//拡張子.gct
struct GctData
{
	static const int MAX_SIZE_X = 17;//決め打ち
	static const int MAX_SIZE_Y = 13;//決め打ち
	static const int INDENTION = SHRT_MAX;//改行
	std::vector<unsigned short> numArray; //文字の格納データ
};

//独自フォーマット
class GctLoader
{
private:
	GctLoader() = delete;
	~GctLoader() = delete;
public:
	static GctData ReadGctData(const std::string& _fileName);
};
} // namespace gamelib
