#pragma once
#include "BaseScene.h"
#include <memory>
namespace gamelib
{
//シーン管理クラス
class SceneManager : public ISceneManager
{
private:
	/// <summary>
	/// シーンのポインタ
	/// </summary>
	std::unique_ptr<BaseScene> scene;

	/// <summary>
	/// 次のシーン
	/// </summary>
	void Next();

	/// <summary>
	/// 今のシーンを初期化
	/// </summary>
	void ResetScene();
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SceneManager(BaseScene* pMainRootScene);
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~SceneManager();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// ポインタ付け替え
	/// </summary>
	/// <param name="newScene">次のシーンのポインタ</param>
	void ChangeScene(BaseScene* newScene) override;
};
} // namespace scene