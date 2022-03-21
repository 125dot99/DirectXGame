#include "GctLoader.h"

#include <fstream>
#include <assert.h>

gamelib::GctData gamelib::GctLoader::ReadGctData(const std::string& _fileName)
{
	const std::string GCT_PATH = "Resources/Text/";
	std::string fullPath = GCT_PATH + _fileName;
	
	//バイナリモードで開く
	std::ifstream file(fullPath, std::ios::binary);
	assert(file.is_open() && "読み取りに失敗しました");
	
	GctData output;
	char id[2];
	file.read((char*)&id, sizeof(id));
	assert(std::strncmp(id, "ID", 2) == 0);
	unsigned short key = 0;
	std::vector<unsigned short>& data = output.numArray;
	while (!file.eof())//読み込み処理
	{
		//識別番号
		file.read((char*)&key, sizeof(key));
		data.emplace_back(key);
	}
	file.close();
	data.pop_back();//削除
	data.shrink_to_fit();
	return output;
}

