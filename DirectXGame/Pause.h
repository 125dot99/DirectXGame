#pragma once
#include <GameLibHeader.h>
#include "SimpleFilter.h"
#include "dx12/GpuCopyPixels.h"

//�|�[�Y���̕\�����A���͂��Ǘ�����N���X
class Pause : public BaseChildScene
{
private:
	bool nextState;
	bool stickEnable;
	int selectIndex;
	Vector2 buttonSize;

	SpriteRenderer* spriteRenderer;
	std::vector<std::weak_ptr<Texture>> textures;
public:
	Pause(SpriteRenderer* spriteRenderer);

	void Update() override;
	void Draw() override;
	bool IsState(const char* typeName) const;
	bool IsNext() const override;
};
