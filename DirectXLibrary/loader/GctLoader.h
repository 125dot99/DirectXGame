#pragma once
#include <vector>
#include <string>

namespace gamelib
{ 
//�g���q.gct
struct GctData
{
	static const int MAX_SIZE_X = 17;//���ߑł�
	static const int MAX_SIZE_Y = 13;//���ߑł�
	static const int INDENTION = SHRT_MAX;//���s
	std::vector<unsigned short> numArray; //�����̊i�[�f�[�^
};

//�Ǝ��t�H�[�}�b�g
class GctLoader
{
private:
	GctLoader() = delete;
	~GctLoader() = delete;
public:
	static GctData ReadGctData(const std::string& _fileName);
};
} // namespace gamelib
