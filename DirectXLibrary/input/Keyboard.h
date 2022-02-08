#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <wrl.h>

#include "IKeyborad.h"

#pragma comment(lib, "dinput8.lib")

namespace gamelib
{
class Keyboard : public IKeyboard
{
private:
	//キー入力を格納する数
	static const int MAX_KEY = 256;
	//キー入力格納
	BYTE key[MAX_KEY] = {};
	//前フレームのキー番号
	BYTE keyPre[MAX_KEY] = {};
	//キーのデバイスデータ
	Microsoft::WRL::ComPtr<IDirectInputDevice8W> dInputDevice;
	//入力デバイスの生成
	void CreateInputDevice();
public:
	Keyboard();
	void Update() override;
	bool IsKey(KEY_CODE keyCode) const override;
	bool IsKeyDown(KEY_CODE keyCode) const override;
	bool IsKeyUp(KEY_CODE keyCode) const override;
};
} // namespace gamelib